#include <iostream>
#include <bitset>
#include "SHA_256.h"

using namespace std;

int main(int argc, char* argv[])
{
	SHA_256 sha_256;
	sha_256.sha_256();

	uint32_t* hash = sha_256.get_ptr_hash();
	for (int index = 0; index < 8; index++)
		cout << std::hex << hash[index] << " ";

}