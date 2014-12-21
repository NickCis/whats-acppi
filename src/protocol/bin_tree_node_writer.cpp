#include "bin_tree_node_writer.h"

#include "token_map.h"

#include <memory>

using std::map;
using std::vector;
using std::string;
using std::shared_ptr;

using WhatsAcppi::Protocol::tryGetToken;
using WhatsAcppi::Protocol::BinTreeNodeWriter;

BinTreeNodeWriter::BinTreeNodeWriter()
{
}

void BinTreeNodeWriter::streamStart(const string& domain, const string& resource){
	this->write8('W');
	this->write8('A');
	this->write8(1);
	this->write8(5);

	std::map<std::string, std::string> attributes;
	attributes.clear();
	attributes["to"] = domain;
	attributes["resource"] = resource;

	this->writeListStart(attributes.size() * 2 + 1);

	this->write8(1);

	this->writeAttributes(attributes);
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

	//TODO
	//int atIndex = tag.indexOf('@');
	//if (atIndex < 1){
	//	writeArray(tag.toUtf8(), out);
	//}else{
	//	QString server = tag.right(tag.length()-atIndex-1);
	//	QString user = tag.left(atIndex);
	//	writeJid(user, server, out);
	//}
}

void BinTreeNodeWriter::writeToken(const int& token){
	if(token < 0xf5){
		this->write8(token);
	}else{
		this->write8(0xfe);
		this->write8(token-0xf5);
	}
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

void BinTreeNodeWriter::writeBytes(const vector<char>& bytes){
	if(bytes.size() >= 256){
		this->write8(253);
		this->write24(bytes.size());
	} else {
		this->write8(252);
		this->write8(bytes.size());
	}

	const char *constData = bytes.data();
	for(size_t i=0; i < bytes.size(); i++)
		this->write8(constData[i]);
}

void BinTreeNodeWriter::write24(const int &c){
	this->write8((c & 0xFF0000) >> 16);
	this->write16(c & 0xFFFF);
}
