#include <iostream>
#include <string>
#include "../../src/phone.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

using WhatsAcppi::Phone;

int main(int argc, char*argv[]){
	if(argc < 2){
		cout << "Ussage:" << endl;
		cout << "\t" << argv[0] << " PHONE_NUMBER [CARRIER NAME]" << endl;
		return 0;
	}

	Phone phone(argv[1]);

	if(argc > 2)
		phone.guessPhoneInformation(argv[2]);
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

	return 0;
}
