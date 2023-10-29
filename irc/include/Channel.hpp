#pragma once
#include <string>
#include <vector>
#include "Client.hpp"

using std::string;
using std::vector;

class Client;

class Channel
{
public:
	friend class Command;

	Channel(const string name);
	~Channel();
	const string toString() const;
	const string getName() const;
	Client	*getMember(const string nickname);
	void addMember(Client *client);
	void removeMember(const string nickname);
	void kickMember(const string nickname, Client &by);
	void giveOperator(const string nickname);
	void takeOperator(const string nickname);
	bool isOperator(const string nickname) const;
	void sendRaw(const string data, const Client *expect);
	void sendRaw(const string data);
	void quitMember(const string nickname);
	const string getNameList() const;

private:
	void	removeFromVector(vector<Client *> &vec, const string nickname);
	Client	*getClientFromVector(vector<Client *> &vec, const string nickname);
	void removeFromChannel(const Client &client, const string message);

	vector<Client *>	members;
	vector<Client *>	operators;
	string				name;
};