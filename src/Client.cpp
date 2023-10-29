#include <unistd.h>
#include <sstream>
#include <fcntl.h>
#include <cstring>
#include <map>
#include "Exceptions/ParseException.hpp"
#include "Exceptions/IrcException.hpp"
#include "IO.hpp"
#include "Log.hpp"
#include "Message.hpp"
#include "Client.hpp"
#include "getCommandHandler.hpp"

using std::map;

Client::Client(int fd, IP4Address address, Server &at)
		: fd(fd), address(address), server(at), isRegistered(false), isEnteredPassword(false), status(Connected)
{
	log_log("Client connected from " << address.toString());

	struct sockaddr_in hostnameAddr;
	socklen_t addrlen = sizeof(hostnameAddr);
	getsockname(fd, (struct sockaddr *)&hostnameAddr, &addrlen);

	this->hostname = IP4Address(hostnameAddr);
}

Client::~Client()
{
	close(this->fd);
	log_log(this->identifier() << " destroyed");
}

void Client::parseBuffer()
{
	size_t pos = 0;
	string line;
	while ((pos = this->read_buffer.find("\n")) != string::npos)
	{
		bool crlf = pos > 0 && this->read_buffer[pos - 1] == '\r';

		line = this->read_buffer.substr(0, pos - (crlf ? 1 : 0));
		this->read_buffer.erase(0, pos + 1);

		if (line.empty())
			continue ;

		log_log(this->identifier() << ": >" << line << "<");

		try
		{
			Message m(line);

			if (!this->isEnteredPassword && m.getCommand() != "PASS")
				throw IrcException(ERR_UNKNOWNERROR, "You first have to enter the password with PASS <password>");

			CommandHandler handler = getCommandHandler(m.getCommand());
			handler(*this, this->server, m);
		}
		catch (ParseException &ex)
		{
			log_error(ex.what());
		}
		catch (IrcException &ex)
		{
			log_error(ex.what());

			int code = ex.code;
			std::stringstream ss;
			ss << code;
			string codeStr = ss.str();

			this->sendRaw(codeStr + " :" + ex.message);
		}
	}
}

int Client::getFd() const
{
	return (this->fd);
}

void Client::sendRaw(string data)
{
	this->write_buffer += data;
	this->write_buffer += "\r\n";
}

void Client::disconnect()
{
	this->status = Disconnected;
	log_log(this->identifier() << " marked as disconnected");
}

void Client::handle()
{
	if (!this->write_buffer.empty() && IO::isFdWritable(this->getFd()))
	{
		ssize_t sentByte = send(this->fd, write_buffer.c_str(), write_buffer.length(), 0);
		write_buffer.erase(0, sentByte);
	}

	char buffer[1024];
	ssize_t readBytes = 0;
	string totalData;

	while (IO::isFdReadable(this->getFd()))
	{
		readBytes = recv(this->getFd(), buffer, sizeof(buffer) - 1, 0);
		if (readBytes <= 0)
		{
			this->disconnect();
			return;
		}
		buffer[readBytes] = 0;
		totalData += buffer;
	}

	if (totalData.length() > 0)
	{
		this->read_buffer += totalData;
		this->parseBuffer();
	}
}

void Client::checkForRegistration()
{
	if (isRegistered)
		return;
	if (!this->nickname.empty() && !this->username.empty())
	{
		string prefix = this->createPrefix().toString();
		this->sendRaw(prefix + " 001 " + this->nickname + " :Welcome to the ft_irc network.");
		this->sendRaw(prefix + " 002 " + this->nickname + " :Your host is " + this->hostname.toString(false) + ", running version " VERSION);
		this->sendRaw(prefix + " 003 " + this->nickname + " :This server was created " __DATE__ " " __TIME__);

		this->isRegistered = true;
	}
}

const IP4Address &Client::getAddress() const
{
	return (this->address);
}

Prefix Client::createPrefix() const
{
	return (Prefix(this->nickname, this->username, this->hostname.toString(false)));
}

const string &Client::getNickname() const
{
	return (this->nickname);
}

ConnectionStatus Client::getStatus() const
{
	return (this->status);
}

const string Client::identifier() const
{
	string formatted = "";

	if (this->isRegistered)
		formatted = this->createPrefix().toString();
	else
		formatted = this->getAddress().toString();
	return (formatted);
}