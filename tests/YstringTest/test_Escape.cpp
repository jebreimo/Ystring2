//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-02-02.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Escape.hpp"
#include <catch2/catch_test_macros.hpp>
#include "U8Adapter.hpp"

using namespace ystring;

TEST_CASE("Test find_first_escaped_character")
{
    std::string_view str = R"(Foo \a \x12FG \uAbcDE\\\10)";
    auto sub = find_first_escaped_character(str, 0);
    REQUIRE(str.substr(sub.offset, sub.length) == "\\a");
    sub = find_first_escaped_character(str, sub.end());
    REQUIRE(str.substr(sub.offset, sub.length) == "\\x12F");
    sub = find_first_escaped_character(str, sub.end());
    REQUIRE(str.substr(sub.offset, sub.length) == "\\uAbcD");
    sub = find_first_escaped_character(str, sub.end());
    REQUIRE(str.substr(sub.offset, sub.length) == "\\\\");
    sub = find_first_escaped_character(str, sub.end());
    REQUIRE(str.substr(sub.offset, sub.length) == "\\10");
}

TEST_CASE("Test has_escaped_characters")
{
    REQUIRE(has_escaped_characters("AB \\x41 \\' \\a"));
    REQUIRE_FALSE(has_escaped_characters("AB CD EF"));
}

TEST_CASE("Test escape_json")
{
    REQUIRE(escape_json("AB\t \\ \17ty") == "AB\\t \\\\ \\u000Fty");
}

TEST_CASE("Test unescape")
{
    REQUIRE(unescape("AB \\x41 \\' \\a") == "AB A \' \a");
    REQUIRE(unescape("\\040") == "\040");
    REQUIRE(unescape("A \\uD800\\uDC00 B") == U8("A \U00010000 B"));
}
