#pragma once
#include <string>
#include <vector>
#include "Prefix.hpp"

using std::string;
using std::vector;

class Message
{
public:
	Message(const string line);
	~Message();

	const Prefix			&getPrefix() const;
	const string			&getCommand() const;
	const vector<string>	&getParams() const;

	/**
	 * @brief Checks the nickname is a valid nickname.
	 * @throws ParseException
	*/
	static void				validateNickname(const string &nickname);

	static vector<string> splitCommas(const string &str);

private:
	void			parsePrefix();
	void			parseCommand();
	void			parseParams();
	void			skipSpaces();
	string			parseNick();
	string			parseUser();
	string			parseHost();
	string			parseParam();

	Prefix			prefix;
	string			command;
	string			line;
	vector<string>	params;

	size_t				i;
};