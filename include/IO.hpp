#pragma once

class IO
{
public:
	static bool isFdWritable(int fd);
	static bool isFdReadable(int fd);

private:
	static bool checkPoll(int fd, short flag);
};