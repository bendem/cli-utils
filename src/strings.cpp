#include <cassert>

#include "strings.hpp"

/*
 * From the rfc (https://tools.ietf.org/html/rfc3629#section-3):
 *
 * The table below summarizes the format of these different octet types.
 * The letter x indicates bits available for encoding bits of the
 * character number.

 * Char. number range  |        UTF-8 octet sequence
 *    (hexadecimal)    |              (binary)
 * --------------------+---------------------------------------------
 * 0000 0000-0000 007F | 0xxxxxxx
 * 0000 0080-0000 07FF | 110xxxxx 10xxxxxx
 * 0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
 * 0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 *
 * Encoding a character to UTF-8 proceeds as follows:
 *
 * 1.  Determine the number of octets required from the character number
 *     and the first column of the table above.  It is important to note
 *     that the rows of the table are mutually exclusive, i.e., there is
 *     only one valid way to encode a given character.
 *
 * 2.  Prepare the high-order bits of the octets as per the second
 *     column of the table.
 *
 * 3.  Fill in the bits marked x from the bits of the character number,
 *     expressed in binary.  Start by putting the lowest-order bit of
 *     the character number in the lowest-order position of the last
 *     octet of the sequence, then put the next higher-order bit of the
 *     character number in the next higher-order position of that octet,
 *     etc.  When the x bits of the last octet are filled in, move on to
 *     the next to last octet, then to the preceding one, etc. until all
 *     x bits are filled in.
 */

uint32_t next_codepoint(std::string::const_iterator& it) {
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

template<typename Function>
std::string map_codepoints(const std::string& string, Function function, const std::locale& locale) {
    std::string out;
    out.reserve(string.length());

    for (auto it = string.begin(), end = string.end(); it != end; ) {
        auto codepoint = next_codepoint(it);
        auto str = function(codepoint, locale);
        out += codepoint_to_string(str);
    }

    return out;
}

std::string to_upper(const std::string& string, const std::locale& locale) {
    return map_codepoints(string, codepoint_to_upper, locale);
}

std::string to_lower(const std::string& string, const std::locale& locale) {
    return map_codepoints(string, codepoint_to_lower, locale);
}
