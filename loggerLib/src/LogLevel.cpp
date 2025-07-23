#include "../include/LogLevel.hpp"
// TODO: include в cmake
std::string LogLeveltoString(LogLevel level) {
    switch(level)
    {
    case LogLevel::DEBUG:
        return "DEBUG";
        break;
    case LogLevel::INFO:
        return "DEBUG";
        break;
    case LogLevel::WARNING:
        return "DEBUG";
        break;
    case LogLevel::ERROR:
        return "DEBUG";
        break;
    default:
        return "UNKNOWN";
    }
}
std::ostream&  operator<< (std::ostream& out, LogLevel level) {
    return out << LogLeveltoString(level);
}