#ifndef LOGER_
#define LOGER_
#include <string>
#include "LogLevel.hpp"

class Logger {
public:
    virtual void setLogLevel(LogLevel level) = 0;
    virtual void addLog(const std::string& message, LogLevel level) = 0;
    virtual ~Logger() =  default;
};


#endif
