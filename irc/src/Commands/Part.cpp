#include "Client.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Log.hpp"
#include "Exceptions/IrcException.hpp"
#include "Exceptions/ParseException.hpp"
#include "Commands.hpp"

void Command::Part(Client &client, Server &server, Message &message)
{
	const vector<string> &params = message.getParams();

  string reason = "Leave from channel";

	if (params.size() < 1)
		throw IrcException(ERR_NEEDMOREPARAMS, ERR_NEEDMOREPARAMS_MSG);

  if (params.size() >= 2)
    reason = params[1];

	vector<string> targets = Message::splitCommas(params[0]);
	
	for (size_t i = 0; i < targets.size(); i++)
	{
		if (targets[i].size() < 2 || targets[i][0] != '#')
			throw IrcException(ERR_NOSUCHNICK, ERR_NOSUCHNICK_MSG);
    
		string channelName = string(targets[i].c_str() + 1);
		Channel *channel = server.getChannel(channelName);

		if (!channel)
			throw IrcException(ERR_NOSUCHNICK, ERR_NOSUCHNICK_MSG);

		if (channel->getMember(client.nickname) == NULL)
			throw IrcException(ERR_NOTONCHANNEL, "Not joined to channel " + channelName);
		
		channel->sendRaw(client.createPrefix().toString() +
				" PART #" +
				channelName +
				" :" +
				reason);
		channel->removeMember(client.nickname);
	}
}