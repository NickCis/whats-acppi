#ifndef __WHATS_ACPPI_RC4_H__
#define __WHATS_ACPPI_RC4_H__

#include <vector>
#include <memory>

#define RC4_S_LENGTH (0x100)

namespace WhatsAcppi {
	namespace Util {
		class Rc4 {
			public:
				Rc4(const char* key, size_t size, int drop);
				void cipher(char *data, size_t offset, size_t length);
				void cipher(std::vector<char> &data);

			protected:
				int s[RC4_S_LENGTH];
				int i;
				int j;
		};
	}
}

#endif
