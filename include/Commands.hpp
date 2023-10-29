#pragma once
#include "Client.hpp"
#include "Server.hpp"
#include "Message.hpp"

class Command
{
public:
  static void Nick(Client &client, Server &server, Message &message);
  static void Pass(Client &client, Server &server, Message &message);
  static void User(Client &client, Server &server, Message &message);
  static void Privmsg(Client &client, Server &server, Message &message);
  static void Join(Client &client, Server &server, Message &message);
  static void Cap(Client &client, Server &server, Message &message);
  static void Ping(Client &client, Server &server, Message &message);
  static void Notice(Client &client, Server &server, Message &message);
  static void Quit(Client &client, Server &server, Message &message);
  static void Mode(Client &client, Server &server, Message &message);
  static void Kick(Client &client, Server &server, Message &message);
  static void Part(Client &client, Server &server, Message &message);
  static void Who(Client &client, Server &server, Message &message);
};
