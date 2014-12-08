#ifndef __WHATS_ACPPI_BASE64_H__
#define __WHATS_ACPPI_BASE64_H__

#include <string>
#include <vector>

namespace WhatsAcppi {
	namespace Util {
		std::string base64_encode(const char *buf, unsigned int bufLen);
		std::string base64_encode(const std::vector<char>& buf);
		std::vector<char> base64_decode(const std::string& encoded_string);
	}
}

#endif
