#include <signal.h>
#include <sstream>
#include "Server.hpp"
#include "Log.hpp"
#include "Exceptions/SocketException.hpp"

Server *server = NULL;

void signalHandler(int num)
{
	if (num == SIGINT && server != NULL)
		server->stop();
}

bool control(char **argv)
{
	int port = 0;

	std::stringstream ss;
	ss << argv[1];
	ss >> port;

	if(port >= 1024 && port <= 65535)
		return true;
	return false;

}

int main(int argc, char **argv)
{
	if (argc != 3 || !control(argv))
	{
		log_error("Usage: " << argv[0] << " <port> <password>");
		return 1;
	}

	int port = 0;
	string password = argv[2];

	std::stringstream ss;
	ss << argv[1];
	ss >> port;

	signal(SIGINT, signalHandler);
	try
	{
		server = new Server(port, password);
		server->start();
	}
	catch (SocketException &ex)
	{ log_error(ex.what()); }
	
	delete server;
	return (0);
}