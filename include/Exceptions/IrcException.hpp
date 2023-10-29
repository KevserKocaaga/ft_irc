#pragma once
#include <stdexcept>
#include <exception>
#include "BaseException.hpp"
#include "NumericCodes.hpp"
#include "Prefix.hpp"

class IrcException : public BaseException
{
public:
	IrcException(NumericCode code, string message, string param = "");
	~IrcException() throw();

	NumericCode	code;
	string		message;
	string		param;
};
