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
    CHECK_CHAR_SEARCH(findFirstOf(u8"qwe≠≠rty", Char32Span(chars)), 3, 3, U'≠');
    REQUIRE(!findFirstOf("qwerty", Char32Span(chars)).first);
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
    CHECK_CHAR_SEARCH(findLastOf(u8"qwe≠≠rty", Char32Span(chars)), 6, 3, U'≠');
    REQUIRE(!findLastOf("qwerty", Char32Span(chars)).first);
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

TEST_CASE("Test getClampedCodePointPos")
{
    REQUIRE(getClampedCodePointPos(u8"AB£ƒCD‹ß∂GHR", 0) == 0);
    REQUIRE(getClampedCodePointPos(u8"AB£ƒCD‹ß∂GHR", 12) == 19);
    REQUIRE(getClampedCodePointPos(u8"AB£ƒCD‹ß∂GHR", 13) == 19);
    REQUIRE(getClampedCodePointPos(u8"AB£ƒCD‹ß∂GHR", -1) == 18);
    REQUIRE(getClampedCodePointPos(u8"AB£ƒCD‹ß∂GHR", -12) == 0);
    REQUIRE(getClampedCodePointPos(u8"AB£ƒCD‹ß∂GHR", -13) == 0);
}

TEST_CASE("Test insertCodePoint")
{
    REQUIRE(insertCodePoint(u8"AB£ƒCD‹ß", 0, U'Å') == u8"ÅAB£ƒCD‹ß");
    REQUIRE(insertCodePoint(u8"AB£ƒCD‹ß", 3, U'Å') == u8"AB£ÅƒCD‹ß");
    REQUIRE(insertCodePoint(u8"AB£ƒCD‹ß", 8, U'Å') == u8"AB£ƒCD‹ßÅ");
    REQUIRE_THROWS(insertCodePoint(u8"AB£ƒCD‹ß", 9, U'Å'));
    REQUIRE(insertCodePoint(u8"AB£ƒCD‹ß", -1, U'Å') == u8"AB£ƒCD‹Åß");
}

TEST_CASE("Test insertCodePoints")
{
    REQUIRE(insertCodePoints(u8"AB£ƒCD‹ß", 0, u8"ØÆÅ") == u8"ØÆÅAB£ƒCD‹ß");
    REQUIRE(insertCodePoints(u8"AB£ƒCD‹ß", 3, u8"ØÆÅ") == u8"AB£ØÆÅƒCD‹ß");
    REQUIRE(insertCodePoints(u8"AB£ƒCD‹ß", 8, u8"ØÆÅ") == u8"AB£ƒCD‹ßØÆÅ");
    REQUIRE_THROWS(insertCodePoints(u8"AB£ƒCD‹ß", 9, u8"ØÆÅ"));
    REQUIRE(insertCodePoints(u8"AB£ƒCD‹ß", -1, u8"ØÆÅ") == u8"AB£ƒCD‹ØÆÅß");
}

TEST_CASE("Test isValidUtf8")
{
    REQUIRE(isValidUtf8(u8"AB£ƒCD‹ß"));
    REQUIRE(!isValidUtf8("Q\xF0\xCA\xCAZ"));
}

TEST_CASE("Test join")
{
    std::string_view strings[] = {"Lorem", "ipsum", "dolor", "sit", "amet"};
    REQUIRE(join(std::begin(strings), std::begin(strings), ", ").empty());
    REQUIRE(join(std::begin(strings), std::end(strings), ", ")
            == "Lorem, ipsum, dolor, sit, amet");
}

TEST_CASE("Test replace")
{
    REQUIRE(replace("abc de fgh de i", "de", u8"øå") == u8"abc øå fgh øå i");
    REQUIRE(replace("abc de fgh de i", "de", u8"øå", 1) == u8"abc øå fgh de i");
    REQUIRE(replace("abc de fgh de i", "de", u8"øå", -2) == u8"abc øå fgh øå i");
}

TEST_CASE("Test replaceCodePoints")
{
    REQUIRE(replaceCodePoints(u8"ABÆØÅäö•", 3, 6, "√ƒ") == u8"ABÆ√ƒö•");
    REQUIRE(replaceCodePoints(u8"ABÆØÅäö•", -12, -1, "√ƒ") == u8"√ƒ•");
    REQUIRE(replaceCodePoints(u8"ABÆØÅäö•", 3, 100, "√ƒ") == u8"ABÆ√ƒ");
}

TEST_CASE("Test replaceCodePoint")
{
    REQUIRE(replaceCodePoint(u8"Øbkøaøhjød", U'ø', U'ß') == u8"Øbkßaßhjßd");
    REQUIRE(replaceCodePoint(u8"Øbkøaøhjød", U'ø', U'ß', 2) == u8"Øbkßaßhjød");
    REQUIRE(replaceCodePoint(u8"Øbkøaøhjød", U'ø', U'ß', -2) == u8"Øbkøaßhjßd");
}

