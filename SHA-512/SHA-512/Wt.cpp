#include "Wt.h"

#define ROTR(value, shift) _rotr64(value, shift)			// �����̼� ��ũ��
#define SIGMA_0(x) (ROTR(x, 1) ^ ROTR(x, 8) ^ (x >> 7))		// �ñ׸�0 ��ũ��
#define SIGMA_1(x) (ROTR(x, 19) ^ ROTR(x, 61) ^ (x >> 6))	// �ñ׸�1 ��ũ��

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

// Wt���� (0 <= t <= 15)�� ���� ���� �� ����ϴ� �޼ҵ�
uint64_t Wt::get_value64(uint8_t* message_block, int start_pos)
{
	uint64_t value64 = 0;		// ��ȯ�� 64��Ʈ ����
	uint64_t tmp_value64 = 0;	// �ӽ� ������ 64��Ʈ ����

	// uint8_t[128](1024bits) �޽��� ���� t�� �´� �޽��� ���� start_pos�� �Էµȴ�.
	// message_index�� start_pos�� ����, 8�� �ݺ��Ѵ�.(64��Ʈ == 1����Ʈ * 8��)
	// shift_count�� �Էµ� start_pos + 8�� ����, shift_count�� message_index���� Ŭ ������ �ݺ��Ѵ�.
	// (message_index�� �׻� shift_count���� �۴�. ���� ��ŭ ����Ʈ�̵��ؾ� ��Ȯ�� 64��Ʈ ������ �����ȴ�.
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
