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
using WhatsAcppi::Util::sha1;

int main(int argc, char*argv[]){
	if(argc < 3){
		cout << "Ussage:" << endl;
		cout << "\t" << argv[0] << " PHONE_NUMBER USERNAME [CARRIER NAME]" << endl;
		return 0;
	}

	WhatsAcppi::Util::Init init;

	Phone phone(argv[1]);

	if(argc > 3)
		phone.guessPhoneInformation(argv[3]);
	else
		phone.guessPhoneInformation();

	#define PRINT(X) cout << #X << ": " << phone.get##X() << endl
	PRINT(PhoneNumber);
	PRINT(Phone);
	PRINT(Country);
	PRINT(Cc);
	PRINT(Mcc);
	PRINT(Iso3166);
	PRINT(Iso639);
	PRINT(Mnc);
	#undef PRINT

	cout << "Registering" << endl;

	vector<char> identity = sha1(argv[2]);
	Register reg(phone, identity);

	int ret = reg.codeRequest("sms");

	cout << "return " << ret << endl;

	return 0;
}
