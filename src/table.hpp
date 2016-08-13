#ifndef TABLE_T_HPP
#define TABLE_T_HPP

#include <algorithm>
#include <array>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "strings.hpp"


enum class table_alignment {
    left, right
};

template<typename T>
using formatter_t = std::function<std::string(const T&)>;

inline std::string ident_formatter(const std::string& str) {
    return str;
}
inline std::string unsigned_to_string_formatter(const unsigned& something) {
    return std::to_string(something);
}
inline std::string uint8_t_to_string_formatter(const uint8_t& something) {
    return std::to_string(something);
}

template<typename... Types>
class table_t {
public:
    static const size_t column_count { sizeof...(Types) };
    using self_t = table_t<Types...>;
    using headers_t = std::array<std::string, self_t::column_count>;
    using formatters_t = std::tuple<formatter_t<Types>...>;
    using line_t = std::tuple<Types...>;
    using alignments_t = std::array<table_alignment, self_t::column_count>;
    using column_widths_t = std::array<size_t, self_t::column_count>;

    table_t(headers_t headers, formatters_t formatters, alignments_t alignments, size_t sort_index = 0)
            : formatters(formatters), alignments(alignments), sort_index(sort_index) {
        std::transform(headers.begin(), headers.end(), this->headers.begin(), [](auto& x) {
            return to_upper(x);
        });
        update_column_max_width(headers);
    }

    self_t& add(line_t line);
    std::ostream& print(std::ostream& out) const;

private:
    headers_t headers;
    formatters_t formatters;
    alignments_t alignments;
    size_t sort_index;
    std::vector<line_t> lines {};
    column_widths_t column_max_width {};

    void update_column_max_width(const headers_t& line);
    void update_column_max_width(const line_t& line);

};

template<typename... Types>
table_t<Types...>& table_t<Types...>::add(table_t<Types...>::line_t line) {
    // TODO Insert sorted
    lines.push_back(line);
    update_column_max_width(line);

    return *this;
}

template<typename function, typename values_t, typename formatters_t>
void for_each_formatted_value_in_tuple(function f, values_t values, formatters_t formatters) {
    helper_thing(
        f, values, formatters,
        std::make_index_sequence<std::tuple_size<decltype(formatters)>::value>{}
    );
}

template <typename F, typename value_t, typename formatter_t, size_t ...indexes>
void helper_thing(F& f, const value_t& value, const formatter_t& formatter, std::index_sequence<indexes...>) {
    int disc[] {
        0,
        (f(std::get<indexes>(formatter)(std::get<indexes>(value)), indexes), 0)...
    };
    (void)disc;
    //(f(std::get<indexes>(value), std::get<indexes>(formatter)), ...); // C++17 syntax
}

template<typename... Types>
std::ostream& table_t<Types...>::print(std::ostream& out) const {
    auto f = [&out, this](auto value, auto i) {
        out
            << std::setw(column_max_width[i])
            << (alignments[i] == table_alignment::left ? std::left : std::right)
            << value;

        if (i + 1 == column_count) {
            out << std::endl;
        } else {
            out << " | ";
        }
    };

    for (size_t i = 0; i < column_count; ++i) {
        f(headers[i], i);
    }

    for (auto it = lines.begin(); it != lines.end(); ++it) {
        for_each_formatted_value_in_tuple(f, *it, formatters);
    }

    return out;
}

template<typename... Types>
void table_t<Types...>::update_column_max_width(const table_t<Types...>::headers_t& line) {
    for (size_t i = 0; i < column_count; ++i) {
        if (line[i].length() > column_max_width[i]) {
            column_max_width[i] = line[i].length();
        }
    }
}

template<typename... Types>
void table_t<Types...>::update_column_max_width(const table_t<Types...>::line_t& line) {
    for_each_formatted_value_in_tuple([this](auto value, auto i) {
        if (value.length() > column_max_width[i]) {
            column_max_width[i] = value.length();
        }
    }, line, formatters);
}

#endif
