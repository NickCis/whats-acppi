#include "register.h"
#include "util/sha1.h"
#include "util/base64.h"
#include "util/request.h"
#include "util/json.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#define TOKEN_SIGNATURE "MIIDMjCCAvCgAwIBAgIETCU2pDALBgcqhkjOOAQDBQAwfDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFDASBgNVBAcTC1NhbnRhIENsYXJhMRYwFAYDVQQKEw1XaGF0c0FwcCBJbmMuMRQwEgYDVQQLEwtFbmdpbmVlcmluZzEUMBIGA1UEAxMLQnJpYW4gQWN0b24wHhcNMTAwNjI1MjMwNzE2WhcNNDQwMjE1MjMwNzE2WjB8MQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEUMBIGA1UEBxMLU2FudGEgQ2xhcmExFjAUBgNVBAoTDVdoYXRzQXBwIEluYy4xFDASBgNVBAsTC0VuZ2luZWVyaW5nMRQwEgYDVQQDEwtCcmlhbiBBY3RvbjCCAbgwggEsBgcqhkjOOAQBMIIBHwKBgQD9f1OBHXUSKVLfSpwu7OTn9hG3UjzvRADDHj+AtlEmaUVdQCJR+1k9jVj6v8X1ujD2y5tVbNeBO4AdNG/yZmC3a5lQpaSfn+gEexAiwk+7qdf+t8Yb+DtX58aophUPBPuD9tPFHsMCNVQTWhaRMvZ1864rYdcq7/IiAxmd0UgBxwIVAJdgUI8VIwvMspK5gqLrhAvwWBz1AoGBAPfhoIXWmz3ey7yrXDa4V7l5lK+7+jrqgvlXTAs9B4JnUVlXjrrUWU/mcQcQgYC0SRZxI+hMKBYTt88JMozIpuE8FnqLVHyNKOCjrh4rs6Z1kW6jfwv6ITVi8ftiegEkO8yk8b6oUZCJqIPf4VrlnwaSi2ZegHtVJWQBTDv+z0kqA4GFAAKBgQDRGYtLgWh7zyRtQainJfCpiaUbzjJuhMgo4fVWZIvXHaSHBU1t5w//S0lDK2hiqkj8KpMWGywVov9eZxZy37V26dEqr/c2m5qZ0E+ynSu7sqUD7kGx/zeIcGT0H+KAVgkGNQCo5Uc0koLRWYHNtYoIvt5R3X6YZylbPftF/8ayWTALBgcqhkjOOAQDBQADLwAwLAIUAKYCp0d6z4QQdyN74JDfQ2WCyi8CFDUM4CaNB+ceVXdKtOrNTQcc0e+t"

 // 2.11.473
#define CLASSES_MD5 "2BemshOrsTglFob3C6No8w=="

#define KEY2 "/UIGKU1FVQa+ATM2A0za7G2KI9S/CwPYjgAbc67v7ep42eO/WeTLx1lb1cHwxpsEgF4+PmYpLd2YpGUdX/A2JQitsHzDwgcdBpUf7psX1BU="

#define WHATSAPP_REQUEST_HOST "v.whatsapp.net/v2/code"
#define WHATSAPP_REGISTER_HOST "v.whatsapp.net/v2/register"

using std::vector;
using std::string;
using std::stringstream;

using WhatsAcppi::Register;
using WhatsAcppi::Phone;
using WhatsAcppi::Util::sha1;
using WhatsAcppi::Util::Request;
using WhatsAcppi::Util::base64_encode;
using WhatsAcppi::Util::base64_decode;
using WhatsAcppi::Util::simpleJsonParser;

Register::Register(const Phone& p, const string& i) : phone(p), identity(i), reason(""), retryAfter(0), st(REGISTER_UNKNOWN) {
}

Register::~Register(){
}

