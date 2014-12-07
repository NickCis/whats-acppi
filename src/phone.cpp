#include "phone.h"
#include "countries.h"
namespace WhatsAcppi {
	Phone::Phone(
		const std::string& p,
		const std::string& c,
		const std::string& i3166,
		const std::string& i639
	) :
		isPhoneDisect(false),
		phoneNumber(p),
		carrier(c),
		iso3166(i3166),
		iso639(i639){
	}

	void Phone::dissectPhone(){
		for(int i=0; COUNTRIES_DATA[i].country != NULL; i++){
			if(phone.find(COUNTRIES_DATA[i].cc) == 0){
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
		this->isPhoneDisect = true;
	}

	const std::string& Phone::getCountry() const {
		return this->country;
	}

	const std::string& Phone::getCc() const {
		return this->cc;
	}

	const std::string& Phone::getPhone() const {
		return this->phone;
	}

	const std::string& Phone::getPhoneNumber() const {
		return this->phoneNumber;
	}

	const std::string& Phone::getMcc() const {
		return this->mcc;
	}
	const std::string& Phone::getIso3166() const {
		return this->iso3166;
	}

	const std::string& Phone::getIso639() const {
		return this->iso639;
	}

	const std::string& Phone::getMnc() const {
		return this->mnc;
	}

}
