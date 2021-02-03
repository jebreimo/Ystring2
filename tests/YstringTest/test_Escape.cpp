//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-02-02.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Escape.hpp"
#include <catch2/catch.hpp>

using namespace Ystring;

TEST_CASE("Test findFirstEscapedCharacter")
{
    std::string_view str = R"(Foo \a \x12FG \uAbcDE\\\10)";
    auto sub = findFirstEscapedCharacter(str, 0);
    REQUIRE(str.substr(sub.offset, sub.length) == "\\a");
    sub = findFirstEscapedCharacter(str, sub.end());
    REQUIRE(str.substr(sub.offset, sub.length) == "\\x12F");
    sub = findFirstEscapedCharacter(str, sub.end());
    REQUIRE(str.substr(sub.offset, sub.length) == "\\uAbcD");
    sub = findFirstEscapedCharacter(str, sub.end());
    REQUIRE(str.substr(sub.offset, sub.length) == "\\\\");
    sub = findFirstEscapedCharacter(str, sub.end());
    REQUIRE(str.substr(sub.offset, sub.length) == "\\10");
}

TEST_CASE("Test hasEscapedCharacters")
{
    REQUIRE(hasEscapedCharacters("AB \\x41 \\' \\a"));
    REQUIRE_FALSE(hasEscapedCharacters("AB CD EF"));
}

TEST_CASE("Test escapeJson")
{
    REQUIRE(escapeJson("AB\t \\ \17ty") == "AB\\t \\\\ \\u000Fty");
}

TEST_CASE("Test unescape")
{
    REQUIRE(unescape("AB \\x41 \\' \\a") == "AB A \' \a");
    REQUIRE(unescape("\\040") == "\040");
    REQUIRE(unescape("A \\uD800\\uDC00 B") == u8"A \U00010000 B");
}
