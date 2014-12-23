#include "log.h"

#include <iostream>

using WhatsAcppi::Util::Log;

using std::cout;
using std::endl;

Log::LogMsgType Log::logLevel = CriticalMsg;

Log::Log(Log::LogMsgType type) : level(type) {
}

Log::~Log(){
	cout << this->ss.str() << endl;
}

void Log::setLogLevel(Log::LogMsgType level){
	Log::logLevel = level;
}
