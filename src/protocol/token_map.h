#ifndef __WHATS_ACPPI_TOKEN_MAP_H__
#define __WHATS_ACPPI_TOKEN_MAP_H__
#include <string>

namespace WhatsAcppi {
	namespace Protocol {
		/**
		 * @return -1: not found, 0>= token
		 */
		int tryGetToken(const std::string& token, bool& secondary);
	}
}

#endif
