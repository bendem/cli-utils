#include <ctime>
#include <iomanip>

#include "utils.hpp"

int exec(const std::string& line) {
    std::time_t t = std::time(nullptr);
    std::cout << std::put_time(std::localtime(&t), "[%F %T] ") << line << std::endl;

    return 0;
}

int main(int argc, char const *argv[]) {
    return for_lines_in(std::cin, exec);
}
