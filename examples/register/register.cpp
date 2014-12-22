#include <iostream>
#include <string>
#include "../../src/phone.h"
#include "../../src/register.h"
#include "../../src/util/util.h"
#include "../../src/util/sha1.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

using WhatsAcppi::Phone;
using WhatsAcppi::Register;
using WhatsAcppi::Util::sha1Str;

int main(int argc, char*argv[]){
	if(argc < 4){
		cout << "Ussage:" << endl;
		cout << "\t" << argv[0] << " PHONE_NUMBER USERNAME CODE" << endl;
		return 0;
	}

	WhatsAcppi::Util::Init init;

	Phone phone(argv[1]);

	phone.guessPhoneInformation();

	#define PRINT(X) cout << #X << ": " << phone.get##X() << endl
	PRINT(PhoneNumber);
	PRINT(Phone);
	PRINT(Country);
	PRINT(Cc);
	PRINT(Iso3166);
	PRINT(Iso639);
	#undef PRINT

	cout << "** Registering code" << endl;

	string identity = sha1Str(argv[2]);
	Register reg(phone, identity);

	int ret = reg.codeRegister(argv[3]);
	if(ret < 0){
		cout << "Error in http request! ret:" << ret << endl;
		return -1;
	}else if(ret){
		cout << "Whats app error! ret: " << ret << endl;
		return 1;
	}

	cout << "Success! ret: " << ret << endl;
	cout << "Pw: " << reg.getPw() << endl;
	cout << "Login: " << reg.getLogin() << endl;

	return 0;
}
