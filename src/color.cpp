#include <regex>

#include "find.hpp"
#include "utils.hpp"

const std::string BEFORE = ANSI_RED;
const std::string AFTER  = ANSI_RESET;

template<typename Search>
void color(str line, Search search) {
    match_state_t match_state {};
    match_result_t match;
    size_t offset_remaining = 0;

    while ((match = find(line, search, match_state)).start != std::string::npos) {
        std::cout << line.substr(offset_remaining, match.start - offset_remaining) << BEFORE << match.match << AFTER;
        offset_remaining = match.start + match.match.length();
    }
    std::cout << line.substr(offset_remaining) << std::endl;
}

int main(int argc, char const *argv[]) {
    auto args = parse_args(argc, argv);
    if (HELP(args,
        "[--regex [--case-insensitive]] <search>",
        "Colors the provided argument in each line of input.",
        {
            "--regex:            Treats the search argument as a regex",
            "--case-insensitive: If --regex was provided, makes the search case insensitive"
        }
    )) return 0;

    if (args.arguments.empty()) {
        std::cerr << "Missing search parameter, see --help for more information." << std::endl;
        return 1;
    }

    if (args.has_flag("regex")) {
        auto regex = make_regex(args.arguments[0], args.has_flag("case-insensitive"));
        for_lines_in(std::cin, color<const std::regex&>, regex);
    } else {
        for_lines_in(std::cin, color<str>, args.arguments[0]);
    }

    return 0;
}
