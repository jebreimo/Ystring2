//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-02-02.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Unescape.hpp"
#include <catch2/catch_test_macros.hpp>
#include "U8Adapter.hpp"
#include "Ystring/YstringException.hpp"

using namespace ystring;

TEST_CASE("Test find_first_escaped_character")
{
    std::string_view str = R"(Foo \a \x12FG \uAbcDE\\\10)";
    auto sub = find_first_escaped_char(str, 0);
    REQUIRE(str.substr(sub.offset, sub.length) == "\\a");
    sub = find_first_escaped_char(str, sub.end());
    REQUIRE(str.substr(sub.offset, sub.length) == "\\x12F");
    sub = find_first_escaped_char(str, sub.end());
    REQUIRE(str.substr(sub.offset, sub.length) == "\\uAbcD");
    sub = find_first_escaped_char(str, sub.end());
    REQUIRE(str.substr(sub.offset, sub.length) == "\\\\");
    sub = find_first_escaped_char(str, sub.end());
    REQUIRE(str.substr(sub.offset, sub.length) == "\\10");
}

TEST_CASE("Test has_escaped_characters")
{
    REQUIRE(has_escaped_chars("AB \\x41 \\' \\a"));
    REQUIRE_FALSE(has_escaped_chars("AB CD EF"));
}

TEST_CASE("Test unescape")
{
    REQUIRE(unescape("AB \\x41 \\' \\a") == "AB A \' \a");
    REQUIRE(unescape("\\040") == "\040");
    REQUIRE(unescape("A \\uD800\\uDC00 B") == U8("A \U00010000 B"));
}

TEST_CASE("Test unescape regular ASCII characters")
{
    REQUIRE(unescape("\\[\\^\\{\\-\\}\\]") == "[^{-}]");
}

TEST_CASE("unescape_next with single-char escapes")
{
    std::string_view str = R"(\a\b\f\n\r\t\v\0)";
    auto c = ystring::unescape_next(str);
    REQUIRE(c);
    REQUIRE(*c == '\a');
    REQUIRE(str == R"(\b\f\n\r\t\v\0)");
    c = ystring::unescape_next(str);
    REQUIRE(c);
    REQUIRE(*c == '\b');
    REQUIRE(str == R"(\f\n\r\t\v\0)");
    c = ystring::unescape_next(str);
    REQUIRE(c);
    REQUIRE(*c == '\f');
    REQUIRE(str == R"(\n\r\t\v\0)");
    c = ystring::unescape_next(str);
    REQUIRE(c);
    REQUIRE(*c == '\n');
    REQUIRE(str == R"(\r\t\v\0)");
    c = ystring::unescape_next(str);
    REQUIRE(c);
    REQUIRE(*c == '\r');
    REQUIRE(str == R"(\t\v\0)");
    c = ystring::unescape_next(str);
    REQUIRE(c);
    REQUIRE(*c == '\t');
    REQUIRE(str == R"(\v\0)");
    c = ystring::unescape_next(str);
    REQUIRE(c);
    REQUIRE(*c == '\v');
    REQUIRE(str == R"(\0)");
    c = ystring::unescape_next(str);
    REQUIRE(c);
    REQUIRE(*c == '\0');
    REQUIRE(str.empty());
    REQUIRE(!ystring::unescape_next(str));
}

TEST_CASE("unescape_next with escaped octal characters")
{
    std::string_view str = R"(\40\3771\0)";
    auto c = ystring::unescape_next(str);
    REQUIRE(c);
    REQUIRE(*c == '\40');
    REQUIRE(str == R"(\3771\0)");
    c = ystring::unescape_next(str);
    REQUIRE(c);
    REQUIRE(*c == u'\377');
    REQUIRE(str == R"(1\0)");
    c = ystring::unescape_next(str);
    REQUIRE(c);
    REQUIRE(*c == '1');
    REQUIRE(str == R"(\0)");
    c = ystring::unescape_next(str);
    REQUIRE(c);
    REQUIRE(*c == '\0');
    REQUIRE(str.empty());
    REQUIRE(!ystring::unescape_next(str));
}

TEST_CASE("unescape_next with escaped UTF-16 surrogate pair")
{
    std::string_view str = R"(\uD800\uDC00)";
    auto c = ystring::unescape_next(str);
    REQUIRE(c);
    REQUIRE(*c == U'\U00010000');
    REQUIRE(str.empty());
}

TEST_CASE("unescape_next with UTF-8 char")
{
    std::string_view str = U8("ÆØ");
    auto c = ystring::unescape_next(str);
    REQUIRE(c);
    REQUIRE(*c == u'Æ');
    REQUIRE(str == U8("Ø"));
    c = ystring::unescape_next(str);
    REQUIRE(c);
    REQUIRE(*c == u'Ø');
    REQUIRE(str.empty());
}

TEST_CASE("unescape_next with invalid UTF-8 char")
{
    std::string_view str = "\xC0\x40";
    REQUIRE_THROWS_AS(ystring::unescape_next(str), YstringException);
}

TEST_CASE("unescape_next with invalid escape sequence")
{
    std::string_view str = R"(\x\a)";
    REQUIRE_THROWS_AS(ystring::unescape_next(str), YstringException);
    REQUIRE(str == R"(\x\a)");
}

TEST_CASE("unescape_next with escaped newlines")
{
    std::string_view str = "\\\n\\\nF\\\n";
    auto c = ystring::unescape_next(str);
    REQUIRE(c);
    REQUIRE(*c == u'F');
    REQUIRE(str == "\\\n");
    c = ystring::unescape_next(str);
    REQUIRE(!c);
    REQUIRE(str.empty());
}
