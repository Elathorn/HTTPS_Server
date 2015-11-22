#include "RequestManager.h"
const char* RequestManager::_askPage = "<html><body>\
                       What's your name, Sir?<br>\
                       <form action=\"/namepost\" method=\"post\">\
                       <input name=\"name\" type=\"text\"\
                       <input type=\"submit\" value=\" Send \"></form>\
                       </body></html>";

const char* RequestManager::_greatingPage = "<html><body><h1>Welcome, %s!</center></h1></body></html>";

const char* RequestManager::_errorPage = "<html><body>This doesn't seem to be right.</body></html>";

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
			answerstring = (char*)malloc(MAXANSWERSIZE);
			if (!answerstring) return MHD_NO;

			snprintf(answerstring, MAXANSWERSIZE, _greatingPage, data);
			con_info->_answerString = answerstring;
		}
		else con_info->_answerString = NULL;

		return MHD_NO;
	}

	return MHD_YES;
}

void RequestManager::requestCompleted(void * cls, MHD_Connection * connection, void ** con_cls, MHD_RequestTerminationCode toe)
{
	ConnectionInfo *conInfo = (ConnectionInfo*)*con_cls; //<-w razie b³êdu mo¿liwe Ÿród³o
	if (conInfo == NULL) return;
	if (conInfo->_connectionType == POST)
	{
		MHD_destroy_post_processor(conInfo->_postProcessor);
		if (conInfo->_answerString) free(conInfo->_answerString);
	}
	free(conInfo);
	*con_cls = NULL;
}


int RequestManager::requestHandler(void *cls, struct MHD_Connection *connection,
	const char *url,
	const char *method, const char *version,
	const char *upload_data,
	size_t *upload_data_size, void **con_cls)
{
	if (NULL == *con_cls)
	{
		ConnectionInfo *con_info;

		con_info = (ConnectionInfo*)malloc(sizeof(struct ConnectionInfo));
		if (NULL == con_info) return MHD_NO;
		con_info->_answerString = NULL;

		if (0 == strcmp(method, "POST"))
		{
			con_info->_postProcessor =
				MHD_create_post_processor(connection, POSTBUFFERSIZE,
					iteratePost, (void *)con_info);

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

	if (0 == strcmp(method, "GET"))
	{
		return sendPage(connection, _askPage);
	}

	if (0 == strcmp(method, "POST"))
	{
		ConnectionInfo *con_info = (ConnectionInfo*)*con_cls;

		if (*upload_data_size != 0)
		{
			MHD_post_process(con_info->_postProcessor, upload_data,
				*upload_data_size);
			*upload_data_size = 0;

			return MHD_YES;
		}
		else if (NULL != con_info->_answerString)
			return sendPage(connection, con_info->_answerString);
	}

	return sendPage(connection, _errorPage);
}

int RequestManager::sendPage(MHD_Connection * connection, const char * page)
{

	struct MHD_Response *response;
	int ret;
	response = MHD_create_response_from_buffer(strlen(page), (void*)page, MHD_RESPMEM_PERSISTENT);
	if (!response) return MHD_NO;

	ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
	MHD_destroy_response(response);
	return ret;
}
