#pragma once
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>

using std::string;

class IP4Address
{
public:

	IP4Address(int port = 0);
	IP4Address(struct sockaddr_in addr);
	IP4Address(string addr, int port);
	~IP4Address();
	int							getPort() const;
	string						toString(bool includePort = true) const;
	const struct sockaddr_in	&getAddr() const;

private:
	void				construct(int port);

	struct sockaddr_in	addr;
};