#include "catch.hpp"
#include "strings.hpp"

TEST_CASE("ascii", "[strings]") {
    std::string str = "hi bob";
    auto it = str.cbegin();

    REQUIRE(next_codepoint(it) == 'h');
    REQUIRE(next_codepoint(it) == 'i');
    REQUIRE(next_codepoint(it) == ' ');
    REQUIRE(next_codepoint(it) == 'b');
    REQUIRE(next_codepoint(it) == 'o');
    REQUIRE(next_codepoint(it) == 'b');

    REQUIRE(codepoint_to_string('h') == "h");
    REQUIRE(codepoint_to_string('i') == "i");
    REQUIRE(codepoint_to_string(' ') == " ");
    REQUIRE(codepoint_to_string('b') == "b");
    REQUIRE(codepoint_to_string('o') == "o");
    REQUIRE(codepoint_to_string('b') == "b");
}

TEST_CASE("multibytes", "[strings]") {
    std::string str = "涊";
    auto it = str.cbegin();

    REQUIRE(next_codepoint(it) == 0x6d8a);
    REQUIRE(codepoint_to_string(0x6d8a) == str);
}

TEST_CASE("multibytes multichars", "[strings]") {
    std::string str = "¯\\_(ツ)_/¯";
    auto it = str.cbegin();

    REQUIRE(next_codepoint(it) == 175);
    REQUIRE(next_codepoint(it) == 92);
    REQUIRE(next_codepoint(it) == 95);
    REQUIRE(next_codepoint(it) == 40);
    REQUIRE(next_codepoint(it) == 12484);
    REQUIRE(next_codepoint(it) == 41);
    REQUIRE(next_codepoint(it) == 95);
    REQUIRE(next_codepoint(it) == 47);
    REQUIRE(next_codepoint(it) == 175);

    REQUIRE(codepoint_to_string(175) == "¯");
    REQUIRE(codepoint_to_string(92) == "\\");
    REQUIRE(codepoint_to_string(95) == "_");
    REQUIRE(codepoint_to_string(40) == "(");
    REQUIRE(codepoint_to_string(12484) == "ツ");
    REQUIRE(codepoint_to_string(41) == ")");
    REQUIRE(codepoint_to_string(95) == "_");
    REQUIRE(codepoint_to_string(47) == "/");
    REQUIRE(codepoint_to_string(175) == "¯");
}

TEST_CASE("turkey test", "[strings]") {
    REQUIRE(to_upper("integer", std::locale("en_US.UTF-8")) == "INTEGER");
    REQUIRE(to_upper("integer", std::locale("tr_TR.UTF-8")) == "İNTEGER");

    REQUIRE(to_lower("INTEGER", std::locale("en_US.UTF-8")) == "integer");
    REQUIRE(to_lower("INTEGER", std::locale("tr_TR.UTF-8")) == "ınteger");
}
