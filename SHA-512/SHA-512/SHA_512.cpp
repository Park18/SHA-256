#include "SHA_512.h"

using namespace std;

void SHA_512::input()
{
	cin >> this->inputMessage;
	this->messageLength = inputMessage.length();
}
