#pragma once
#ifndef Wt_h
#define Wt_h

#include <iostream>

#define MAX_T_SIZE 64

#define ROTR(value, shift) _rotr(value, shift)				// 로테이션 매크로
#define SIGMA_0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3))	// 시그마0 매크로
#define SIGMA_1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10))	// 시그마1 매크로

/**
 * @class	Wt
 * @date	2019/11/07
 * @author	박현우
 * @brief	SHA-2의 Wt를 위한 클래스
 * @details	SHA-2(256) 알고리즘의 라운드 함수의 Wt를 관리하는 클래스
 */
class Wt
{
private:
	uint32_t* ptr_Wt;

	/**
	 * @brief	ptr_Wt set 메소드
	 * @detail	객체 생성시 한번 사용하고 사용하지 않는다.
	 */
	void set_ptr_Wt(uint32_t* ptr_Wt) { this->ptr_Wt = ptr_Wt; }

	/**
	 * @brief	ptr_Wt get 메소드
	 * @return	ptr_Wt이 저장한 주소 값
	 * @detail	필요 없는거 같다.
	 */
	uint32_t* get_ptr_Wt() { return ptr_Wt; }

public:
	/**
	 * @bridef	Wt클래스의 생성자
	 * @detail	t의 최대크기(64)만큼의 동적 메모리 할당한다.
	 */
	Wt() { set_ptr_Wt(new uint32_t[MAX_T_SIZE]); }

	/**
	 * @bridef	Wt클래스의 소멸자
	 * @detail	동적 메모리 할당받은 것을 해제한다.
	 */
	~Wt() { delete get_ptr_Wt(); }

	/**
	 * @brief	Wt를 설정하는 메소드
	 * @param	padding_block: 압축 함수에서 입력되는 패딩된 메시지 블럭(512비트)
	 * @detail	압축 함수에 사용되는 모든 Wt를 배열에 저장한다. 
	 */
	void set_Wt(uint8_t* padding_block);

	/**
	 * @brief	Wt의 값을 반환하는 메소드
	 * @param	t: W의 순서
	 * @return	W(t)의 값
	 */
	uint32_t get_Wt(int round_count) { return get_ptr_Wt()[round_count]; }

	/**
	 * @brief	uint8_t 배열을 받아 uint32_t 값으로 반환하는 
	 * @param	- message_block: 512bits 메시지 블럭(uint8_t[MAX_T_SIZE])
	 *			- start_pos: 512bits 메시지 블럭에서 읽어야할 위치
	 * @return	변환된 32비트 값
	 * @detail	패딩된 메시지에서 uint8_t[4]만큼의 저장된 값을
	 *			uint32_t로 변환하여 반환하는 메소드
	 *			uint8_t[4] -> uint32_t
	 *			Wt에서 (0 <= t <= 15)의 값을 얻을 때 사용
	 */
	uint32_t get_value32(uint8_t* message_block, int start_pos);
};


#endif // !Wt_h

/* 참고 사이트
 * https://docs.microsoft.com/ko-kr/cpp/c-runtime-library/reference/rotl-rotl64-rotr-rotr64?view=vs-2019
 * https://csrc.nist.gov/csrc/media/publications/fips/180/4/final/documents/fips180-4-draft-aug2014.pdf
 * https://blog.naver.com/tpinlab/10121820313
 * https://m.blog.naver.com/PostView.nhn?blogId=vjhh0712v&logNo=221453210356&proxyReferer=https%3A%2F%2Fwww.google.com%2F
 * https://medium.com/sunhyoups-story/doxygen-%EC%82%AC%EC%9A%A9-%EA%B0%80%EC%9D%B4%EB%93%9C-e6cf5a1b24e9 (doxygen 키워드 정리)
 */