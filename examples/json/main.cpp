#include <iostream>
#include <string>
#include <sstream>

#include "../../src/util/json.h"

using std::string;
using std::stringstream;
using std::cout;
using std::endl;

using WhatsAcppi::Util::simpleJsonParser;

int main(){
	const string json1 = "{\"status\":\"fail\",\"reason\":\"too_recent\",\"retry_after\":8988}";

	cout << "json1: \"" << json1 << "\"" << endl;
	cout << "pre" << endl;
	simpleJsonParser(json1, [](const stringstream& name, /*const*/ stringstream& value) {
		cout << "name: \"" << name.str() << "\" value: \"" << value.str() << "\"" << endl;
	});

	cout << "end" << endl;

	const string json2 = "{\"status\":\"sent\",\"length\":6,\"method\":\"sms\",\"retry_after\":25205}";

	cout << "json2: \"" << json2 << "\"" << endl;
	cout << "pre" << endl;
	simpleJsonParser(json2, [](const stringstream& name, /*const*/ stringstream& value) {
		cout << "name: \"" << name.str() << "\" value: \"" << value.str() << "\"" << endl;
	});

	cout << "end" << endl;

	return 0;
}
