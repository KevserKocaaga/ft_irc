#include <map>
#include "Exceptions/IrcException.hpp"
#include "getCommandHandler.hpp"
#include "Commands.hpp"

using std::map;

static map<string, CommandHandler> commandHandlers;
static bool mapInitiated = false;

void initMap()
{
  commandHandlers["NICK"] = Command::Nick;
  commandHandlers["PASS"] = Command::Pass;
  commandHandlers["USER"] = Command::User;
  commandHandlers["PRIVMSG"] = Command::Privmsg;
  commandHandlers["JOIN"] = Command::Join;
  commandHandlers["CAP"] = Command::Cap;
  commandHandlers["PING"] = Command::Ping;
  commandHandlers["NOTICE"] = Command::Notice;
  commandHandlers["QUIT"] = Command::Quit;
  commandHandlers["MODE"] = Command::Mode;
  commandHandlers["KICK"] = Command::Kick;
  commandHandlers["PART"] = Command::Part;
  commandHandlers["WHO"] = Command::Who;
  mapInitiated = true;
}

CommandHandler getCommandHandler(const string &command)
{
	if (!mapInitiated)
		initMap();
	string commandName = "";
	for (size_t i = 0; i < command.size(); i++)
		commandName += toupper(command.at(i));

	map<string, CommandHandler>::iterator pos = commandHandlers.find(commandName);
	if (pos == commandHandlers.end())
		throw IrcException(ERR_UNKNOWNCOMMAND, ERR_UNKNOWNCOMMAND_MSG + string(" ") + commandName);
	return pos->second;
}