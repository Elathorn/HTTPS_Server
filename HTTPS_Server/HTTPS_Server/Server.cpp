#include "Server.h"



Server::Server(int port)
{
	_port = port;
	_daemon = NULL;
}


Server::~Server()
{
	MHD_stop_daemon(_daemon);
	if (_keyPem) free(_keyPem);
	if (_certPem) free(_certPem);
}

int Server::start()
{

//* ssh	
	_keyPem = _loadFile(SERVERKEYFILE); //wczytujemy klucz i certyfikat
	_certPem = _loadFile(SERVERCERTFILE);

	if ((_keyPem == NULL) || (_certPem == NULL))
	{
		printf("The key/certificate files could not be read.\n");
		return 1;
	}
	//*/
	_daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY //| MHD_USE_SSL
		,_port, NULL, NULL, 
		&RequestManager::requestHandler, NULL, 
		//ssh //
		//MHD_OPTION_HTTPS_MEM_KEY, _keyPem, MHD_OPTION_HTTPS_MEM_CERT, _certPem, 
		MHD_OPTION_NOTIFY_COMPLETED, &RequestManager::requestCompleted, NULL, 
		MHD_OPTION_END); 
	/*ssh
	
	_daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, _port, NULL, NULL, &RequestManager::requestHandler, NULL, 
		MHD_OPTION_HTTPS_MEM_KEY, _keyPem, MHD_OPTION_HTTPS_MEM_CERT, _certPem, 
		MHD_OPTION_NOTIFY_COMPLETED, &RequestManager::requestCompleted, NULL, MHD_OPTION_END); */
	//inicjalizujemy daemona i oczekujemy na requesty
	if (NULL == _daemon) //jesli nie udalo sie zainicjalizowac daemona
	{
	//ssh	
		free(_keyPem);
	//ssh	
		free(_certPem);
		std::cerr << "Blad inicjalizacji daemona." << std::endl;
		return _ERROR;
	}
}

long Server::_getFileSize(const char *filename)
{
	FILE *fp;

	fp = fopen(filename, "r");
	if (fp)
	{
		long size;

		if ((0 != fseek(fp, 0, SEEK_END)) || (-1 == (size = ftell(fp))))
			size = 0;

		fclose(fp);

		return size;
	}
	else
		return 0;
}

char * Server::_loadFile(const char * filename)
{
//*C
	FILE *fp;
	char *buffer;
	long size;

	size = _getFileSize(filename);
	if (size == 0)
		return NULL;

	fp = fopen(filename, "rb");
	if (!fp)
		return NULL;

	buffer = (char*) malloc(size);
	if (!buffer)
	{
		fclose(fp);
		return NULL;
	}

	if (size != fread(buffer, 1, size, fp))
	{
		free(buffer);
		buffer = NULL;
	}

	fclose(fp);
	return buffer;
	//*/
/*c++
	ifstream in(filename);
	string tmpStr;
	string finStr = "";
	while (in >> tmpStr)
		finStr += tmpStr;
	char* cFinStr = new char[finStr.length() + 1];
	strcpy(cFinStr, finStr.c_str());
	return cFinStr;
	*/
}