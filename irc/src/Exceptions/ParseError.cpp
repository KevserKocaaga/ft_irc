#include "Exceptions/ParseException.hpp"

ParseException::ParseException(string message)
	: BaseException(string("Parsing error: ") + string(message))
{  }