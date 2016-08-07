#include "utils.hpp"

int main(int argc, char const *argv[]) {
    auto args = parse_args(argc, argv);
    if (args.has_flag("help")) {
        // todo write help
        return 0;
    }

    if (args.arguments.empty()) {
        std::cerr << "Missing argument, see --help for more information." << std::endl;
        return 1;
    }

    auto prepend = args.arguments[0] + (args.has_flag("no-space") ? "" : " ");
    return for_lines_in(std::cin, [&prepend](auto line) {
        std::cout << prepend << line << std::endl;
        return 0;
    });
}
