#include "Prefix.hpp"

Prefix::Prefix(const string nick, const string user, const string host) : nick(nick), user(user), host(host)
{ }

Prefix::Prefix() : nick(""), user(""), host("")
{ }

Prefix::~Prefix()
{ }

bool Prefix::isEmpty() const
{
	return (
		this->nick == "" &&
		this->user == "" &&
		this->host == ""
	);
}

const string Prefix::getNick() const
{
	return (this->nick);
}

const string Prefix::getUser() const
{
	return (this->user);
}

const string Prefix::getHost() const
{
	return (this->host);
}

const string Prefix::toString() const
{
	string prefix = ":" + this->nick;
	
	if (this->user != "")
		prefix += "!" + this->user;
	if (this->host != "")
		prefix += "@" + this->host;

	return (prefix);
}