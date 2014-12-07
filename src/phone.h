#ifndef __WHATS_ACPPI_PHONE_H__
#define __WHATS_ACPPI_PHONE_H__
#include <string>

namespace WhatsAcppi {
	class Phone {
		public:
			Phone(
				const std::string& phoneNumber,
				const std::string& carrier = std::string(),
				const std::string& iso3166 = std::string(),
				const std::string& iso639 = std::string()
			);

			~Phone();

			const std::string& getCountry() const;
			const std::string& getCc() const;
			// International format
			const std::string& getPhoneNumber() const;
			// Without cc
			const std::string& getPhone() const;
			const std::string& getMcc() const;
			const std::string& getIso3166() const;
			const std::string& getIso639() const;
			const std::string& getMnc() const;

			// TODO: add setters (to avoid automatic recongnition)

		protected:
			bool isPhoneDisect;
			std::string phoneNumber;
			std::string carrier;
			std::string iso3166;
			std::string iso639;
			std::string country;
			std::string cc;
			std::string mcc;
			std::string mnc;
			std::string phone;

			void dissectPhone();
			void detectMnc();
	};
}
#endif
