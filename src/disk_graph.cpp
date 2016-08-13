#include <cmath>
#include <experimental/filesystem>
#include <fstream>
#include <iomanip>

#include "filesystem.hpp"
#include "table.hpp"
#include "utils.hpp"

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

using dg_table = table_t<std::string, std::string, std::string, uintmax_t, uintmax_t, uintmax_t, double>;

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
    bool all = args.has_flag("all");

    dg_table table({
            "name",
            "mount",
            "graph",
            "capacity",
            "used",
            "free",
            "percent free",
        }, {
            ident_formatter,
            ident_formatter,
            ident_formatter,
            format_memory,
            format_memory,
            format_memory,
            [](auto value) {
                std::ostringstream buffer;
                if (std::isnan(value)) {
                    buffer << "- %";
                } else {
                    buffer << std::setprecision(0) << std::fixed << value << " %";
                }
                return buffer.str();
            },
        }, {
            table_alignment::left,
            table_alignment::left,
            table_alignment::left,
            table_alignment::right,
            table_alignment::right,
            table_alignment::right,
            table_alignment::right,
        }
    );

    for_lines_in(mounts, [all, &table](str line) {
        auto descriptor = parse_mount_line(line);

        if (descriptor.name[0] != '/' && !all) {
            return;
        }

        auto space = fs::space(descriptor.mount_point);
        auto percent = (double) space.available / space.capacity * 100;
        unsigned width = 10;
        unsigned width_full = (double) width / 100 * percent;

        std::ostringstream graph_buffer;
        for (unsigned i = 0; i < width; ++i) {
            graph_buffer << (i < width_full ? '=' : ' ');
        }

        table.add({
            descriptor.name,
            descriptor.mount_point.native(),
            graph_buffer.str(),
            space.capacity,
            space.capacity - space.available,
            space.available,
            percent,
        });
    });

    table.print(std::cout);

    return 0;
}
