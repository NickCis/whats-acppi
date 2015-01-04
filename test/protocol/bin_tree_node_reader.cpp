extern "C" {
	#include <assert.h>
}

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <map>

#include <protocol/bin_tree_node_reader.h>
#include <util/util.h>
#include <util/log.h>

using std::map;
using std::vector;
using std::string;
using std::shared_ptr;

using WhatsAcppi::Util::Log;
using WhatsAcppi::Protocol::Node;
using WhatsAcppi::Protocol::BinTreeNodeReader;
using WhatsAcppi::Util::hex2bin;


Node buildTargetNode(){
	Node targetNode;
	targetNode.setTag("message");
	targetNode.setAttribute("form", "abc");
	targetNode.setAttribute("to", "xyz");

	shared_ptr<Node> mediaNode(new Node);
	mediaNode->setTag("media");
	mediaNode->setAttribute("width", "123");
	string _data = "123456";
	shared_ptr<vector<char>> data(new vector<char>(_data.begin(), _data.end()));
	mediaNode->setData(data);

	targetNode.addChild(mediaNode);

	return targetNode;
}

void equalNode(const Node& node1, const Node& node2){
	assert(node1.getTag() == node2.getTag());

	const map<string, string>& map1 = node1.getAttributes();
	const map<string, string>& map2 = node2.getAttributes();

	assert(map1.size() == map2.size());
	for(auto it = map1.begin(); it != map1.end(); it++){
		auto it2 = map2.find(it->first);

		assert(it2 != map2.end());

		assert(it->second == it2->second);
	}

	if(node1.getData() && node2.getData())
		assert((*(node1.getData())) == (*(node2.getData())));
	else
		assert(node1.getData() == node2.getData());

	assert(((node1.getChildren().size()) == (node2.getChildren().size())));

	if(node1.getChildren().size() && node2.getChildren().size()){
		assert(((node1.getChildren().size()) == (node2.getChildren().size())) == 1);
		equalNode((*(node1.getChildren()[0])), (*(node2.getChildren()[0])));
	}
}

int main(){
	BinTreeNodeReader reader;
	unsigned char _data[] = { 0, 0, 40, 248, 6, 89, 165, 252, 3, 120, 121, 122, 252, 4, 102, 111, 114, 109, 252, 3, 97, 98, 99, 248, 1, 248, 4, 87, 236, 99, 252, 3, 49, 50, 51, 252, 6, 49, 50, 51, 52, 53, 54 };
	vector<char> data(_data, _data + sizeof(_data)/sizeof(unsigned char));

	Log::setLogLevel(Log::DebugMsg);

	Node node = reader.nextTree(data);
	Node targetNode = buildTargetNode();

	equalNode(node, targetNode);

	return 0;
}
