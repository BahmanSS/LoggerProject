#include <iostream>
#include <string>
#include <fstream>
#include <queue>


#include "FileLogger.hpp"
#include "LogLevel.hpp"

// структура для хранения сообщения полученного от пользователя вместе с его уровнем важности
struct LogMessage{
    std::string message;
    LogLevel level;
};

// очередь сообщений, полученных от пользователя
std::queue<LogMessage> messageQueue; 


// Переводит строку в LogLevel(Уровень важности для логгера)
bool strtologlvl(const std::string& str, LogLevel& level);


int main(int argc, char* argv[]) {

    // Проверка на корректное кол-во аргументов
    if (argc != 3) {
        std::cerr << "You must write 2 arguments:\n"
                  <<"\t FILE_NAME,\n"
                  <<"\t IMPORTANCE_LEVEL\n";
        return 1;
    }
    
    std::string filename = argv[1]; // имя файла
    LogLevel level; // уровень важности по умолчанию 
    
    // Проверка на корректность 2-го введённого аргумента
    if (!strtologlvl(argv[2], level)) {
        std::cerr << "ERROR: Unknown level:" << level << " !\n";
        return 2;
    }

    FileLogger logger(filename, level);

    // Проверка на корректность 3-го введённого аргумента
    if (!logger.File_is_open()) {
        std::cerr << "ERROR: Can not open file: " << filename << " !\n";
        return 3;
    }

    std::cout << "Enter messages (optionally with level):\n"
              << "Format: <message> [LEVEL]\n"
              << "Type 'exit' to quit.\n";


    std::string input;
    while(true) {
        std::getline(std::cin, input);
        if (input == "exit") {
            break;
        }

        std::string message;
        LogLevel lvl;

        size_t startmes = input.find('<');
        size_t endmes = input.find('>');

        size_t startlvl = input.find('[');
        size_t endlvl = input.find(']');

        if (startmes != std::string::npos && endmes != std::string::npos) {
            if (startlvl != std::string::npos && endlvl != std::string::npos) {
                std::string strlevel = input.substr(startlvl + 1, endlvl - startlvl -1);
                std::string strmes = input.substr(startmes +1, endmes - startmes -1);
                if (!strtologlvl(strlevel,lvl)) {
                    std::cerr << "Unknown format!\n";
                }
                else {
                    LogMessage M = {strmes, lvl};
                    messageQueue.push(M);
                }
            }
            else if(startlvl == std::string::npos && endlvl == std::string::npos) {
                std::string strmes = input.substr(startmes +1, endmes - startmes -1);
                LogMessage M = {strmes,level};
                messageQueue.push(M);
            }
            else {
                std::cerr << "Unknown format!\n";
            }
        }
        else {
            std::cerr << "Unknown format!\n";
        }
    }

    return 0;
}

bool strtologlvl(const std::string& str, LogLevel& level) {
    if (str == "DEBUG") { level = LogLevel::DEBUG;  return true;}
    else if (str == "INFO") {level = LogLevel::INFO; return true;}
    else if (str == "WARNING") {level =  LogLevel::WARNING; return true;}
    else if (str == "ERROR") {level = LogLevel::ERROR; return true;}
    else {return false;}
}