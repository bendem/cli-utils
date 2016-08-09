#include <iomanip>
#include <sstream>

#include "utils.hpp"

bool args_t::has_flag(str name) const {
    return flags.find(name) != flags.end();
}

str args_t::get_or(str name, str def) const {
    auto it = options.find(name);
    if (it == options.end()) {
        return def;
    }
    return it->second;
}

args_t parse_args(int argc, const char* argv[]) {
    args_t args;
    args.program_name = argv[0];

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.substr(0, 2) == "--") {
            size_t pos = arg.find('=');
            if (pos == std::string::npos) {
                args.flags.insert(arg.substr(2));
            } else {
                args.options.insert({ arg.substr(2, pos - 2), arg.substr(pos + 1) });
            }

        } else {
            args.arguments.push_back(arg);
        }
    }

    return args;
}

bool HELP(args_t args, const std::string& usage, const std::string& description, const std::vector<std::string>& arguments) {
    if (args.has_flag("help")) {
        std::cout
            << "Usage: " << args.program_name << ' ' << usage << std::endl
            << description << std::endl << std::endl
            << "Options are:" << std::endl;

        for (auto argument : arguments) {
            std::cout << "    " << argument << std::endl;
        }

        return true;
    }
    return false;
}

std::string format_memory(std::uintmax_t amount) {
    std::array<char, 9> units {
        ' ',
        'K',
        'M',
        'G',
        'T',
        'P',
        'E',
        'Z',
        'Y',
    };
    unsigned unit = 0;
    while (amount > 1024 && unit < units.size()) {
        ++unit;
        amount >>= 10;
    }

    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << std::setw(6) << amount << ' ' << units[unit] << 'B';
    return out.str();
}
