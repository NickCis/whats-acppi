#include "register.h"
#include "define.h"
#include "util/log.h"
#include "util/sha1.h"
#include "util/base64.h"
#include "util/request.h"
#include "util/json.h"

#include <vector>
#include <string>
#include <sstream>

using std::vector;
using std::string;
using std::stringstream;

using WhatsAcppi::Register;
using WhatsAcppi::Phone;
using WhatsAcppi::Util::Log;
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

	Log() << " == req.getData() == ";
	Log() << req.getData();
	Log() << " =================== ";

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
		} else{
			Log(Log::WarningMsg) << "Unknown name: \"" << n << "\": " << value.str();
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
		} else{
			Log(Log::WarningMsg) << "Unknown name: \"" << n << "\": " << value.str();
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
