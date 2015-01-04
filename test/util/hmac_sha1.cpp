extern "C" {
	#include <assert.h>
}

#include <vector>
#include <string>

#include <util/hmac_sha1.h>
#include <util/util.h>

using std::vector;
using std::string;

using WhatsAcppi::Util::hmac_sha1;
using WhatsAcppi::Util::hex2bin;

int main(){
	vector<char> input = hex2bin("add4c8376b49e1194f519911fcece95196d89531");
	vector<char> password = hex2bin("f55e2194d9a824a977f1c72c75543174e76fbf12");
	vector<char> result = hmac_sha1(password, input);
	vector<char> data = hex2bin("3817ae9676f11bdf16e0855229796946ef4af186");
	assert(data == result);

	vector<char> dataEmpty = hex2bin("fbdb1d1b18aa6c08324b7d64b71fb76370690e1d");
	vector<char> resultEmpty = hmac_sha1("", "");
	assert(dataEmpty == resultEmpty);

	return 0;
}

