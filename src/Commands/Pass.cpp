#include "Client.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Log.hpp"
#include "Exceptions/IrcException.hpp"
#include "Exceptions/ParseException.hpp"
#include "Commands.hpp"

void Command::Pass(Client &client, Server &server, Message &message)
{
	(void)server;
	
	if (message.getParams().size() < 1)
		throw IrcException(ERR_NEEDMOREPARAMS, ERR_NEEDMOREPARAMS_MSG);
	if (client.isRegistered)
		throw IrcException(ERR_ALREADYREGISTRED, ERR_ALREADYREGISTRED_MSG);
	if (server.password != message.getParams()[0])
	{
		log_log("User " + client.nickname + " entered incorrect password: " + message.getParams()[0]);
		throw IrcException(ERR_PASSWDMISSMATCH, "Password incorrect");
	}
	client.isEnteredPassword = true;
	log_log("User " + client.nickname + " entered correct password!");
}