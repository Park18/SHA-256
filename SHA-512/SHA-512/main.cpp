#include <iostream>
#include <bitset>
#include "SHA_512.h"

using namespace std;

int main(int argc, char* argv[])
{
	uint8_t num = 128;
	cout << bitset<8>(num) << endl;
}