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
	Log() << __func__ << " :: streamStart :: writer.size() " << writer.getData().size() << " writer.data() " << writer.getData();
	this->sock.write(writer.getData());

	Node featuresNode = this->createFeaturesNode();
	writer.write(featuresNode);
	Log() << __func__ << " :: featuresNode :: writer.size() " << writer.getData().size() << " writer.data() " << writer.getData();
	Log() << featuresNode.toString();
	this->sock.write(writer.getData());

	Node auth = this->createAuthNode();
	writer.write(auth);
	Log() << __func__ << " :: authNode :: writer.size() " << writer.getData().size() << " writer.data() " << writer.getData();
	Log() << auth.toString();
	this->sock.write(writer.getData());


	if(!this->pollMessage())
		Log(Log::WarningMsg) << "Error 1";

	if(!this->pollMessage())
		Log(Log::WarningMsg) << "Error 2";

	if(!this->pollMessage())
		Log(Log::WarningMsg) << "Error 3";


	// TODO: createAuthResponse!

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
	this->sock.recv(output, 3);

	if(output.size() != 3){ // TODO: error
		Log(Log::WarningMsg) << __func__ << " :: Error reding stanza. size: " << output.size();
		Log() << output;
		return vector<char>();
	}

	unsigned int treeLength = (output[0] & 0x0F) << 16;
	treeLength |= output[1] << 8;
	treeLength |= output[2] << 0;

	ssize_t readSize = this->sock.recv(output, treeLength);
	if(readSize <= 0){ // TODO: error
		Log(Log::WarningMsg) << __func__ << " :: Error. readSize: " << readSize;
		return vector<char>();
	}

	while(output.size() < treeLength){
		int toRead = treeLength - output.size();
		ssize_t readSize = this->sock.recv(output, toRead);

		if(readSize <= 0){ // TODO: error
			Log(Log::WarningMsg) << "Error. while: readSize: " << readSize;
			break;
		}
	}

	return output;
}

bool WA::pollMessage(bool autoReceipt, ProcessType type){
	vector<char> stanza = this->readStanza();
	if(stanza.size() > 0){
		this->processInboundData(stanza, autoReceipt, type);
		return true;
	}
	return false;
}

void WA::processInboundData(std::vector<char>& data, bool autoReceipt, ProcessType type){
	BinTreeNodeReader reader;

	Log() << "Msg arrive - size: " << data.size() << " data: " << data;

	Node node = reader.nextTree(data, inputKey ? inputKey.get() : NULL);
	if(node){
		this->processInboundDataNode(node, autoReceipt, type);
	}else{
		Log() << "Invalid node";
	}
}

void WA::processInboundDataNode(Node &node, bool autoReceipt, ProcessType type){
	Log() << node.toString();

	// TODO: implement handlers!
	if(node.getTag() == "challenge"){
		this->challengeData = node.getData();
	} else if(node.getTag() == "success"){
		this->challengeData = node.getData();
	}
}
