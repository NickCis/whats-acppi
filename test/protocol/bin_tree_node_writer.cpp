extern "C" {
	#include <assert.h>
}

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <map>

#include <util/log.h>
#include <util/util.h>
#include <protocol/bin_tree_node_writer.h>

using std::map;
using std::vector;
using std::string;
using std::shared_ptr;

using WhatsAcppi::Util::Log;
using WhatsAcppi::Util::hex2bin;
using WhatsAcppi::Protocol::Node;
using WhatsAcppi::Protocol::BinTreeNodeWriter;


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

int main(){
	BinTreeNodeWriter writer;
	vector<char> data = hex2bin("000028f80659fc04666f726dfc03616263a5fc0378797af801f80457ec63fc03313233fc06313233343536");

	Log::setLogLevel(Log::DebugMsg);

	Node targetNode = buildTargetNode();
	writer.write(targetNode);
	const vector<char>& result = writer.getData();

	assert(data == result);

	return 0;
}
