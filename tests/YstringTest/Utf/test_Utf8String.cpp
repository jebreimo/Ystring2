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

namespace
{
    void requireIdentical(std::string_view a, std::string_view b)
    {
        CAPTURE(a, b);
        REQUIRE(a.data() == b.data());
        REQUIRE(a.size() == b.size());
    }

    std::string_view view(const std::string& s,
                          size_t offset = 0,
                          size_t length = std::string::npos)
    {
        return std::string_view(s.data() + offset,
                                std::min(s.size() - offset, length));
    }
}

TEST_CASE("Test append")
{
    std::string s;
    REQUIRE(append(s, GREEK_SMALL_FINAL_SIGMA) == UTF8_GREEK_SMALL_FINAL_SIGMA);
    REQUIRE(append(s, PUNCTUATION_SPACE) ==
            UTF8_GREEK_SMALL_FINAL_SIGMA UTF8_PUNCTUATION_SPACE);
    REQUIRE_THROWS(append(s, 0x200000));
}

TEST_CASE("Test contains")
{
    REQUIRE(contains("ABCDE", 'D'));
    REQUIRE(!contains("ABCDE", 'F'));
    REQUIRE(contains("ABC" UTF8_GREEK_SMALL_FINAL_SIGMA UTF8_PUNCTUATION_SPACE "EFG", PUNCTUATION_SPACE));
}

TEST_CASE("Test countCodePoints")
{
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
    requireIdentical(findFirst(s, "CDE"), view(s, 2, 3));
    requireIdentical(findFirst(s, "ABCD"), view(s, 0, 4));
    requireIdentical(findFirst(s, "JK"), view(s, 12, 2));
    REQUIRE(findFirst(s, "BCE").empty());
}

TEST_CASE("Test findFirstNewline")
{
    std::string s;
    s = "abc\nd\nef";
    requireIdentical(findFirstNewline(s), view(s, 3, 1));
    s = "abc\rd\nef";
    requireIdentical(findFirstNewline(s), view(s, 3, 1));
    s = "abc\r\nd\nef";
    requireIdentical(findFirstNewline(s), view(s, 3, 2));
    s = "abc\n\rd\nef";
    requireIdentical(findFirstNewline(s), view(s, 3, 1));
    s = "abc" UTF8_PARAGRAPH_SEPARATOR "d\nef";
    requireIdentical(findFirstNewline(s), view(s, 3, 3));
    s = "\nabc\nd\nef";
    requireIdentical(findFirstNewline(s), view(s, 0, 1));
    s = "abcdef\n";
    requireIdentical(findFirstNewline(s), view(s, 6, 1));
}

TEST_CASE("Test findLast")
{
    std::string s("ABCDEFGHCDEIJK");
    requireIdentical(findLast(s, "CDE"), view(s, 8, 3));
    requireIdentical(findLast(s, "ABCD"), view(s, 0, 4));
    requireIdentical(findLast(s, "JK"), view(s, 12, 2));
    REQUIRE(findLast(s, "BCE").empty());
}
