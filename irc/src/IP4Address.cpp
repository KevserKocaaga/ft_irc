#include <stdexcept>
#include <arpa/inet.h>
#include <sstream>
#include "IP4Address.hpp"


IP4Address::IP4Address(struct sockaddr_in addr)
{
	this->addr = addr;
}

IP4Address::IP4Address(int port)
{
	this->construct(port);
}

IP4Address::IP4Address(string addr, int port)
{
	this->construct(port);
	this->addr.sin_addr.s_addr = inet_addr(addr.c_str());
}

IP4Address::~IP4Address()
{ }

void IP4Address::construct(int port)
{
	this->addr.sin_family = AF_INET;
	this->addr.sin_addr.s_addr = INADDR_ANY;
	this->addr.sin_port = htons(port);
}

int IP4Address::getPort() const
{
	return ((ntohs(this->addr.sin_port)));
}

const struct sockaddr_in &IP4Address::getAddr() const
{
	return (this->addr);
}

string IP4Address::toString(bool includePort) const
{
	string addressStr = string(inet_ntoa(this->addr.sin_addr));

	if (!includePort)
		return addressStr;

	std::stringstream port;
	port << this->getPort();
	return (addressStr + string(":") + port.str());
}
