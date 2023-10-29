#include "Client.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Log.hpp"
#include "Exceptions/IrcException.hpp"
#include "Exceptions/ParseException.hpp"
#include "Commands.hpp"

void Command::Nick(Client &client, Server &server, Message &message)
{
	if (message.getParams().size() < 1)
		throw IrcException(ERR_NONICKNAMEGIVEN, "No nickname given");

	const string &nick = message.getParams()[0];

	try
	{
		Message::validateNickname(nick);
	}
	catch (ParseException &e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
		throw IrcException(ERR_ERRONEUSNICKNAME, e.what(), nick);
	}

	for (size_t i = 0; i < server.clients.size(); i++)
	{
		if (server.clients[i] == &client)
			continue;
		if (server.clients[i]->nickname == nick)
			throw IrcException(ERR_NICKNAMEINUSE, "Nickname is already in use", nick);
	}

	for (size_t i = 0; i < server.clients.size(); i++)
	{
		if (server.clients[i] == &client)
			continue;
		server.clients[i]->sendRaw(client.createPrefix().toString() + " NICK " + nick);
	}
	
	client.nickname = nick;
	client.checkForRegistration();
}