#include "node.h"

using std::map;
using std::string;
using std::vector;
using std::shared_ptr;

using WhatsAcppi::Protocol::Node;

Node::Node(const string& t) : tag(t) {
}

void Node::setAttribute(const string& name, const string& value){
	this->attributes[name] = value;
}

void Node::addChild(const shared_ptr<Node>& node) {
	this->children.push_back(node);
}

void Node::setData(const shared_ptr<vector<char>>& data) {
	this->data = data;
}

const string& Node::getTag() const {
	return this->tag;
}

const map<string, string>& Node::getAttributes() const{
	return this->attributes;
}

const vector<shared_ptr<Node>>& Node::getChildren() const {
	return this->children;
}

const shared_ptr<vector<char>>& Node::getData() const {
	return this->data;
}
