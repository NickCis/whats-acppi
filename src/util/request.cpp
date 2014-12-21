#include "request.h"

#define WHATS_APP_USER_AGENT "WhatsApp/2.11.464 Android/4.3 Device/GalaxyS3"

#ifdef __USE_CURL__
extern "C" {
	#include <curl/curl.h>
}
#endif

using std::string;
using std::vector;
using std::unique_ptr;
using std::stringstream;

using WhatsAcppi::Util::Request;

#ifdef __USE_CURL__
class Request::RequestInternal {
	public:
		RequestInternal() : curl(NULL), slist(NULL) {
			this->curl = curl_easy_init();
		}

		~RequestInternal(){
			if(curl)
				curl_easy_cleanup(curl);
			if(slist)
				curl_slist_free_all(slist);
		}

		CURL *curl;
		CURLcode res;
		struct curl_slist *slist;
};
#endif

#ifdef __USE_CURL__
static size_t curlWrite(void *contents, size_t size, size_t nmemb, void *userp){
	size_t realSize = size * nmemb;
	vector<char>* response = (vector<char>*) userp;
	response->reserve(response->size()+realSize);
	response->insert(response->end(), (char*)contents, ((char*)contents)+realSize);
	return realSize;
}
#endif

Request::Request(const string& h, const std::string& proto) :
	me(new Request::RequestInternal()),
	host(h),
	protocol(proto)
{
	#ifdef __USE_CURL__
	curl_easy_setopt(this->me->curl, CURLOPT_HEADER, 0);
	curl_easy_setopt(this->me->curl, CURLOPT_USERAGENT, WHATS_APP_USER_AGENT);
	curl_easy_setopt(this->me->curl, CURLOPT_WRITEFUNCTION, curlWrite);
	curl_easy_setopt(this->me->curl, CURLOPT_WRITEDATA, (void*) &this->response);
	curl_easy_setopt(this->me->curl, CURLOPT_SSL_VERIFYPEER, 0L);
	//curl_easy_setopt(this->me->curl, CURLOPT_SSL_VERIFYHOST, 0L);
	#endif
}

void Request::setHeader(const std::string& header){
	this->setHeader(header.c_str());
}

void Request::setHeader(const char* header){
	#ifdef __USE_CURL__
	this->me->slist = curl_slist_append(this->me->slist, header);
	#endif
}

string Request::urlEncode(const char *data, size_t size){
	string ret;

	#ifdef __USE_CURL__
	char *encoded = curl_easy_escape(this->me->curl, data, size);
	ret = encoded;
	curl_free(encoded);
	#endif

	return ret;
}

string Request::urlEncode(const vector<char>& data){
	return this->urlEncode(data.data(), data.size());
}

string Request::urlEncode(const string& data){
	return this->urlEncode(data.c_str(), data.size());
}

void Request::setUrlParam(const string& name, const string& value){
	this->params << name << "=" << value << "&";
}

void Request::setUrlParam(const string& name, const std::vector<char>& value){
	this->params << name << "=" << this->urlEncode(value) << "&";
}

void Request::setUrlParam(const string& name, const int& value){
	this->params << name << "=" << value << "&";
}

int Request::get(){
	stringstream ss;
	int ret = 0;

	ss << this->protocol << "://" << this->host << "?" << this->params.str();
	#ifdef __USE_CURL__
	if(this->me->slist)
		curl_easy_setopt(this->me->curl, CURLOPT_HTTPHEADER, this->me->slist);
	curl_easy_setopt(this->me->curl, CURLOPT_URL, ss.str().c_str());
	this->me->res = curl_easy_perform(this->me->curl);
	if(this->me->res)
		ret = -1;
	#endif

	return ret;
}

const std::vector<char>& Request::getData() const{
	return this->response;
}

Request::~Request(){
}
