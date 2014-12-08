#include "register.h"
#include "util/sha1.h"
#include "util/base64.h"
#include "util/request.h"

#define TOKEN_SIGNATURE "MIIDMjCCAvCgAwIBAgIETCU2pDALBgcqhkjOOAQDBQAwfDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFDASBgNVBAcTC1NhbnRhIENsYXJhMRYwFAYDVQQKEw1XaGF0c0FwcCBJbmMuMRQwEgYDVQQLEwtFbmdpbmVlcmluZzEUMBIGA1UEAxMLQnJpYW4gQWN0b24wHhcNMTAwNjI1MjMwNzE2WhcNNDQwMjE1MjMwNzE2WjB8MQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEUMBIGA1UEBxMLU2FudGEgQ2xhcmExFjAUBgNVBAoTDVdoYXRzQXBwIEluYy4xFDASBgNVBAsTC0VuZ2luZWVyaW5nMRQwEgYDVQQDEwtCcmlhbiBBY3RvbjCCAbgwggEsBgcqhkjOOAQBMIIBHwKBgQD9f1OBHXUSKVLfSpwu7OTn9hG3UjzvRADDHj+AtlEmaUVdQCJR+1k9jVj6v8X1ujD2y5tVbNeBO4AdNG/yZmC3a5lQpaSfn+gEexAiwk+7qdf+t8Yb+DtX58aophUPBPuD9tPFHsMCNVQTWhaRMvZ1864rYdcq7/IiAxmd0UgBxwIVAJdgUI8VIwvMspK5gqLrhAvwWBz1AoGBAPfhoIXWmz3ey7yrXDa4V7l5lK+7+jrqgvlXTAs9B4JnUVlXjrrUWU/mcQcQgYC0SRZxI+hMKBYTt88JMozIpuE8FnqLVHyNKOCjrh4rs6Z1kW6jfwv6ITVi8ftiegEkO8yk8b6oUZCJqIPf4VrlnwaSi2ZegHtVJWQBTDv+z0kqA4GFAAKBgQDRGYtLgWh7zyRtQainJfCpiaUbzjJuhMgo4fVWZIvXHaSHBU1t5w//S0lDK2hiqkj8KpMWGywVov9eZxZy37V26dEqr/c2m5qZ0E+ynSu7sqUD7kGx/zeIcGT0H+KAVgkGNQCo5Uc0koLRWYHNtYoIvt5R3X6YZylbPftF/8ayWTALBgcqhkjOOAQDBQADLwAwLAIUAKYCp0d6z4QQdyN74JDfQ2WCyi8CFDUM4CaNB+ceVXdKtOrNTQcc0e+t"

#define CLASSES_MD5 "ZiVLoYj05SJyvAR90dEqaw=="

#define KEY2 "/UIGKU1FVQa+ATM2A0za7G2KI9S/CwPYjgAbc67v7ep42eO/WeTLx1lb1cHwxpsEgF4+PmYpLd2YpGUdX/A2JQitsHzDwgcdBpUf7psX1BU="

#define WHATSAPP_REQUEST_HOST "v.whatsapp.net/v2/code"

using std::vector;
using std::string;

using WhatsAcppi::Register;
using WhatsAcppi::Phone;
using WhatsAcppi::Util::sha1;
using WhatsAcppi::Util::Request;
using WhatsAcppi::Util::base64_encode;
using WhatsAcppi::Util::base64_decode;

Register::Register(const Phone& p, const vector<char>& i) : phone(p), identity(i)  {
}

Register::~Register(){
}

#include <iostream>

int Register::codeRequest(const string& method){
	vector<char> token = this->generateRequestToken();

	Request req(WHATSAPP_REQUEST_HOST, "https");
	req.setHeader("Accept: text/json");
	req.setUrlParam("in", this->phone.getPhone());
	req.setUrlParam("cc", this->phone.getCc());
	req.setUrlParam("id", this->identity);
	req.setUrlParam("lg", this->phone.getIso639());
	req.setUrlParam("lc", this->phone.getIso3166());
	req.setUrlParam("mcc", this->phone.getMcc());
	req.setUrlParam("mnc", this->phone.getMnc());
	req.setUrlParam("sim_mcc", this->phone.getMcc());
	req.setUrlParam("sim_mnc", this->phone.getMnc());
	req.setUrlParam("sim_mnc", this->phone.getMnc());
	req.setUrlParam("method", method);
	req.setUrlParam("token", token);
	req.setUrlParam("network_radio_type", "1");

	int ret = req.get();

	if(ret)
		std::cout << "Request failed. ret: " << ret << std::endl;

	for(auto c : req.getData())
		std::cout << c;

	std::cout <<  std::endl;

	return 0;
}

vector<char> Register::generateRequestToken(){

	vector<char> key2 = base64_decode(KEY2);
	vector<char> signature = base64_decode(TOKEN_SIGNATURE);
	vector<char> classesMd5 = base64_decode(CLASSES_MD5);

	vector<char> opad;
	vector<char> ipad;
	opad.reserve(64);
	ipad.reserve(64);

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

	return sha1(output);
}
