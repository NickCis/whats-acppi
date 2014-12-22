#ifndef __WHATS_ACPPI_BIN_TREE_NODE_READER_H__
#define __WHATS_ACPPI_BIN_TREE_NODE_READER_H__

#include <vector>

#include "key_stream.h"
#include "node.h"

namespace WhatsAcppi {
	namespace Protocol {
		class BinTreeNodeReader {
			public:
				BinTreeNodeReader();
				Node nextTree(std::vector<char>& data, KeyStream* key);

			protected:
				Node nextTreeInternal(const std::vector<char> & data);
				/** Methods for peeking data
				 */
				int peek24(const std::vector<char>& data, int offset = 0) const;
				int peek16(const std::vector<char>& data,int offset = 0) const;
				int peek8(const std::vector<char>& data,int offset = 0) const;

				/** Methods for reading data, (they increment the counter)
				 */
				int read24(const std::vector<char>& data);
				int read16(const std::vector<char>& data);
				int read8(const std::vector<char>& data);

				/** Reads 'string' from data according to the token
				 */
				std::vector<char> readString(const std::vector<char>& data, int token);

				/** Reads len from data (increments counter);
				 * @param data
				 * @param len: length desired to be read
				 * @return data that has benn read
				 */
				std::vector<char> fillArray(const std::vector<char>&data, int len);

				std::string getToken(const std::vector<char>& data, int token);

				std::vector<char> readNibble(const std::vector<char>& data);

				int readListSize(const std::vector<char> & data, int token);

				bool isListTag(int token);
				void readList(std::vector<char>&data, int token, Node &node);

				void readAttributes(const std::vector<char>& data, int size, Node &node);

				size_t pos; // -> Counter of the data read
		};
	}
}
#endif
