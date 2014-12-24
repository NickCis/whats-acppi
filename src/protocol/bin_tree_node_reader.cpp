#include "bin_tree_node_reader.h"

#include <memory>

#include "token_map.h"
#include "../util/log.h"

using std::vector;
using std::string;
using std::shared_ptr;

using WhatsAcppi::Util::Log;
using WhatsAcppi::Protocol::Node;
using WhatsAcppi::Protocol::getToken;
using WhatsAcppi::Protocol::KeyStream;
using WhatsAcppi::Protocol::BinTreeNodeReader;

BinTreeNodeReader::BinTreeNodeReader() : pos(0){
}

Node BinTreeNodeReader::nextTree(vector<char>& data, KeyStream* key){
	if(data.size() == 0)
		return Node();

	int stanzaFlag = (this->peek8(data)  & 0xF0) >> 4;
	size_t stanzaSize = this->peek16(data, 1) | ((this->peek8(data) & 0x0F) << 16);

	if(stanzaSize > data.size()){ // TODO: error
		Log() << "Incomplete Node. stanzaSize [" << stanzaSize << "] > data.size() [" << data.size() << ']';
		return Node();
	}

	this->read24(data);

	if(stanzaFlag & 8){
		if(key){
			int realSize = stanzaSize - 4;
			key->decodeMessage(data, realSize, 0, realSize);
		}else{ // XXX: Error
			Log(Log::WarningMsg) << __func__ << " :: stanzaFlag & 1 -> true, but key isn't set! stanzaFlag: " << stanzaFlag;
		}
	}

	if(stanzaSize > 0)
		return this->nextTreeInternal(data);

	return Node();
}

Node BinTreeNodeReader::nextTreeInternal(const vector<char> & data){
	unsigned int token = this->read8(data);
	int size = this->readListSize(data, token);
	Node node;
	token = this->read8(data);
	if(token == 1){
		node.setTag("start");
		this->readAttributes(data, size, node);
		return node;
	}else if(token == 2){ // TODO: error
		Log() << __func__ << " :: token == 2. Return null node";
		return node;
	}

	node.setTag(this->readString(data, token));
	this->readAttributes(data, size, node);

	if((size % 2) == 1){
		return node;
	}

	token = this->read8(data);
	if(this->isListTag(token)){
		// XXX readList para los childresn!
		return node;
	}

	node.setData(shared_ptr<vector<char>>(new vector<char>(this->readString(data, token))));

	return node;
}

void BinTreeNodeReader::readList(vector<char>&data, int token, Node &node){
	int size = this->readListSize(data, token);

	for(int i=0; i < size; i++){
		node.addChild(shared_ptr<Node>(new Node(this->nextTreeInternal(data))));
	}
}

bool BinTreeNodeReader::isListTag(int token){
	return ((token == 248) || (token == 0) || (token == 249));
}

vector<char> BinTreeNodeReader::readString(const vector<char>&data, unsigned int token){
	if(token == ((unsigned int) -1)){ // TODO: ERROR
		Log(Log::WarningMsg) << __func__ << " :: known token val: " << token;
		return vector<char>();
	}

	if( (token > 2) && (token < 0xF5)){
		string t = this->getToken(data, token);
		return vector<char>(t.begin(), t.end());
	}else if(token == 0){
		return vector<char>();
	}else if(token == 0xFC){
		return this->fillArray(data, this->read8(data));
	}else if(token == 0xFD){
		return this->fillArray(data, this->read24(data));
	}else if(token == 0xFA){
		vector<char> user = this->readString(data, this->read8(data));
		vector<char> server = this->readString(data, this->read8(data));
		if(user.size() > 0 && server.size() > 0){
			user.reserve(user.size()+1+server.size());
			user.push_back('@');
			user.insert(user.end(), server.begin(), server.end());
			return user;
		}else if(server.size() > 0){
			return server;
		}
	}else if(token == 0xFF){
		return this->readNibble(data);
	}

	return vector<char>();
}

vector<char> BinTreeNodeReader::readNibble(const vector<char>& data){
	unsigned int byte = this->read8(data);
	unsigned int ignoreLastNibble =  !!(byte & 0x80);
	unsigned int size = byte & 0x7F;
	unsigned int nrOfNibbles = size * 2 - ignoreLastNibble;

	vector<char> d = this->fillArray(data, size);
	vector<char> ret;

	for(unsigned int i=0; i < nrOfNibbles; i++){
		byte = data[i/2];
		unsigned int shift = 4 * (1 - i %2);
		unsigned int decimal = (byte & (15 << shift)) >> shift;
		switch(decimal){
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				ret.push_back(decimal);
				break;
			case 10:
			case 11:
				ret.push_back(decimal - 10 + 45);
				break;
			default: // TODO: error
				Log(Log::WarningMsg) << __func__ << " :: unknown decimal: " << decimal;
				break;
		}
	}

	return ret;
}

string BinTreeNodeReader::getToken(const vector<char>& data, int token){
	bool subdict = false;
	string ret = ::getToken(token, subdict);
	if(ret == ""){
		token = this->read8(data);
		ret = ::getToken(token, subdict);
	}

	return ret;
}

vector<char> BinTreeNodeReader::fillArray(const vector<char>&data, int len){
	vector<char> ret;
	if(data.size() >= pos+len){
		ret.reserve(len);
		ret.insert(ret.end(), data.begin()+pos, data.begin()+pos+len);
		pos += len;
	}

	return ret;
}

void BinTreeNodeReader::readAttributes(const vector<char>& data, int size, Node &node){
	int attrCount = (size -2 + size % 2) / 2;

	for(int i =0; i < attrCount; i++){
		vector<char> key = this->readString(data, this->read8(data));
		node.setAttribute(key, this->readString(data, this->read8(data)));
	}
}

int BinTreeNodeReader::readListSize(const vector<char> & data, int token){
	int size = 0;
	if(token == 0xF8){
		size = this->read8(data);
	}else if(token == 0xF9){
		size = this->read16(data);
	}else{ // TODO: error
		Log(Log::WarningMsg) << __func__ << " :: unknown token value: " << token;
	}

	return size;
}

unsigned char BinTreeNodeReader::read8(const vector<char> &data){
	unsigned char ret = this->peek8(data);
	pos++;
	return ret;
}

unsigned int BinTreeNodeReader::read16(const vector<char> &data){
	unsigned int ret = this->peek16(data);
	pos += 2;
	return ret;
}

unsigned int BinTreeNodeReader::read24(const vector<char> &data){
	unsigned int ret = this->peek24(data);
	pos += 3;
	return ret;
}

unsigned char BinTreeNodeReader::peek8(const vector<char> &data, unsigned int offset) const{
	return data[offset+pos];
}

unsigned int BinTreeNodeReader::peek16(const vector<char> &data, unsigned int offset) const{
	if(!data.size() >= pos+offset+2){ // TODO: error
		Log(Log::WarningMsg) << __func__ << " :: buffer is too small data.size() " << data.size() << " needed " << (pos+offset+2);
		return 0;
	}

	return (data[pos+offset] << 8) | (data[pos+offset+1]);
}

unsigned int BinTreeNodeReader::peek24(const vector<char> &data, unsigned int offset) const{
	if(!data.size() >= pos+offset+3){ // TODO: error
		Log(Log::WarningMsg) << __func__ << " :: buffer is too small data.size() " << data.size() << " needed " << (pos+offset+3);
		return 0;
	}

	return (this->peek16(data, offset) << 8) | this->peek8(data, offset+2);
}
