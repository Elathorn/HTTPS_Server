#include "RequestManager.h"
const char* RequestManager::_askPage = "<html><body>\
                       Podaj Twoje imie zebym powiedzial Ci jak dlugie jest.<br>\
                       <form action=\"/namepost\" method=\"post\">\
                       <input name=\"name\" type=\"text\"\
                       <input type=\"submit\" value=\" Wyslij \"></form>\
                       </body></html>";

const char* RequestManager::_greatingPage = "<html><body><h1>Twoje imie ma %i znakow!</center></h1></body></html>";

const char* RequestManager::_errorPage = "<html><body>Cos nie wyszlo.</body></html>";

RequestManager::RequestManager()
{
}


RequestManager::~RequestManager()
{
}

int RequestManager::iteratePost(void * coninfo_cls, MHD_ValueKind kind, const char * key, const char * filename, 
								const char * content_type, const char * transfer_encoding, const char * data, 
								uint64_t off, size_t size)
{
	ConnectionInfo *con_info = (ConnectionInfo*) coninfo_cls; //<-w razie b³êdu mo¿liwe Ÿród³o

	if (0 == strcmp(key, "name"))
	{
		if ((size > 0) && (size <= MAXNAMESIZE))
		{
			char *answerstring;
			answerstring = (char*)malloc(MAXANSWERSIZE); //alokujemy pamiec dla maksymalnej odpowiedzi
			if (!answerstring) return MHD_NO;
			int nameLength = strlen(data); //dlugosc stringa 
			snprintf(answerstring, MAXANSWERSIZE, _greatingPage, nameLength); //wklejamy dane do stringa
			con_info->_answerString = answerstring; //zmieniamy stringa dla danego polaczenia
		}
		else con_info->_answerString = NULL;

		return MHD_NO;
	}

	return MHD_YES;
}

void RequestManager::requestCompleted(void * cls, MHD_Connection * connection, void ** con_cls, MHD_RequestTerminationCode toe)
//funkcja wywolywana automatycznie przez daemona
{
	ConnectionInfo *conInfo = (ConnectionInfo*)*con_cls; //<-w razie b³êdu mo¿liwe Ÿród³o
	if (conInfo == NULL) return;
	if (conInfo->_connectionType == POST) //jesli typ POST
	{
		MHD_destroy_post_processor(conInfo->_postProcessor); //usuwamy postprocessor
		if (conInfo->_answerString) free(conInfo->_answerString); //i ew istniejacy _answerString
	}
	free(conInfo); //zwalniamy pamiec
	*con_cls = NULL;
}


int RequestManager::requestHandler(void *cls, struct MHD_Connection *connection,
	const char *url,
	const char *method, const char *version,
	const char *upload_data,
	size_t *upload_data_size, void **con_cls)
{
	if (NULL == *con_cls) //jesli polaczenie nie zostalo zainicjalizowane
	{
		ConnectionInfo *con_info; //inicjaluzujemy je

		con_info = (ConnectionInfo*)malloc(sizeof(struct ConnectionInfo)); //alokujemy pamiec
		if (NULL == con_info) return MHD_NO; 
		con_info->_answerString = NULL;

		if (0 == strcmp(method, "POST")) //ustalamy metode
		{
			con_info->_postProcessor =
				MHD_create_post_processor(connection, POSTBUFFERSIZE,
					iteratePost, (void *)con_info); //w wypadku POST musimy stworzyc postprocesor

			if (NULL == con_info->_postProcessor)
			{
				free(con_info);
				return MHD_NO;
			}

			con_info->_connectionType = POST;
		}
		else
			con_info->_connectionType = GET;

		*con_cls = (void *)con_info;

		return MHD_YES;
	}
	//jesli polaczenie zostalo juz zaincjalizowane
	if (0 == strcmp(method, "GET")) 
	{
		return sendPage(connection, _askPage); //wysylamy strone z pytaniem o imie
	}

	if (0 == strcmp(method, "POST"))
	{
		ConnectionInfo *con_info = (ConnectionInfo*)*con_cls; //aktualizujemy dane polaczenia

		if (*upload_data_size != 0) //jezeli jeszcze nie wyslalismy danych
		{
			MHD_post_process(con_info->_postProcessor, upload_data,
				*upload_data_size); //przetwarzamy zlecenie POST i wysylamy do niego dane
			*upload_data_size = 0; //i zmieniamy znacznik

			return MHD_YES;
		}
		else if (NULL != con_info->_answerString) //jesli dane zostaly juz wyslane
			return sendPage(connection, con_info->_answerString); //wyswietlamy strone
	}

	return sendPage(connection, _errorPage);
}

int RequestManager::sendPage(MHD_Connection * connection, const char * page)
{

	struct MHD_Response *response; 
	int ret; //return
	response = MHD_create_response_from_buffer(strlen(page), (void*)page, MHD_RESPMEM_PERSISTENT); //tworzymy odpowiedz serwera
	if (!response) return MHD_NO; //jesli nie udalo sie jej utworzyc, zwracamy blad

	ret = MHD_queue_response(connection, MHD_HTTP_OK, response); //w innym wypadku dodajemy ja do kolejki wyslania
	MHD_destroy_response(response); //usuwamy odpowiedz
	return ret;
}
