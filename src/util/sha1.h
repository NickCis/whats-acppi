#ifndef __WHATS_ACPPI_SHA1_H__
#define __WHATS_ACPPI_SHA1_H__

#include <string>
#include <vector>

namespace WhatsAcppi {
	namespace Util {
		std::vector<char> sha1(const std::vector<char> & data);
		std::string sha1Str(const std::vector<char> & data);
		std::vector<char> sha1(const std::string & data);
		std::string sha1Str(const std::string & data);
		std::vector<char> sha1(const char* data, size_t size);
		std::string sha1Str(const char* data, size_t size);
	}
}

#endif
