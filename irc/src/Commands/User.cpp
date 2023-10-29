#include "Client.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Log.hpp"
#include "Exceptions/IrcException.hpp"
#include "Exceptions/ParseException.hpp"
#include "Commands.hpp"

void Command::User(Client &client, Server &server, Message &message)
{
	(void)server;

	if (client.isRegistered)
		throw IrcException(ERR_ALREADYREGISTRED, ERR_ALREADYREGISTRED_MSG);

	if (message.getParams().size() < 4)
		throw IrcException(ERR_NEEDMOREPARAMS, ERR_NEEDMOREPARAMS_MSG);

	client.username = message.getParams()[0];
	client.realname = message.getParams()[3];
	
	client.checkForRegistration();
}