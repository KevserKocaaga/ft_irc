#include "Client.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Log.hpp"
#include "Exceptions/IrcException.hpp"
#include "Exceptions/ParseException.hpp"
#include "Commands.hpp"

void Command::Notice(Client &client, Server &server, Message &message)
{
	const vector<string> params = message.getParams();

	if (params.size() < 2)
		return ;

	const string &targets_str = params[0];
	const string &message_str = params[1];

	vector<string> targets = Message::splitCommas(targets_str);

	for (size_t i = 0; i < targets.size(); i++)
	{
		if (targets[i].empty())
			return ;
		
		if (targets[i].size() > 1 && targets[i][0] == '#')
		{
			string channelName = string(targets[i].c_str() + 1);
			Channel *channel = server.getChannel(channelName);

			if (!channel)
				return ;

			string prefix = client.createPrefix().toString();
			channel->sendRaw(prefix + " NOTICE #" + channelName + " :" + message_str, &client);
		}
		else
		{
			Client *targetClient = server.getClient(targets[i]);
			if (targetClient == NULL)
				return ;
			string prefix = client.createPrefix().toString();
			targetClient->sendRaw(prefix + " NOTICE " + targets[i] + " :" + message_str);
		}
	}
}