#include <iostream>
#include "SHA_512.h"

using namespace std;

int main(int argc, char* argv[])
{
	cout << CONDITIONAL(10, 11, 12) << endl;

	SHA_512 sha512;
	cout << sha512.conditional(10, 11, 12) << endl;
}