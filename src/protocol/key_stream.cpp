#include "key_stream.h"

#include "../util/hmac_sha1.h"
#include "../util/pbkdf2.h"

#define DROP (0x300)

using std::vector;
using std::shared_ptr;

using WhatsAcppi::Util::Rc4;
using WhatsAcppi::Util::hmac_sha1;
using WhatsAcppi::Util::pbkdf2;
using WhatsAcppi::Protocol::KeyStream;

KeyStream::KeyStream(shared_ptr<vector<char>> k, shared_ptr<vector<char>> mk) :
	key(k),
	macKey(mk),
	seq(0),
	rc4(key->data(), key->size(), DROP)
{
}

KeyStream::KeyStream(char k, char mk) :
	key(new vector<char>(1, k)),
	macKey(new vector<char>(1, mk)),
	seq(0),
	rc4(key->data(), key->size(), DROP)
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

	//buffer.insert(buffer.begin()+macOffset, mac.begin(), mac.begin()+4);

	for(int i=0 ; i < 4; i++)
		buffer[macOffset+i] = mac[i];
}

vector<char> KeyStream::computeMac(const vector<char>& buffer, int offset, int length){
	vector<char> output;
	output.reserve(buffer.size()-offset);
	output.insert(output.end(), buffer.begin()+offset, buffer.begin()+offset+length);
	output.push_back(this->seq >> 24);
	output.push_back(this->seq >> 16);
	output.push_back(this->seq >> 8);
	output.push_back(this->seq);
	this->seq++;

	return hmac_sha1(macKey->data(), macKey->size(), output.data(), output.size());
}

vector<shared_ptr<vector<char>>> KeyStream::generateKeys(const vector<char>& password, const vector<char>& nonce){
	vector<shared_ptr<vector<char>>> output;
	vector<char> nnonce = nonce;
	nnonce.push_back(0);

	for(int i=1; i <= 4 ; i++){
		shared_ptr<vector<char>> key(new vector<char>);
		nnonce.back() = i;
		*key = pbkdf2(password, nnonce, 2);
		output.push_back(key);
	}

	return output;
}
