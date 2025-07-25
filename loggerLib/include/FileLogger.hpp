#ifndef FILE_LOGGER_
#define FILE_LOGGER_
#include "Logger.hpp"
#include <string>
#include <mutex>
#include <fstream>

class FileLogger: public Logger {
public:
    FileLogger(const std::string& filename, LogLevel level);
    void setLogLevel(LogLevel level) override;
    bool addLog(const std::string& message, LogLevel level) override;
    bool File_is_open();
    ~FileLogger();
private:
    LogLevel CurrentLevel; // текущий уровень важности
    std::mutex LogMutex; // для потокобезопасности
    std::ofstream outFile; // "журнал для записи"
    bool openFile; // "Открылся ли файл?"

    std::string getCurrentTime(); // Получение текущего времени
}; 

#endif