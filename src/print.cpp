#include "AlfSimulator/print.h"
#include "AlfSimulator/main.h"
#include <chrono>
#include <iostream>
#include <iomanip>

void Print(const std::string &type, const std::string &message) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;

    // print type
    std::cout << '[' << type << ']';
    std::cout << ' ';

    // print timestamp
    //std::cout << '[' << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S") << '.' << std::setw(6) << std::setfill('0') << us.count() << ']';
    std::cout << ' ';

    // print message
    std::cout << message;
    std::cout << std::endl;
}

void PrintError(const std::string &message) {
    Print(std::string(ANSI_COLOR_RED) + "ERROR" + ANSI_COLOR_RESET, message);
}

void PrintWarning(const std::string &message) {
    Print(std::string(ANSI_COLOR_YELLOW) + "WARNING" + ANSI_COLOR_RESET, message);
}

void PrintInfo(const std::string &message) {
    Print(std::string(ANSI_COLOR_GREEN) + "INFO" + ANSI_COLOR_RESET, message);
}

void PrintVerbose(const std::string &message) {
    if (config->isVerbose()) {
        Print(std::string(ANSI_COLOR_BLUE) + "VERBOSE" + ANSI_COLOR_RESET, message);
    }
}
