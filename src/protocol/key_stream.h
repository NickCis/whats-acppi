#ifndef __WHATS_ACPPI_KEYSTREAM_H__
#define __WHATS_ACPPI_KEYSTREAM_H__

#include <vector>
#include <string>

#include "../util/rc4.h"

namespace WhatsAcppi {
	namespace Protocol {
		class KeyStream {
			public:
				KeyStream(char key, char macKey);
				~KeyStream();
				bool decodeMessage(std::vector<char>& buffer, int macOffset, int offset, int length);
				void encodeMessage(std::vector<char>& buffer, int macOffset, int offset, int length);
				std::vector<char> computeMac(const std::vector<char>& buffer, int offset, int length);

			protected:
				char key;
				char macKey;
				int seq;
				Util::Rc4 rc4;
		};
	}
}

#endif
