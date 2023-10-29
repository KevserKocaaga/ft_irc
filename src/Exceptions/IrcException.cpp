#include "Exceptions/IrcException.hpp"

IrcException::IrcException(NumericCode code, string message, string param)
	: std::runtime_error(string("IrcException: ") + param + string(" :") + message), code(code), message(message), param(param)
{ }

IrcException::~IrcException() throw()
{

}