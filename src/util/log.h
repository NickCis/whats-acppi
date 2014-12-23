#ifndef __WHATS_ACPPI_LOG_H__
#define __WHATS_ACPPI_LOG_H__

#include <sstream>
#include <vector>

namespace WhatsAcppi {
	namespace Util {
		class Log {
			public:
				typedef enum {
					DebugMsg=0,
					WarningMsg,
					CriticalMsg,
					FatalMsg
				} LogMsgType;

				Log(LogMsgType type = DebugMsg);
				~Log();

				Log &operator<<(const std::vector<char> &v){
					if(Log::logLevel <= this->level)
						for(auto it = v.begin(); it != v.end(); it++)
							this->ss << *it;

					return *this;
				}

				template<typename T>
				Log &operator<<(const T &t){
					if(Log::logLevel <= this->level)
						this->ss << t;

					return *this;
				}

				static void setLogLevel(LogMsgType);

			protected:
				std::ostringstream ss;
				static LogMsgType logLevel;
				LogMsgType level;
		};
	}
}
#endif
