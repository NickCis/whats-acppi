#include "node.h"

#include <sstream>
#include <iomanip>
#include <iostream>

using std::map;
using std::hex;
using std::endl;
using std::setw;
using std::string;
using std::vector;
using std::setfill;
using std::shared_ptr;
using std::stringstream;

using WhatsAcppi::Protocol::Node;

Node::Node(const string& t) : tag(t) {
}

void Node::setAttribute(const string& name, const string& value){
	this->attributes[name] = value;
}

void Node::setAttribute(const vector<char>& name, const vector<char>& value){
	this->attributes[string(name.begin(), name.end())] = string(value.begin(), value.end());
}

void Node::setTag(const string& tag){
	this->tag = tag;
}

void Node::setTag(const vector<char>& tag){
	this->tag = string(tag.begin(), tag.end());
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

Node::operator bool() const {
	return !this->tag.empty();
}

const string Node::getAttribute(const string& name) const {
	auto it = this->attributes.find(name);

	if(it == this->attributes.end())
		return string();

	return it->second;
}

string Node::toString(const std::string& sep) const {
	stringstream ss;
	ss << sep << '<' << this->getTag();

	for(auto it=this->attributes.begin(); it != this->attributes.end(); it++)
		ss << ' ' << it->first << "=\"" << it->second << "\"";

	ss << '>' << endl;

	for(auto it=this->children.begin(); it != this->children.end(); it++)
		ss << (*it)->toString(sep+"  ") << endl;

	if(this->data){
		ss << sep << ' ';
		for(auto it=this->data->begin(); it != this->data->end(); it++)
			ss << ' ' << setfill('0') << setw(2) << hex << (unsigned int) ((unsigned char) (*it));
		ss << endl;
	}

	ss << sep << "</" << this->getTag() << '>';

	return ss.str();
}
