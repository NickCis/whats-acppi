extern "C" {
	#include <assert.h>
}

#include <vector>
#include <string>

#include <util/rc4.h>
#include <util/util.h>

using std::vector;
using std::string;

using WhatsAcppi::Util::Rc4;
using WhatsAcppi::Util::hex2bin;

int main(){
	const string keyPlain = "Key";
	Rc4 rc4_plain(keyPlain.data(), keyPlain.size(), 0);
	string plainText_str =  "Plaintext";
	vector<char> plainText;
	plainText.insert(plainText.end(), plainText_str.begin(), plainText_str.end());
	rc4_plain.cipher(plainText.data(), 0, plainText.size());
	vector<char> data = hex2bin("bbf316e8d940af0ad3");
	assert(plainText == data);

	vector<char> key = hex2bin("ff0f083bb974fa9411b865b53098bec671689ee0");
	Rc4 rc4(key.data(), key.size(), 0x300);
	vector<char> result = hex2bin("add4c8376b49e1194f519911fcece95196d89531");
	rc4.cipher(result.data(), 0, result.size());
	vector<char> expected = hex2bin("6a13997daa9be36294ac7c5851483570737e8a22");
	assert(result == expected);

	return 0;
}

