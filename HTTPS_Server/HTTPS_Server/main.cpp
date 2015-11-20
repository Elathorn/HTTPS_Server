#include "Server.h"
#include "ProjectDefines.h"
int main()
{
	Server *server = new Server(8080);
	server->start();
}