#include "json.h"

using std::string;
using std::vector;
using std::function;
using std::stringstream;

void WhatsAcppi::Util::simpleJsonParser(const char* data, size_t l, function<void (const stringstream& name, /*const*/ stringstream& value)> func){
	stringstream name;
	stringstream value;
	bool inName = false,
		 inValue = false,
		 lookForNameSep = true,
		 lookForValueSep = false;

	char c;

	for(size_t i=0; i < l /*&& (c = data[i])*/; i++){
		c = data[i];
		if(c == '"'){
			if(lookForNameSep){
				if(inName){
					inName = false;
					lookForNameSep = false;
					lookForValueSep = true;
				}else{

					inName = true;
				}
			}else if(lookForValueSep){
				if(inValue){
					inValue = false;
					lookForNameSep = true;
					lookForValueSep = false;
					func(name, value);
					name.str("");
					name.clear();
					value.str("");
					value.clear();
				}else{
					inValue = true;
				}
			}
			continue;
		}

		if(inName){
			name << c;
		}else{
			if(lookForValueSep && !inValue){
				switch(c){
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						inValue = true;
						lookForValueSep = false;
						break;
					default:
						break;
				}
			}
			if(inValue){
				if(lookForValueSep){
					value << c;
				}else{
					switch(c){
						case '-':
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
						case '.':
							value << c;
							break;
						case ',':
							inValue = false;
							lookForNameSep = true;
							lookForValueSep = false;
							func(name, value);
							name.str("");
							name.clear();
							value.str("");
							value.clear();
							break;
						default:
							break;
					}
				}
			}
		}
	}
	if(!name.str().empty())
		func(name, value);
}

void WhatsAcppi::Util::simpleJsonParser(const string& data, std::function<void (const stringstream& name, /*const*/ stringstream& value)> func){
	simpleJsonParser(data.c_str(), data.size(), func);
}

void WhatsAcppi::Util::simpleJsonParser(const vector<char>& data, std::function<void (const stringstream& name, /*const*/ stringstream& value)> func){
	simpleJsonParser(data.data(), data.size(), func);
}
