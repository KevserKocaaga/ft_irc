#include "Client.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Log.hpp"
#include "Exceptions/IrcException.hpp"
#include "Exceptions/ParseException.hpp"
#include "Commands.hpp"

void Command::Cap(Client &client, Server &server, Message &message)
{
	(void)message;
	(void)server;
	
	client.sendRaw(client.createPrefix().toString() + " CAP * LS :");
}