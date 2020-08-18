//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-06-02.
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Utf8Chars.hpp"
#include "Ystring/Algorithms.hpp"
#include "Ystring/CodePointPredicates.hpp"
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

TEST_CASE("Test caseInsensitiveCompare")
{
    REQUIRE(caseInsensitiveCompare(u8"aBcØæ", u8"AbCøÆ") == 0);
    REQUIRE(caseInsensitiveCompare(u8"aBcÅd", u8"AbCå") > 0);
    REQUIRE(caseInsensitiveCompare(u8"aBcæØå", u8"AbCæØø") < 0);
}

TEST_CASE("Test caseInsensitiveEqual")
{
    REQUIRE(caseInsensitiveEqual(u8"aBcØæ", u8"AbCøÆ"));
    REQUIRE(!caseInsensitiveEqual(u8"aBcÅd", u8"AbCå"));
    REQUIRE(!caseInsensitiveEqual(u8"aBcæØå", u8"AbCæØø"));
}

TEST_CASE("Test caseInsensitiveLess")
{
    REQUIRE(!caseInsensitiveLess(u8"aBcØæ", u8"AbCøÆ"));
    REQUIRE(!caseInsensitiveLess(u8"aBcÅd", u8"AbCå"));
    REQUIRE(caseInsensitiveLess(u8"aBcæØå", u8"AbCæØø"));
}

TEST_CASE("Test caseInsensitiveEndsWith")
{
    REQUIRE(caseInsensitiveEndsWith(u8"ABCDÆøÅQRS", u8"ØåQrS"));
    REQUIRE(!caseInsensitiveEndsWith(u8"ABCDÆØÅQRS", u8"æÅQRs"));
    REQUIRE(caseInsensitiveEndsWith(u8"ØåQRS", u8"ØÅQrS"));
    REQUIRE(!caseInsensitiveEndsWith(u8"ÅQRS", u8"ØÅQRS"));
    REQUIRE(caseInsensitiveEndsWith(u8"ABCDÆØåQRS", u8""));
}

TEST_CASE("Test caseInsensitiveFindFirst")
{
    std::string s = "ABCÆØÅäöü";
    REQUIRE(caseInsensitiveFindFirst(s, "Cæø") == Subrange(2, 5));
    REQUIRE(caseInsensitiveFindFirst(s, "abcæ") == Subrange(0, 5));
    REQUIRE(caseInsensitiveFindFirst(s, "åÄö") == Subrange(7, 6));
    REQUIRE(!caseInsensitiveFindFirst(s, "äÖÜÅ"));
}

TEST_CASE("Test caseInsensitiveFindFirstOf")
{
    char32_t chars[4] = {U'≠', U'Ø', U'Å', U'¿'};
    CHECK_CHAR_SEARCH(caseInsensitiveFindFirstOf(u8"qweåørty", Char32Span(chars)), 3, 2, U'å');
    REQUIRE(!caseInsensitiveFindFirstOf("qwerty", Char32Span(chars)).first);
}

TEST_CASE("Test caseInsensitiveFindLast")
{
    std::string s = "ABCÆØÅäöü";
    REQUIRE(caseInsensitiveFindLast(s, "Cæø") == Subrange(2, 5));
    REQUIRE(caseInsensitiveFindLast(s, "abcæ") == Subrange(0, 5));
    REQUIRE(caseInsensitiveFindLast(s, "Åäöü") == Subrange(7, 8));
    REQUIRE(!caseInsensitiveFindLast(s, "åaBC"));
}

TEST_CASE("Test caseInsensitiveFindLastOf")
{
    char32_t chars[4] = {U'≠', U'Ø', U'Å', U'¿'};
    CHECK_CHAR_SEARCH(caseInsensitiveFindLastOf(u8"qweåørty", Char32Span(chars)), 5, 2, U'ø');
    REQUIRE(!caseInsensitiveFindLastOf("qwerty", Char32Span(chars)).first);
}

TEST_CASE("Test caseInsensitiveReplace")
{
    REQUIRE(caseInsensitiveReplace("abc dæ fgh DÆ i", "Dæ", u8"øå") == u8"abc øå fgh øå i");
    REQUIRE(caseInsensitiveReplace("abc dæ fgh DÆ i", "Dæ", u8"øå", 1) == u8"abc øå fgh DÆ i");
    REQUIRE(caseInsensitiveReplace("abc dæ fgh DÆ i", "Dæ", u8"øå", -2) == u8"abc øå fgh øå i");
}

