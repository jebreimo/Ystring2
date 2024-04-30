//****************************************************************************
// Copyright © 2024 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2024-03-31.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/GlobPattern.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Test character set")
{
    std::string_view str = "[-+0-9A-Fa-f]";
    auto charset = ystring::extract_char_set(str);
    REQUIRE(str.empty());
    REQUIRE(charset.negated == false);
    REQUIRE(charset.ranges.size() == 5);

    using P = std::pair<char32_t, char32_t>;
    REQUIRE(charset.ranges[0] == P('-', '-'));
    REQUIRE(charset.ranges[1] == P('+', '+'));
    REQUIRE(charset.ranges[2] == P('0', '9'));
    REQUIRE(charset.ranges[3] == P('A', 'F'));
    REQUIRE(charset.ranges[4] == P('a', 'f'));
}

TEST_CASE("Test negated character set")
{
    std::string_view str = R"([^+\-0-9\n\]]ABC)";
    auto charset = ystring::extract_char_set(str);
    REQUIRE(str == "ABC");
    REQUIRE(charset.negated == true);
    REQUIRE(charset.ranges.size() == 5);

    using P = std::pair<char32_t, char32_t>;
    REQUIRE(charset.ranges[0] == P('+', '+'));
    REQUIRE(charset.ranges[1] == P('-', '-'));
    REQUIRE(charset.ranges[2] == P('0', '9'));
    REQUIRE(charset.ranges[3] == P('\n', '\n'));
    REQUIRE(charset.ranges[4] == P(']', ']'));
}

TEST_CASE("Extract wildcards")
{
    std::string_view str = "**?AB";
    std::ignore = ystring::extract_stars(str);
    REQUIRE(str == "?AB");
}

TEST_CASE("Extract string")
{
    std::string_view pattern = "foo_*";
    auto str = ystring::extract_string(pattern, {});
    REQUIRE(str == "foo_");
    REQUIRE(pattern == "*");
}

TEST_CASE("Parse glob pattern")
{
    std::string_view str = "foo_*.{png,jpg}";
    auto glob = ystring::parse_glob_pattern(str, {});
    REQUIRE(bool(glob));
    REQUIRE(glob->parts.size() == 4);
    REQUIRE(glob->tail_length == 2);
}
