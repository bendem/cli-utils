#ifndef UTILS_HPP
#define UTILS_HPP

#include <functional>
#include <iostream>
#include <string>

const std::string ANSI_ESCAPE  = "\033[";
const std::string ANSI_BLACK   = ANSI_ESCAPE + "30m";
const std::string ANSI_RED     = ANSI_ESCAPE + "31m";
const std::string ANSI_GREEN   = ANSI_ESCAPE + "32m";
const std::string ANSI_YELLOW  = ANSI_ESCAPE + "33m";
const std::string ANSI_BLUE    = ANSI_ESCAPE + "34m";
const std::string ANSI_MAGENTA = ANSI_ESCAPE + "35m";
const std::string ANSI_CYAN    = ANSI_ESCAPE + "36m";
const std::string ANSI_WHITE   = ANSI_ESCAPE + "37m";
const std::string ANSI_RESET   = ANSI_ESCAPE + "0m";

template<typename Action, typename... Args>
int for_lines_in(std::istream& in, Action action, Args... args) {
    while (true) {
        std::string line;
        std::getline(in, line);
        if (!in) break;

        int res;
        if (res = action(line, args...) != 0) {
            return res;
        }
    }
    return 0;
}

#endif
