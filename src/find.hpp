#ifndef CLI_UTILS_FIND_HPP
#define CLI_UTILS_FIND_HPP

#include <regex>
#include <string>
#include <utility>

struct match_state_t {
    size_t offset { 0 };
    std::sregex_iterator regex_iterator {};
};

struct match_result_t {
    size_t start;
    std::string match;
};

std::regex make_regex(const std::string search, bool case_insensitive = false);

match_result_t find(std::string line, std::string search, match_state_t& match_state);
match_result_t find(std::string line, std::regex  search, match_state_t& match_state);

bool contains(std::string line, std::string search);
bool contains(std::string line, std::regex  search);

#endif
