#pragma once
#include <string>

using std::string;

class Prefix
{
public:
	Prefix();
	Prefix(const string nick, const string user, const string host);
	~Prefix();

	const string	getNick() const;
	const string	getUser() const;
	const string	getHost() const;
	bool			isEmpty() const;

	const string	toString() const;

private:
	string	nick;
	string	user;
	string	host;
};