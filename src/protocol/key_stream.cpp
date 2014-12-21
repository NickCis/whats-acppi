#include "key_stream.h"

#include "../util/hmac_sha1.h"

#define DROP (0x300)

using std::vector;

using WhatsAcppi::Util::hmac_sha1;
using WhatsAcppi::Util::Rc4;
using WhatsAcppi::Protocol::KeyStream;

KeyStream::KeyStream(char k, char mk) :
	key(k),
	macKey(mk),
	seq(0),
	rc4(&key, 1, DROP)
{
}

KeyStream::~KeyStream(){
}

bool KeyStream::decodeMessage(vector<char>& buffer, int macOffset, int offset, int length){
	vector<char> mac = this->computeMac(buffer, offset, length);
	vector<char> hmac(4);
	hmac.insert(hmac.begin(), buffer.begin()+macOffset, buffer.begin()+macOffset+4);

	this->rc4.cipher(buffer.data(), offset, length);

	for(int i=0; i < 4; i++){
		if(hmac[i] != mac[i]){
			return false;
		}
	}
	return true;
}

void KeyStream::encodeMessage(vector<char>& buffer, int macOffset, int offset, int length){
	this->rc4.cipher(buffer.data(), offset, length);
	vector<char> mac = this->computeMac(buffer, offset, length);
	buffer.insert(buffer.begin()+macOffset, mac.begin(), mac.begin()+4);
}

vector<char> KeyStream::computeMac(const vector<char>& buffer, int offset, int length){
	vector<char> output;
	output.reserve(buffer.size());
	output.insert(output.begin(), buffer.begin()+offset, buffer.begin()+offset+length);
	output.push_back(this->seq >> 24);
	output.push_back(this->seq >> 16);
	output.push_back(this->seq >> 8);
	output.push_back(this->seq);
	this->seq++;

	return hmac_sha1(&key, 1, output.data(), output.size());
}
