#ifndef __WHATS_ACPPI_LOG_H__
#define __WHATS_ACPPI_LOG_H__

#include <sstream>
#include <vector>
#include <iomanip>

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

				class VectorStr {
					public:
						VectorStr(const std::vector<char> &vec) : v(vec){}
						const char& operator[] (size_t n) const { return this->v[n]; }
						size_t size() const { return this->v.size(); }
						std::vector<char>::const_iterator begin() const { return this->v.begin(); }
						std::vector<char>::const_iterator end() const { return this->v.end(); }

					protected:
						const std::vector<char> &v;
				};

				Log(LogMsgType type = DebugMsg);
				~Log();

				Log &operator<<(const VectorStr &v){
					if(Log::logLevel <= this->level)
						for(auto it = v.begin(); it != v.end(); it++)
							this->ss << (*it);
					return *this;
				}

				Log &operator<<(const std::vector<char> &v){
					if(Log::logLevel <= this->level)
						for(auto it = v.begin(); it != v.end(); it++)
							this->ss << std::setfill('0') << std::setw(2) << std::hex << (unsigned int) ((unsigned char) (*it));
					return *this;
				}

				Log &operator<<(const std::vector<unsigned char> &v){
					if(Log::logLevel <= this->level)
						for(auto it = v.begin(); it != v.end(); it++)
							this->ss << std::setfill('0') << std::setw(2) << std::hex << (unsigned int) (*it);
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
