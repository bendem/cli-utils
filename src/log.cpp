#include <ctime>
#include <iomanip>

#include "utils.hpp"

int main(int argc, char const *argv[]) {
    auto args = parse_args(argc, argv);

    if (HELP(args,
        "[--format=<format>]",
        "Prepends each line of input with the time it was output at.",
        {
            "--format=<value>: Specifies the format of the time to prepend (default: [%F %T])."
        }
    )) return 0;

    auto format = args.get_or("format", "[%F %T]");
    for_lines_in(std::cin, [&format](auto& line) {
        std::time_t t = std::time(nullptr);
        std::cout << std::put_time(std::localtime(&t), format.data()) << ' ' << line << std::endl;
    });

    return 0;
}
