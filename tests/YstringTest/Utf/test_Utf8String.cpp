//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-06-02.
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Utf/Utf8Chars.hpp"
#include "Ystring/Utf/Utf8String.hpp"
#include <catch2/catch.hpp>

using namespace Ystring;

#define CHECK_CHAR_SEARCH(funcCall, offset, length, chr) \
    do { \
        auto r = (funcCall); \
        REQUIRE(r.first == Subrange((offset), (length))); \
        REQUIRE(r.second == char32_t(chr)); \
    } while (false)

TEST_CASE("Test append")
{
    std::string s;
    REQUIRE(append(s, U'∑') == u8"∑");
    REQUIRE(append(s, U'ı') == u8"∑ı");
    REQUIRE_THROWS(append(s, 0x200000));
}

TEST_CASE("Test contains")
{
    REQUIRE(contains("ABCDE", 'D'));
    REQUIRE(!contains("ABCDE", 'F'));
    REQUIRE(contains(u8"ABC∑ßÖ’Ü‹›ƒ¸√EFG", U'√'));
}

TEST_CASE("Test countCodePoints")
{
    REQUIRE(countCodePoints("") == 0);
    REQUIRE(countCodePoints("A" UTF8_COMBINING_RING_ABOVE "BCDE" UTF8_COMBINING_TILDE) == 7);
}

TEST_CASE("Test endsWith")
{
    REQUIRE(endsWith(u8"ABCDÆØÅQRS", u8"ØÅQRS"));
    REQUIRE(!endsWith(u8"ABCDÆØÅQRS", u8"ØÅQR"));
}

TEST_CASE("Test findFirst")
{
    std::string s;
    s = "ABCDEFGHCDEIJK";
    REQUIRE(findFirst(s, "CDE") == Subrange(2, 3));
    REQUIRE(findFirst(s, "ABCD") == Subrange(0, 4));
    REQUIRE(findFirst(s, "JK") == Subrange(12, 2));
    REQUIRE(!findFirst(s, "BCE"));
}

TEST_CASE("Test findFirstNewline")
{
    REQUIRE(findFirstNewline("abc\nd\nef") == Subrange(3, 1));
    REQUIRE(findFirstNewline("abc\rd\nef") == Subrange(3, 1));
    REQUIRE(findFirstNewline("abc\r\nd\nef") == Subrange(3, 2));
    REQUIRE(findFirstNewline("abc\n\rd\nef") == Subrange(3, 1));
    REQUIRE(findFirstNewline("abc" UTF8_PARAGRAPH_SEPARATOR "d\nef") == Subrange(3, 3));
    REQUIRE(findFirstNewline("\nabc\nd\nef") == Subrange(0, 1));
    REQUIRE(findFirstNewline("abcdef\n") == Subrange(6, 1));
    REQUIRE(!findFirstNewline("abcdef"));
}

TEST_CASE("Test findFirstOf")
{
    char32_t chars[4] = {U'≠', 'A', 'B', U'¿'};
    CHECK_CHAR_SEARCH(findFirstOf(u8"qwe≠≠rty", chars, 4), 3, 3, U'≠');
    REQUIRE(!findFirstOf("qwerty", chars, 4).first);
}

TEST_CASE("Test findLast")
{
    std::string s("ABCDEFGHCDEIJK");
    REQUIRE(findLast(s, "CDE") == Subrange(8, 3));
    REQUIRE(findLast(s, "ABCD") == Subrange(0, 4));
    REQUIRE(findLast(s, "JK") == Subrange(12, 2));
    REQUIRE(!findLast(s, "BCE"));
}

TEST_CASE("Test findLastNewline")
{
    REQUIRE(findLastNewline("abc\nd\nef") == Subrange(5, 1));
    REQUIRE(findLastNewline("abc\nd\ref") == Subrange(5, 1));
    REQUIRE(findLastNewline("abc\nd\r\nef") == Subrange(5, 2));
    REQUIRE(findLastNewline("abc\nd\n\ref") == Subrange(6, 1));
    REQUIRE(findLastNewline("abc\nd" UTF8_PARAGRAPH_SEPARATOR "ef") == Subrange(5, 3));
    REQUIRE(findLastNewline("\nabc\nd\nef\n") == Subrange(9, 1));
    REQUIRE(findLastNewline("\nabcdef") == Subrange(0, 1));
    REQUIRE(!findLastNewline("abcdef"));
}

TEST_CASE("Test findLastOf")
{
    char32_t chars[4] = {U'≠', 'A', 'B', U'¿'};
    CHECK_CHAR_SEARCH(findLastOf(u8"qwe≠≠rty", chars, 4), 6, 3, U'≠');
    REQUIRE(!findLastOf("qwerty", chars, 4).first);
}

TEST_CASE("Test getCodePoint")
{
    CHECK_CHAR_SEARCH(getCodePoint(u8"AB£ƒCD‹ß∂GHR", 0), 0, 1, U'A');
    CHECK_CHAR_SEARCH(getCodePoint(u8"AB£ƒCD‹ß∂GHR", 6), 8, 3, U'‹');
    CHECK_CHAR_SEARCH(getCodePoint(u8"AB£ƒCD‹ß∂GHR", 11), 18, 1, U'R');
    REQUIRE(!getCodePoint(u8"AB£ƒCD‹ß∂GHR", 12).first);
    CHECK_CHAR_SEARCH(getCodePoint(u8"AB£ƒCD‹ß∂GHR", -1), 18, 1, U'R');
    CHECK_CHAR_SEARCH(getCodePoint(u8"AB£ƒCD‹ß∂GHR", -4), 13, 3, U'∂');
    CHECK_CHAR_SEARCH(getCodePoint(u8"AB£ƒCD‹ß∂GHR", -12), 0, 1, U'A');
    REQUIRE(!getCodePoint(u8"AB£ƒCD‹ß∂GHR", -13).first);
}

TEST_CASE("Test getCodePointPos")
{
    REQUIRE(getCodePointPos(u8"AB£ƒCD‹ß∂GHR", 0) == 0);
    REQUIRE(getCodePointPos(u8"AB£ƒCD‹ß∂GHR", 6) == 8);
    REQUIRE(getCodePointPos(u8"AB£ƒCD‹ß∂GHR", 11) == 18);
    REQUIRE(getCodePointPos(u8"AB£ƒCD‹ß∂GHR", 12) == 19);
    REQUIRE(getCodePointPos(u8"AB£ƒCD‹ß∂GHR", 13) == std::string_view::npos);
    REQUIRE(getCodePointPos(u8"AB£ƒCD‹ß∂GHR", -1) == 18);
    REQUIRE(getCodePointPos(u8"AB£ƒCD‹ß∂GHR", -4) == 13);
    REQUIRE(getCodePointPos(u8"AB£ƒCD‹ß∂GHR", -12) == 0);
    REQUIRE(getCodePointPos(u8"AB£ƒCD‹ß∂GHR", -13) == std::string_view::npos);
}
