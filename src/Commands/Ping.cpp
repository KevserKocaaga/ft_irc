#include "Client.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Log.hpp"
#include "Exceptions/IrcException.hpp"
#include "Commands.hpp"

void Command::Ping(Client &client, Server &server, Message &message)
{
	(void)server;

	string token;

	if (message.getParams().size() >= 1)
		token = message.getParams()[0];

	client.sendRaw(client.createPrefix().toString() + " PONG ft_irc " + token);
}