#include "phone.h"
#include "countries.h"
#include "network_info.h"

#include <algorithm>
#include <locale>

using WhatsAcppi::Phone;
using std::string;

Phone::Phone(const string& p) : phoneNumber(p){
}

#define COPY_VALUE(X) X(p.X)
Phone::Phone(const Phone& p) :
	COPY_VALUE(phoneNumber),
	COPY_VALUE(iso3166),
	COPY_VALUE(iso639),
	COPY_VALUE(country),
	COPY_VALUE(cc),
	COPY_VALUE(mcc),
	COPY_VALUE(mnc),
	COPY_VALUE(phone)
{
}
#undef COPY_VALUE

Phone& Phone::operator=(const Phone& p){
	if(this != &p){
		#define COPY_VALUE(X) this->X = p.X
		COPY_VALUE(phoneNumber);
		COPY_VALUE(iso3166);
		COPY_VALUE(iso639);
		COPY_VALUE(country);
		COPY_VALUE(cc);
		COPY_VALUE(mcc);
		COPY_VALUE(mnc);
		COPY_VALUE(phone);
		#undef COPY_VALUE
	}
	return *this;
}

Phone::~Phone(){
}

void Phone::guessPhoneInformation(const string& carrier){
	this->dissectPhone();
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
	if(carrier.empty())
		return;

	string iso3166Lower(this->iso3166);
	std::transform(iso3166Lower.begin(), iso3166Lower.end(), iso3166Lower.begin(), ::tolower);

	for(int i=0; NETWORK_INFO[i].mnc != NULL; i++){
		if(iso3166Lower == NETWORK_INFO[i].iso3166 && carrier == NETWORK_INFO[i].carrierName){
			this->mnc = NETWORK_INFO[i].mnc;
			break;
		}
	}
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
