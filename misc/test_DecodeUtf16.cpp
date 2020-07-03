//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-06-02.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "DecodeUtf16.hpp"
#include <catch2/catch.hpp>

using namespace Ystring;

template <typename T>
void testNextUtf16LE(const std::vector<T>& v, char32_t expected)
{
    CAPTURE(v, expected);
    auto it = v.begin();
    REQUIRE(nextUtf16LECodePoint(it, v.end()) == expected);
    REQUIRE(it == v.end());
}

template <typename T>
void testInvalidNextUtf16LE(const std::vector<T>& v)
{
    CAPTURE(v);
    auto it = v.begin();
    REQUIRE(nextUtf16LECodePoint(it, v.end()) == INVALID);
    REQUIRE(it == v.begin());
}

TEST_CASE("Test next UTF-16 LE")
{
    auto le = [](char16_t v){return getLittleEndian(v);};
    testNextUtf16LE<char16_t>({le('N')}, 'N');
    testNextUtf16LE<char16_t>({le(0xD7FF)}, 0xD7FF);
    testNextUtf16LE<char16_t>({le(0xD800), le(0xDC00)}, 0x10000);
    testInvalidNextUtf16LE<char16_t>({le(0xD800), le(0xD800)});
    testNextUtf16LE<char16_t>({le(0xDBFF), le(0xDFFF)}, 0x10FFFF);
    testNextUtf16LE<char16_t>({le(0xE000)}, 0xE000);
    testInvalidNextUtf16LE<char16_t>({});
    testInvalidNextUtf16LE<char16_t>({le(0xDC00)});
    testInvalidNextUtf16LE<char16_t>({le(0xD800)});

    testInvalidNextUtf16LE<char>({'A'});
    testNextUtf16LE<char>({'A', '\0'}, 'A');
    testInvalidNextUtf16LE<uint8_t>({0x00u, 0xD8u});
    testInvalidNextUtf16LE<uint8_t>({0x00u, 0xD8u, 0x00u});
    testNextUtf16LE<uint8_t>({0x00u, 0xD8u, 0x00u, 0xDCu}, 0x10000);
    testInvalidNextUtf16LE<uint8_t>({0x00u, 0xDCu});
}

template <typename T>
void testNextUtf16BE(const std::vector<T>& v, char32_t expected)
{
    CAPTURE(v, expected);
    auto it = v.begin();
    REQUIRE(nextUtf16BECodePoint(it, v.end()) == expected);
    REQUIRE(it == v.end());
}

template <typename T>
void testInvalidNextUtf16BE(const std::vector<T>& v)
{
    CAPTURE(v);
    auto it = v.begin();
    REQUIRE(nextUtf16BECodePoint(it, v.end()) == INVALID);
    REQUIRE(it == v.begin());
}

TEST_CASE("Test next UTF-16 BE")
{
    auto be = [](char16_t v) {return getBigEndian(v);};
    testNextUtf16BE<char16_t>({be('N')}, 'N');
    testNextUtf16BE<char16_t>({be(0xD7FF)}, 0xD7FF);
    testNextUtf16BE<char16_t>({be(0xD800), be(0xDC00)}, 0x10000);
    testNextUtf16BE<char16_t>({be(0xDBFF), be(0xDFFF)}, 0x10FFFF);
    testNextUtf16BE<char16_t>({be(0xE000)}, 0xE000);
    testInvalidNextUtf16BE<char16_t>({});
    testInvalidNextUtf16BE<char16_t>({be(0xDC00)});
    testInvalidNextUtf16BE<char16_t>({be(0xD800)});

    testInvalidNextUtf16BE<char>({'A'});
    testNextUtf16BE<char>({'\0', 'A'}, 'A');
    testInvalidNextUtf16BE<uint8_t>({0xD8u, 0x00u});
    testInvalidNextUtf16BE<uint8_t>({0xD8u, 0x00u, 0x00u});
    testNextUtf16BE<uint8_t>({0xD8u, 0x00u, 0xDCu, 0x00u}, 0x10000);
    testInvalidNextUtf16BE<uint8_t>({0xDCu, 0x00u});
}

template <typename T>
void testPrevUtf16LE(const std::vector<T>& v, char32_t expected)
{
    CAPTURE(v, expected);
    auto it = v.end();
    REQUIRE(prevUtf16LECodePoint(v.begin(), it) == expected);
    REQUIRE(it == v.begin());
}

template <typename T>
void testInvalidPrevUtf16LE(const std::vector<T>& v)
{
    CAPTURE(v);
    auto it = v.end();
    REQUIRE(prevUtf16LECodePoint(v.begin(), it) == INVALID);
    REQUIRE(it == v.end());
}

