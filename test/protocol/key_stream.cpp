extern "C" {
	#include <assert.h>
}

#include <memory>
#include <vector>
#include <string>

#include <util/util.h>
#include <protocol/key_stream.h>

using std::vector;
using std::string;
using std::shared_ptr;

using WhatsAcppi::Util::hex2bin;
using WhatsAcppi::Protocol::KeyStream;

const char *keys_expected[] = {
	"ff0f083bb974fa9411b865b53098bec671689ee0",
	"35f181b39a8bc37805ac4ce1725197e80789f4fc",
	"208d1f7e29359500aeefb502dc566f5dc53376e0",
	"4b1c1e4ac9cd2a1e1087b0905492a7a6eed98e65"
};

int main(){
	vector<char> input = hex2bin("add4c8376b49e1194f519911fcece95196d89531");
	vector<char> password = hex2bin("f55e2194d9a824a977f1c72c75543174e76fbf12");
	vector<shared_ptr<vector<char>>> keys = KeyStream::generateKeys(password, input);

	int i = 0;
	for(auto it = keys.begin(); it != keys.end(); it++){
		assert(hex2bin(string(keys_expected[i])) == (*(*it)));
		i++;
	}

	KeyStream outputKey(keys[0], keys[1]);
	vector<char> message = hex2bin("00000000353431313335363833303132b58a99f63c335469f45d1305688ddb5b32492e4a");
	outputKey.encodeMessage(message, 0, 4, message.size() -4);
	vector<char> expected = hex2bin("64db21c6f2f3607bf2e73443e8cdd47b182e45d7d9954b7aeec56ff8f9b189f5fb0eed1b");
	assert(message == expected);

	return 0;
}

