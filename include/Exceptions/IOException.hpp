#pragma once
#include "BaseException.hpp"

class IOException : public BaseException
{
public:
	IOException(string message = string("fatal"));
};
