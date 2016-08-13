#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <locale>
#include <string>

uint32_t next_codepoint(std::string::const_iterator& it);
std::string codepoint_to_string(uint32_t codepoint);

template<typename Function>
std::string map_codepoints(const std::string& string, Function function, const std::locale& locale);
std::string to_upper(const std::string& string, const std::locale& locale = std::locale(""));
std::string to_lower(const std::string& string, const std::locale& locale = std::locale(""));

inline uint32_t codepoint_to_upper(uint32_t c, const std::locale& loc) {
    static_assert(sizeof(wchar_t) >= 4, "std::toupper will not work with some other type than wchar_t and wchar_t can't hold a codepoint");
    return static_cast<uint32_t>(std::toupper(static_cast<wchar_t>(c), loc));
}

inline uint32_t codepoint_to_lower(uint32_t c, const std::locale& loc) {
    static_assert(sizeof(wchar_t) >= 4, "std::toupper will not work with some other type than wchar_t and wchar_t can't hold a codepoint");
    return static_cast<uint32_t>(std::tolower(static_cast<wchar_t>(c), loc));
}

#endif
