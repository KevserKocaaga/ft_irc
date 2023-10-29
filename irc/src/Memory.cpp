#include "Memory.hpp"

void *Memory::memset(void *a, int c, size_t len)
{
	unsigned char *p = (unsigned char *)a;
	for (size_t i = 0; i < len; i++)
		*p = c;
	return (a);
}