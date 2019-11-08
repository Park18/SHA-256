#pragma once
#ifndef Wt_h
#define Wt_h

#include <iostream>

#define MAX_T_SIZE 80

/**
 * @class	Wt
 * @date	2019/11/07
 * @author	������
 * @brief	SHA-2�� Wt�� ���� Ŭ����v
 * @details	SHA-2 �˰����� ���� �Լ��� Wt�� �����ϴ� Ŭ����
 */
class Wt
{
private:
	uint64_t* ptr_Wt;
	//uint8_t _Wt[80][8];	// W0 ~ W79(80��), Wt�� ũ��� 64bits(8bites * 8��)

public:
	Wt() { set_ptr_Wt(new uint64_t[80]); }
	~Wt() { delete get_ptr_Wt(); }

	/**
	 * @brief	set/get
	 */
	void set_ptr_Wt(uint64_t* ptr_Wt) { this->ptr_Wt = ptr_Wt; }
	uint64_t* get_ptr_Wt() { return ptr_Wt; }

	/**
	 * @brief	Wt�� �����ϴ� �޼ҵ�
	 * @param	items: Wt�� �� ���� ������ uint8_t[128] �迭 ������
	 * @detail	���� �Լ��� ���Ǵ� ��� Wt�� �迭�� �����Ѵ�. 
	 */
	void set_Wt(uint8_t* message_block);

	/**
	 * @brief	Wt�� ���� ���� �迭 �����͸� ��ȯ�ϴ� �޼ҵ�
	 * @param	t: W�� ����
	 * @return	Wt�� ���� ���� �迭 ������
	 */
	uint64_t get_Wt(int t) { return get_ptr_Wt()[t]; }

	/**
	 * @brief	�迭�� ����� �������� 10������ ��ȯ
	 * @param	- message_block: 1024bits �޽��� ��(uint8_t[128])
	 *			- start_pos: 1024bits �޽��� ������ �о���� ��ġ
	 * @return	��ȯ�� 64��Ʈ�� 10����
	 * @detail	�е��� �޽������� uint8_t[8]��ŭ�� ����� ����
	 *			uint64_t�� ��ȯ�Ͽ� ��ȯ�ϴ� �޼ҵ�
	 *			uint8_t[8] -> uint64_t
	 *			Wt���� (0 <= t <= 15)�� ���� ���� �� ���
	 */
	uint64_t get_value64(uint8_t* message_block, int start_pos);
};


#endif // !Wt_h

/* ���� ����Ʈ
 * https://docs.microsoft.com/ko-kr/cpp/c-runtime-library/reference/rotl-rotl64-rotr-rotr64?view=vs-2019
 * https://csrc.nist.gov/csrc/media/publications/fips/180/4/final/documents/fips180-4-draft-aug2014.pdf
 * https://blog.naver.com/tpinlab/10121820313
 * https://m.blog.naver.com/PostView.nhn?blogId=vjhh0712v&logNo=221453210356&proxyReferer=https%3A%2F%2Fwww.google.com%2F
 * https://medium.com/sunhyoups-story/doxygen-%EC%82%AC%EC%9A%A9-%EA%B0%80%EC%9D%B4%EB%93%9C-e6cf5a1b24e9 (doxygen Ű���� ����)
 */