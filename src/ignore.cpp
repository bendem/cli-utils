#include "utils.hpp"
#include "find.hpp"

template<typename Pattern>
auto ignore_before(Pattern&& pattern, bool& ignore, bool include_match) {
    return [&](auto& line) {
        if (contains(line, pattern)) {
            ignore = false;
            if (include_match) {
                return;
            }
        }

        if (!ignore) {
            std::cout << line << std::endl;
            return;
        }
    };
}


template<typename Pattern>
auto ignore_after(Pattern&& pattern, bool& ignore, bool include_match) {
    return [&](auto& line) {
        if (ignore) return;

        if (contains(line, pattern)) {
            ignore = true;
            if (include_match) {
                return;
            }
        }

        std::cout << line << std::endl;
    };
}

int main(int argc, const char* argv[]) {
    auto args = parse_args(argc, argv);
    if (HELP(args,
        "[--before --after --include-match --regex --case-insensitive] <search>",
        "Ignores all lines before/after a line containing the provided string.",
        {
            "--before:           Ignore the lines before the match (default)",
            "--after:            Ignore the lines after the match",
            "--include-match:    Also ignore the line matching the provided string",
            "--regex:            Treat the provided string as a regex",
            "--case-insensitive: Make the regex case insensitive",
        }
    )) return 0;

    if (args.arguments.empty()) {
        std::cerr << "Missing search parameter, see --help for more information." << std::endl;
        return 1;
    }

    bool before = args.has_flag("before");
    bool after = args.has_flag("after");
    if (!after && !before) {
        before = true;
    }

    bool include_match = args.has_flag("include-match");
    bool regex = args.has_flag("regex");
    bool case_insensitive = args.has_flag("case-insensitive");
    std::string search = args.arguments[0];

    if (before && !after) {
        bool ignore = true;

        if (regex) {
            for_lines_in(std::cin, ignore_before(make_regex(search, case_insensitive), ignore, include_match));
        } else {
            for_lines_in(std::cin, ignore_before(search, ignore, include_match));
        }
    } else if (after && !before) {
        bool ignore = false;

        if (regex) {
            for_lines_in(std::cin, ignore_after(make_regex(search, case_insensitive), ignore, include_match));
        } else {
            for_lines_in(std::cin, ignore_after(search, ignore, include_match));
        }
    } else {
        std::cerr << "Cannot use both --after and --before together" << std::endl;
        return 1;
    }

    return 0;
}
