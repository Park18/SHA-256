#include "SHA_512.h"

using namespace std;

void SHA_512::input()
{
	string str;
	getline(cin, str);
	set_message(str);

	set_message_byte_size(get_message().length());
}

void SHA_512::padding()
{
	input();

	// 입력된 메시지가 1024비트가 아니면
	if (get_message_byte_size() % 128 != 0 || get_message_byte_size() == 0);
	{
		// 패딩된 메시지의 128byte의 개수
		int padding_byte_size = (get_message_byte_size() / 128.0 > get_message_byte_size() / 128) ? get_message_byte_size() / 128 + 1 : get_message_byte_size() / 128;

		// 패딩 메시지를 저장할 uint8_t배열 할당, 메시지 복사
		uint8_t* padding_message = new uint8_t[padding_byte_size * 128];
		std::copy(get_ptr_message()->begin(), get_ptr_message()->end(), padding_message);

		// 패딩 1단계, 끝을 알리는 1을 넣는다.
		// 128 = 1000 0000
		padding_message[get_message_byte_size()] = 128;

		// 패딩 2단계, 0을 채운다.
		for (int zero_index = get_message_byte_size() + 1; zero_index < padding_byte_size * 128 - 16; zero_index++)
			padding_message[zero_index] = 0;
		
		// 패딩 3단계, 나머지 128비트에 메시지 길이를 채운다.
		// 다시 생각해 보기
		// date 19.11.08
		uint64_t message_byte_size = get_message_byte_size();
		for (int length_index = padding_byte_size * 128 - 1; length_index > padding_byte_size * 128 - 16; length_index--)
		{
			uint8_t tmp_value;
			
		}
	}
}
