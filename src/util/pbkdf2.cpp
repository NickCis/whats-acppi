#include "pbkdf2.h"

#include "sha1.h"
#include "hmac_sha1.h"
#include "log.h"

/*
#ifdef __USE_OPENSSL__
extern "C" {
	#include <openssl/evp.h>
}
#endif
*/

using std::vector;


using WhatsAcppi::Util::Log;
using WhatsAcppi::Util::hmac_sha1;
/*
#ifdef __USE_OPENSSL__
vector<char> WhatsAcppi::Util::pbkdf2(const vector<char>& password, const vector<char>& salt, int iterations){
	vector<char> output;
	output.resize(SHA1_BIN_LENGTH);
	PKCS5_PBKDF2_HMAC_SHA1(password.data(), password.size(), (const unsigned char*) salt.data(), salt.size(), iterations, SHA1_BIN_LENGTH, (unsigned char*) output.data());
	return output;
}
#endif*/

vector<char> WhatsAcppi::Util::pbkdf2(const vector<char>& password, const vector<char>& salt, int iterations){
	vector<char> output;
	vector<char> obuf;
	if(iterations <= 0){
		Log(Log::WarningMsg) << __func__ << " :: iterations [" << iterations << "] <= 0";
		return output;
	}

	if(password.size() == 0){
		Log(Log::WarningMsg) << __func__ << " :: password.size() == 0";
		return output;
	}

	vector<char> asalt;
	asalt.reserve(salt.size()+4);
	asalt = salt;
	asalt.resize(salt.size()+4);
	//vector<char> key = password;
	size_t key_len = password.size() > SHA1_BIN_LENGTH ? SHA1_BIN_LENGTH : password.size();
	
	for(size_t count=1; count < key_len && output.size() < key_len; count++){
		asalt[salt.size()+0] = (count >> 24) & 0xff;
		asalt[salt.size()+1] = (count >> 16) & 0xff;
		asalt[salt.size()+2] = (count >> 8) & 0xff;
		asalt[salt.size()+3] = count & 0xff;

		vector<char> d1 = hmac_sha1(password, asalt);
		obuf = d1;

		for(int i=1; i < iterations; i++){
			d1 = hmac_sha1(password, d1);

			for(size_t j=0; j < obuf.size(); j++)
				obuf[j] ^= d1[j];
		}

		output.insert(output.end(), obuf.begin(), obuf.end());
	}

	output.resize(key_len);
	return output;
}
