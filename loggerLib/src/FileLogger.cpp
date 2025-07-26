#include "../include/FileLogger.hpp"
#include <iostream>
#include <chrono> 
#include <ctime> // std::time_t
#include <sstream> // std::ostringstream
#include <iomanip> // std::put_time()

// WARNING: std::localtime не безопасен !!!
// Эта функция должна быть вызвана только с заблокированной LogMutex !!!
std::string FileLogger::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t t_now = std::chrono::system_clock::to_time_t(now);

    std::tm local_tm;
    
    local_tm = *std::localtime(&t_now);
    
    std::ostringstream oss;
    const char* format = "%Y-%m-%d %H:%M:%S";

    oss << std::put_time(&local_tm, format);

    return oss.str();
}

FileLogger::FileLogger(const std::string& filename, LogLevel level): CurrentLevel(level) {
    outFile = std::ofstream(filename, std::ios::app);
    openFile = (outFile.is_open()) ? true : false; 
}
void FileLogger::setLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(LogMutex);
    CurrentLevel = level;
}
bool FileLogger::addLog(const std::string& message, LogLevel level) {
    std::lock_guard<std::mutex> lock(LogMutex);
    if (CurrentLevel > level) {
        return true;
    }
    if (!openFile) {
        return false;
    }
    outFile << "[" << level << "]" 
            << " ["<< getCurrentTime() << "] "
            << message << '\n';
    return true;
}
FileLogger::~FileLogger() {
    if (outFile) {
        outFile.close();
    }
}
bool FileLogger::File_is_open() { return openFile;}