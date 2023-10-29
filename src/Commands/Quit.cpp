#include "Client.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Log.hpp"
#include "Exceptions/IrcException.hpp"
#include "Exceptions/ParseException.hpp"
#include "Commands.hpp"

void Command::Quit(Client &client, Server &server, Message &message)
{
	(void)server;
	(void)message;
	client.disconnect();
}