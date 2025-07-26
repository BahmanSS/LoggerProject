#ifndef LOGGER_
#define LOGGER_
#include <string>
#include "LogLevel.hpp"

class Logger {
public:
    virtual void setLogLevel(LogLevel level) = 0;
    virtual bool addLog(const std::string& message, LogLevel level) = 0;
    virtual ~Logger() =  default;
};


#endif
