#include "Server.h"



Server::Server(int port)
{
	_port = port;
	_daemon = NULL;
}


Server::~Server()
{
	MHD_stop_daemon(_daemon);
}

int Server::start()
{
	_daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, _port, NULL, NULL, &_requestHandler, NULL, MHD_OPTION_END);
	//inicjalizujemy daemona i oczekujemy na requesty
	if (NULL == _daemon) //jesli nie udalo sie zainicjalizowac daemona
	{
		std::cerr << "Blad inicjalizacji daemona." << std::endl;
		return _ERROR;
	}
	while (true)
	{
		//sleep(10000); //todo: sprawdzic czy jest to potrzebne
	}
	//todo: sprawdzic czy to potrzebne
	MHD_stop_daemon(_daemon);
}

int Server::_requestHandler(void *cls, struct MHD_Connection *connection,
	const char *url,
	const char *method, const char *version,
	const char *upload_data,
	size_t *upload_data_size, void **con_cls)
{
	struct MHD_Response *response;
	int ret;
	const char *page = "<html><body>Hello!</body></html>";
	response = MHD_create_response_from_buffer(strlen(page), (void*)page, MHD_RESPMEM_PERSISTENT);

	ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
	MHD_destroy_response(response);
	return ret;



}
