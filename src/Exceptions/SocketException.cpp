#include "Exceptions/SocketException.hpp"

SocketException::SocketException(string message)
	: BaseException(string("Socket error: ") + string(message))
{  }