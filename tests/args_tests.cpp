#include <catch.hpp>

#include "utils.hpp"

TEST_CASE("", "[args]") {
    const char* args[] = {
        "program",
        "--help",
        "--format=blue",
        "hi"
    };
    auto parsed = parse_args(4, args);

    std::vector<std::string> expected_arguments = { "hi" };
    std::unordered_map<std::string, std::string> expected_options = { { "format" , "blue" } };
    std::unordered_set<std::string> expected_flags = { "help" };

    REQUIRE(parsed.program_name == "program");
    REQUIRE(parsed.arguments == expected_arguments);
    REQUIRE(parsed.options == expected_options);
    REQUIRE(parsed.flags == expected_flags);
}
