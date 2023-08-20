//****************************************************************************
// Copyright © 2013 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2013-05-13.
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/DecodeUtf8.hpp"
#include <catch2/catch_test_macros.hpp>

namespace
{
    using namespace ystring;

    void testValidNext(std::string_view s, char32_t expected,
                       ptrdiff_t length = -1)
    {
        CAPTURE(s);
        auto it = s.begin();
        REQUIRE(decode_next(it, s.end()) == expected);
        if (length == -1)
            length = s.length();
        REQUIRE(std::distance(s.begin(), it) == length);
    }

    void testInvalidNext(const std::string& s)
    {
        auto it = s.begin();
        REQUIRE(decode_next(it, s.end()) == INVALID_CHAR);
        REQUIRE(it == s.begin());
    }

    void testValidPrev(std::string_view s, char32_t expected,
                       ptrdiff_t length = -1)
    {
        CAPTURE(s);
        auto it = s.end();
        REQUIRE(decode_prev(s.begin(), it) == expected);
        if (length == -1)
            length = s.length();
        REQUIRE(std::distance(it, s.end()) == length);
    }

    void testInvalidPrev(std::string_view s)
    {
        CAPTURE(s);
        auto it = s.end();
        REQUIRE(decode_prev(s.begin(), it) == INVALID_CHAR);
        REQUIRE(it == s.end());
    }

    void testSkipNext(std::string_view s, ptrdiff_t length)
    {
        CAPTURE(s);
        auto it = s.begin();
        REQUIRE(skip_next(it, s.end()) == (length != 0));
        REQUIRE(std::distance(s.begin(), it) == length);
    }

    void testSkipPrev(std::string_view s, ptrdiff_t length)
    {
        CAPTURE(s);
        auto it = s.end();
        REQUIRE(skip_prev(s.begin(), it) == (length != 0));
        REQUIRE(std::distance(it, s.end()) == length);
    }
}

TEST_CASE("Test nextUtf8Value on valid UTF-8")
{
    testValidNext(std::string_view("\0", 1), 0x00);
    testValidNext("A", 'A');
    testValidNext("\x7F", 0x7F);
    testValidNext("\xC1\x80", 0x40);
    testValidNext("\xDF\xBF", 0x7FF);
    testValidNext("\xE0\xA0\x80", 0x800);
    testValidNext("\xEF\xBF\xBF", 0xFFFF);
    testValidNext("\xF0\x90\x80\x80", 0x10000);
    testValidNext("\xF7\xBF\xBF\xBF", 0x1FFFFF);
}

TEST_CASE("Test nextUtf8Value on valid UTF-8 followed by invalid UTF-8")
{
    testValidNext("\x40\x80", 0x40, 1);
    testValidNext("\xD0\x80\x80", 0x400, 2);
    testValidNext("\xE8\x80\x80\x80", 0x8000, 3);
    testValidNext("\xF4\x80\x80\x80\x80", 0x100000, 4);
}

TEST_CASE("Test nextUtf8Value on empty string.")
{
    testInvalidNext({});
}

TEST_CASE("Test nextUtf8Value on continuation.")
{
    testInvalidNext("\x80");
    testInvalidNext("\xBF");
}

TEST_CASE("Test nextUtf8Value on incomplete codepoint.")
{
    testInvalidNext("\xC1");
    testInvalidNext("\xE1");
    testInvalidNext("\xE1\x80");
    testInvalidNext("\xF1");
    testInvalidNext("\xF1\x80");
    testInvalidNext("\xF1\x80\x80");
}

TEST_CASE("Test nextUtf8Value on too long codepoints.")
{
    testInvalidNext("\xF9\x80\x80\x80\x80");
    testInvalidNext("\xFD\x80\x80\x80\x80\x80");
    testInvalidNext("\xFE\xA0\x80\x80\x80\x80\x80");
    testInvalidNext("\xFF\xA0\x80\x80\x80\x80\x80\x80");
}

TEST_CASE("Test prevUtf8Value on valid UTF-8")
{
    testValidPrev(std::string_view("\0", 1), 0x00);
    testValidPrev("A", 'A');
    testValidPrev("\x7F", 0x7F);
    testValidPrev("\xC1\x80", 0x40);
    testValidPrev("\xDF\xBF", 0x7FF);
    testValidPrev("\xE0\xA0\x80", 0x800);
    testValidPrev("\xEF\xBF\xBF", 0xFFFF);
    testValidPrev("\xF0\x90\x80\x80", 0x10000);
    testValidPrev("\xF7\xBF\xBF\xBF", 0x1FFFFF);
}

TEST_CASE("Test prevUtf8Value on valid UTF-8 preceded by more UTF-8")
{
    testValidPrev("\x80\x40", 0x40, 1);
    testValidPrev("\x80\xD0\x80", 0x400, 2);
    testValidPrev("\x80\xE8\x80\x80", 0x8000, 3);
    testValidPrev("\x80\xF4\x80\x80\x80", 0x100000, 4);
}

TEST_CASE("Test prevUtf8Value on empty string.")
{
    testInvalidPrev({});
}

TEST_CASE("Test prevUtf8Value on continuation.")
{
    testInvalidPrev("\x80");
    testInvalidPrev("\xBF");
}

TEST_CASE("Test prevUtf8Value on incomplete codepoint.")
{
    testInvalidPrev("\xC1");
    testInvalidPrev("\xE1");
    testInvalidPrev("\xE1\x80");
    testInvalidPrev("\xF1");
    testInvalidPrev("\xF1\x80");
    testInvalidPrev("\xF1\x80\x80");
}

TEST_CASE("Test prevUtf8Value on too long codepoints.")
{
    testInvalidPrev("\xF9\x80\x80\x80\x80");
    testInvalidPrev("\xFD\x80\x80\x80\x80\x80");
    testInvalidPrev("\xFE\xA0\x80\x80\x80\x80\x80");
    testInvalidPrev("\xFF\xA0\x80\x80\x80\x80\x80\x80");
}

TEST_CASE("Test skipNextUtf8Value")
{
    testSkipNext({}, 0);
    testSkipNext("ZZ", 1);
    testSkipNext("\xE0\xA0\x80Z", 3);
    testSkipNext("\xE0\xA0\x80\x80\x80Z", 3);
    testSkipNext("\xF0\xA0\x80Z", 3);
    testSkipNext("\xF0Z", 1);
    testSkipNext("\x80\xA0\x80\x80\x80\x80\x80\x80Z", 8);
    testSkipNext("\xFF\xA0\x80\x80\x80\x80\x80\x80Z", 8);
    testSkipNext("\x80Z", 1);
}

TEST_CASE("Test skipPrevUtf8Value")
{
    testSkipPrev({}, 0);
    testSkipPrev("AB", 1);
    testSkipPrev("B\x80", 1);
    testSkipPrev("B\x80\x80\x80\x80\x80\x80", 6);
    testSkipPrev("\xE0\x80\x80\x80\x80\x80\x80", 4);
    testSkipPrev("\xE0\x80\x80\x80", 1);
    testSkipPrev("A\xE0\x80\x80", 3);
    testSkipPrev("A\xF0", 1);
    testSkipPrev("A\xF0\x80", 2);
    testSkipPrev("A\xF0\x80\x80", 3);
    testSkipPrev("A\xF0\x80\x80\x80", 4);
    testSkipPrev("A\xF0\x80\x80\x80\x80", 1);
}
