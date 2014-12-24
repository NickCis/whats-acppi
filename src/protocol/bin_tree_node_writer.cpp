#include "bin_tree_node_writer.h"

#include "token_map.h"
#include "../util/log.h"

#include <memory>
extern "C" {
	#include <stdint.h>
}

using std::map;
using std::vector;
using std::string;
using std::shared_ptr;

using WhatsAcppi::Util::Log;
using WhatsAcppi::Protocol::KeyStream;
using WhatsAcppi::Protocol::tryGetToken;
using WhatsAcppi::Protocol::BinTreeNodeWriter;

BinTreeNodeWriter::BinTreeNodeWriter(){
}

void BinTreeNodeWriter::streamStart(const string& domain, const string& resource){
	this->clear();

	this->write8('W'); // 0x57
	this->write8('A'); // 0x41
	this->write8(1); // 0x01
	this->write8(5); // 0x05

	size_t headerPos = this->data.size(); // Es fija? 4?
	this->write24(0); // -> son 3 bytes

	std::map<std::string, std::string> attributes;
	// XXX: Is it importart attributes order?
	attributes["resource"] = resource;
	attributes["to"] = domain;
	this->writeListStart(attributes.size() * 2 + 1); // -> 0xf8 0x05

	this->write8(1); // -> 0x01

	this->writeAttributes(attributes);

	this->processBuffer(headerPos, NULL);
}

void BinTreeNodeWriter::writeListStart(int i){
	if(i == 0){
		this->write8(0);
	}else if(i < 256){
		this->write8(248);
		this->write8(i);
	}else{
		this->write8(249);
		this->write16(i);
	}
}

void BinTreeNodeWriter::writeAttributes(const map<string, string>& attributes){
	for(auto it = attributes.begin(); it != attributes.end(); it++){
		this->writeString(it->first);
		this->writeString(it->second);
	}
}

void BinTreeNodeWriter::writeString(const string& tag){
	int token;
	bool secondary;
	token = tryGetToken(tag, secondary);

	if(token >= 0){
		if(secondary)
			this->writeToken(236);

		this->writeToken(token);
		return;
	}

	int atIndex = tag.find('@');
	if (atIndex < 1){
		this->writeBytes(tag);
	}else{
		string user = tag.substr(0, atIndex);
		string server = tag.substr(atIndex);
		this->writeJid(user, server);
	}
}

void BinTreeNodeWriter::writeJid(const string& user, const string& server){
	this->write8(250);
	if(user.size() > 0)
		this->writeString(user);
	else
		this->writeToken(0);

	this->writeString(server);
}

void BinTreeNodeWriter::writeToken(const int& token){
	if(token < 0xf5){
		this->write8(token);
	}else{
		this->write8(0xfe);
		this->write8(token-0xf5);
	}
}

void BinTreeNodeWriter::write(const Node& node, KeyStream* key){
	this->clear();
	size_t headerPos = this->data.size(); // Es fija? 0?
	this->write24(0); // -> son 3 bytes

	if(node.getTag() == ""){
		this->write8(0);
	}else{
		this->writeNode(node);
	}

	this->processBuffer(headerPos, key);
}

void BinTreeNodeWriter::writeNode(const Node& node){
	int len = 1 + node.getAttributes().size() * 2;

	if(node.getChildren().size() > 0)
		len += 1;

	if(node.getData() && node.getData()->size() > 0)
		len += 1;

	this->writeListStart(len);
	this->writeString(node.getTag());
	this->writeAttributes(node.getAttributes());

	if(node.getData() && node.getData()->size() > 0)
		this->writeBytes(* node.getData());

	if(node.getChildren().size() > 0){
		this->writeListStart(node.getChildren().size());

		const vector<shared_ptr<Node>> &children = node.getChildren();

		for(auto it = children.begin(); it != children.end(); it++)
			this->writeNode(*(*it));
	}
}

void BinTreeNodeWriter::write8(const char &c){
	this->data.push_back(c);
}

void BinTreeNodeWriter::write16(const int &c){
	this->write8((char)(c & 0xFF00) >> 8);
	this->write8((char)(c & 0xFF));
}

const vector<char>& BinTreeNodeWriter::getData() const {
	return this->data;
}

void BinTreeNodeWriter::clear() {
	this->data.resize(0);
}

void BinTreeNodeWriter::writeBytes(const char* bytes, const size_t size){
	if(size >= 256){
		this->write8(253);
		this->write24(size);
	} else {
		this->write8(252);
		this->write8(size);
	}

	for(size_t i=0; i < size; i++)
		this->write8(bytes[i]);
}

void BinTreeNodeWriter::writeBytes(const string bytes){
	this->writeBytes(bytes.data(), bytes.size());
}

void BinTreeNodeWriter::writeBytes(const vector<char>& bytes){
	this->writeBytes(bytes.data(), bytes.size());
}

void BinTreeNodeWriter::write24(const int &c){
	this->write8((c & 0xFF0000) >> 16);
	this->write16(c & 0xFFFF);
}

void BinTreeNodeWriter::processBuffer(size_t headerPosition, KeyStream* crypto){
	int num = 0;
	int64_t num3 = this->data.size() - 3 - headerPosition;

	if(crypto){
		num = 8;
		num3 += 4;
	}

	if (num3 > 0x1000000) { // TODO: error. Buffer too large
	}

	if(crypto){
		int length = ((int) num3) - 4;
		// El 3 viene de que el header del size son 3 bytes (se escribio como 0 anteriormente por qe no se sabia el size)
		crypto->encodeMessage(this->data, (headerPosition + 3) + length, headerPosition + 3, length);
	}

	// Corrijo el size que antes se escribio como 0
	this->data[headerPosition] = ((num << 4) | (num3 & 0xff0000) >> 0x10);
	this->data[headerPosition+1] = ((num3 & 0xff00) >> 8);
	this->data[headerPosition+2] = (num3 & 0xff);
}
