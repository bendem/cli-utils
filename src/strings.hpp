#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <locale>
#include <string>

uint32_t next_codepoint(std::string::const_iterator& it);
std::string codepoint_to_string(uint32_t codepoint);

template<typename Function, typename... Args>
std::string map_codepoints(const std::string& string, Function function, Args... args);
std::string to_upper(const std::string& string, const std::locale& locale = std::locale(""));
std::string to_lower(const std::string& string, const std::locale& locale = std::locale(""));

#endif
