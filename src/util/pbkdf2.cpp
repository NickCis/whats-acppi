#include "pbkdf2.h"

#include "sha1.h"

#ifdef __USE_OPENSSL__
extern "C" {
	#include <openssl/evp.h>
}
#endif

using std::vector;

#ifdef __USE_OPENSSL__
vector<char> WhatsAcppi::Util::pkbdf2(const vector<char>& password, const vector<char>& salt, int iterations){
	vector<char> output;
	output.resize(SHA1_BIN_LENGTH);
	PKCS5_PBKDF2_HMAC_SHA1(password.data(), password.size(), (const unsigned char*) salt.data(), salt.size(), iterations, SHA1_BIN_LENGTH, (unsigned char*) output.data());
	return output;
}
#endif
