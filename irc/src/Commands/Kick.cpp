#include "Client.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Log.hpp"
#include "Exceptions/IrcException.hpp"
#include "Exceptions/ParseException.hpp"
#include "Commands.hpp"

void Command::Kick(Client &client, Server &server, Message &message)
{
	const vector<string> &params = message.getParams();

	if (params.size() < 2)
		throw IrcException(ERR_NEEDMOREPARAMS, ERR_NEEDMOREPARAMS_MSG);

	if (params[0].size() < 2 || params[0][0] != '#')
		throw IrcException(ERR_NOSUCHCHANNEL, ERR_NOSUCHNICK_MSG);

	string channelName = string(params[0].c_str() + 1);
	Channel *ch = server.getChannel(channelName);

	if (ch == NULL)
		throw IrcException(ERR_NOSUCHCHANNEL, ERR_NOSUCHNICK_MSG);

	if (ch->getMember(client.nickname) == NULL)
		throw IrcException(ERR_NOTONCHANNEL, "You're not on that channel");

	if (!ch->isOperator(client.nickname))
		throw IrcException(ERR_CHANOPRIVISNEED, "You're not channel operator");

	vector<string> users = Message::splitCommas(params[1]);
	for (size_t i = 0; i < users.size(); i++)
	{
		if (ch->getMember(users[i]) == NULL)
			throw IrcException(ERR_USERNOTINCHANNEL, "User " + users[i] + " is not in channel " + channelName);
		ch->kickMember(users[i], client);
	}
}