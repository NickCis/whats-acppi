#include "sha1.h"

#ifdef __USE_OPENSSL__
#include <openssl/sha.h>
#endif

using std::vector;
using std::string;

#ifdef __USE_OPENSSL__
vector<char> WhatsAcppi::Util::sha1(const char* data, size_t size){
	vector<char> ret;
	ret.reserve(SHA_DIGEST_LENGTH);
	SHA1((const unsigned char*)data, size, (unsigned char*)ret.data());

	return ret;
}
#endif

vector<char> WhatsAcppi::Util::sha1(vector<char> const& data){
	return WhatsAcppi::Util::sha1(data.data(), data.size());
}

vector<char> WhatsAcppi::Util::sha1(const string& data){
	return WhatsAcppi::Util::sha1(data.c_str(), data.size());
}
