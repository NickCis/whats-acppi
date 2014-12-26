#ifndef __WHATS_ACPPI_PBKDF2_H__
#define __WHATS_ACPPI_PBKDF2_H__

#include <vector>

namespace WhatsAcppi {
	namespace Util {
		std::vector<char> pbkdf2(const std::vector<char>& password, const std::vector<char>& salt, int iterations);
	}
}
#endif
