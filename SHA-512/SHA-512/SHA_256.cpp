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
	// 입력된 메시지가 512비트(64바이트)가 아니면
	if (get_message_byte_size() % PADDING_BLOCK_BYTE_SIZE != 0 || get_message_byte_size() == 0);
	{
		// 패딩되야할 메시지의 64byte의 개수
		int padding_byte_size = (get_message_byte_size() / (double)PADDING_BLOCK_BYTE_SIZE > get_message_byte_size() / PADDING_BLOCK_BYTE_SIZE) ?
			get_message_byte_size() / PADDING_BLOCK_BYTE_SIZE + 1 : get_message_byte_size() / PADDING_BLOCK_BYTE_SIZE;

		// 패딩 메시지를 저장할 uint8_t배열 할당, 메시지 복사 
		// ptr_message_padding_size 설정
		uint8_t* padding_message = new uint8_t[padding_byte_size * PADDING_BLOCK_BYTE_SIZE];
		set_ptr_message_padding_size(padding_byte_size * PADDING_BLOCK_BYTE_SIZE);
		std::copy(get_ptr_message()->begin(), get_ptr_message()->end(), padding_message);

		// 패딩 1단계, 끝을 알리는 1을 넣는다.
		// 128 = 1000 0000
		padding_message[get_message_byte_size()] = 128;

		// 패딩 2단계, 0을 채운다.
		// zero_index : 0을 채우기 시작하는 인덱스
		for (int zero_index = get_message_byte_size() + 1; zero_index < padding_byte_size * PADDING_BLOCK_BYTE_SIZE - PADDING_INPUT_BIYTE_SIZSE; zero_index++)
			padding_message[zero_index] = 0;
		
		// 패딩 3단계, 나머지 64비트에 메시지 길이를 채운다.
		// 시작위치(start_pos)는 끝에서 부터 64비트(8바이트) 이므로 패딩 블럭 크기(512비트(64바이트) - 64비트(8바이트)
		int start_pos = padding_byte_size * PADDING_BLOCK_BYTE_SIZE - PADDING_INPUT_BIYTE_SIZSE;
		for (int length_index = start_pos; length_index < start_pos + PADDING_INPUT_BIYTE_SIZSE - 1; length_index++)
		{
			// 64비트의 변수에 메시지의 길이를 넣는다.
			uint64_t tmp_value64 = (uint64_t)get_message_byte_size() * 8;

			// 64비트 값을 쉬프트 시킨 후 8비트 변수에 넣는다.
			// error : 문자 길이가 제대로 안들어감
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
