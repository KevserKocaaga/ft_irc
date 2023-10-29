#include "Exceptions/IOException.hpp"

IOException::IOException(string message)
	: BaseException(string("I/O error: ") + string(message))
{  }