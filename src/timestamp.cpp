#include <chrono>
#include <iomanip>

#include "utils.hpp"

int main(int argc, char const *argv[]) {
    auto args = parse_args(argc, argv);

    if (HELP(args,
        "",
        "Prepends each line of input with the timestamp (ms) it was output at.",
        {}
    )) return 0;

    for_lines_in(std::cin, [](auto& line) {
        auto epoch = std::chrono::system_clock::now().time_since_epoch();
        auto epoch_ms = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
        std::cout << epoch_ms << ' ' << line << std::endl;
    });

    return 0;
}
