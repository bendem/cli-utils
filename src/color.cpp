#include "utils.hpp"

const std::string BEFORE = ANSI_RED;
const std::string AFTER  = ANSI_RESET;

int color(const std::string& line, const std::string& search) {
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
    if (argc < 2) {
        std::cerr << "Missing search parameter" << std::endl;
        return 1;
    }

    std::string search = argv[1];
    return for_lines_in(std::cin, color, search);
}
