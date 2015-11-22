#include "Server.h"
#include "ProjectDefines.h"
int main()
{
	Server *server = new Server(443);
	server->start();
	while (true);
}