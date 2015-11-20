#pragma once
#include "ProjectDefines.h"
#include <iostream>
#include <Windows.h>
class Server
{
public:
	Server(int port);
	~Server();
	int start(); //rozpoczyna dzialanie serwera
private:
	int _port;
	struct MHD_Daemon * _daemon;

	static int _requestHandler(void *cls, struct MHD_Connection *connection,
		const char *url,
		const char *method, const char *version,
		const char *upload_data,
		size_t *upload_data_size, void **con_cls);
public:
	//magic numbers
	const static int _ERROR = 1;
	const static int _NO_ERROR = 0;
};

