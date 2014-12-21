#ifndef __WHATS_ACPPI_PROTOCOL_NODE_H__
#define __WHATS_ACPPI_PROTOCOL_NODE_H__

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace WhatsAcppi {
	namespace Protocol {
		class Node {
			public:
				Node(const std::string& tag);

				void setAttribute(const std::string& name, const std::string& value);
				void addChild(const std::shared_ptr<Node>& node);
				void setData(const std::shared_ptr<std::vector<char>>& data);

				const std::string& getTag() const;
				const std::map<std::string, std::string>& getAttributes() const;
				const std::vector<std::shared_ptr<Node>>& getChildren() const;
				const std::shared_ptr<std::vector<char>>& getData() const;

			protected:
				std::string tag;
				std::map<std::string, std::string> attributes;
				std::vector<std::shared_ptr<Node>> children;
				std::shared_ptr<std::vector<char>> data;
		};
	}
}

#endif