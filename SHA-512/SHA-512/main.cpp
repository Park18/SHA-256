#include <iostream>
#include <bitset>
#include "SHA_512.h"

using namespace std;

#define ROTR(value, shift) _rotr64(value, shift)			// �����̼� ��ũ��
#define SIGMA_0(x) (ROTR(x, 1) ^ ROTR(x, 1) ^ (x >> 7))		// �ñ׸�0 ��ũ��

int main(int argc, char* argv[])
{

}