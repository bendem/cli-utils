#ifndef TABLE_T_HPP
#define TABLE_T_HPP

#include <array>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "strings.hpp"

template<int width>
class table_t {
public:
    enum alignment {
        left, right
    };

    std::vector<std::array<std::string, width>> lines;
    std::array<unsigned, width> column_max_width {};
    std::array<alignment, width> alignments {};

    table_t<width>& align(std::array<table_t<width>::alignment, width> alignments);
    table_t<width>& add(std::array<std::string, width> line);

    std::ostream& print(std::ostream& out) const;
};

template<int width>
table_t<width>& table_t<width>::align(std::array<table_t<width>::alignment, width> alignments) {
    this->alignments = alignments;
    return *this;
}

template<int width>
table_t<width>& table_t<width>::add(std::array<std::string, width> line) {
    lines.push_back(line);

    for (size_t i = 0; i < line.size(); ++i) {
        if (line[i].length() > column_max_width[i]) {
            column_max_width[i] = line[i].length();
        }
    }

    return *this;
}

template<int width>
std::ostream& table_t<width>::print(std::ostream& out) const {
    for (auto it = lines.begin(); it != lines.end(); ++it) {
        for (size_t i = 0; i < it->size(); ++i) {
            out
                << std::setw(column_max_width[i])
                << (alignments[i] == left ? std::left : std::right);

            if (it == lines.begin()) {
                out << to_upper((*it)[i]);
            } else {
                out << (*it)[i];
            }

            if (i + 1 != it->size()) {
                out << " | ";
            }
        }
        out << std::endl;
    }

    return out;
}

#endif
