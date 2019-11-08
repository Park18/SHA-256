#pragma once
#ifndef Wt_h
#define Wt_h

#include <iostream>

#define MAX_T_SIZE 80

/**
 * @class	Wt
 * @date	2019/11/07
 * @author	박현우
 * @brief	SHA-2의 Wt를 위한 클래스v
 * @details	SHA-2 알고리즘의 라운드 함수의 Wt를 관리하는 클래스
 */
class Wt
{
private:
	uint64_t* ptr_Wt;
	//uint8_t _Wt[80][8];	// W0 ~ W79(80개), Wt의 크기는 64bits(8bites * 8개)

public:
	Wt() { set_ptr_Wt(new uint64_t[80]); }
	~Wt() { delete get_ptr_Wt(); }

	/**
	 * @brief	set/get
	 */
	void set_ptr_Wt(uint64_t* ptr_Wt) { this->ptr_Wt = ptr_Wt; }
	uint64_t* get_ptr_Wt() { return ptr_Wt; }

	/**
	 * @brief	Wt를 설정하는 메소드
	 * @param	items: Wt에 들어갈 값을 저장한 uint8_t[128] 배열 포인터
	 * @detail	라운드 함수에 사용되는 모든 Wt를 배열에 저장한다. 
	 */
	void set_Wt(uint8_t* message_block);

	/**
	 * @brief	Wt의 값을 가진 배열 포인터를 반환하는 메소드
	 * @param	t: W의 순서
	 * @return	Wt의 값을 가진 배열 포인터
	 */
	uint64_t get_Wt(int t) { return get_ptr_Wt()[t]; }

	/**
	 * @brief	배열에 저장된 이진수를 10진수로 변환
	 * @param	- message_block: 1024bits 메시지 블럭(uint8_t[128])
	 *			- start_pos: 1024bits 메시지 블럭에서 읽어야할 위치
	 * @return	변환된 64비트의 10진수
	 * @detail	패딩된 메시지에서 uint8_t[8]만큼의 저장된 값을
	 *			uint64_t로 변환하여 반환하는 메소드
	 *			uint8_t[8] -> uint64_t
	 *			Wt에서 (0 <= t <= 15)의 값을 얻을 때 사용
	 */
	uint64_t get_value64(uint8_t* message_block, int start_pos);
};


#endif // !Wt_h

/* 참고 사이트
 * https://docs.microsoft.com/ko-kr/cpp/c-runtime-library/reference/rotl-rotl64-rotr-rotr64?view=vs-2019
 * https://csrc.nist.gov/csrc/media/publications/fips/180/4/final/documents/fips180-4-draft-aug2014.pdf
 * https://blog.naver.com/tpinlab/10121820313
 * https://m.blog.naver.com/PostView.nhn?blogId=vjhh0712v&logNo=221453210356&proxyReferer=https%3A%2F%2Fwww.google.com%2F
 * https://medium.com/sunhyoups-story/doxygen-%EC%82%AC%EC%9A%A9-%EA%B0%80%EC%9D%B4%EB%93%9C-e6cf5a1b24e9 (doxygen 키워드 정리)
 */