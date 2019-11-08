#include "Wt.h"

#define ROTR(value, shift) _rotr64(value, shift)			// 로테이션 매크로
#define SIGMA_0(x) (ROTR(x, 1) ^ ROTR(x, 8) ^ (x >> 7))		// 시그마0 매크로
#define SIGMA_1(x) (ROTR(x, 19) ^ ROTR(x, 61) ^ (x >> 6))	// 시그마1 매크로

void Wt::set_Wt(uint8_t* message_block)
{
	for (int t = 0; t < MAX_T_SIZE; t++)
	{
		if (t >= 0 && t <= 15)
		{
			get_ptr_Wt()[t] = get_value64(message_block, t * 8);
		}

		else
		{
			get_ptr_Wt()[t] = get_ptr_Wt()[t - 15] ^ 
			SIGMA_0(get_ptr_Wt()[t - 15]) ^ 
			get_ptr_Wt()[t - 7] ^ 
			SIGMA_1(get_ptr_Wt()[t - 2]);
		}
	}
}

// Wt에서 (0 <= t <= 15)의 값을 얻을 때 사용하는 메소드
uint64_t Wt::get_value64(uint8_t* message_block, int start_pos)
{
	uint64_t value64 = 0;		// 반환할 64비트 변수
	uint64_t tmp_value64 = 0;	// 임시 저장할 64비트 변수

	// uint8_t[128](1024bits) 메시지 블럭과 t에 맞는 메시지 블럭의 start_pos이 입력된다.
	// message_index를 start_pos로 설정, 8번 반복한다.(64비트 == 1바이트 * 8개)
	// shift_count는 입력된 start_pos + 8로 설정, shift_count가 message_index보다 클 때까지 반복한다.
	// (message_index는 항상 shift_count보다 작다. 작은 만큼 쉬프트이동해야 정확한 64비트 값으로 설정된다.
	for (int message_index = start_pos; message_index < start_pos + 8; message_index++)
	{
		tmp_value64 = message_block[message_index];
		for (int shift_count = start_pos + 7; shift_count > message_index; shift_count--)
		{
			tmp_value64 <<= 8;
		}

		value64 += tmp_value64;
	}

	return value64;
}
