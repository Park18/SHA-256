#include <iostream>
#include <bitset>
#include "SHA_512.h"

using namespace std;

#define ROTR(value, shift) _rotr64(value, shift)			// 로테이션 매크로
#define SIGMA_0(x) (ROTR(x, 1) ^ ROTR(x, 1) ^ (x >> 7))		// 시그마0 매크로

int main(int argc, char* argv[])
{

}