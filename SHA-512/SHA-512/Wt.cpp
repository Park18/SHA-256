#include "Wt.h"

void Wt::set_Wt(uint8_t* message_block)
{
	for (int t = 0; t < MAX_T_SIZE; t++)
	{
		if (t >= 0 && t <= 15)
		{
			get_ptr_Wt()[t] = get_value32(message_block, t * 8);
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
uint32_t Wt::get_value32(uint8_t* message_block, int start_pos)
{
	uint32_t value32 = 0;		// ��ȯ�� 32��Ʈ ����
	uint32_t tmp_value32 = 0;	// �ӽ� ������ 32��Ʈ ����

	// uint8_t[64](512bits) �޽��� ���� t�� �´� �޽��� ���� start_pos�� �Էµȴ�.
	// message_index�� start_pos�� ����, 8�� �ݺ��Ѵ�.(32��Ʈ(4����Ʈ) == 1����Ʈ * 4��)
	// shift_count�� �Էµ� start_pos + 4�� ����, shift_count�� message_index���� Ŭ ������ �ݺ��Ѵ�.
	// (message_index�� �׻� shift_count���� �۴�. ���� ��ŭ ����Ʈ�̵��ؾ� ��Ȯ�� 32��Ʈ ������ �����ȴ�.
	for (int message_index = start_pos; message_index < start_pos + 4; message_index++)
	{
		tmp_value32 = message_block[message_index];
		for (int shift_count = start_pos + 3; shift_count > message_index; shift_count--)
		{
			tmp_value32 <<= 8;
		}

		value32 += tmp_value32;
	}

	return value32;
}
