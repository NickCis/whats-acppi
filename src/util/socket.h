#ifndef __WHATS_ACPPI_SOCKET_H__
#define __WHATS_ACPPI_SOCKET_H__

#include <vector>
#include <string>

namespace WhatsAcppi {
	namespace Util {
		class Socket {
			public:
				typedef enum Status {
					SOCK_UNKNOWN = 0,
					SOCK_CONNECTED,
					SOCK_DISCONNECTED,
					SOCK_ERROR
				} Status;
				Socket(const std::string& host, const std::string& port);
				~Socket();

				int connect();
				void setTimeout(int sec, int usec);
				int close();

				int write8(const char c);
				int write16(const int c);
				int write(const void* buf, size_t size);
				int write(const std::vector<char> &data);

				ssize_t recv(std::vector<char> &out, size_t len);
				ssize_t recv(char* buf, size_t len);

			protected:
				int sock;
				std::string host;
				std::string port;
				Status st;
		};
	}
}
#endif
