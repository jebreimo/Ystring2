//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-06-08.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/EncodeUtf16.hpp"
#include <catch2/catch.hpp>

using namespace Ystring;

void checkAddBytesLE(char32_t c, std::string_view expected)
{
    std::string s;
    auto it = back_inserter(s);
    addUtf16LEBytes(c, it);
    REQUIRE(s == expected);
}

void checkAddBytesBE(char32_t c, std::string_view expected)
{
    std::string s;
    auto it = back_inserter(s);
    addUtf16BEBytes(c, it);
    REQUIRE(s == expected);
}

void checkAddWordsLE(char32_t c, char16_t expected1, char16_t expected2 = 0)
{
    std::u16string s;
    auto it = back_inserter(s);
    addUtf16LEWords(c, it);
    REQUIRE(!s.empty());
    REQUIRE(s[0] == getLittleEndian(expected1));
    if (expected2)
    {
        REQUIRE(s.size() == 2);
        REQUIRE(s[1] == getLittleEndian(expected2));
    }
}

void checkAddWordsBE(char32_t c, char16_t expected1, char16_t expected2 = 0)
{
    std::u16string s;
    auto it = back_inserter(s);
    addUtf16BEWords(c, it);
    REQUIRE(!s.empty());
    REQUIRE(s[0] == getBigEndian(expected1));
    if (expected2)
    {
        REQUIRE(s.size() == 2);
        REQUIRE(s[1] == getBigEndian(expected2));
    }
}

TEST_CASE("Test add UTF-16 LE bytes.")
{
    checkAddBytesLE(U'', {"\xFF\xF8", 2});
}

TEST_CASE("Test add UTF-16 BE bytes.")
{
    checkAddBytesBE(U'', {"\xF8\xFF", 2});
}

TEST_CASE("Test add UTF-16 LE words.")
{
    checkAddWordsLE(U'', 0xF8FF);
}

TEST_CASE("Test add UTF-16 BE words.")
{
    checkAddWordsBE(U'', 0xF8FF);
}

void checkEncodeBytesLE(char32_t c, std::string_view expected)
{
    char str[4] = {};
    REQUIRE(encodeUtf16LE(c, str, 4) == expected.size());
    for (size_t i = 0; i < expected.size(); ++i)
        REQUIRE(str[i] == expected[i]);
}

void checkEncodeBytesBE(char32_t c, std::string_view expected)
{
    char str[4] = {};
    REQUIRE(encodeUtf16BE(c, str, 4) == expected.size());
    for (size_t i = 0; i < expected.size(); ++i)
        REQUIRE(str[i] == expected[i]);
}

void checkEncodeWordsLE(char32_t c, char16_t expected1, char16_t expected2 = 0)
{
    char16_t str[2] = {};
    REQUIRE(encodeUtf16LE(c, str, 2) == (expected2 ? 2 : 1));
    REQUIRE(str[0] == getLittleEndian(expected1));
    if (expected2)
        REQUIRE(str[1] == getLittleEndian(expected2));
}

void checkEncodeWordsBE(char32_t c, char16_t expected1, char16_t expected2 = 0)
{
    char16_t str[2] = {};
    REQUIRE(encodeUtf16BE(c, str, 2) == (expected2 ? 2 : 1));
    REQUIRE(str[0] == getBigEndian(expected1));
    if (expected2)
        REQUIRE(str[1] == getBigEndian(expected2));
}

TEST_CASE("Test encode ASCII")
{
    checkEncodeBytesLE(U'A', {"A\0", 2});
    checkEncodeBytesBE(U'A', {"\0A", 2});
    checkEncodeWordsLE(U'A', 'A');
    checkEncodeWordsBE(U'A', 'A');
}
