#ifndef UTILS_HPP
#define UTILS_HPP

#include <functional>
#include <iostream>

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
