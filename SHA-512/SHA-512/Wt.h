#pragma once
#ifndef Wt_h
#define Wt_h

#include <iostream>

#define MAX_T_SIZE 64

#define ROTR(value, shift) _rotr(value, shift)				// �����̼� ��ũ��
#define SIGMA_0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3))	// �ñ׸�0 ��ũ��
#define SIGMA_1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10))	// �ñ׸�1 ��ũ��

/**
 * @class	Wt
 * @date	2019/11/07
 * @author	������
 * @brief	SHA-2�� Wt�� ���� Ŭ����
 * @details	SHA-2(256) �˰����� ���� �Լ��� Wt�� �����ϴ� Ŭ����
 */
class Wt
{
private:
	uint32_t* ptr_Wt;

	/**
	 * @brief	ptr_Wt set �޼ҵ�
	 * @detail	��ü ������ �ѹ� ����ϰ� ������� �ʴ´�.
	 */
	void set_ptr_Wt(uint32_t* ptr_Wt) { this->ptr_Wt = ptr_Wt; }

	/**
	 * @brief	ptr_Wt get �޼ҵ�
	 * @return	ptr_Wt�� ������ �ּ� ��
	 * @detail	�ʿ� ���°� ����.
	 */
	uint32_t* get_ptr_Wt() { return ptr_Wt; }

public:
	/**
	 * @bridef	WtŬ������ ������
	 * @detail	t�� �ִ�ũ��(64)��ŭ�� ���� �޸� �Ҵ��Ѵ�.
	 */
	Wt() { set_ptr_Wt(new uint32_t[MAX_T_SIZE]); }

	/**
	 * @bridef	WtŬ������ �Ҹ���
	 * @detail	���� �޸� �Ҵ���� ���� �����Ѵ�.
	 */
	~Wt() { delete get_ptr_Wt(); }

	/**
	 * @brief	Wt�� �����ϴ� �޼ҵ�
	 * @param	padding_block: ���� �Լ����� �ԷµǴ� �е��� �޽��� ��(512��Ʈ)
	 * @detail	���� �Լ��� ���Ǵ� ��� Wt�� �迭�� �����Ѵ�. 
	 */
	void set_Wt(uint8_t* padding_block);

	/**
	 * @brief	Wt�� ���� ��ȯ�ϴ� �޼ҵ�
	 * @param	t: W�� ����
	 * @return	W(t)�� ��
	 */
	uint32_t get_Wt(int round_count) { return get_ptr_Wt()[round_count]; }

	/**
	 * @brief	uint8_t �迭�� �޾� uint32_t ������ ��ȯ�ϴ� 
	 * @param	- message_block: 512bits �޽��� ��(uint8_t[MAX_T_SIZE])
	 *			- start_pos: 512bits �޽��� ������ �о���� ��ġ
	 * @return	��ȯ�� 32��Ʈ ��
	 * @detail	�е��� �޽������� uint8_t[4]��ŭ�� ����� ����
	 *			uint32_t�� ��ȯ�Ͽ� ��ȯ�ϴ� �޼ҵ�
	 *			uint8_t[4] -> uint32_t
	 *			Wt���� (0 <= t <= 15)�� ���� ���� �� ���
	 */
	uint32_t get_value32(uint8_t* message_block, int start_pos);
};


#endif // !Wt_h

/* ���� ����Ʈ
 * https://docs.microsoft.com/ko-kr/cpp/c-runtime-library/reference/rotl-rotl64-rotr-rotr64?view=vs-2019
 * https://csrc.nist.gov/csrc/media/publications/fips/180/4/final/documents/fips180-4-draft-aug2014.pdf
 * https://blog.naver.com/tpinlab/10121820313
 * https://m.blog.naver.com/PostView.nhn?blogId=vjhh0712v&logNo=221453210356&proxyReferer=https%3A%2F%2Fwww.google.com%2F
 * https://medium.com/sunhyoups-story/doxygen-%EC%82%AC%EC%9A%A9-%EA%B0%80%EC%9D%B4%EB%93%9C-e6cf5a1b24e9 (doxygen Ű���� ����)
 */