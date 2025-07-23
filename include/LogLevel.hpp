#ifndef LVL_LOG_
#define LVL_LOG_
#include <string>
#include <ostream>

enum class LogLevel {
    ERROR = 0,
    WARNING = 1,
    INFO = 2,
    DEBUG = 3
};

std::string LogLeveltoString(LogLevel level);
std::ostream&  operator<< (std::ostream& out, LogLevel level);



#endif