#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include "../LoggerLib/include/FileLogger.hpp"
#include "../LoggerLib/include/LogLevel.hpp"

void test_file_open() {
    FileLogger logger("test1.log", LogLevel::INFO);
    assert(logger.File_is_open());
    std::remove("test1.log");
}

void test_write_and_filter() {
    const std::string filename = "test2.log";
    {
        FileLogger logger(filename, LogLevel::WARNING);
        // Ниже уровня WARNING — не должно писаться
        assert(logger.addLog("This is DEBUG", LogLevel::DEBUG) == false);
        // Равен или выше — должно писаться
        assert(logger.addLog("This is ERROR", LogLevel::ERROR) == true);
    }

    std::ifstream file(filename);
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    file.close();

    assert(content.find("This is DEBUG") == std::string::npos);
    assert(content.find("This is ERROR") != std::string::npos);

    std::remove(filename.c_str());
}

void test_set_log_level() {
    FileLogger logger("test3.log", LogLevel::ERROR);
    logger.setLogLevel(LogLevel::DEBUG);
    assert(logger.addLog("Debug now allowed", LogLevel::DEBUG) == true);
    std::remove("test3.log");
}

int main() {
    test_file_open();
    test_write_and_filter();
    test_set_log_level();

    std::cout << "All LoggerLib tests passed!\n";
    return 0;
}
