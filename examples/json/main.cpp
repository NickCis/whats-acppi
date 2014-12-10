#include <iostream>
#include <string>
#include <sstream>

#include "../../src/util/json.h"

using std::string;
using std::stringstream;
using std::cout;
using std::endl;

using WhatsAcppi::Util::simpleJsonParser;

#define PARSE_PRINT(X) \
	cout << #X << ": \"" << X << "\"" << endl;\
	cout << "pre" << endl;\
	simpleJsonParser(X, [](const stringstream& name, /*const*/ stringstream& value) {\
		cout << "name: \"" << name.str() << "\" value: \"" << value.str() << "\"" << endl;\
	});\
	cout << "end" << endl

int main(){
	const string json1 = "{\"status\":\"fail\",\"reason\":\"too_recent\",\"retry_after\":8988}";
	PARSE_PRINT(json1);

	const string json2 = "{\"status\":\"sent\",\"length\":6,\"method\":\"sms\",\"retry_after\":25205}";
	PARSE_PRINT(json2);

	const string json3 = "{\"status\":\"ok\",\"login\":\"54911XXXXXXXX\",\"pw\":\"XXXXXXXXXXXXXXXXXXXXXXXXXXXX\",\"type\":\"new\",\"expiration\":1449725133,\"kind\":\"free\",\"price\":\"US$0.99\",\"cost\":\"0.99\",\"currency\":\"USD\",\"price_expiration\":1421297918}";
	PARSE_PRINT(json3);

	return 0;
}
