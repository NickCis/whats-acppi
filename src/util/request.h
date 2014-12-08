#ifndef __WHATS_ACPPI_REQEUST__
#define __WHATS_ACPPI_REQEUST__

#include <string>
#include <memory>
#include <vector>
#include <sstream>

namespace WhatsAcppi {
	namespace Util {
	/** A blocking HTTPS request
	 */
		class Request {
			public:
				Request(const std::string& host, const std::string& protocol = "https");
				void setUrlParam(const std::string& name, const std::string& value);
				void setUrlParam(const std::string& name, const std::vector<char>& value);
				void setUrlParam(const std::string& name, const int& value);
				void setHeader(const std::string& header);
				void setHeader(const char* header);
				int get();
				~Request();
				std::string urlEncode(const std::string& data);
				std::string urlEncode(const std::vector<char>& data);
				std::string urlEncode(const char *data, size_t size);

				const std::vector<char>& getData() const;

			protected:
				class RequestInternal;
				std::unique_ptr<RequestInternal> me;
				std::string host;
				std::string protocol;
				std::stringstream params;
				std::vector<char> response;
		};
	}
}

#endif
