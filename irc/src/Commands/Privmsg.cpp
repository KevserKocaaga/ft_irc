#include "Client.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Log.hpp"
#include "Exceptions/IrcException.hpp"
#include "Exceptions/ParseException.hpp"
#include "Commands.hpp"

void Command::Privmsg(Client &client, Server &server, Message &message)
{
	const vector<string> params = message.getParams();

	if (params.size() < 1)
		throw IrcException(ERR_NORECIPIENT, "No recipient given (PRIVMSG)");

	if (params.size() < 2)
		throw IrcException(ERR_NOTEXTTOSEND, "No text to send");

	const string &targets_str = params[0];
	const string &message_str = params[1];

	vector<string> targets = Message::splitCommas(targets_str);

	for (size_t i = 0; i < targets.size(); i++)
	{
		if (targets[i].empty())
			throw IrcException(ERR_NOSUCHNICK, ERR_NOSUCHNICK_MSG);
		
		if (targets[i].size() > 1 && targets[i][0] == '#')
		{
			string channelName = string(targets[i].c_str() + 1);
			Channel *channel = server.getChannel(channelName);

			if (!channel)
				throw IrcException(ERR_NOSUCHNICK, ERR_NOSUCHNICK_MSG);

			if (channel->getMember(client.nickname) == NULL)
				throw IrcException(ERR_CANNOTSENDTOCHAN, "Cannot send to nick/channel");
			
			channel->sendRaw(client.createPrefix().toString() +
							" PRIVMSG #" +
							channelName +
							" :" +
							message_str, &client);
		}
		else
		{
			Client *targetClient = server.getClient(targets[i]);
			if (targetClient == NULL)
				throw IrcException(ERR_NOSUCHNICK, ERR_NOSUCHNICK_MSG);
			targetClient->sendRaw(client.createPrefix().toString() +
									" PRIVMSG " +
									targets[i] +
									" :" +
									message_str);
		}
	}
}