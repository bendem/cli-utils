#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using str = const std::string&;

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

struct args_t {
    std::unordered_map<std::string, std::string> options;
    std::unordered_set<std::string> flags;
    std::vector<std::string> arguments;
    std::string program_name;

    bool has_flag(str name) const;
    str get_or(str name, str def) const;

};

args_t parse_args(int argc, const char* argv[]);

bool HELP(args_t args, str usage, str description, const std::vector<std::string>& arguments);

template<typename Action, typename... Args>
void for_lines_in(std::istream& in, Action action, Args... args) {
    while (true) {
        std::string line;
        std::getline(in, line);
        if (!in) break;

        action(line, args...);
    }
}

std::string format_memory(std::uintmax_t amount);

#endif
