#include "sha1.h"

#ifdef __USE_OPENSSL__
#include <openssl/sha.h>
#endif

using std::vector;
using std::string;

#ifdef __USE_OPENSSL__
vector<char> WhatsAcppi::Util::sha1(const char* data, size_t size){
	vector<char> ret;
	ret.resize(SHA_DIGEST_LENGTH);
	SHA1((const unsigned char*)data, size, (unsigned char*)ret.data());

	return ret;
}
#endif

string WhatsAcppi::Util::sha1Str(const char* data, size_t size){
	vector<char> sha = WhatsAcppi::Util::sha1(data, size);
	char out[SHA1_STR_LENGTH + 1];
	out[SHA1_STR_LENGTH] = 0;
	for(int i=0; i < SHA1_BIN_LENGTH; i++)
		snprintf(out+i*2, 3, "%02x", sha[i]);

	return string(out);
}

vector<char> WhatsAcppi::Util::sha1(vector<char> const& data){
	return WhatsAcppi::Util::sha1(data.data(), data.size());
}

string WhatsAcppi::Util::sha1Str(vector<char> const& data){
	return WhatsAcppi::Util::sha1Str(data.data(), data.size());
}

vector<char> WhatsAcppi::Util::sha1(const string& data){
	return WhatsAcppi::Util::sha1(data.c_str(), data.size());
}

string WhatsAcppi::Util::sha1Str(string const& data){
	return WhatsAcppi::Util::sha1Str(data.c_str(), data.size());
}
