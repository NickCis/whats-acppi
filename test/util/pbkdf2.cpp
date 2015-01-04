extern "C" {
	#include <assert.h>
}

#include <vector>
#include <string>

#include <util/pbkdf2.h>
#include <util/util.h>

using std::vector;
using std::string;

using WhatsAcppi::Util::pbkdf2;
using WhatsAcppi::Util::hex2bin;

int main(){
	vector<char> input = hex2bin("add4c8376b49e1194f519911fcece95196d89531");
	vector<char> password = hex2bin("f55e2194d9a824a977f1c72c75543174e76fbf12");
	vector<char> result = pbkdf2(password, input, 2);
	vector<char> data = hex2bin("50ca64bfbb369e3b6b24ee954b720b4b3a575ba6");
	assert(data == result);

	return 0;
}

