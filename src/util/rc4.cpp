#include "rc4.h"

using std::vector;

using WhatsAcppi::Util::Rc4;

Rc4::Rc4(const char *key, size_t size, int drop){
	for(i=0; i < RC4_S_LENGTH; i++){
		s[i] = i;
	}

	for(i=0; i < RC4_S_LENGTH; i++){
		j = (char) ((j + s[i]) + key[i % size]);
		int tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
	}

	i = j = 0;

	vector<char> dropArray(drop, 0);
	this->cipher(dropArray);
}

void Rc4::cipher(char *data, size_t offset, size_t length){
	int i, j;
	while (length-- != 0) {
		i = (i + 1) % 0x100;
		j = (j + s[i]) % 0x100;

		int tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;

		data[offset] = (char) (data[offset] ^ ((char) s[(s[i] + s[j]) % 0x100]));
		offset++;
	}
}

void Rc4::cipher(vector<char> &data){
	this->cipher(data.data(), 0, data.size());
}

