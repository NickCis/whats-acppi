#include "socket.h"

extern "C" {
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/tcp.h>
	#include <netdb.h>
	#include <string.h>
	#include <unistd.h>
}

using std::vector;
using std::string;

using WhatsAcppi::Util::Socket;

Socket::Socket(const string& h, const string& p) :
	sock(-1),
	host(h),
	port(p),
	st(SOCK_UNKNOWN)
{

}

Socket::~Socket(){
	this->close();
}

int Socket::connect(){
	int ret;

	this->st = SOCK_ERROR;

	struct addrinfo hints, *res;
	::memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = SOL_TCP;

	ret = ::getaddrinfo(this->host.c_str(), this->port.c_str(), &hints, &res);

	if(ret || !res)
		return -1;

	this->sock = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(this->sock == -1){
		::freeaddrinfo(res);
		return -1;
	}

	ret = ::connect(this->sock, (struct sockaddr*) res->ai_addr, res->ai_addrlen);
	::freeaddrinfo(res);
	if(ret){
		this->close();
		return -1;
	}

	this->st = SOCK_CONNECTED;

	return 0;
}

int Socket::close(){
	if(this->sock > 0){
		::close(this->sock);
		this->st = SOCK_DISCONNECTED;
		return -1;
	}

	return 0;
}

void Socket::setTimeout(int sec, int usec){
	// TODO: this is posix specific, in windows things go different!
	struct timeval tv;
	tv.tv_sec = sec;
	tv.tv_usec = usec;
	::memset(&tv, 0, sizeof(struct timeval));
	::setsockopt(this->sock, SOL_SOCKET, SO_RCVTIMEO, (void*) &tv, sizeof(struct timeval));
	::setsockopt(this->sock, SOL_SOCKET, SO_SNDTIMEO, (void*) &tv, sizeof(struct timeval));
}

int Socket::write(const void* buf, size_t size){
	return ::send(this->sock, buf, size, 0);
}

int Socket::write(const std::vector<char> &data){
	return this->write((void*) data.data(), data.size());
}

int Socket::write8(const char c){
	return this->write((void*) &c, sizeof(char));
}

int Socket::write16(const int c){
	this->write8((char)(c & 0xFF00) >> 8);
	return this->write8((char)(c & 0xFF));
}

ssize_t Socket::recv(char* buf, size_t len){
	return ::recv(this->sock, (void*) buf, len, 0);
}

ssize_t Socket::recv(vector<char>& out, size_t len){
	size_t start = out.size();
	out.resize(start+len);

	ssize_t readSize = this->recv(out.data()+start, len);
	if(readSize > 0)
		out.resize(start+readSize);
	else
		out.resize(start);

	return readSize;
}
