#include "Client.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Log.hpp"
#include "Exceptions/IrcException.hpp"
#include "Exceptions/ParseException.hpp"
#include "Commands.hpp"

void Command::Join(Client &client, Server &server, Message &message)
{
	const vector<string> params = message.getParams();

	if (params.size() < 1)
		throw IrcException(ERR_NEEDMOREPARAMS, ERR_NEEDMOREPARAMS_MSG);

	const string &targets_str = params[0];

	vector<string> targets = Message::splitCommas(targets_str);
	vector<string>::iterator it;

	for (it = targets.begin(); it != targets.end(); it++)
	{
		if ((*it).size() < 2 || (*it)[0] != '#')
			throw IrcException(ERR_NOSUCHCHANNEL, ERR_NOSUCHNICK_MSG);
			
		string channelName = string((*it).c_str() + 1);
		Channel *ch = server.getChannel(channelName);

		if (!ch)
		{
			ch = server.createChannel(channelName);
			ch->addMember(&client);
			ch->giveOperator(client.nickname);
		}
		else
		{
			// Silently ignore if user is already in that channel
			if (ch->getMember(client.nickname))
				continue;
			ch->addMember(&client);
		}

		// Notify channel members, including the client
		ch->sendRaw(client.createPrefix().toString() + " JOIN #" + channelName);

		client.sendRaw(":ft_irc 353 " + client.nickname + " = #" + channelName + " :" + ch->getNameList());
		client.sendRaw(":ft_irc 366 " + client.nickname + " " + channelName + " :End of NAMES list");
	}
}