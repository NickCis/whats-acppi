#ifndef __WHATS_ACPPI_UTIL_H__
#define __WHATS_ACPPI_UTIL_H__

#include <vector>
#include <string>

namespace WhatsAcppi {
	namespace Util {
		class Init {
			public:
				Init();
				~Init();
		};

		std::vector<char> hex2bin(const std::string &hex);
		std::string bin2hex(const std::vector<char> &bin);
	}
}

#endif
