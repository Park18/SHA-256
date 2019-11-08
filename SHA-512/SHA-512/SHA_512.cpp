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

	// �Էµ� �޽����� 1024��Ʈ�� �ƴϸ�
	if (get_message_byte_size() % 128 != 0 || get_message_byte_size() == 0);
	{
		// �е��� �޽����� 128byte�� ����
		int padding_byte_size = (get_message_byte_size() / 128.0 > get_message_byte_size() / 128) ? get_message_byte_size() / 128 + 1 : get_message_byte_size() / 128;

		// �е� �޽����� ������ uint8_t�迭 �Ҵ�, �޽��� ����
		uint8_t* padding_message = new uint8_t[padding_byte_size * 128];
		std::copy(get_ptr_message()->begin(), get_ptr_message()->end(), padding_message);

		// �е� 1�ܰ�, ���� �˸��� 1�� �ִ´�.
		// 128 = 1000 0000
		padding_message[get_message_byte_size()] = 128;

		// �е� 2�ܰ�, 0�� ä���.
		for (int zero_index = get_message_byte_size() + 1; zero_index < padding_byte_size * 128 - 16; zero_index++)
			padding_message[zero_index] = 0;
		
		// �е� 3�ܰ�, ������ 128��Ʈ�� �޽��� ���̸� ä���.
		// �ٽ� ������ ����
		// date 19.11.08
		uint64_t message_byte_size = get_message_byte_size();
		for (int length_index = padding_byte_size * 128 - 1; length_index > padding_byte_size * 128 - 16; length_index--)
		{
			uint8_t tmp_value;
			
		}
	}
}
