#include "SHA_256.h"

using namespace std;

void SHA_256::set_initial_hash()
{
	for (int index = 0; index < 8; index++)
		this->hash[index] = INITIAL_HASH[index];
}

void SHA_256::input()
{
	string str;
	getline(cin, str);
	set_message(str);

	set_message_byte_size(get_message().length());
}

void SHA_256::padding()
{
	// �Էµ� �޽����� 512��Ʈ(64����Ʈ)�� �ƴϸ�
	if (get_message_byte_size() % PADDING_BLOCK_BYTE_SIZE != 0 || get_message_byte_size() == 0);
	{
		// �е��Ǿ��� �޽����� 64byte�� ����
		int padding_byte_size = (get_message_byte_size() / (double)PADDING_BLOCK_BYTE_SIZE > get_message_byte_size() / PADDING_BLOCK_BYTE_SIZE) ?
			get_message_byte_size() / PADDING_BLOCK_BYTE_SIZE + 1 : get_message_byte_size() / PADDING_BLOCK_BYTE_SIZE;

		// �е� �޽����� ������ uint8_t�迭 �Ҵ�, �޽��� ���� 
		// ptr_message_padding_size ����
		uint8_t* padding_message = new uint8_t[padding_byte_size * PADDING_BLOCK_BYTE_SIZE];
		set_ptr_message_padding_size(padding_byte_size * PADDING_BLOCK_BYTE_SIZE);
		std::copy(get_ptr_message()->begin(), get_ptr_message()->end(), padding_message);

		// �е� 1�ܰ�, ���� �˸��� 1�� �ִ´�.
		// 128 = 1000 0000
		padding_message[get_message_byte_size()] = 128;

		// �е� 2�ܰ�, 0�� ä���.
		// zero_index : 0�� ä��� �����ϴ� �ε���
		for (int zero_index = get_message_byte_size() + 1; zero_index < padding_byte_size * PADDING_BLOCK_BYTE_SIZE - PADDING_INPUT_BIYTE_SIZSE; zero_index++)
			padding_message[zero_index] = 0;
		
		// �е� 3�ܰ�, ������ 64��Ʈ�� �޽��� ���̸� ä���.
		// ������ġ(start_pos)�� ������ ���� 64��Ʈ(8����Ʈ) �̹Ƿ� �е� �� ũ��(512��Ʈ(64����Ʈ) - 64��Ʈ(8����Ʈ)
		int start_pos = padding_byte_size * PADDING_BLOCK_BYTE_SIZE - PADDING_INPUT_BIYTE_SIZSE;
		for (int length_index = start_pos; length_index < start_pos + PADDING_INPUT_BIYTE_SIZSE - 1; length_index++)
		{
			// 64��Ʈ�� ������ �޽����� ���̸� �ִ´�.
			uint64_t tmp_value64 = (uint64_t)get_message_byte_size() * 8;

			// 64��Ʈ ���� ����Ʈ ��Ų �� 8��Ʈ ������ �ִ´�.
			// error : ���� ���̰� ����� �ȵ�
			for (int shift_count = start_pos + PADDING_INPUT_BIYTE_SIZSE-1; shift_count > length_index; shift_count--)
			{
				tmp_value64 >>= 8;
			}
			
			padding_message[length_index] = tmp_value64;
		}

		set_ptr_message_padding(padding_message);
	}
}

void SHA_256::compression_function(uint8_t* padding_block)
{
	set_Wt(padding_block);

	for (int round_count = 0; round_count < 64; round_count++)
	{
		cout <<"round : "<< round_count << endl;
		round(round_count);
		cout << "round : " << round_count << endl;
	}
	
	for (int index = 0; index < 8; index++)
		set_hash(MODULO_32(get_hash_at(index) + INITIAL_HASH[index]), index);
}

void SHA_256::round(int round_count)
{
	if (round_count > 63) return;

	// |A|B|C|D|E|F|G|H|
	// |0|1|2|3|4|5|6|7|
	uint32_t copy_hash[8];
	copy(this->hash, this->hash+8, copy_hash);

	cout << std::oct << "----------" << round_count << "----------" << endl;;
	for (int index = 0; index < 8; index++)
		cout << std::hex << copy_hash[index] << endl;
	cout << std::oct << "----------" << round_count << "----------" << endl;;
	/*
	*/

	uint32_t mixer1_value = MAJORITY(copy_hash[0], copy_hash[1], copy_hash[2]) + ROTATE_0(copy_hash[0]);
	uint32_t mixer2_value = CONDITIONAL(copy_hash[4], copy_hash[5], copy_hash[6]) + ROTATE_1(copy_hash[4]) +
		copy_hash[7] + get_Wt(round_count) + CONSTANT_K[round_count];

	for (int hash_index = 0; hash_index < 8; hash_index++)
	{
		if (hash_index == 0)
			set_hash(MODULO_32(mixer1_value + mixer2_value), hash_index);

		else if (hash_index == 3)
			set_hash(MODULO_32(mixer2_value + copy_hash[3]), hash_index);
		
		else
			set_hash(copy_hash[hash_index - 1], hash_index);
	}
}

SHA_256::SHA_256()
{
	set_message("");
	set_message_byte_size(0);

	set_ptr_message_padding(NULL);
	set_ptr_message_padding_size(0);
	set_ptr_padding_block(new uint8_t[64]);

	set_initial_hash();
}

SHA_256::~SHA_256()
{
	delete get_ptr_message_padding();
	delete get_ptr_padding_block();
}

void SHA_256::sha_256()
{
	input();
	padding();

	for (int count = 0; count < get_ptr_message_padding_size() / PADDING_BLOCK_BYTE_SIZE; count++)
	{
		int padding_start_pos = count * PADDING_BLOCK_BYTE_SIZE;
		int padding_last_pos = (count + 1) * PADDING_BLOCK_BYTE_SIZE;
		copy(get_ptr_message_padding() + padding_start_pos, get_ptr_message_padding() + padding_last_pos, get_ptr_padding_block());

		compression_function(get_ptr_padding_block());
	}
}

uint32_t* SHA_256::get_ptr_hash()
{
	uint32_t* return_hash = new uint32_t[8];
	copy(this->hash, this->hash + 8, return_hash);

	return return_hash;
}