TEST_CASE("Test replaceInvalidUtf8")
{
    REQUIRE(replaceInvalidUtf8(u8"", U'ƒ').empty());
    REQUIRE(replaceInvalidUtf8(u8"Øbkæø", U'ƒ') == u8"Øbkæø");
    REQUIRE(replaceInvalidUtf8(u8"Øbk\xE0\xAFæø", U'ƒ') == u8"Øbkƒæø");
}

TEST_CASE("Test replaceInvalidUtf8, mutable string")
{
    std::string s(u8"Øbk\xE0\xAFæø");
    REQUIRE(replaceInvalidUtf8(s, U'™') == u8"Øbk™æø");
    REQUIRE(replaceInvalidUtf8(s, U'™') == u8"Øbk™æø");
    REQUIRE(s == u8"Øbk™æø");
}

std::vector<std::string_view> sv(std::vector<std::string_view> strs)
{
    return strs;
}

TEST_CASE("Test split on characters")
{
    char32_t chars[] = {U'Æ', U'Ø', U'Å', U'Q'};
    REQUIRE(split(u8"ÅABØQCDÆ", Char32Span(chars)) == sv({"", "AB", "", "CD", ""}));
    REQUIRE(split(u8"ÅABØQCDÆ", Char32Span(chars), IGNORE_EMPTY) == sv({"AB", "CD"}));
    REQUIRE(split(u8"ÅABØQCDÆ", Char32Span(chars), {2}) == sv({"", "AB", u8"QCDÆ"}));
    REQUIRE(split(u8"ÅABØQCDÆ", Char32Span(chars), {1, true}) == sv({"AB", u8"QCDÆ"}));
    REQUIRE(split(u8"ÅABØQCDÆ", Char32Span(chars), {2, true}) == sv({"AB", "CD"}));
}

TEST_CASE("Test split on substring")
{
    REQUIRE(split(u8"BØABC BØBØ cfgå BØ", u8"BØ") == sv({"", "ABC ", "", u8" cfgå ", ""}));
    REQUIRE(split(u8"BØABC BØBØ cfgå BØ", u8"BØ", IGNORE_EMPTY) == sv({"ABC ", u8" cfgå "}));
    REQUIRE(split(u8"BØABC BØBØ cfgå BØ", u8"BØ", {3}) == sv({"", "ABC ", "", u8" cfgå BØ"}));
}

TEST_CASE("Test splitLines")
{
    REQUIRE(splitLines(u8"\nABC\r\n\rcfgå\n") == sv({"", "ABC", "", u8"cfgå", ""}));
    REQUIRE(splitLines(u8"\nABC\r\n\rcfgå\n", IGNORE_EMPTY) == sv({"ABC", u8"cfgå"}));
    REQUIRE(splitLines(u8"\nABC\r\n\rcfgå\n", {2}) == sv({"", "ABC", u8"\rcfgå\n"}));
    REQUIRE(splitLines(u8"\nABC\r\n\rcfgå\n", {1, true}) == sv({"ABC", u8"\rcfgå\n"}));
}

TEST_CASE("Test startsWith")
{
    REQUIRE(startsWith(u8"BØABC BØBØ cfgå BØ", u8"BØA"));
    REQUIRE(!startsWith(u8"BØABC BØBØ cfgå BØ", u8"BØAD"));
    REQUIRE(startsWith(u8"BØABC", u8"BØABC"));
    REQUIRE(!startsWith(u8"BØABC", u8"BØABCE"));
}

TEST_CASE("Test substring")
{
    REQUIRE(substring(u8"ABCDÆØÅæøå€µ", 0, 0) == u8"");
    REQUIRE(substring(u8"ABCDÆØÅæøå€µ", 0, 5) == u8"ABCDÆ");
    REQUIRE(substring(u8"ABCDÆØÅæøå€µ", 8) == u8"øå€µ");
    REQUIRE(substring(u8"ABCDÆØÅæøå€µ", 8, 12) == u8"øå€µ");
    REQUIRE(substring(u8"ABCDÆØÅæøå€µ", 8, 13) == u8"øå€µ");
    REQUIRE(substring(u8"ABCDÆØÅæøå€µ", 8, 7) == u8"");
    REQUIRE(substring(u8"ABCDÆØÅæøå€µ", 12, 13) == u8"");
    REQUIRE(substring(u8"ABCDÆØÅæøå€µ", 13, 14) == u8"");
    REQUIRE(substring(u8"ABCDÆØÅæøå€µ", -4) == u8"øå€µ");
    REQUIRE(substring(u8"ABCDÆØÅæøå€µ", -100, 5) == u8"ABCDÆ");
    REQUIRE(substring(u8"ABCDÆØÅæøå€µ", -4, -1) == u8"øå€");
    REQUIRE(substring(u8"ABCDÆØÅæøå€µ", 2, -2) == u8"CDÆØÅæøå");
}
