#ifndef __WHATS_ACPPI_BIN_TREE_NODE_WRITER_H__
#define __WHATS_ACPPI_BIN_TREE_NODE_WRITER_H__

#include <map>
#include <string>
#include <vector>

#include "node.h"

namespace WhatsAcppi {
	namespace Protocol {
		class BinTreeNodeWriter {
			public:
				BinTreeNodeWriter();

				void streamStart(const std::string& domain, const std::string& resource);
				const std::vector<char>& getData() const;
				void clear();

				void writeNode(const Node& node);
				void encrypt();

			protected:
				std::vector<char> data;

				void writeListStart(int i);
				void writeAttributes(const std::map<std::string, std::string>& attributes);
				void writeString(const std::string& tag);
				void writeToken(const int& token);
				void write8(const char &c);
				void write16(const int &c);
				void write24(const int &c);
				void writeBytes(const std::vector<char>& bytes);
		};
	}
}

#endif
