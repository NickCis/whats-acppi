#include <iostream>
#include <string>
#include "../../src/util/log.h"
#include "../../src/util/sha1.h"
#include "../../src/util/util.h"
#include "../../src/util/base64.h"
#include "../../src/util/pbkdf2.h"
#include "../../src/util/hmac_sha1.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

using WhatsAcppi::Util::Log;
using WhatsAcppi::Util::sha1;
using WhatsAcppi::Util::pbkdf2;
using WhatsAcppi::Util::hmac_sha1;
using WhatsAcppi::Util::base64_decode;

char getBinVal(char c){
	switch(c){
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case 'a':
		case 'A':
			return 10;
		case 'b':
		case 'B':
			return 11;
		case 'c':
		case 'C':
			return 12;
		case 'd':
		case 'D':
			return 13;
		case 'e':
		case 'E':
			return 14;
		case 'f':
		case 'F':
			return 15;
		default:
			return 0;
	}
}

vector<char> hex2bin(const string &hex){
	vector<char> ret;
	if(hex.size() % 2)
		return vector<char>();

	ret.reserve(hex.size()/2);

	for(size_t i=0; i < hex.size(); i+=2){
		char c = getBinVal(hex[i]) * 16 + getBinVal(hex[i+1]);
		ret.push_back(c);
	}

	return ret;
}

int main(/*int argc, char*argv[]*/){
	WhatsAcppi::Util::Init init;
	Log::setLogLevel(Log::DebugMsg);

	const string base64Password = "9V4hlNmoJKl38ccsdVQxdOdvvxI=";
	vector<char>  password = base64_decode(base64Password);
	Log() << "base64Password: " << base64Password;
	Log() << "password: " << password;

	const string hexNonce = "add4c8376b49e1194f519911fcece95196d89531";
	vector<char> nonce = hex2bin(hexNonce);
	Log() << "hexNonce: " << hexNonce;
	Log() << "nonce: " << nonce;

	vector<char> sha1Nonce = sha1(nonce);
	Log() << "sha1Nonce: " << sha1Nonce;

	vector<char> hmacNonce = hmac_sha1(password, nonce);
	Log() << "hmacNonce: " << hmacNonce;

	Log() << "hmac empty: " << hmac_sha1("", "");

	nonce.push_back(0);

	nonce.back() = '1';
	vector<char> key1 = pbkdf2(password, nonce, 2);
	Log() << "key0: " << key1;

	nonce.back() = '2';
	vector<char> key2 = pbkdf2(password, nonce, 2);
	Log() << "key1: " << key2;

	nonce.back() = '3';
	vector<char> key3 = pbkdf2(password, nonce, 2);
	Log() << "key2: " << key3;

	nonce.back() = '4';
	vector<char> key4 = pbkdf2(password, nonce, 2);
	Log() << "key3: " << key4;

	return 0;
}
