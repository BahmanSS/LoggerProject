#include "FileLogger.hpp"
#include <iostream>
std::string FileLogger::getCurrentTime() {
    // TODO: реализовать функцию для получения текущего времени.
}

FileLogger::FileLogger(const std::string& filename, LogLevel level): CurrentLevel(level) {
    outFile = std::ofstream(filename, std::ios::app);
    if (!outFile) {
        throw std::invalid_argument("Сan not be open file: " + filename);
    }
}
void FileLogger::setLogLevel(LogLevel level) {
    //TODO: mutex
    CurrentLevel = level;
}
void FileLogger::addLog(const std::string& message, LogLevel level) {
    if (CurrentLevel > level) {
        return;
    }

    // TODO: mutex
    // TODO: сделать до конца запись
    outFile << "[" << level << "] [";

}
FileLogger::~FileLogger() {
    if (outFile) {
        outFile.close();
    }
}