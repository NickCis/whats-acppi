#ifndef __WHATS_ACPPI_HMAC_SHA1_H__
#define __WHATS_ACPPI_HMAC_SHA1_H__

#include <string>
#include <vector>

namespace WhatsAcppi {
	namespace Util {
		std::vector<char> hmac_sha1(const char* key, size_t keySize, const char* data, size_t size);
	}
}

#endif
