#ifndef __PRINT
#define __PRINT

#include <string>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLUE    "\x1b[38;5;27m"

void PrintError(const std::string& message);

void PrintWarning(const std::string& message);

void PrintInfo(const std::string& message);

void PrintVerbose(const std::string& message);

#endif