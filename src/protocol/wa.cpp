#include "wa.h"
#include "bin_tree_node_writer.h"
#include "node.h"
#include "../util/pbkdf2.h"
#include "../util/base64.h"

#include <cmath>
#include <sstream>

extern "C" {
	#include <time.h>
}

#define WHATSAPP_HOST "c.whatsapp.net"
#define WHATSAPP_PORT 443
#define WHATSAPP_PORT_STR "443"
#define WHATSAPP_DEVICE "Android"
#define WHATSAPP_VER "2.11.473"
#define WHATSAPP_SERVER "s.whatsapp.net"
#define WHATSAPP_USER_AGENT "WhatsApp/2.11.473 Android/4.3 Device/GalaxyS3"

#define TIMEOUT_SEC 2
#define TIMEOUT_USEC 0

using std::vector;
using std::string;
using std::shared_ptr;
using std::unique_ptr;
using std::stringstream;

using WhatsAcppi::Phone;
using WhatsAcppi::Util::pkbdf2;
using WhatsAcppi::Util::base64_decode;
using WhatsAcppi::Protocol::WA;
using WhatsAcppi::Protocol::Node;
using WhatsAcppi::Protocol::BinTreeNodeWriter;

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
	this->password = password;
	return this->doLogin();
}

int WA::doLogin(){
	stringstream ss;
	ss << WHATSAPP_DEVICE << "-" << WHATSAPP_VER << "-" << WHATSAPP_PORT_STR;

	BinTreeNodeWriter writer;

	writer.streamStart(WHATSAPP_SERVER, ss.str());
	this->sock.write(writer.getData());
	writer.clear();

	Node featuresNode = this->createFeaturesNode();
	writer.writeNode(featuresNode);
	this->sock.write(writer.getData());
	writer.clear();

	Node auth = this->createAuthNode();
	writer.writeNode(auth);
	this->sock.write(writer.getData());
	writer.clear();

	this->pollMessage();
	this->pollMessage();
	this->pollMessage();

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
	if(this->challengeData->size()){
		std::vector<char> base64_decode(const std::string& encoded_string);
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
	vector<char> output = this->sock.recv(3);

	if(output.size() != 3){ // TODO: error
		return vector<char>();
	}

	unsigned int treeLength = (output[0] & 0x0F) << 16;
	treeLength |= output[1] << 8;
	treeLength |= output[2] << 0;

	output.reserve(output.size() + treeLength);

	ssize_t readSize = this->sock.recv(output.data()+output.size(), treeLength);
	if(readSize <= 0){ // TODO: error
	}

	output.resize(output.size()+readSize);

	while(output.size() < treeLength){
		int toRead = treeLength - output.size();
		ssize_t readSize = this->sock.recv(output.data()+output.size(), toRead);

		if(readSize <= 0){ // TODO: error
			break;
		}

		output.resize(output.size()+readSize);
	}

	return output;
}

bool WA::pollMessage(){
	vector<char> stanza = this->readStanza();
	return true;
}
