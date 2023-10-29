#pragma once

#include <string>
#include <map>
#include "Client.hpp"
#include "Server.hpp"
#include "Message.hpp"

typedef void (*CommandHandler)(Client &client, Server &server, Message &message);

CommandHandler getCommandHandler(const string &command);