#ifndef __WHATS_ACPPI_REGISTER_H__
#define __WHATS_ACPPI_REGISTER_H__
#include <string>
#include <vector>

#include "phone.h"

namespace WhatsAcppi {
	class Register {
		public:
			Register(const Phone& phone, const std::vector<char>& identity);
			~Register();

			int codeRequest(const std::string& method);

		protected:
			const Phone& phone;
			std::vector<char> generateRequestToken();
			const std::vector<char>& identity;
	};
}

#endif
