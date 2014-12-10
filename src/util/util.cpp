#include "util.h"

#ifdef __USE_CURL__
#include <curl/curl.h>
#endif

using WhatsAcppi::Util::Init;

Init::Init(){
	#ifdef __USE_CURL__
	// TODO: is it the correct flag?
	curl_global_init(CURL_GLOBAL_DEFAULT);
	#endif
	#ifdef __USE_OPENSSL__
	// TODO: init openssl?
	#endif
}

Init::~Init(){
	#ifdef __USE_CURL__
	curl_global_cleanup();
	#endif
}