TEST_CASE("Test caseInsensitiveStartsWith")
{
    REQUIRE(caseInsensitiveStartsWith(u8"BØABC BØBØ cfgå BØ", u8"Bøa"));
    REQUIRE(!caseInsensitiveStartsWith(u8"BØABC BØBØ cfgå BØ", u8"BøAD"));
    REQUIRE(caseInsensitiveStartsWith(u8"BøABC", u8"BØABC"));
    REQUIRE(!caseInsensitiveStartsWith(u8"BøABC", u8"BØABCE"));
}

TEST_CASE("Test contains")
{
    REQUIRE(contains("ABCDE", 'D'));
    REQUIRE(!contains("ABCDE", 'F'));
    REQUIRE(contains(u8"ABC∑ßÖ’Ü‹›ƒ¸√EFG", U'√'));
}

TEST_CASE("Test countCharacters")
{
    REQUIRE(countCodeCharacters("P\314\220s") == 2);
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
    REQUIRE(endsWith(u8"ØÅQRS", u8"ØÅQRS"));
    REQUIRE(!endsWith(u8"ÅQRS", u8"ØÅQRS"));
    REQUIRE(endsWith(u8"ABCDÆØÅQRS", u8""));
}

TEST_CASE("Test findFirst")
{
    std::string s = "ABCDEFGHCDEIJK";
    REQUIRE(findFirst(s, "CDE") == Subrange(2, 3));
    REQUIRE(findFirst(s, "CDE", 3) == Subrange(8, 3));
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

TEST_CASE("Test findFirstWhere")
{
    auto result = findFirstWhere(u8"qWeÅty", [](auto c) {return isUpper(c);});
    REQUIRE(result.first == Subrange(1, 1));
    REQUIRE(result.second == U'W');
    result = findFirstWhere(u8"qWeÅty", [](auto c) {return isUpper(c);}, 2);
    REQUIRE(result.first == Subrange(3, 2));
    REQUIRE(result.second == U'Å');
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
    REQUIRE(findLastNewline("abc\nd\ref", 5) == Subrange(3, 1));
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

TEST_CASE("Test findLastWhere")
{
    auto result = findLastWhere(u8"qWeÅty", [](auto c){return isUpper(c);});
    REQUIRE(result.first == Subrange(3, 2));
    REQUIRE(result.second == U'Å');
}

TEST_CASE("Test getCharacterPos")
{
    REQUIRE(getCharacterPos(u8"PΩ\314\220sÅ", 0) == 0);
    REQUIRE(getCharacterPos(u8"PΩ\314\220sÅ", 1) == 1);
    REQUIRE(getCharacterPos(u8"PΩ\314\220sÅ", 2) == 5);
    REQUIRE(getCharacterPos(u8"PΩ\314\220sÅ", 3) == 6);
    REQUIRE(getCharacterPos(u8"PΩ\314\220sÅ", 4) == 8);
    REQUIRE(getCharacterPos(u8"PΩ\314\220sÅ", 5) == std::string_view::npos);
    REQUIRE(getCharacterPos(u8"PΩ\314\220sÅ", -1) == 6);
    REQUIRE(getCharacterPos(u8"PΩ\314\220sÅ", -2) == 5);
    REQUIRE(getCharacterPos(u8"PΩ\314\220sÅ", -3) == 1);
    REQUIRE(getCharacterPos(u8"PΩ\314\220sÅ", -4) == 0);
    REQUIRE(getCharacterPos(u8"PΩ\314\220sÅ", -5) == std::string_view::npos);
}

TEST_CASE("Test getCharacterRange")
{
    REQUIRE(getCharacterRange(u8"PΩ\314\220sÅ", 0) == Subrange(0, 1));
    REQUIRE(getCharacterRange(u8"PΩ\314\220sÅ", 1) == Subrange(1, 4));
    REQUIRE(getCharacterRange(u8"PΩ\314\220sÅ", 2) == Subrange(5, 1));
    REQUIRE(getCharacterRange(u8"PΩ\314\220sÅ", 3) == Subrange(6, 2));
    REQUIRE(getCharacterRange(u8"PΩ\314\220sÅ", 4) == Subrange(8, 0));
    REQUIRE(getCharacterRange(u8"PΩ\314\220sÅ", 5) == Subrange(8, 0));
    REQUIRE(getCharacterRange(u8"PΩ\314\220sÅ", -1) == Subrange(6, 2));
    REQUIRE(getCharacterRange(u8"PΩ\314\220sÅ", -2) == Subrange(5, 1));
    REQUIRE(getCharacterRange(u8"PΩ\314\220sÅ", -3) == Subrange(1, 4));
    REQUIRE(getCharacterRange(u8"PΩ\314\220sÅ", -4) == Subrange(0, 1));
    REQUIRE(getCharacterRange(u8"PΩ\314\220sÅ", -5) == Subrange(0, 0));
}

TEST_CASE("Test getCharacterSubstring")
{
    REQUIRE(getCharacterSubstring(u8"PΩ\314\220\314\221s\314\220Å", 0) == u8"PΩ\314\220\314\221s\314\220Å");
    REQUIRE(getCharacterSubstring(u8"PΩ\314\220\314\221s\314\220Å", 0, 2) == u8"PΩ\314\220\314\221");
    REQUIRE(getCharacterSubstring(u8"PΩ\314\220\314\221s\314\220Å", 2, 6) == u8"s\314\220Å");
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

TEST_CASE("Test getCodePointSubstring")
{
    REQUIRE(getCodePointSubstring(u8"ABCDÆØÅæøå€µ", 0, 0).empty());
    REQUIRE(getCodePointSubstring(u8"ABCDÆØÅæøå€µ", 0, 5) == u8"ABCDÆ");
    REQUIRE(getCodePointSubstring(u8"ABCDÆØÅæøå€µ", 8) == u8"øå€µ");
    REQUIRE(getCodePointSubstring(u8"ABCDÆØÅæøå€µ", 8, 12) == u8"øå€µ");
    REQUIRE(getCodePointSubstring(u8"ABCDÆØÅæøå€µ", 8, 13) == u8"øå€µ");
    REQUIRE(getCodePointSubstring(u8"ABCDÆØÅæøå€µ", 8, 7).empty());
    REQUIRE(getCodePointSubstring(u8"ABCDÆØÅæøå€µ", 12, 13).empty());
    REQUIRE(getCodePointSubstring(u8"ABCDÆØÅæøå€µ", 13, 14).empty());
    REQUIRE(getCodePointSubstring(u8"ABCDÆØÅæøå€µ", -4) == u8"øå€µ");
    REQUIRE(getCodePointSubstring(u8"ABCDÆØÅæøå€µ", -100, 5) == u8"ABCDÆ");
    REQUIRE(getCodePointSubstring(u8"ABCDÆØÅæøå€µ", -4, -1) == u8"øå€");
    REQUIRE(getCodePointSubstring(u8"ABCDÆØÅæøå€µ", 2, -2) == u8"CDÆØÅæøå");
}

TEST_CASE("Test getNextCharacter")
{
    REQUIRE(getNextCharacterRange("A†µ", 1) == Subrange(1, 3));
    REQUIRE(getNextCharacterRange("P\314\220s", 0) == Subrange(0, 3));
    REQUIRE(getNextCharacterRange("P\314\220s", 3) == Subrange(3, 1));
}

TEST_CASE("Test getPrevCharacter")
{
    REQUIRE(getPrevCharacterRange("A†µ", 4) == Subrange(1, 3));
    REQUIRE(getPrevCharacterRange("P\314\220s", 4) == Subrange(3, 1));
    REQUIRE(getPrevCharacterRange("P\314\220s", 3) == Subrange(0, 3));
    REQUIRE_THROWS(getPrevCharacterRange("P\314\220s", 2));
}

TEST_CASE("Test insertCharacter")
{
    REQUIRE(insertCharacter(u8"PΩ\314\220sÅ", 0, U'Ø') == u8"ØPΩ\314\220sÅ");
    REQUIRE(insertCharacter(u8"PΩ\314\220sÅ", 3, U'Ø') == u8"PΩ\314\220sØÅ");
    REQUIRE(insertCharacter(u8"PΩ\314\220sÅ", 4, U'Ø') == u8"PΩ\314\220sÅØ");
    REQUIRE_THROWS(insertCharacter(u8"PΩ\314\220sÅ", 5, U'Ø'));
    REQUIRE(insertCharacter(u8"PΩ\314\220sÅ", -3, U'Ø') == u8"PØΩ\314\220sÅ");
}

TEST_CASE("Test insertCharacters")
{
    REQUIRE(insertCharacters(u8"PΩ\314\220sÅ", 0, u8"æØå") == u8"æØåPΩ\314\220sÅ");
    REQUIRE(insertCharacters(u8"PΩ\314\220sÅ", 3, u8"æØå") == u8"PΩ\314\220sæØåÅ");
    REQUIRE(insertCharacters(u8"PΩ\314\220sÅ", 4, u8"æØå") == u8"PΩ\314\220sÅæØå");
    REQUIRE_THROWS(insertCharacters(u8"PΩ\314\220sÅ", 5, u8"æØå"));
    REQUIRE(insertCharacters(u8"PΩ\314\220sÅ", -3, u8"æØå") == u8"PæØåΩ\314\220sÅ");
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

TEST_CASE("Test replaceCharacters")
{
    REQUIRE(replaceCharacters(u8"AB\314\220ÆØÅäö•", 3, 6, "√ƒ") == u8"AB\314\220Æ√ƒö•");
    REQUIRE(replaceCharacters(u8"AB\314\220ÆØÅäö•", -12, -1, "√ƒ") == u8"√ƒ•");
    REQUIRE(replaceCharacters(u8"AB\314\220ÆØÅäö•", 3, 100, "√ƒ") == u8"AB\314\220Æ√ƒ");
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

TEST_CASE("Test reverse")
{
    REQUIRE(reverse("P\314\220s") == "sP\314\220");
}

std::vector<std::string_view> sv(std::vector<std::string_view> strs)
{
    return strs;
}

TEST_CASE("Test split on characters")
{
    char32_t chars[] = {U'Æ', U'Ø', U'Å', U'Q'};
    REQUIRE(split(u8"ÅABØQCDÆ", Char32Span(chars)) == sv({"", "AB", "", "CD", ""}));
    REQUIRE(split(u8"ÅABØQCDÆ", Char32Span(chars), IGNORE_EMPTY_SPLIT) == sv({"AB", "CD"}));
    REQUIRE(split(u8"ÅABØQCDÆ", Char32Span(chars), {2}) == sv({"", "AB", u8"QCDÆ"}));
    REQUIRE(split(u8"ÅABØQCDÆ", Char32Span(chars), {1, true}) == sv({"AB", u8"QCDÆ"}));
    REQUIRE(split(u8"ÅABØQCDÆ", Char32Span(chars), {2, true}) == sv({"AB", "CD"}));
}

TEST_CASE("Test split on substring")
{
    REQUIRE(split(u8"BØABC BØBØ cfgå BØ", u8"BØ") == sv({"", "ABC ", "", u8" cfgå ", ""}));
    REQUIRE(split(u8"BØABC BØBØ cfgå BØ", u8"BØ", IGNORE_EMPTY_SPLIT) == sv({"ABC ", u8" cfgå "}));
    REQUIRE(split(u8"BØABC BØBØ cfgå BØ", u8"BØ", {3}) == sv({"", "ABC ", "", u8" cfgå BØ"}));
}

TEST_CASE("Test case-insensitive split on substring")
{
    REQUIRE(split(u8"BØABC BøbØ cfgå bØ", u8"Bø", CASE_INSENSITIVE_SPLIT) == sv({"", "ABC ", "", u8" cfgå ", ""}));
}

TEST_CASE("Test splitLines")
{
    REQUIRE(splitLines(u8"\nABC\r\n\rcfgå\n") == sv({"", "ABC", "", u8"cfgå", ""}));
    REQUIRE(splitLines(u8"\nABC\r\n\rcfgå\n", IGNORE_EMPTY_SPLIT) == sv({"ABC", u8"cfgå"}));
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

TEST_CASE("Test trim")
{
    char32_t CHARS[] = {' ', U'Ø', U'ø'};
    Char32Span CHAR_SPAN(CHARS);
    REQUIRE(trim(u8"", CHAR_SPAN).empty());
    REQUIRE(trim(u8"f oøo", CHAR_SPAN) == u8"f oøo");
    REQUIRE(trim(u8" øf oøo", CHAR_SPAN) == u8"f oøo");
    REQUIRE(trim(u8"f oøo Ø", CHAR_SPAN) == u8"f oøo");
    REQUIRE(trim(u8" øf oøo Ø", CHAR_SPAN) == u8"f oøo");
}

TEST_CASE("Test trimEnd")
{
    char32_t CHARS[] = {' ', U'Ø', U'ø'};
    Char32Span CHAR_SPAN(CHARS);
    REQUIRE(trimEnd(u8"", CHAR_SPAN).empty());
    REQUIRE(trimEnd(u8"f oøo", CHAR_SPAN) == u8"f oøo");
    REQUIRE(trimEnd(u8" øf oøo", CHAR_SPAN) == u8" øf oøo");
    REQUIRE(trimEnd(u8"f oøo Ø", CHAR_SPAN) == u8"f oøo");
    REQUIRE(trimEnd(u8" øf oøo Ø", CHAR_SPAN) == u8" øf oøo");
}

TEST_CASE("Test trimStart")
{
    char32_t CHARS[] = {' ', U'Ø', U'ø'};
    Char32Span CHAR_SPAN(CHARS);
    REQUIRE(trimStart(u8"", CHAR_SPAN).empty());
    REQUIRE(trimStart(u8"f oøo", CHAR_SPAN) == u8"f oøo");
    REQUIRE(trimStart(u8" øf oøo", CHAR_SPAN) == u8"f oøo");
    REQUIRE(trimStart(u8"f oøo Ø", CHAR_SPAN) == u8"f oøo Ø");
    REQUIRE(trimStart(u8" øf oøo Ø", CHAR_SPAN) == u8"f oøo Ø");
}
