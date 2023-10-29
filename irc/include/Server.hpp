#pragma once
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include <vector>
#include "IP4Address.hpp"
#include "Channel.hpp"
#include "Client.hpp"

#define MAX_CLIENT 10
#define SERVER_NAME "ircserv"
#define VERSION "1.0"

using std::string;
using std::vector;

class Channel;
class Client;

class Server
{
public:
	friend class Command;

	Server(int port, string password);
	~Server();
	void	start();
	void	stop();
	Client *getClient(const string &nickname);
	Channel *getChannel(const string &channelName);
	Channel *createChannel(const string &channelName);
	const string getHostname() const;

private:
	void	handleIncomingConnections();
	void	handleClients();
	vector<Client *>::iterator removeClient(Client *client);

	vector<Client *>	clients;
	vector<Channel *>	channels;
	string				password;
	IP4Address			address;
	bool				isRunning;
	int					fd;

};