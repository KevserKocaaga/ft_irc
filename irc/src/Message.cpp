#include <cctype>
#include "Exceptions/ParseException.hpp"
#include "Message.hpp"
#include <iostream>
void print_info(Message &msg);

// Constructors
Message::Message(const string line)
{
	i = 0;
	if (line.length() == 0)
		return ;

	this->line = line;
	this->parsePrefix();
	this->parseCommand();
	this->parseParams();
	(void)print_info;
}

void print_info(Message &msg)
{
	std::cout << "OOOOOOOO" << std::endl;
	std::cout << "host: " << msg.getPrefix().getHost() << std::endl;
	std::cout << "nick: " << msg.getPrefix().getNick() << std::endl;
	std::cout << "user: " << msg.getPrefix().getUser() << std::endl;
	std::cout << "command: " << msg.getCommand() << "<" << std::endl;
	std::cout << "===" << std::endl;
	vector<string> params = msg.getParams();
	for (size_t i = 0; i < params.size(); i++)
		std::cout << params[i] << "<" << std::endl;
	std::cout << "OOOOOOOO" << std::endl;
}

Message::~Message()
{ }

void Message::validateNickname(const string &nickname)
{
	if (nickname == "")
		throw ParseException("Nick name cannot be empty");

	if (nickname.length() > 9)
		throw ParseException("Nick name cannot be longer than 9 character.");
		
	for (size_t i = 0; i < nickname.length(); i++)
		if (!isalpha(nickname[i]) && !isdigit(nickname[i]) && nickname[i] != '_')
			throw ParseException(string("Unallowed character in nickname: ") + nickname[i]);
}

string Message::parseNick()
{
	int start;

	start = i;
	while (isalpha(line[i]) || isdigit(line[i]) || line[i] == '*')
		i++;

	string nick = line.substr(start, i - start);

	Message::validateNickname(nick);
		
	return line.substr(start, i - start);
}

string Message::parseUser()
{
	int start = i;

	while (i < line.size() && line[i] != '@' && isalpha(line[i]))
		i++;

	return line.substr(start, i - start);
}

string Message::parseHost()
{
	int start = i;

	while (isalpha(line[i]) || isdigit(line[i]) || line[i] == '-' || line[i] == '.')
		i++;

	if (i - start > 24)
		throw ParseException("Server name cannot be longer than 24 character");

	return line.substr(start, i - start);
}

void Message::skipSpaces()
{
	while (line[i] == ' ')
		i++;
}

string Message::parseParam()
{
	int start = i;
	
	if (line[i] == ':')
	{
		i = line.size();
		return (line.substr(start + 1));
	}

	while (line[i] != ' ' && i < line.size())
		i++;

	return (line.substr(start, i - start));
}

void Message::parseParams()
{
	size_t mem_i;

	while (i < line.size())
	{
		this->skipSpaces();
		if (i >= line.size())
			return ;
		mem_i = i;
		this->params.push_back(this->parseParam());
		if (mem_i == i)
			throw ParseException("Infinite loop dedected while parsing");
	}
}

void Message::parseCommand()
{
	// TODO: Support for error messages here
	if (i >= line.size())
		throw ParseException("No command found");
	if (!isalpha(line[i]))
		throw ParseException("Command contains non alpha character");

	int start = i;
	while (isalpha(line[i]))
		i++;

	if (i < line.size() && line[i] != ' ')
		throw ParseException("Found non alpha character in command");
	
	this->command = line.substr(start, i - start);
}

void Message::parsePrefix()
{
	if (line[i] == ':')
	{
		i++;
		string nickName = this->parseNick();
		string user = "";
		string host = "";

		if (line[i] == '!')
		{
			i++;
			user = this->parseUser();
		} 

		if (line[i] == '@')
		{
			i++;
			host = this->parseHost();
		}
		this->skipSpaces();

		this->prefix = Prefix(nickName, user, host);
	}
}

// Public methods
const Prefix &Message::getPrefix() const
{
	return (this->prefix);
}

const string &Message::getCommand() const
{
	return (this->command);
}

const vector<string> &Message::getParams() const
{
	return (this->params);
}

vector<string> Message::splitCommas(const string &str)
{
	vector<string> result;
	int start = 0;
	size_t i = 0;

	while (i < str.size())
	{
		if (str[i] == ',')
		{
			result.push_back(str.substr(start, i - start));
			start = i + 1;
		}
		i++;
	}
	result.push_back(str.substr(start, i - start));
	return (result);
}