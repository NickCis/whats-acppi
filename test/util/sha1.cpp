extern "C" {
	#include <assert.h>
}

#include <vector>
#include <string>

#include <util/sha1.h>
#include <util/util.h>

using std::vector;
using std::string;

using WhatsAcppi::Util::sha1;
using WhatsAcppi::Util::sha1Str;
using WhatsAcppi::Util::hex2bin;

int main(){
	string hexData = "2432351e81c9fe124cd3eec2aea09777abea99d1";
	vector<char> data = hex2bin(hexData);
	vector<char> input = hex2bin("add4c8376b49e1194f519911fcece95196d89531");
	vector<char> result = sha1(input);

	assert(data == result);

	string hexResult = sha1Str(input);

	assert(hexData == hexResult);

	return 0;
}
