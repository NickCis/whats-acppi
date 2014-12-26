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
					DataRead=0,
					DataWrite
				} ProcessType;

				Node createFeaturesNode();
				Node createAuthNode();
				Node createAuthResponseNode();

				int doLogin();
				void authenticate();
				std::vector<char> readStanza();
				bool pollMessage(bool autoReceipt=true, ProcessType type=DataRead);
				void processInboundData(std::vector<char>& data, bool autoReceipt, ProcessType type);
				void processInboundDataNode(const Node &node, bool autoReceipt, ProcessType type);

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
