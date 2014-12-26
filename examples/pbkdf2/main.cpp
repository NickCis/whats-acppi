#include <iostream>
#include <string>
#include <memory>
#include "../../src/util/log.h"
#include "../../src/util/rc4.h"
#include "../../src/util/sha1.h"
#include "../../src/util/util.h"
#include "../../src/util/base64.h"
#include "../../src/util/pbkdf2.h"
#include "../../src/util/hmac_sha1.h"
#include "../../src/protocol/key_stream.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::shared_ptr;

using WhatsAcppi::Util::Log;
using WhatsAcppi::Util::Rc4;
using WhatsAcppi::Util::sha1;
using WhatsAcppi::Util::pbkdf2;
using WhatsAcppi::Util::hex2bin;
using WhatsAcppi::Util::hmac_sha1;
using WhatsAcppi::Util::base64_decode;
using WhatsAcppi::Protocol::KeyStream;

const char *keys_expected[] = {
	"ff0f083bb974fa9411b865b53098bec671689ee0",
	"35f181b39a8bc37805ac4ce1725197e80789f4fc",
	"208d1f7e29359500aeefb502dc566f5dc53376e0",
	"4b1c1e4ac9cd2a1e1087b0905492a7a6eed98e65"
};

int main(/*int argc, char*argv[]*/){
	WhatsAcppi::Util::Init init;
	Log::setLogLevel(Log::DebugMsg);

	const string base64Password = "9V4hlNmoJKl38ccsdVQxdOdvvxI=";
	vector<char>  password = base64_decode(base64Password);
	Log() << "base64Password: " << base64Password;
	Log() << "password: " << password;

	const string hexNonce = "add4c8376b49e1194f519911fcece95196d89531";
	vector<char> nonce = hex2bin(hexNonce);
	Log() << "hexNonce: " << hexNonce;
	Log() << "nonce: " << nonce;

	vector<char> sha1Nonce = sha1(nonce);
	Log() << "sha1Nonce: " << sha1Nonce << " -> expected:  2432351e81c9fe124cd3eec2aea09777abea99d1";

	vector<char> hmacNonce = hmac_sha1(password, nonce);
	Log() << "hmacNonce: " << hmacNonce << " -> expected: 3817ae9676f11bdf16e0855229796946ef4af186";

	Log() << "hmac empty: " << hmac_sha1("", "") << " -> expected: fbdb1d1b18aa6c08324b7d64b71fb76370690e1d"; 

	Log() << "pbkdf2(nonce): " << pbkdf2(password, nonce, 2) << " -> expected: 50ca64bfbb369e3b6b24ee954b720b4b3a575ba6";

	vector<shared_ptr<vector<char>>> keys = KeyStream::generateKeys(password, nonce);
	int i = 0;
	for(auto it = keys.begin(); it != keys.end(); it++){
		Log() << "key" << i << ": " << (*(*it)) << " -> expected: " << keys_expected[i];
		i++;
	}

	const string keyPlain = "Key";
	Rc4 rc4_plain(keyPlain.data(), keyPlain.size(), 0);
	string plainText_str =  "Plaintext";
	vector<char> plainText;
	plainText.insert(plainText.end(), plainText_str.begin(), plainText_str.end());
	auto plainTextSize = plainText.size();
	rc4_plain.cipher(plainText.data(), 0, plainText.size());
	Log() << "rc4.cipher( plainText, 0, " << plainTextSize << "): " << plainText << " -> expected: bbf316e8d940af0ad3";

	Rc4 rc4(keys[0]->data(), keys[0]->size(), 0x300);
	vector<char> cipherNonce = nonce;
	Log() << "nonce: " << cipherNonce;
	auto nonceSize = cipherNonce.size();
	rc4.cipher(cipherNonce.data(), 0, cipherNonce.size());
	Log() << "rc4.cipher( nonce, 0, " << nonceSize << "): " << cipherNonce << " -> expected: 6a13997daa9be36294ac7c5851483570737e8a22";

	KeyStream outputKey(keys[0], keys[1]);
	vector<char> message = hex2bin("00000000353431313335363833303132b58a99f63c335469f45d1305688ddb5b32492e4a");
	Log() << "message (" << message.size() << "): " << message;
	outputKey.encodeMessage(message, 0, 4, message.size() -4);
	Log() << "message encoded (" << message.size() << "): " << message << " -> expected: 64db21c6f2f3607bf2e73443e8cdd47b182e45d7d9954b7aeec56ff8f9b189f5fb0eed1b";

	return 0;
}
