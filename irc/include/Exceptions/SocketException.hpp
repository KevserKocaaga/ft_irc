#pragma once
#include "BaseException.hpp"

class SocketException : public BaseException
{
public:
	SocketException(string message = string("fatal"));
};
