#include "rc4.h"
#include "log.h"


#define RC4_S_LENGTH (0x100)

using std::vector;

using WhatsAcppi::Util::Rc4;

Rc4::Rc4(const char *key, size_t size, int drop){
	s.resize(256);
	for(i=0; i < 256; i++)
		s[i] = i;

	for(i = j = 0; i < 256; i++){
		j = (j + key[i % size] + s[i]) & 255;
		this->swap(i, j);
	}

	i = j = 0;

	vector<char> dropArray(drop, 0);
	this->cipher(dropArray.data(), 0, dropArray.size());
}

void Rc4::cipher(char *data, size_t offset, size_t length){
	for(size_t n = 0; n < length; n++){
		i = (i + 1) & 255;
		j = (j + s[i]) & 255;
		this->swap(i, j);
		data[offset] = (unsigned char) data[offset] ^ ((unsigned char) (s[(s[i] + s[j]) & 255]));
		offset++;
	}
}

void Rc4::cipher(vector<char> &data){
	this->cipher(data.data(), 0, data.size());
}


void Rc4::swap(unsigned int i, unsigned int j){
	unsigned char c = s[i];
	s[i] = s[j];
	s[j] = c;
}
