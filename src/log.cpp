#include <ctime>
#include <iomanip>

#include "utils.hpp"

int exec(str line, str format) {
    std::time_t t = std::time(nullptr);
    std::cout << std::put_time(std::localtime(&t), format.data()) << ' ' << line << std::endl;

    return 0;
}

int main(int argc, char const *argv[]) {
    auto args = parse_args(argc, argv);
    if (args.has_flag("help")) {
        // todo write help
        return 0;
    }

    return for_lines_in(std::cin, exec, args.get_or("format", "[%F %T]"));
}
