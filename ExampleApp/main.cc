#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <regex>

#include "FileLogger.hpp"
#include "LogLevel.hpp"

// структура для хранения сообщения полученного от пользователя вместе с его уровнем важности
struct LogMessage {
    std::string message;
    LogLevel level;
};

// очередь сообщений, полученных от пользователя
std::queue<LogMessage> messageQueue; 
std::mutex queueMutex;
std::condition_variable queueCV;
std::atomic<bool> stopFlag(false);

// Переводит строку в LogLevel(Уровень важности для логгера)
bool strtologlvl(const std::string& str, LogLevel& level);

// Поток для записи в лог
void loggerThreadFunc(FileLogger& logger) {
    while (true) {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueCV.wait(lock, [] { return !messageQueue.empty() || stopFlag.load(); });

        while (!messageQueue.empty()) {
            LogMessage msg = messageQueue.front();
            messageQueue.pop();
            lock.unlock();  // Разблокируем мьютекс на время записи
            logger.addLog(msg.message, msg.level);  // пишем с уровнем сообщения
            lock.lock();
        }

        if (stopFlag && messageQueue.empty()) {
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    // Проверка на корректное кол-во аргументов
    if (argc != 3) {
        std::cerr << "You must write 2 arguments:\n"
                  << "\t FILE_NAME,\n"
                  << "\t IMPORTANCE_LEVEL\n";
        return 1;
    }
    
    std::string filename = argv[1]; // имя файла
    LogLevel defaultLevel; // уровень важности по умолчанию 
    
    // Проверка на корректность 2-го введённого аргумента
    if (!strtologlvl(argv[2], defaultLevel)) {
        std::cerr << "ERROR: Unknown level!\n";
        return 2;
    }

    FileLogger logger(filename, defaultLevel);

    // Проверка на корректность открытия файла
    if (!logger.File_is_open()) {
        std::cerr << "ERROR: Can not open file: " << filename << " !\n";
        return 3;
    }

    std::thread loggerThread(loggerThreadFunc, std::ref(logger));

    std::cout << "Enter messages (optionally with level):\n"
              << "Format: <message> [LEVEL]\n"
              << "Type 'exit' to quit.\n";

    std::regex pattern(R"(^<([^<>]+)>(?: \[(DEBUG|INFO|WARNING|ERROR)\])?$)");

    std::string input;
    while (true) {
        std::getline(std::cin, input);
        if (input == "exit") {
            break;
        }

        std::smatch match;
        if (!std::regex_match(input, match, pattern)) {
            std::cerr << "Format error! Use: <message> [LEVEL]\n";
            continue;
        }

        std::string message = match[1];
        LogLevel lvl = defaultLevel;
        if (match[2].matched) {
            if (!strtologlvl(std::string(match[2]), lvl)) {
                std::cerr << "Unknown log level!\n";
                continue;
            }
        }

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            messageQueue.push({message, lvl});  // пушим с выбранным уровнем
        }
        queueCV.notify_one();
    }

    // Завершение потока логгера
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stopFlag = true;
    }
    queueCV.notify_one();
    loggerThread.join();

    return 0;
}

bool strtologlvl(const std::string& str, LogLevel& level) {
    if (str == "DEBUG") { level = LogLevel::DEBUG;  return true; }
    else if (str == "INFO") { level = LogLevel::INFO; return true; }
    else if (str == "WARNING") { level = LogLevel::WARNING; return true; }
    else if (str == "ERROR") { level = LogLevel::ERROR; return true; }
    else { return false; }
}
