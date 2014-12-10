#ifndef __WHATS_ACPPI_JSON_H__
#define __WHATS_ACPPI_JSON_H__
#include <functional>
#include <vector>
#include <string>
#include <sstream>

namespace WhatsAcppi {
	namespace Util {
		void simpleJsonParser(const char* data, size_t l, std::function<void (const std::stringstream& name, /*const*/ std::stringstream& value)> func);
		void simpleJsonParser(const std::vector<char>& data, std::function<void (const std::stringstream& name, /*const*/ std::stringstream& value)> func);
		void simpleJsonParser(const std::string& data, std::function<void (const std::stringstream& name, /*const*/ std::stringstream& value)> func);
	}
}

#endif
