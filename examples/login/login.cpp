#include <iostream>
#include <string>
#include "../../src/phone.h"
#include "../../src/register.h"
#include "../../src/util/log.h"
#include "../../src/util/util.h"
#include "../../src/util/sha1.h"
#include "../../src/protocol/wa.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

using WhatsAcppi::Phone;
using WhatsAcppi::Util::Log;
using WhatsAcppi::Util::sha1Str;
using WhatsAcppi::Protocol::WA;

int main(int argc, char*argv[]){
	if(argc < 4){
		cout << "Ussage:" << endl;
		cout << "\t" << argv[0] << " PHONE_NUMBER USERNAME PASSWORD [CARRIER NAME]" << endl;
		return 0;
	}

	WhatsAcppi::Util::Init init;
	Log::setLogLevel(Log::DebugMsg);

	Phone phone(argv[1]);
	if(argc > 4)
		phone.guessPhoneInformation(argv[3]);
	else
		phone.guessPhoneInformation();

	#define PRINT(X) cout << #X << ": " << phone.get##X() << endl
	PRINT(PhoneNumber);
	PRINT(Phone);
	PRINT(Country);
	PRINT(Cc);
	PRINT(Iso3166);
	PRINT(Iso639);
	#undef PRINT

	cout << "** Trying to login" << endl;

	string identity = sha1Str(argv[2]);

	WA wa(phone, identity, argv[2]);
	wa.connect();
	wa.login(argv[3]);

	return 0;
}
