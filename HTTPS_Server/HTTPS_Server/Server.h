#pragma once
#include "RequestManager.h"
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

	char *_keyPem;
	char *_certPem;

	static long _getFileSize(const char *filename);
	static char* _loadFile(const char *filename);

public:
	//magic numbers
	const static int _ERROR = 1;
	const static int _NO_ERROR = 0;
};

