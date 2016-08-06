#include <regex>

#include "utils.hpp"

const std::string BEFORE = ANSI_RED;
const std::string AFTER  = ANSI_RESET;

int color_string(str line, str search) {
    size_t current = 0;
    size_t pos = line.find(search);
    while (pos != std::string::npos) {
        std::cout << line.substr(current, pos - current) << BEFORE << search << AFTER;
        current = pos + search.length();
        pos = line.find(search, current);
    }
    std::cout << line.substr(current) << std::endl;

    return 0;
}

int color_regex(str line, std::regex search) {
    std::string suffix = line;
    for (std::sregex_iterator it(line.begin(), line.end(), search), end; it != end; ++it) {
        std::cout << it->prefix() << BEFORE << it->str() << AFTER;
        suffix = it->suffix();
    }
    std::cout << suffix << std::endl;

    return 0;
}

int main(int argc, char const *argv[]) {
    auto args = parse_args(argc, argv);
    if (args.has_flag("help")) {
        // todo write help
        return 0;
    }

    if (args.arguments.empty()) {
        std::cerr << "Missing search parameter, see --help for more information." << std::endl;
    }

    if (args.has_flag("regex")) {
        std::regex search(args.arguments[0], std::regex::extended
            | std::regex::optimize
            | std::regex::nosubs
            | std::regex::collate
            | args.has_flag("case-insensitive")
                ? std::regex::icase
                : static_cast<std::regex_constants::syntax_option_type>(0));

        return for_lines_in(std::cin, color_regex, search);
    } else {
        return for_lines_in(std::cin, color_string, args.arguments[0]);
    }

}
