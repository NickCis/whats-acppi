#include <iostream>
#include <string>
#include "../../src/phone.h"
#include "../../src/register.h"
#include "../../src/util/log.h"
#include "../../src/util/util.h"
#include "../../src/util/sha1.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

using WhatsAcppi::Phone;
using WhatsAcppi::Register;
using WhatsAcppi::Util::Log;
using WhatsAcppi::Util::sha1Str;

int main(int argc, char*argv[]){
	if(argc < 3){
		cout << "Ussage:" << endl;
		cout << "\t" << argv[0] << " PHONE_NUMBER USERNAME [METHOD: sms/voice] [CARRIER NAME]" << endl;
		return 0;
	}

	WhatsAcppi::Util::Init init;
	Log::setLogLevel(Log::DebugMsg);

	Phone phone(argv[1]);

	string method = "sms";
	if(argc > 3)
		method = argv[3];

	if(argc > 4)
		phone.guessPhoneInformation(argv[4]);
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


	string identity = sha1Str(argv[2]);
	Register reg(phone, identity);
	cout << "identity" << identity <<  endl;

	cout << "** Requesting code" << endl;

	int ret = reg.codeRequest("voice");
	if(ret < 0){
		cout << "Error in http request! ret:" << ret << endl;
		return -1;
	}else if(ret){
		cout << "Error in whats app! ret:" << ret << endl;
		cout << "Status: " << reg.getStatus() << endl;
		cout << "Reason: " << reg.getReason() << endl;
		cout << "Retry after: " << reg.getRetryAfter() << endl;

		return 1;
	}

	cout << "Success! ret: " << ret << endl;

	return 0;
}
