#include <cassert>

#include "strings.hpp"

uint32_t next_codepoint(std::string::const_iterator& it) {
    static_assert(sizeof(wchar_t) >= 4);

    unsigned char c = *it;
    ++it;
    if ((c & (1 << 7)) == 0) {
        return c;
    }

    uint8_t remaining;
    if ((c & 0b100000) == 0) {
        remaining = 1;
        c &= 0b11111;
    } else if ((c & 0b10000) == 0) {
        remaining = 2;
        c &= 0b1111;
    } else if ((c & 0b1000) == 0) {
        remaining = 3;
        c &= 0b111;
    } else {
        assert(false);
    }

    uint32_t result = c;
    for (uint8_t i = 0; i < remaining; ++i, ++it) {
        result = (result << 6) | (*it & 0x3f);
    }

    return result;
}

std::string codepoint_to_string(uint32_t codepoint) {
    static_assert(sizeof(wchar_t) >= 4);

    unsigned size;
    uint8_t result[4] = { 0 };
    if (codepoint > 0x00000000 && codepoint < 0x0000007F) {
        size = 1;
        result[3] = codepoint & 0x7f;
    } else if (codepoint > 0x00000080 && codepoint < 0x000007FF) {
        result[3] = 0x80 | codepoint & 0x3f;
        result[2] = 0xc0 | codepoint >> 6 & 0x1f;
        size = 2;
    } else if (codepoint > 0x00000800 && codepoint < 0x0000FFFF) {
        result[3] = 0x80 | codepoint & 0x3f;
        result[2] = 0x80 | codepoint >> 6 & 0x3f;
        result[1] = 0xe0 | codepoint >> 12 & 0x0f;
        size = 3;
    } else if (codepoint > 0x00010000 && codepoint < 0x0010FFFF) {
        result[3] = 0x80 | codepoint & 0x3f;
        result[2] = 0x80 | codepoint >> 6 & 0x3f;
        result[1] = 0x80 | codepoint >> 12 & 0x3f;
        result[0] = 0xf0 | codepoint >> 18 & 0x70;
        size = 4;
    } else {
        assert(false);
    }

    return std::string(reinterpret_cast<const char*>(&result) + (4 - size), size);
}

#include <iostream>

template<typename Function, typename... Args>
std::string map_codepoints(const std::string& string, Function function, Args... args) {
    std::string out;
    out.reserve(string.length());

    for (auto it = string.begin(), end = string.end(); it != end; ) {
        auto codepoint = next_codepoint(it);
        out += codepoint_to_string(function(codepoint, args...));
    }

    return out;
}

std::string to_upper(const std::string& string, const std::locale& locale) {
    return map_codepoints<wchar_t(wchar_t, const std::locale&), const std::locale&>(string, std::toupper, locale);
}

std::string to_lower(const std::string& string, const std::locale& locale) {
    return map_codepoints<wchar_t(wchar_t, const std::locale&), const std::locale&>(string, std::tolower, locale);
}
