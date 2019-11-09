#pragma once
/**
 * @file SHA_512.h
 * @brief SHA-2�� 512�� �����ϴ� ���
 * @detail 512���� 256���� ����
 */
#ifndef SHA_256_H
#define SHA_256_H

#include <iostream>
#include <string>
#include <algorithm>
#include <math.h>
#include "Wt.h"

#define PADDING_BLOCK_BIT_SIZSE	512	// SHA-256�� �е��� �޽��� ���� ��Ʈ ũ��
#define PADDING_BLOCK_BYTE_SIZE 64	// SHA-256�� �е��� �޽��� ���� ����Ʈ ũ��
#define PADDING_INPUT_BIT_SIZE 64	// �е� �������� �޽��� ���̰� ���� ��Ʈ ũ��
#define PADDING_INPUT_BIYTE_SIZSE 8	// �е� �������� �޽��� ���̰� ���� ����Ʈ ũ��
#define ROUND_SIZE 64				// ���� ũ��
#define INITIAL_HASH initial_hash	// �ʱ� �ؽ�
#define CONSTANT_K constantK		// ��� K	

// �ʱ� �ؽ�
const uint32_t initial_hash[8] = {
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

// ��� K
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

#define MAJORITY(X, Y, Z) ((X & Y) ^ (Y & Z) ^ (Z & X))	// ���� ���� Majority �Լ�
#define CONDITIONAL(X, Y, Z) ((X & Y) ^ (~X & Z))		// ���� ���� Conditional �Լ�
#define ROTATE_0(X) (ROTR(X, 2) ^ ROTR(X, 13) ^ ROTR(X, 22))	// ���� ���� �����̼� �Լ� A���� �� ���
#define ROTATE_1(X) (ROTR(X, 6) ^ ROTR(X, 11) ^ ROTR(X, 25))	// ���� ���� �����̼� �Լ� E���� �� ���
#define MODULO_32(X) X % (uint64_t)pow(2, 32)// ���� ��ⷯ 2^32

/**
 * @brief SHA-512 Ŭ����
 */
class SHA_256
{
private:
	/*-----�� �޽��� ����-----*/
	std::string message;	// �� �޽����� �����ϴ� ����
	int message_byte_size;	// �Է� �޽����� ����Ʈ�� ũ�⸦ �����ϴ� ���� // ����ũ ũ�� * 8 = ��Ʈ ����

	/*-----�е� �޽��� ����-----*/
	uint8_t* ptr_message_padding;	// �� �޽����� �е��� ���ڿ��� �� ������ ����
	int ptr_message_padding_size;	// �� �޽����� �е��� ���ڿ��� ����
	uint8_t* ptr_padding_block;		// ���� �Լ��� �� �е��� ���ڿ� (512 bits = 64byte)
	
	/*-----���� �Լ� ����-----*/
	Wt wt;	// ���� �Լ����� ���Ǵ� Wt�� ������ ��ü ����

	/*-----�ؽ� �� ����-----*/
	uint32_t hash[8];	// �ؽ��� �����ϴ� uint32_t�迭, 32��Ʈ * 8�� = 256��Ʈ

private:
//----------------------------------------------------------------------//
//						�ۿ� ����� �ʿ䰡 ���� �޼ҵ�						//
//----------------------------------------------------------------------//

	//----------------------------------------------------------------------//
	//								�� ����								//
	//----------------------------------------------------------------------//

	/**
	 * @brief	message set �޼ҵ�
	 */
	void set_message(std::string message) { this->message = message; }

	/**
	 * @brief	message_byte_size set �޼ҵ�
	 */
	void set_message_byte_size(int message_byte_size) { this->message_byte_size = message_byte_size; }

	/**
	 * @brief	message get �޼ҵ�
	 * @return	message
	 */
	std::string get_message() { return message; }

	/**
	 * @brief	message�� �ּҸ� ��ȯ�ϴ� �޼ҵ�
	 * @return	message�� �ּ�
	 */
	std::string* get_ptr_message() { return &message; }

	/**
	 * @brief	message_byte_size get �޼ҵ�
	 * @return	�� �޽����� ����
	 */
	int get_message_byte_size() { return message_byte_size; }

	//----------------------------------------------------------------------//
	//							�е� �޽��� ����								//
	//----------------------------------------------------------------------//

	/**
	 * @brief	ptr_message_padding set �޼ҵ�
	 */
	void set_ptr_message_padding(uint8_t* ptr_message_padding) { this->ptr_message_padding = ptr_message_padding; }
	
	/**
	 * @brief	ptr_message_padding_size set �޼ҵ�
	 */
	void set_ptr_message_padding_size(int ptr_message_padding_size) { this->ptr_message_padding_size = ptr_message_padding_size; }

	/**
	 * @brief	ptr_padding_block set �޼ҵ�
	 */
	void set_ptr_padding_block(uint8_t* ptr_padding_block) { this->ptr_padding_block = ptr_padding_block; }

	/**
	 * @brief	ptr_message_padding get �޼ҵ�
	 * @return	ptr_message_padding�� ������ �ּ� ��
	 */
	uint8_t* get_ptr_message_padding() { return ptr_message_padding; }
	
	/**
	 * @brief	ptr_message_padding_size get �޼ҵ�
	 * @return	ptr_message_padding�� ����
	 */
	int get_ptr_message_padding_size() { return ptr_message_padding_size; }

	/**
	 * @brief	ptr_padding_block get �޼ҵ�
	 * @return	ptr_padding_block�� ������ �ּ� ��
	 */
	uint8_t* get_ptr_padding_block() { return ptr_padding_block; }

	//----------------------------------------------------------------------//
	//							���� �Լ� ����								//
	//----------------------------------------------------------------------//

	/**
	 * @brief	wt set �޼ҵ�
	 */
	void set_Wt(uint8_t* padding_block) { wt.set_Wt(padding_block); }

	/**
	 * @brief	wt get �޼ҵ�
	 * @param	- round_count: �� ����
	 * @return	W(t)�� �ش��ϴ� ��
	 */
	uint32_t get_Wt(int round_count) { return wt.get_Wt(round_count); }

	//----------------------------------------------------------------------//
	//								�ؽ� ����								//
	//----------------------------------------------------------------------//
	
	/**
	 * @brief	hash set �޼ҵ�
	 */
	void set_hash(uint32_t hash, int index) { if (index < sizeof(this->hash) / sizeof(uint32_t)) this->hash[index] = hash; }
	
	/**
	 * @brief	hash get �޼ҵ�
	 * @return	hash[index]
	 */
	uint32_t get_hash_at(int index) { return this->hash[index]; }

	//----------------------------------------------------------------------//
	//								SHA-256 ����								//
	//----------------------------------------------------------------------//

	/**
	 * @brief index�� �ش��ϴ� �ؽ� ���� �����ϴ� �޼ҵ�
	 */
	void set_initial_hash();

	/**
	 * @brief	�� �޽����� �Է¹޴� �޼ҵ�
	 */
	void input();

	/**
	 * @brief	�޽����� �е���Ű�� �޼ҵ�
	 * @date	19.11.09
	 */
	void padding();

	/**
	 * @brief	Ȯ���Լ��� ����ϴ� �޼ҵ�
	 * @param	- padding_block: Ȯ�� �Լ��� �ԷµǴ� padding_block
	 * @error	����� 64, ����Ǵ� ���� 78
	 */
	void compression_function(uint8_t* padding_block);

	/**
	 * @brief	���带 ����ϴ� �޼ҵ�
	 * @param	- round_count: ���� ���� ��
	 * @error	����� 64, ����Ǵ� ���� 78
	 */
	void round(int round_count);

public:
	SHA_256();
	~SHA_256();

	/**
	 * @brief	SHA-256 �޼ҵ�
	 * @date	19/11/10
	 */
	void sha_256();

	uint32_t* get_ptr_hash();
};

#endif // !SHA_512_H