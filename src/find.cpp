#include "find.hpp"

std::regex make_regex(const std::string search, bool case_insensitive) {
    return std::regex(search, std::regex::extended
        | std::regex::optimize
        | std::regex::nosubs
        | std::regex::collate
        | case_insensitive
            ? std::regex::icase
            : static_cast<std::regex_constants::syntax_option_type>(0));;
}

match_result_t find(std::string line, std::string search, match_state_t& match_state) {
    const size_t pos = line.find(search, match_state.offset);
    if (pos != std::string::npos) {
        match_state.offset = pos + search.length();
        return { pos, line.substr(pos, search.length()) };
    }

    return { std::string::npos, {} };
}

match_result_t find(std::string line, std::regex search, match_state_t& match_state) {
    static const std::sregex_iterator empty {};
    if (match_state.regex_iterator == empty) {
        match_state.regex_iterator = std::sregex_iterator(line.begin(), line.end(), search);
    }

    if (match_state.regex_iterator != empty) {
        match_result_t result {
            static_cast<size_t>(match_state.regex_iterator->position()),
            match_state.regex_iterator->str()
        };
        ++match_state.regex_iterator;
        return result;
    }

    return { std::string::npos, {} };
}

bool contains(std::string line, std::string search) {
    match_state_t ignored {};
    return find(line, search, ignored).start != std::string::npos;
}

bool contains(std::string line, std::regex search) {
    match_state_t ignored {};
    return find(line, search, ignored).start != std::string::npos;
}
