#include <unistd.h>
#include <sys/poll.h>
#include <fcntl.h>
#include "Exceptions/SocketException.hpp"
#include "Exceptions/IOException.hpp"
#include "Server.hpp"
#include "Memory.hpp"
#include "Log.hpp"
#include "IO.hpp"

Server::Server(int port, string password)
{
	this->password = password;
	this->address = IP4Address(port);

	if ((this->fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw(SocketException());

	int opt = 1;
	if (setsockopt(this->fd,
					SOL_SOCKET,
					SO_REUSEADDR,
					&opt,
					sizeof(opt)) != 0)
	{
		close(this->fd);
		throw(SocketException("unable to set socket option"));
	}
	if (setsockopt(this->fd,
					SOL_SOCKET,
					SO_REUSEPORT,
					&opt,
					sizeof(opt)) != 0)
	{
		close(this->fd);
		throw(SocketException("unable to set socket option"));
	}

	if (bind(this->fd,
					 (struct sockaddr *)&this->address.getAddr(),
					 sizeof(this->address.getAddr())) < 0)
	{
		close(this->fd);
		throw(SocketException("unable to bind"));
	}

	if (listen(this->fd, MAX_CLIENT) < 0)
	{
		close(this->fd);
		throw(SocketException("unable to listen"));
	}
}

Server::~Server()
{
	for (vector<Client *>::iterator it = this->clients.begin(); it != this->clients.end();)
		it = this->removeClient(*it);

	for (vector<Channel *>::iterator it = this->channels.begin(); it != this->channels.end(); it++)
		delete (*it);

	if (this->fd > 0)
		close(this->fd);
}

void Server::start()
{
	this->isRunning = true;
	log_log("Server started at " << this->address.toString());
	while (this->isRunning)
	{
		this->handleIncomingConnections();
		this->handleClients();
	}
}

void Server::stop()
{
	this->isRunning = false;
	log_log("Server stopped");
}

void Server::handleClients()
{
	for (vector<Client *>::iterator it = this->clients.begin(); this->isRunning && it != this->clients.end(); )
	{
		if ((*it)->getStatus() == Disconnected)
		{
			it = this->removeClient(*it);
		}
		else
		{
			(*it)->handle();
			it++;
		}
	}
}

void Server::handleIncomingConnections()
{
	if (this->isRunning && IO::isFdReadable(this->fd))
	{
		struct sockaddr_in clientAddr;
		socklen_t addrlen = sizeof(clientAddr);
		int fd = accept(this->fd, (struct sockaddr *)&clientAddr, &addrlen);

		this->clients.push_back(new Client(fd, IP4Address(clientAddr), *this));
	}
}

const string Server::getHostname() const
{
	return (this->address.toString(false));
}

Channel *Server::getChannel(const string &channelName)
{
	vector<Channel *>::iterator it;

	for (it = this->channels.begin(); it != this->channels.end(); it++)
		if ((*it)->getName() == channelName)
			return (*it);
	return (NULL);
}

Channel *Server::createChannel(const string &channelName)
{
	Channel *channel = new Channel(channelName);
	this->channels.push_back(channel);
	return (channel);
}

Client *Server::getClient(const string &nickname)
{
	for (vector<Client *>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		if ((*it)->getNickname() == nickname)
			return (*it);
	}
	return NULL;
}

vector<Client *>::iterator Server::removeClient(Client *client)
{
	for (vector<Channel *>::iterator it = this->channels.begin(); it != this->channels.end(); it++)
		(*it)->quitMember(client->getNickname());

	for (vector<Client *>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		if ((*it) == client)
		{
			delete (*it);
			return (this->clients.erase(it));
		}
	}

	return (this->clients.end());
}