#include "hmac_sha1.h"
#include "sha1.h"

using std::vector;
using std::string;

using WhatsAcppi::Util::sha1;

vector<char> WhatsAcppi::Util::hmac_sha1(const char* key, size_t keySize, const char* data, size_t size){
	vector<char> myKey;

	if(keySize > SHA1_BIN_LENGTH)
		myKey = sha1(key, keySize);
	else if(keySize < SHA1_BIN_LENGTH){
		myKey.reserve(SHA1_BIN_LENGTH);
		myKey.insert(myKey.begin(), key, key+keySize);
		int max = SHA1_BIN_LENGTH - keySize;
		for(int i=0; i < max; i++)
			myKey.push_back(0);
	}

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
