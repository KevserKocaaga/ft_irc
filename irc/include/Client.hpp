#pragma once
#include <string>
#include <stack>
#include "Server.hpp"
#include "IP4Address.hpp"
#include "Prefix.hpp"
#include "Channel.hpp"

using std::stack;
using std::string;

class Command;
class Server;
class Channel;

enum ConnectionStatus {
	Connected,
	Disconnected
};

class Client
{
public:
	friend class Command;
	friend class Channel;

	Client(int fd, IP4Address address, Server &at);
	~Client();
	const IP4Address &getAddress() const;
	int getFd() const;
	void handle();
	void sendRaw(string data);
	Prefix createPrefix() const;
	const string &getNickname() const;
	ConnectionStatus getStatus() const;
	void disconnect();
	void checkForRegistration();
	const string identifier() const;

private:
	
	void parseBuffer();

	string				read_buffer;
	string				write_buffer;
	int 				fd;
	IP4Address			address;
	IP4Address			hostname;
	Server				&server;

	bool				isRegistered;
	bool		 	 	isEnteredPassword;
	string				username;
	string				nickname;
	string				realname;
	ConnectionStatus	status;
};