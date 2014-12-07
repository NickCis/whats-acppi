#include "phone.h"
#include "countries.h"

using WhatsAcppi::Phone;
using std::string;

Phone::Phone(const string& p) : phoneNumber(p){
}

Phone::~Phone(){
}

void Phone::guessPhoneInformation(const string& carrier){
	this->dissectPhone();

	if(! carrier.empty())
		this->detectMnc(carrier);
}

void Phone::dissectPhone(){
	for(int i=0; COUNTRIES_DATA[i].country != NULL; i++){
		if(phoneNumber.find(COUNTRIES_DATA[i].cc) == 0){
			#define COPY_VALUE(X) if(this->X.empty()) this->X = COUNTRIES_DATA[i].X
			COPY_VALUE(country);
			COPY_VALUE(cc);
			COPY_VALUE(mcc);
			COPY_VALUE(iso3166);
			COPY_VALUE(iso639);
			COPY_VALUE(mnc);
			#undef COPY_VALUE
			break;
		}
	}

	if(this->iso3166.empty())
		this->iso3166 = "US";

	if(this->iso639.empty())
		this->iso3166 = "en";

	this->phone = this->phoneNumber.substr(this->cc.length());
}

void Phone::detectMnc(const string& carrier){
}

const string& Phone::getCountry() const {
	return this->country;
}

const string& Phone::getCc() const {
	return this->cc;
}

const string& Phone::getPhone() const {
	return this->phone;
}

const string& Phone::getPhoneNumber() const {
	return this->phoneNumber;
}

const string& Phone::getMcc() const {
	return this->mcc;
}
const string& Phone::getIso3166() const {
	return this->iso3166;
}

const string& Phone::getIso639() const {
	return this->iso639;
}

const string& Phone::getMnc() const {
	return this->mnc;
}
