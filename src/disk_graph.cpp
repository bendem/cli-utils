#include <cmath>
#include <experimental/filesystem>
#include <fstream>
#include <iomanip>

#include "table.hpp"
#include "utils.hpp"

namespace fs = std::experimental::filesystem;

struct descriptor_t {
    std::string name;
    fs::path mount_point;
};

descriptor_t parse_mount_line(str line) {
    auto start = line.find(' ') + 1;
    auto end = line.find(' ', start);

    return {
        line.substr(0, start - 1),
        line.substr(start, end - start)
    };
}

int main(int argc, char const *argv[]) {
    auto args = parse_args(argc, argv);
    if (HELP(args,
        "",
        "",
        {
            "--all: Show all mount points"
        }
    )) return 0;

    std::ifstream mounts("/proc/mounts");
    auto all = args.has_flag("all");

    using table_t7 = table_t<7>;
    table_t7 table;
    table
        .align({
            table_t7::left,
            table_t7::left,
            table_t7::left,
            table_t7::right,
            table_t7::right,
            table_t7::right,
            table_t7::right,
        })
        .add({
            "name",
            "mount",
            "graph",
            "capacity",
            "used",
            "free",
            "percent",
        });

    for_lines_in(mounts, [&all, &table](str line) {
        auto descriptor = parse_mount_line(line);

        if (descriptor.name[0] != '/' && !all) {
            return;
        }

        auto space = fs::space(descriptor.mount_point);
        auto percent = (double) space.available / space.capacity * 100;
        unsigned width = 15;
        unsigned width_full = (double) width / 100 * percent;

        std::ostringstream graph_buffer;
        for (int i = 0; i < width; ++i) {
            graph_buffer << (i < width_full ? '=' : ' ');
        }

        std::ostringstream percent_buffer;
        if (std::isnan(percent)) {
            percent_buffer << "- %";
        } else {
            percent_buffer << std::setprecision(0) << std::fixed << percent << " %";
        }

        table.add({
            descriptor.name,
            descriptor.mount_point.native(),
            graph_buffer.str(),
            format_memory(space.capacity),
            format_memory(space.capacity - space.available),
            format_memory(space.available),
            percent_buffer.str(),
        });
    });

    table.print(std::cout);

    return 0;
}
