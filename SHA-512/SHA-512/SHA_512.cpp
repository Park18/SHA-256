#include "SHA_512.h"

using namespace std;

void SHA_512::input()
{
	cin >> this->inputMessage;
	this->messageLength = inputMessage.length();
}

uint64_t SHA_512::majority(uint64_t x, uint64_t y, uint64_t z)
{
	return ((x & y) ^ (y & z) ^ (z & x));
}

uint64_t SHA_512::conditional(uint64_t x, uint64_t y, uint64_t z)
{
	return (x & y) ^ (~x & z);
}
