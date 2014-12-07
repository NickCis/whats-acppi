#ifndef __WHATS_ACPPI_PHONE_H__
#define __WHATS_ACPPI_PHONE_H__
#include <string>

namespace WhatsAcppi {
	class Phone {
		public:
			Phone(
				const std::string& phoneNumber
			);
			Phone(const Phone& p);
			Phone& operator=(const Phone& p);

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

			void guessPhoneInformation(const std::string& carrier = std::string());

			// TODO: add setters (to avoid automatic recongnition)

		protected:
			std::string phoneNumber;
			std::string iso3166;
			std::string iso639;
			std::string country;
			std::string cc;
			std::string mcc;
			std::string mnc;
			std::string phone;

			void dissectPhone();
			void detectMnc(const std::string& carrier = std::string());
	};
}
#endif
