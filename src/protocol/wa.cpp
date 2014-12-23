#include "wa.h"
#include "node.h"
#include "bin_tree_node_writer.h"
#include "bin_tree_node_reader.h"
#include "../define.h"
#include "../util/log.h"
#include "../util/pbkdf2.h"
#include "../util/base64.h"

#include <cmath>
#include <sstream>

extern "C" {
	#include <time.h>
}

using std::vector;
using std::string;
using std::shared_ptr;
using std::unique_ptr;
using std::stringstream;

using WhatsAcppi::Phone;
using WhatsAcppi::Util::Log;
using WhatsAcppi::Util::pkbdf2;
using WhatsAcppi::Util::base64_decode;
using WhatsAcppi::Protocol::WA;
using WhatsAcppi::Protocol::Node;
using WhatsAcppi::Protocol::BinTreeNodeWriter;
using WhatsAcppi::Protocol::BinTreeNodeReader;

WA::WA(const Phone& p, const string& i, const string& n) :
	phone(p),
	identity(i),
	nickname(n),
	sock(WHATSAPP_HOST, WHATSAPP_PORT_STR)
{
}

int WA::connect(){
	if(this->sock.connect())
		return -1;

	this->sock.setTimeout(TIMEOUT_SEC, TIMEOUT_USEC);

	return 0;
}

int WA::login(const string& password){
	// TODO: verify conenct
	this->password = password;
	return this->doLogin();
}

int WA::doLogin(){
	stringstream ss;
	ss << WHATSAPP_DEVICE << "-" << WHATSAPP_VER << "-" << WHATSAPP_PORT_STR;

	BinTreeNodeWriter writer;

	writer.streamStart(WHATSAPP_SERVER, ss.str());
	Log() << "streamStart :: size " << writer.getData().size();
	Log() << writer.getData();
	this->sock.write(writer.getData());

	Node featuresNode = this->createFeaturesNode();
	writer.write(featuresNode);
	Log() << "featuresNode :: size " << writer.getData().size();
	Log() << writer.getData();
	this->sock.write(writer.getData());

	Node auth = this->createAuthNode();
	writer.write(auth);
	Log() << "authNode :: size " << writer.getData().size();
	Log() << writer.getData();
	this->sock.write(writer.getData());

	Log() << "Polling messages";

	if(!this->pollMessage())
		Log() << "Error 1";

	if(!this->pollMessage())
		Log() << "Error 2";

	if(!this->pollMessage())
		Log() << "Error 3";

	return 0;
}

Node WA::createFeaturesNode(){
	Node parent("stream:features");

	parent.addChild(shared_ptr<Node>(new Node("readreceipts")));
	parent.addChild(shared_ptr<Node>(new Node("groups_v2")));
	parent.addChild(shared_ptr<Node>(new Node("privacy")));
	parent.addChild(shared_ptr<Node>(new Node("presence")));

	return parent;
}

Node WA::createAuthNode(){
	Node node("auth");

	node.setAttribute("mechanism", "WAUTH-2");
	node.setAttribute("user", this->phone.getPhoneNumber());

	node.setData(this->createAuthBlob());

	return node;
}

shared_ptr<vector<char>> WA::createAuthBlob(){
	shared_ptr<vector<char>> data;
	if(this->challengeData && this->challengeData->size()){
		vector<char> key = pkbdf2(base64_decode(this->password), *this->challengeData, 16);

		inputKey = unique_ptr<KeyStream>(new KeyStream(key[2], key[3]));
		outputKey = unique_ptr<KeyStream>(new KeyStream(key[0], key[1]));

		//$this->reader->setKey($this->inputKey);
		////$this->writer->setKey($this->outputKey);

		data = shared_ptr<vector<char>>(new vector<char>(4, 0));
		const string &phoneNumber = this->phone.getPhoneNumber();
		time_t now = time(NULL);
		stringstream nowSs;
		nowSs << now;
		string userAgent(WHATSAPP_USER_AGENT);
		string mccMnc(" MccMnc/");
		const string &mcc = this->phone.getMcc();
		const string &mnc = this->phone.getMnc();

		data->reserve(data->size()+phoneNumber.size()+this->challengeData->size()+nowSs.str().size()+userAgent.size()+mccMnc.size()+mcc.size()+mnc.size());

		data->insert(data->end(), phoneNumber.c_str(), phoneNumber.c_str()+phoneNumber.size());
		data->insert(data->end(), this->challengeData->begin(), this->challengeData->end());
		data->insert(data->end(), nowSs.str().c_str(), nowSs.str().c_str()+nowSs.str().size());
		data->insert(data->end(), userAgent.c_str(), userAgent.c_str()+userAgent.size());
		data->insert(data->end(), mccMnc.c_str(), mccMnc.c_str()+mccMnc.size());
		data->insert(data->end(), mcc.c_str(), mcc.c_str()+mcc.size());
		data->insert(data->end(), mnc.c_str(), mnc.c_str()+mnc.size());

		this->outputKey->encodeMessage(*data, 0, 4, data->size() - 4);
	}

	return data;
}

vector<char> WA::readStanza(){
	vector<char> output;
	ssize_t sSize = this->sock.recv(output, 3);
	Log() << "sSize " << sSize;

	if(output.size() != 3){ // TODO: error
		return vector<char>();
	}

	unsigned int treeLength = (output[0] & 0x0F) << 16;
	treeLength |= output[1] << 8;
	treeLength |= output[2] << 0;

	Log() << "output[0] " << output[0];
	Log() << "output[1] " << output[1];
	Log() << "output[2] " << output[2];

	Log() << "treeLength " << treeLength;

	ssize_t readSize = this->sock.recv(output, treeLength);
	if(readSize <= 0){ // TODO: error
	}

	while(output.size() < treeLength){
		int toRead = treeLength - output.size();
		ssize_t readSize = this->sock.recv(output, toRead);

		if(readSize <= 0){ // TODO: error
			break;
		}
	}

	return output;
}

bool WA::pollMessage(bool autoReceipt, ProcessType type){
	vector<char> stanza = this->readStanza();
	if(stanza.size() > 0){
		Log() << "stanza size " << stanza.size();
		Log() << stanza;
		this->processInboundData(stanza, autoReceipt, type);
		return true;
	}
	return false;
}

void WA::processInboundData(std::vector<char>& data, bool autoReceipt, ProcessType type){
	BinTreeNodeReader reader;

	Node node = reader.nextTree(data, inputKey ? inputKey.get() : NULL);
	if(node){
		this->processInboundDataNode(node, autoReceipt, type);
	}
}

void WA::processInboundDataNode(Node &node, bool autoReceipt, ProcessType type){
	Log() << "Llego node -> tag: " << node.getTag();
	Log() << "id: " << node.getTag();
	Log() << "data:";
	Log() << node.getData();

	// TODO: implement handlers!
	if(node.getTag() == "challenge"){
		this->challengeData = node.getData();
	} else if(node.getTag() == "success"){
		this->challengeData = node.getData();
	}
}
