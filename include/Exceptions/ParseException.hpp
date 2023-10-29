#pragma once
#include "BaseException.hpp"

class ParseException : public BaseException
{
public:
	ParseException(string message = string("error"));
};
