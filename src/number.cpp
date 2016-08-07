#include <iomanip>

#include "utils.hpp"

int main(int argc, char const *argv[]) {
    auto args = parse_args(argc, argv);
    if (args.has_flag("help")) {
        // todo write help
        return 0;
    }

    size_t current = 0;
    size_t padding = args.get_or("expect", "100").length();
    for_lines_in(std::cin, [&current, &padding](auto line) {
        std::cout << std::setw(padding) << ++current << ' ' << line << std::endl;
    });

    return 0;
}