TEST_CASE("Test prev UTF-16 LE")
{
    auto le = [](char16_t v) {return getLittleEndian(v);};
    testPrevUtf16LE<char16_t>({le('N')}, 'N');
    testPrevUtf16LE<char16_t>({le(0xD7FF)}, 0xD7FF);
    testPrevUtf16LE<char16_t>({le(0xD800), le(0xDC00)}, 0x10000);
    testPrevUtf16LE<char16_t>({le(0xDBFF), le(0xDFFF)}, 0x10FFFF);
    testPrevUtf16LE<char16_t>({le(0xE000)}, 0xE000);
    testInvalidPrevUtf16LE<char16_t>({});
    testInvalidPrevUtf16LE<char16_t>({le(0xDC00)});
    testInvalidPrevUtf16LE<char16_t>({le(0xD800)});

    testInvalidPrevUtf16LE<char>({'A'});
    testPrevUtf16LE<char>({'A', '\0'}, 'A');
    testInvalidPrevUtf16LE<uint8_t>({0x00u, 0xD8u});
    testInvalidPrevUtf16LE<uint8_t>({0xD8u, 0x00u, 0xDCu});
    testPrevUtf16LE<uint8_t>({0x00u, 0xD8u, 0x00u, 0xDCu}, 0x10000);
    testInvalidPrevUtf16LE<uint8_t>({0x00u, 0xDCu});
}

template <typename T>
void testPrevUtf16BE(const std::vector<T>& v, char32_t expected)
{
    CAPTURE(v, expected);
    auto it = v.end();
    REQUIRE(prevUtf16BECodePoint(v.begin(), it) == expected);
    REQUIRE(it == v.begin());
}

template <typename T>
void testInvalidPrevUtf16BE(const std::vector<T>& v)
{
    CAPTURE(v);
    auto it = v.end();
    REQUIRE(prevUtf16BECodePoint(v.begin(), it) == INVALID);
    REQUIRE(it == v.end());
}

TEST_CASE("Test prev UTF-16 BE")
{
    auto be = [](char16_t v) {return getBigEndian(v);};
    testPrevUtf16BE<char16_t>({be('N')}, 'N');
    testPrevUtf16BE<char16_t>({be(0xD7FF)}, 0xD7FF);
    testPrevUtf16BE<char16_t>({be(0xD800), be(0xDC00)}, 0x10000);
    testPrevUtf16BE<char16_t>({be(0xDBFF), be(0xDFFF)}, 0x10FFFF);
    testPrevUtf16BE<char16_t>({be(0xE000)}, 0xE000);
    testInvalidPrevUtf16BE<char16_t>({});
    testInvalidPrevUtf16BE<char16_t>({be(0xDC00)});
    testInvalidPrevUtf16BE<char16_t>({be(0xD800)});

    testInvalidPrevUtf16BE<char>({'A'});
    testPrevUtf16BE<char>({'\0', 'A'}, 'A');
    testInvalidPrevUtf16BE<uint8_t>({0xD8u, 0x00u});
    testInvalidPrevUtf16BE<uint8_t>({0x00u, 0xDCu, 0x00u});
    testPrevUtf16BE<uint8_t>({0xD8u, 0x00u, 0xDCu, 0x00u}, 0x10000);
    testInvalidPrevUtf16BE<uint8_t>({0xDCu, 0x00u});
}

template <typename T>
void testSkipUtf16LE(const std::vector<T>& v)
{
    CAPTURE(v);
    auto it = v.begin();
    REQUIRE(skipNextUtf16LECodePoint(it, v.end()));
    REQUIRE(it == v.end());
}

TEST_CASE("Test skip UTF-16 LE")
{
    auto le = [](char16_t v) {return getLittleEndian(v);};
    testSkipUtf16LE<char16_t>({le('N')});
    testSkipUtf16LE<char16_t>({le(0xD800)});
    testSkipUtf16LE<uint8_t>({0x00u, 0xD8u, 0x00u});
}

template <typename T>
void testSkipUtf16BE(const std::vector<T>& v)
{
    CAPTURE(v);
    auto it = v.begin();
    REQUIRE(skipNextUtf16BECodePoint(it, v.end()));
    REQUIRE(it == v.end());
}

TEST_CASE("Test skip UTF-16 BE")
{
    auto be = [](char16_t v) {return getBigEndian(v);};
    testSkipUtf16BE<char16_t>({be('N')});
    testSkipUtf16BE<char16_t>({be(0xD800)});
    testSkipUtf16BE<char16_t>({be(0xDC00)});
    testSkipUtf16BE<char16_t>({be(0xD800), be(0xDC00)});
    testSkipUtf16BE<uint8_t>({0xD8u, 0x00u, 0x00u});
}

template <typename T>
void testSkipPrevUtf16LE(const std::vector<T>& v)
{
    CAPTURE(v);
    auto it = v.end();
    REQUIRE(skipPrevUtf16LECodePoint(v.begin(), it));
    REQUIRE(it == v.begin());
}

TEST_CASE("Test skip prev UTF-16 LE")
{
    auto le = [](char16_t v) {return getLittleEndian(v);};
    testSkipPrevUtf16LE<char16_t>({le('N')});
    testSkipPrevUtf16LE<char16_t>({le(0xDC00)});
    testSkipPrevUtf16LE<uint8_t>({0x00u, 0x00u, 0xDCu});
}

template <typename T>
void testSkipPrevUtf16BE(const std::vector<T>& v)
{
    CAPTURE(v);
    auto it = v.end();
    REQUIRE(skipPrevUtf16BECodePoint(v.begin(), it));
    REQUIRE(it == v.begin());
}

TEST_CASE("Test skip prev UTF-16 BE")
{
    auto be = [](char16_t v) {return getBigEndian(v);};
    testSkipPrevUtf16BE<char16_t>({be('N')});
    testSkipPrevUtf16BE<char16_t>({be(0xDC00)});
    testSkipPrevUtf16BE<uint8_t>({0x00u, 0xDCu, 0x00u});
}
