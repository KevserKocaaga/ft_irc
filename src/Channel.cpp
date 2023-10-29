#include "Channel.hpp"

Channel::Channel(const string name) : name(name)
{ }

Channel::~Channel()
{ }

const string Channel::toString() const
{
	return (string("#") + this->name);
}

const string Channel::getName() const
{
	return (this->name);
}

void Channel::addMember(Client *client)
{
	if (getMember(client->nickname))
		return;
	this->members.push_back(client);
}

bool Channel::isOperator(const string nickname) const
{
	vector<Client *>::const_iterator it;

	for (it = this->operators.begin(); it != this->operators.end(); it++)
		if ((*it)->nickname == nickname)
			return (true);
	return (false);
}

void Channel::removeMember(const string nickname)
{
	this->removeFromVector(this->members, nickname);
	this->removeFromVector(this->operators, nickname);
}

Client *Channel::getMember(const string nickname)
{
	return (this->getClientFromVector(this->members, nickname));
}

void Channel::giveOperator(const string nickname)
{
	Client *client = this->getMember(nickname);

	// If client in operators return
	if (!client || this->isOperator(nickname))
		return;

	this->operators.push_back(client);
}

void Channel::takeOperator(const string nickname)
{
	this->removeFromVector(this->operators, nickname);
}

void Channel::kickMember(const string nickname, Client &by)
{
	Client *client = this->getMember(nickname);
	this->removeFromChannel(*client, by.createPrefix().toString() + " KICK #" + this->name + " " + nickname);
}

void Channel::quitMember(const string nickname)
{
	Client *client = this->getMember(nickname);
	if (!client)
		return ;
	this->removeFromChannel(*client, client->createPrefix().toString() + " QUIT " + nickname + " :Quit");
}

void Channel::removeFromChannel(const Client &client, const string message)
{
	this->sendRaw(message);
	this->removeMember(client.nickname);
}

Client *Channel::getClientFromVector(vector<Client *> &vec, const string nickname)
{
	vector<Client *>::iterator it;

	for (it = vec.begin(); it != vec.end(); it++)
		if ((*it)->nickname == nickname)
			return (*it);
	return (NULL);
}

void Channel::removeFromVector(vector<Client *> &vec, const string nickname)
{
	vector<Client *>::iterator it;

	for (it = vec.begin(); it != vec.end(); it++)
	{
		if ((*it)->nickname == nickname)
		{
			vec.erase(it);
			return;
		}
	}
}

void Channel::sendRaw(const string data)
{
	Channel::sendRaw(data, NULL);
}

void Channel::sendRaw(const string data, const Client *except)
{
	vector<Client *>::iterator it;

	for (it = this->members.begin(); it != this->members.end(); it++)
	{
		if (except != NULL && (*it) == except)
			continue;
		(*it)->sendRaw(data);
	}
}

const string Channel::getNameList() const
{
	string nameList = "";

	for (vector<Client *>::const_iterator it = this->members.begin(); it != this->members.end(); it++)
	{
		if (this->isOperator((*it)->nickname))
			nameList += "@";
		nameList += (*it)->nickname;

		if (it != this->members.end() - 1)
			nameList += " ";
	}
	return (nameList);
}