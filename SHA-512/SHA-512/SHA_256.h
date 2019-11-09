#pragma once
/**
 * @file SHA_512.h
 * @brief SHA-2의 512를 구현하는 헤더
 * @detail 512에서 256으로 변경
 */
#ifndef SHA_256_H
#define SHA_256_H

#include <iostream>
#include <string>
#include <algorithm>
#include <math.h>
#include "Wt.h"

#define PADDING_BLOCK_BIT_SIZSE	512	// SHA-256의 패딩된 메시지 블럭의 비트 크기
#define PADDING_BLOCK_BYTE_SIZE 64	// SHA-256의 패딩된 메시지 블럭의 바이트 크기
#define PADDING_INPUT_BIT_SIZE 64	// 패딩 마지막에 메시지 길이가 들어가는 비트 크기
#define PADDING_INPUT_BIYTE_SIZSE 8	// 패딩 마지막에 메시지 길이가 들어가는 바이트 크기
#define ROUND_SIZE 64				// 라운드 크기
#define INITIAL_HASH initial_hash	// 초기 해쉬
#define CONSTANT_K constantK		// 상수 K	

// 초기 해쉬
const uint32_t initial_hash[8] = {
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

// 상수 K
const uint64_t constantK[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 
	0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 
	0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 
	0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 
	0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 
	0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 
	0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 
	0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 
	0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 
	0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

#define MAJORITY(X, Y, Z) ((X & Y) ^ (Y & Z) ^ (Z & X))	// 라운드 하위 Majority 함수
#define CONDITIONAL(X, Y, Z) ((X & Y) ^ (~X & Z))		// 라운드 하위 Conditional 함수
#define ROTATE_0(X) (ROTR(X, 2) ^ ROTR(X, 13) ^ ROTR(X, 22))	// 라운드 하위 로테이션 함수 A연산 때 사용
#define ROTATE_1(X) (ROTR(X, 6) ^ ROTR(X, 11) ^ ROTR(X, 25))	// 라운드 하위 로테이션 함수 E연산 때 사용
#define MODULO_32(X) X % (uint64_t)pow(2, 32)// 덧셈 모듈러 2^32

/**
 * @brief SHA-512 클래스
 */
class SHA_256
{
private:
	/*-----평문 메시지 관련-----*/
	std::string message;	// 평문 메시지를 저장하는 변수
	int message_byte_size;	// 입력 메시지의 바이트의 크기를 저장하는 변수 // 바이크 크기 * 8 = 비트 개수

	/*-----패딩 메시지 관련-----*/
	uint8_t* ptr_message_padding;	// 평문 메시지를 패딩한 문자열이 들어갈 포인터 변수
	int ptr_message_padding_size;	// 평문 메시지를 패딩한 문자열의 길이
	uint8_t* ptr_padding_block;		// 압축 함수에 들어갈 패딩된 문자열 (512 bits = 64byte)
	
	/*-----압축 함수 관련-----*/
	Wt wt;	// 라운드 함수에서 사용되는 Wt를 저장한 객체 변수

	/*-----해쉬 값 관련-----*/
	uint32_t hash[8];	// 해쉬를 저장하는 uint32_t배열, 32비트 * 8개 = 256비트

private:
//----------------------------------------------------------------------//
//						밖에 노출될 필요가 없는 메소드						//
//----------------------------------------------------------------------//

	//----------------------------------------------------------------------//
	//								평문 관련								//
	//----------------------------------------------------------------------//

	/**
	 * @brief	message set 메소드
	 */
	void set_message(std::string message) { this->message = message; }

	/**
	 * @brief	message_byte_size set 메소드
	 */
	void set_message_byte_size(int message_byte_size) { this->message_byte_size = message_byte_size; }

	/**
	 * @brief	message get 메소드
	 * @return	message
	 */
	std::string get_message() { return message; }

	/**
	 * @brief	message의 주소를 반환하는 메소드
	 * @return	message의 주소
	 */
	std::string* get_ptr_message() { return &message; }

	/**
	 * @brief	message_byte_size get 메소드
	 * @return	평문 메시지의 길이
	 */
	int get_message_byte_size() { return message_byte_size; }

	//----------------------------------------------------------------------//
	//							패딩 메시지 관련								//
	//----------------------------------------------------------------------//

	/**
	 * @brief	ptr_message_padding set 메소드
	 */
	void set_ptr_message_padding(uint8_t* ptr_message_padding) { this->ptr_message_padding = ptr_message_padding; }
	
	/**
	 * @brief	ptr_message_padding_size set 메소드
	 */
	void set_ptr_message_padding_size(int ptr_message_padding_size) { this->ptr_message_padding_size = ptr_message_padding_size; }

	/**
	 * @brief	ptr_padding_block set 메소드
	 */
	void set_ptr_padding_block(uint8_t* ptr_padding_block) { this->ptr_padding_block = ptr_padding_block; }

	/**
	 * @brief	ptr_message_padding get 메소드
	 * @return	ptr_message_padding이 저장한 주소 값
	 */
	uint8_t* get_ptr_message_padding() { return ptr_message_padding; }
	
	/**
	 * @brief	ptr_message_padding_size get 메소드
	 * @return	ptr_message_padding의 길이
	 */
	int get_ptr_message_padding_size() { return ptr_message_padding_size; }

	/**
	 * @brief	ptr_padding_block get 메소드
	 * @return	ptr_padding_block이 저장한 주소 값
	 */
	uint8_t* get_ptr_padding_block() { return ptr_padding_block; }

	//----------------------------------------------------------------------//
	//							압축 함수 관련								//
	//----------------------------------------------------------------------//

	/**
	 * @brief	wt set 메소드
	 */
	void set_Wt(uint8_t* padding_block) { wt.set_Wt(padding_block); }

	/**
	 * @brief	wt get 메소드
	 * @param	- round_count: 라운스 숫자
	 * @return	W(t)에 해당하는 값
	 */
	uint32_t get_Wt(int round_count) { return wt.get_Wt(round_count); }

	//----------------------------------------------------------------------//
	//								해쉬 관련								//
	//----------------------------------------------------------------------//
	
	/**
	 * @brief	hash set 메소드
	 */
	void set_hash(uint32_t hash, int index) { if (index < sizeof(this->hash) / sizeof(uint32_t)) this->hash[index] = hash; }
	
	/**
	 * @brief	hash get 메소드
	 * @return	hash[index]
	 */
	uint32_t get_hash_at(int index) { return this->hash[index]; }

	//----------------------------------------------------------------------//
	//								SHA-256 관련								//
	//----------------------------------------------------------------------//

	/**
	 * @brief index에 해당하는 해쉬 값을 설정하는 메소드
	 */
	void set_initial_hash();

	/**
	 * @brief	평문 메시지를 입력받는 메소드
	 */
	void input();

	/**
	 * @brief	메시지를 패딩시키는 메소드
	 * @date	19.11.09
	 */
	void padding();

	/**
	 * @brief	확장함수를 담당하는 메소드
	 * @param	- padding_block: 확장 함수에 입력되는 padding_block
	 * @error	라운드는 64, 실행되는 라운드 78
	 */
	void compression_function(uint8_t* padding_block);

	/**
	 * @brief	라운드를 담당하는 메소드
	 * @param	- round_count: 현재 라운드 수
	 * @error	라운드는 64, 실행되는 라운드 78
	 */
	void round(int round_count);

public:
	SHA_256();
	~SHA_256();

	/**
	 * @brief	SHA-256 메소드
	 * @date	19/11/10
	 */
	void sha_256();

	uint32_t* get_ptr_hash();
};

#endif // !SHA_512_H