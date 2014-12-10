#ifndef __WHATS_ACPPI_PHONE_H__
#define __WHATS_ACPPI_PHONE_H__
#include <string>

namespace WhatsAcppi {
	/** Class representing a phone.
	 * It does all the job of guessing phone information (if it isn't provided)
	 */
	class Phone {
		public:
			/** Construct a phone.
			 * @param phoneNumber: Phone number or username, country code + number without + or 00)
			 */
			Phone(const std::string& phoneNumber);
			Phone(const Phone& p);
			Phone& operator=(const Phone& p);

			~Phone();

			/**  Getters (see properties for description)
			 */
			const std::string& getCountry() const;
			const std::string& getCc() const;
			const std::string& getPhoneNumber() const;
			const std::string& getPhone() const;
			const std::string& getMcc() const;
			const std::string& getIso3166() const;
			const std::string& getIso639() const;
			const std::string& getMnc() const;

			/** This method will try to guess phone information that hasn't been setted.
			 * If you are not setting all the values, you should call this method!
			 */
			void guessPhoneInformation(const std::string& carrier = std::string());

			// TODO: add setters (to avoid automatic recongnition)

		protected:
			std::string phoneNumber; // -> Phone number or username, country code + number without + or 00)
			std::string iso3166; // -> ISO3166 country code (uppercase) eg: "AR" for argentina
			std::string iso639; // -> ISO639 language code (lowercase) eg: "es" for spanish
			std::string country; // -> Country name. eg: "Argentina" for Argentina
			std::string cc; // -> Country code. eg: 54 for Argentina
			std::string mcc; // -> mcc
			std::string mnc; // -> mnc
			std::string phone; // -> phone number without country code, + and 00

			void dissectPhone();
			void detectMnc(const std::string& carrier = std::string());
	};
}
#endif
