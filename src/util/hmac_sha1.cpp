#include "hmac_sha1.h"
#include "sha1.h"

using std::vector;
using std::string;

using WhatsAcppi::Util::sha1;

#define HMAC_SHA1_BLOCK_SIZE 64

vector<char> WhatsAcppi::Util::hmac_sha1(const char* key, size_t keySize, const char* data, size_t size){
	vector<char> myKey;
	myKey.reserve(HMAC_SHA1_BLOCK_SIZE);

	if(keySize > HMAC_SHA1_BLOCK_SIZE){
		myKey = sha1(key, keySize);
	}else{
		myKey.resize(0);
		myKey.insert(myKey.end(), key, key+keySize);
	}

	for(size_t i=keySize; i < HMAC_SHA1_BLOCK_SIZE; i++)
		myKey.push_back(0);


	vector<char> opad;
	vector<char> ipad;
	opad.reserve(SHA1_BIN_LENGTH + SHA1_BIN_LENGTH);
	ipad.reserve(SHA1_BIN_LENGTH+size);

	for(size_t i=0; i < myKey.size(); i++){
		opad.push_back(myKey[i] ^ 0x5C);
		ipad.push_back(myKey[i] ^ 0x36);
	}

	ipad.insert(ipad.end(), data, data+size);

	vector<char> ipadSha1 = sha1(ipad);

	opad.insert(opad.end(), ipadSha1.begin(), ipadSha1.end());

	return sha1(opad);
}

vector<char> WhatsAcppi::Util::hmac_sha1(const std::vector<char> &key, const std::vector<char> &data){
	return WhatsAcppi::Util::hmac_sha1(key.data(), key.size(), data.data(), data.size());
}

vector<char> WhatsAcppi::Util::hmac_sha1(const string &key, const string &data){
	return WhatsAcppi::Util::hmac_sha1(key.data(), key.size(), data.data(), data.size());
}
