#ifndef __WHATS_ACPPI_REGISTER_H__
#define __WHATS_ACPPI_REGISTER_H__
#include <string>
#include <vector>

#include "phone.h"

namespace WhatsAcppi {
	class Register {
		public:
			/** Create a Register Object.
			 * The phone must have all it's data setted, if you do not set some values, please call Phone::guessPhoneInformation(), before creating a Register
			 * @param phone[in]: Phone details. 
			 * @param identity[in]: identity, a posible identity si sha1Str(nickname). 
			 * @see WhatsAcppi::Phone::Phone, WhatsAcppi::Phone::guessPhoneInformation()
			 */
			Register(const Phone& phone, const std::string& identity);
			~Register();

			/** Request a verification code
			 * @param method[in]: the method for getting the code ("sms" or "voice")
			 * @return 0: Ok, < 0: Network error, > 0: Protocol error
			 */
			int codeRequest(const std::string& method);

			/** Register code
			 * @param method[in]: the code
			 * @return 0: Ok, < 0: Network error, > 0: Protocol error
			 */
			int codeRegister(const std::string& code);

			const std::string& getStatus() const;
			const std::string& getReason() const;
			const int& getRetryAfter() const;

		protected:
			const Phone& phone;
			std::string generateRequestToken();
			const std::string& identity;

			std::string status;
			std::string reason;
			int retryAfter;
	};
}

#endif
