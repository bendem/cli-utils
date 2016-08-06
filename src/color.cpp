#include <regex>

#include "utils.hpp"

const std::string BEFORE = ANSI_RED;
const std::string AFTER  = ANSI_RESET;

int color(str line, str search) {
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

int main(int argc, char const *argv[]) {
    auto args = parse_args(argc, argv);
    if (args.has_flag("help")) {
        // todo write help
        return 0;
    }

    if (args.arguments.empty()) {
        std::cerr << "Missing search parameter, see --help for more information." << std::endl;
    }

    return for_lines_in(std::cin, color, args.arguments[0]);
}
