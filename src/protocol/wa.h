#ifndef __WHATS_ACPPI_PROTOCOL_H__
#define __WHATS_ACPPI_PROTOCOL_H__

#include <vector>
#include <string>
#include <memory>

#include "../phone.h"
#include "../util/socket.h"
#include "key_stream.h"
#include "node.h"

namespace WhatsAcppi {
	namespace Protocol {
		class WA {
			public:
				/**
				*/
				WA(const Phone& phone, const std::string& identity, const std::string& nickname);
				//~WA();

				int connect();
				int login(const std::string& password);
				//bool isConnected();

			protected:
				typedef enum {
					DATA_READ=0,
					DATA_WRITE
				} ProcessType;
				Node createFeaturesNode();
				Node createAuthNode();
				int doLogin();
				std::vector<char> readStanza();
				bool pollMessage(bool autoReceipt=true, ProcessType type=DATA_READ);
				void processInboundData(std::vector<char>& data, bool autoReceipt, ProcessType type);
				void processInboundDataNode(Node &node, bool autoReceipt, ProcessType type);

				const Phone& phone;
				const std::string& identity;
				const std::string& nickname;
				std::shared_ptr<std::vector<char>> createAuthBlob();
				std::shared_ptr<std::vector<char>> challengeData;
				std::string password;

				std::unique_ptr<KeyStream> inputKey;
				std::unique_ptr<KeyStream> outputKey;
				Util::Socket sock;
		};
	}
}
#endif
