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