int Register::codeRequest(const string& method){
	int ret = 0;
	Request req(WHATSAPP_REQUEST_HOST, "https");
	req.setHeader("Accept: text/json");
	req.setUrlParam("in", this->phone.getPhone());
	req.setUrlParam("cc", this->phone.getCc());
	req.setUrlParam("id", req.urlEncode(this->identity));
	req.setUrlParam("lg", this->phone.getIso639());
	req.setUrlParam("lc", this->phone.getIso3166());
	//req.setUrlParam("mcc", this->phone.getMcc());
	req.setUrlParam("mcc", "000");
	//req.setUrlParam("mnc", this->phone.getMnc());
	req.setUrlParam("mnc", "000");
	req.setUrlParam("sim_mcc", this->phone.getMcc());
	req.setUrlParam("sim_mnc", this->phone.getMnc());
	req.setUrlParam("method", method);
	req.setUrlParam("token", req.urlEncode(this->generateRequestToken()));
	req.setUrlParam("reson", req.urlEncode("self-send-jailbroken"));
	req.setUrlParam("network_radio_type", "1");

	ret = req.get();

	this->st = REGISTER_CODE_REQUESTED_HTTP_ERROR;

	if(ret)
		return -1;

	this->st = REGISTER_CODE_REQUESTED;

	simpleJsonParser(req.getData(), [this, &ret] (const stringstream &name, stringstream &value){
		const string& n = name.str();
		if(n == "status"){
			this->status = value.str();
			if(this->status != "sent"){
				ret = 1;
				this->st = REGISTER_CODE_REQUESTED_WA_ERROR;
			}
		} else if(n == "reason"){
			this->reason = value.str();
		} else if(n == "retry_after"){
			value >> retryAfter;
		} else if(n == "length"){
		} else if(n == "method"){
		} else{ // XXX: use log
			std::cout << "Unknown name: \"" << n << "\": " << value.str() << std::endl;
		}
	});

	return ret;
}

int Register::codeRegister(const string& code){
	int ret = 0;
	Request req(WHATSAPP_REGISTER_HOST, "https");
	req.setHeader("Accept: text/json");
	req.setUrlParam("in", this->phone.getPhone());
	req.setUrlParam("cc", this->phone.getCc());
	req.setUrlParam("id", req.urlEncode(this->identity));
	req.setUrlParam("lg", this->phone.getIso639());
	req.setUrlParam("lc", this->phone.getIso3166());
	req.setUrlParam("code", code);
	req.setUrlParam("network_radio_type", "1");

	ret = req.get();

	this->st = REGISTER_CODE_REGISTERED_HTTP_ERROR;

	if(ret)
		return -1;

	this->st = REGISTER_CODE_REGISTERED;

	simpleJsonParser(req.getData(), [this, &ret] (const stringstream &name, stringstream &value){
		const string& n = name.str();
		if(n == "status"){
			this->status = value.str();
			if(this->status != "ok"){
				ret = 1;
				this->st = REGISTER_CODE_REGISTERED_WA_ERROR;
			}
		}else if(n == "login"){
			this->login = value.str();
		} else if(n == "pw"){
			this->pw = value.str();
		} else if(n == "type"){
		} else if(n == "expiration"){
		} else if(n == "kind"){
		} else if(n == "price"){
		} else if(n == "cost"){
		} else if(n == "currency"){
		} else if(n == "price_expiration"){
		} else{ // XXX: use log
			std::cout << "Unknown name: \"" << n << "\": " << value.str() << std::endl;
		}
	});

	return ret;
}

string Register::generateRequestToken(){
	vector<char> key2 = base64_decode(KEY2);
	vector<char> signature = base64_decode(TOKEN_SIGNATURE);
	vector<char> classesMd5 = base64_decode(CLASSES_MD5);
	vector<char> opad;
	vector<char> ipad;

	opad.resize(64);
	ipad.resize(64);

	for(int i=0; i < 64; i++){
		opad[i] = 0x5c ^ key2[i];
		ipad[i] = 0x36 ^ key2[i];
	}

	vector<char> data;
	data.reserve(64 + signature.size() + classesMd5.size() + this->phone.getPhone().size());
	data.insert(data.end(), ipad.begin(), ipad.end());
	data.insert(data.end(), signature.begin(), signature.end());
	data.insert(data.end(), classesMd5.begin(), classesMd5.end());
	data.insert(data.end(), this->phone.getPhone().begin(), this->phone.getPhone().end());

	vector<char> shaData = sha1(data);

	vector<char> output;
	output.reserve(64 + shaData.size());
	output.insert(output.end(), opad.begin(), opad.end());
	output.insert(output.end(), shaData.begin(), shaData.end());

	return base64_encode(sha1(output));
}

const std::string& Register::getStatus() const {
	return this->status;
}

const std::string& Register::getReason() const {
	return this->reason;
}

const int& Register::getRetryAfter() const {
	return this->retryAfter;
}

const std::string& Register::getPw() const {
	return this->pw;
}

const std::string& Register::getLogin() const {
	return this->login;
}
