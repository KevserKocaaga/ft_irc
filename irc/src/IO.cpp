#include <errno.h>
#include "Log.hpp"
#include "Exceptions/IOException.hpp"
#include "IO.hpp"
#include "Memory.hpp"
#include <unistd.h>
#include <sys/poll.h>
#include <fcntl.h>

bool IO::checkPoll(int fd, short flag)
{
	struct pollfd pfd[1];

	Memory::memset((void *)pfd, 0, sizeof(pfd));

	pfd[0].fd = fd;
	pfd[0].events = flag;
	int pollResult = poll(pfd, 1, 100);

	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
	{
 	 	close(fd);
	 	throw(IOException("fcntl"));
	}

	if (pollResult == -1)
		return (false);
	return ((pfd[0].revents & flag) != 0);
}

bool IO::isFdReadable(int fd)
{
	return IO::checkPoll(fd, POLLIN);
}

bool IO::isFdWritable(int fd)
{
	return IO::checkPoll(fd, POLLOUT);
}