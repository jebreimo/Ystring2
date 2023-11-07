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
#include <catch2/catch_test_macros.hpp>
#include "U8Adapter.hpp"

using namespace ystring;

#define CHECK_CHAR_SEARCH(funcCall, offset, length, chr) \
    do { \
        auto r = (funcCall); \
        REQUIRE(r.first == Subrange((offset), (length))); \
        REQUIRE(r.second == char32_t(chr)); \
    } while (false)

TEST_CASE("Test append")
{
    std::string s;
    REQUIRE(append(s, U'∑') == U8("∑"));
    REQUIRE(append(s, U'ı') == U8("∑ı"));
    REQUIRE_THROWS(append(s, 0x200000));
}

TEST_CASE("Test case_insensitive_compare")
{
    REQUIRE(case_insensitive_compare(U8("aBcØæ"), U8("AbCøÆ")) == 0);
    REQUIRE(case_insensitive_compare(U8("aBcÅd"), U8("AbCå")) > 0);
    REQUIRE(case_insensitive_compare(U8("aBcæØå"), U8("AbCæØø")) < 0);
}

TEST_CASE("Test case_insensitive_equal")
{
    REQUIRE(case_insensitive_equal(U8("aBcØæ"), U8("AbCøÆ")));
    REQUIRE(!case_insensitive_equal(U8("aBcÅd"), U8("AbCå")));
    REQUIRE(!case_insensitive_equal(U8("aBcæØå"), U8("AbCæØø")));
}

TEST_CASE("Test case_insensitive_less")
{
    REQUIRE(!case_insensitive_less(U8("aBcØæ"), U8("AbCøÆ")));
    REQUIRE(!case_insensitive_less(U8("aBcÅd"), U8("AbCå")));
    REQUIRE(case_insensitive_less(U8("aBcæØå"), U8("AbCæØø")));
}

TEST_CASE("Test case_insensitive_ends_with")
{
    REQUIRE(case_insensitive_ends_with(U8("ABCDÆøÅQRS"), U8("ØåQrS")));
    REQUIRE(!case_insensitive_ends_with(U8("ABCDÆØÅQRS"), U8("æÅQRs")));
    REQUIRE(case_insensitive_ends_with(U8("ØåQRS"), U8("ØÅQrS")));
    REQUIRE(!case_insensitive_ends_with(U8("ÅQRS"), U8("ØÅQRS")));
    REQUIRE(case_insensitive_ends_with(U8("ABCDÆØåQRS"), U8("")));
}

TEST_CASE("Test case_insensitive_find_first")
{
    std::string s = "ABCÆØÅäöü";
    REQUIRE(case_insensitive_find_first(s, "Cæø") == Subrange(2, 5));
    REQUIRE(case_insensitive_find_first(s, "abcæ") == Subrange(0, 5));
    REQUIRE(case_insensitive_find_first(s, "åÄö") == Subrange(7, 6));
    REQUIRE(!case_insensitive_find_first(s, "äÖÜÅ"));
}

TEST_CASE("Test case_insensitive_find_first_of")
{
    char32_t chars[4] = {U'≠', U'Ø', U'Å', U'¿'};
    CHECK_CHAR_SEARCH(case_insensitive_find_first_of(U8("qweåørty"), Char32Span(chars)), 3, 2, U'å');
    REQUIRE(!case_insensitive_find_first_of("qwerty", Char32Span(chars)).first);
}

TEST_CASE("Test case_insensitive_find_last")
{
    std::string s = "ABCÆØÅäöü";
    REQUIRE(case_insensitive_find_last(s, "Cæø") == Subrange(2, 5));
    REQUIRE(case_insensitive_find_last(s, "abcæ") == Subrange(0, 5));
    REQUIRE(case_insensitive_find_last(s, "Åäöü") == Subrange(7, 8));
    REQUIRE(!case_insensitive_find_last(s, "åaBC"));
}

TEST_CASE("Test case_insensitive_find_last_of")
{
    char32_t chars[4] = {U'≠', U'Ø', U'Å', U'¿'};
    CHECK_CHAR_SEARCH(case_insensitive_find_last_of(U8("qweåørty"), Char32Span(chars)), 5, 2, U'ø');
    REQUIRE(!case_insensitive_find_last_of("qwerty", Char32Span(chars)).first);
}

TEST_CASE("Test case_insensitive_replace")
{
    REQUIRE(case_insensitive_replace("abc dæ fgh DÆ i", "Dæ", U8("øå")) == U8("abc øå fgh øå i"));
    REQUIRE(case_insensitive_replace("abc dæ fgh DÆ i", "Dæ", U8("øå"), 1) == U8("abc øå fgh DÆ i"));
    REQUIRE(case_insensitive_replace("abc dæ fgh DÆ i", "Dæ", U8("øå"), -2) == U8("abc øå fgh øå i"));
}

TEST_CASE("Test case_insensitive_starts_with")
{
    REQUIRE(case_insensitive_starts_with(U8("BØABC BØBØ cfgå BØ"), U8("Bøa")));
    REQUIRE(!case_insensitive_starts_with(U8("BØABC BØBØ cfgå BØ"), U8("BøAD")));
    REQUIRE(case_insensitive_starts_with(U8("BøABC"), U8("BØABC")));
    REQUIRE(!case_insensitive_starts_with(U8("BøABC"), U8("BØABCE")));
}

TEST_CASE("Test contains")
{
    REQUIRE(contains("ABCDE", 'D'));
    REQUIRE(!contains("ABCDE", 'F'));
    REQUIRE(contains(U8("ABC∑ßÖ’Ü‹›ƒ¸√EFG"), U'√'));
}

TEST_CASE("Test count_characters")
{
    REQUIRE(count_characters("P\u0310s") == 2);
}

TEST_CASE("Test count_code_points")
{
    REQUIRE(count_code_points("") == 0);
    REQUIRE(count_code_points("A" UTF8_COMBINING_RING_ABOVE "BCDE" UTF8_COMBINING_TILDE) == 7);
}

TEST_CASE("Test ends_with")
{
    REQUIRE(ends_with(U8("ABCDÆØÅQRS"), U8("ØÅQRS")));
    REQUIRE(!ends_with(U8("ABCDÆØÅQRS"), U8("ØÅQR")));
    REQUIRE(ends_with(U8("ØÅQRS"), U8("ØÅQRS")));
    REQUIRE(!ends_with(U8("ÅQRS"), U8("ØÅQRS")));
    REQUIRE(ends_with(U8("ABCDÆØÅQRS"), U8("")));
}

TEST_CASE("Test find_first")
{
    std::string s = "ABCDEFGHCDEIJK";
    REQUIRE(find_first(s, "CDE") == Subrange(2, 3));
    REQUIRE(find_first(s, "CDE", 3) == Subrange(8, 3));
    REQUIRE(find_first(s, "ABCD") == Subrange(0, 4));
    REQUIRE(find_first(s, "JK") == Subrange(12, 2));
    REQUIRE(!find_first(s, "BCE"));
}

TEST_CASE("Test find_first_newline")
{
    REQUIRE(find_first_newline("abc\nd\nef") == Subrange(3, 1));
    REQUIRE(find_first_newline("abc\rd\nef") == Subrange(3, 1));
    REQUIRE(find_first_newline("abc\r\nd\nef") == Subrange(3, 2));
    REQUIRE(find_first_newline("abc\n\rd\nef") == Subrange(3, 1));
    REQUIRE(find_first_newline("abc" UTF8_PARAGRAPH_SEPARATOR "d\nef") == Subrange(3, 3));
    REQUIRE(find_first_newline("\nabc\nd\nef") == Subrange(0, 1));
    REQUIRE(find_first_newline("abcdef\n") == Subrange(6, 1));
    REQUIRE(!find_first_newline("abcdef"));
}

TEST_CASE("Test find_first_of")
{
    char32_t chars[4] = {U'≠', 'A', 'B', U'¿'};
    CHECK_CHAR_SEARCH(find_first_of(U8("qwe≠≠rty"), Char32Span(chars)), 3, 3, U'≠');
    REQUIRE(!find_first_of("qwerty", Char32Span(chars)).first);
}

TEST_CASE("Test find_first_where")
{
    auto result = find_first_where(U8("qWeÅty"), [](auto c) {return is_upper(c);});
    REQUIRE(result.first == Subrange(1, 1));
    REQUIRE(result.second == U'W');
    result = find_first_where(U8("qWeÅty"), [](auto c) {return is_upper(c);}, 2);
    REQUIRE(result.first == Subrange(3, 2));
    REQUIRE(result.second == U'Å');
}

TEST_CASE("Test find_last")
{
    std::string s("ABCDEFGHCDEIJK");
    REQUIRE(find_last(s, "CDE") == Subrange(8, 3));
    REQUIRE(find_last(s, "ABCD") == Subrange(0, 4));
    REQUIRE(find_last(s, "JK") == Subrange(12, 2));
    REQUIRE(!find_last(s, "BCE"));
}

TEST_CASE("Test find_last_newline")
{
    REQUIRE(find_last_newline("abc\nd\nef") == Subrange(5, 1));
    REQUIRE(find_last_newline("abc\nd\ref") == Subrange(5, 1));
    REQUIRE(find_last_newline("abc\nd\ref", 5) == Subrange(3, 1));
    REQUIRE(find_last_newline("abc\nd\r\nef") == Subrange(5, 2));
    REQUIRE(find_last_newline("abc\nd\n\ref") == Subrange(6, 1));
    REQUIRE(find_last_newline("abc\nd" UTF8_PARAGRAPH_SEPARATOR "ef") == Subrange(5, 3));
    REQUIRE(find_last_newline("\nabc\nd\nef\n") == Subrange(9, 1));
    REQUIRE(find_last_newline("\nabcdef") == Subrange(0, 1));
    REQUIRE(!find_last_newline("abcdef"));
}

TEST_CASE("Test find_last_of")
{
    char32_t chars[4] = {U'≠', 'A', 'B', U'¿'};
    CHECK_CHAR_SEARCH(find_last_of(U8("qwe≠≠rty"), Char32Span(chars)), 6, 3, U'≠');
    REQUIRE(!find_last_of("qwerty", Char32Span(chars)).first);
}

TEST_CASE("Test find_last_where")
{
    auto result = find_last_where(U8("qWeÅty"), [](auto c) {return is_upper(c);});
    REQUIRE(result.first == Subrange(3, 2));
    REQUIRE(result.second == U'Å');
}

TEST_CASE("Test get_character_pos")
{
    REQUIRE(get_character_pos(U8("PΩ\u0310sÅ"), 0) == 0);
    REQUIRE(get_character_pos(U8("PΩ\u0310sÅ"), 1) == 1);
    REQUIRE(get_character_pos(U8("PΩ\u0310sÅ"), 2) == 5);
    REQUIRE(get_character_pos(U8("PΩ\u0310sÅ"), 3) == 6);
    REQUIRE(get_character_pos(U8("PΩ\u0310sÅ"), 4) == 8);
    REQUIRE(get_character_pos(U8("PΩ\u0310sÅ"), 5) == std::string_view::npos);
    REQUIRE(get_character_pos(U8("PΩ\u0310sÅ"), -1) == 6);
    REQUIRE(get_character_pos(U8("PΩ\u0310sÅ"), -2) == 5);
    REQUIRE(get_character_pos(U8("PΩ\u0310sÅ"), -3) == 1);
    REQUIRE(get_character_pos(U8("PΩ\u0310sÅ"), -4) == 0);
    REQUIRE(get_character_pos(U8("PΩ\u0310sÅ"), -5) == std::string_view::npos);
}

TEST_CASE("Test get_character_range")
{
    REQUIRE(get_character_range(U8("PΩ\u0310sÅ"), 0) == Subrange(0, 1));
    REQUIRE(get_character_range(U8("PΩ\u0310sÅ"), 1) == Subrange(1, 4));
    REQUIRE(get_character_range(U8("PΩ\u0310sÅ"), 2) == Subrange(5, 1));
    REQUIRE(get_character_range(U8("PΩ\u0310sÅ"), 3) == Subrange(6, 2));
    REQUIRE(get_character_range(U8("PΩ\u0310sÅ"), 4) == Subrange(8, 0));
    REQUIRE(get_character_range(U8("PΩ\u0310sÅ"), 5) == Subrange(8, 0));
    REQUIRE(get_character_range(U8("PΩ\u0310sÅ"), -1) == Subrange(6, 2));
    REQUIRE(get_character_range(U8("PΩ\u0310sÅ"), -2) == Subrange(5, 1));
    REQUIRE(get_character_range(U8("PΩ\u0310sÅ"), -3) == Subrange(1, 4));
    REQUIRE(get_character_range(U8("PΩ\u0310sÅ"), -4) == Subrange(0, 1));
    REQUIRE(get_character_range(U8("PΩ\u0310sÅ"), -5) == Subrange(0, 0));
}

TEST_CASE("Test get_character_substring")
{
    REQUIRE(get_character_substring(U8("PΩ\u0310\u0311s\u0310Å"), 0) == U8("PΩ\u0310\u0311s\u0310Å"));
    REQUIRE(get_character_substring(U8("PΩ\u0310\u0311s\u0310Å"), 0, 2) == U8("PΩ\u0310\u0311"));
    REQUIRE(get_character_substring(U8("PΩ\u0310\u0311s\u0310Å"), 2, 6) == U8("s\u0310Å"));
}

TEST_CASE("Test get_code_point")
{
    CHECK_CHAR_SEARCH(get_code_point(U8("AB£ƒCD‹ß∂GHR"), 0), 0, 1, U'A');
    CHECK_CHAR_SEARCH(get_code_point(U8("AB£ƒCD‹ß∂GHR"), 6), 8, 3, U'‹');
    CHECK_CHAR_SEARCH(get_code_point(U8("AB£ƒCD‹ß∂GHR"), 11), 18, 1, U'R');
    REQUIRE(!get_code_point(U8("AB£ƒCD‹ß∂GHR"), 12).first);
    CHECK_CHAR_SEARCH(get_code_point(U8("AB£ƒCD‹ß∂GHR"), -1), 18, 1, U'R');
    CHECK_CHAR_SEARCH(get_code_point(U8("AB£ƒCD‹ß∂GHR"), -4), 13, 3, U'∂');
    CHECK_CHAR_SEARCH(get_code_point(U8("AB£ƒCD‹ß∂GHR"), -12), 0, 1, U'A');
    REQUIRE(!get_code_point(U8("AB£ƒCD‹ß∂GHR"), -13).first);
}

TEST_CASE("Test get_code_point_pos")
{
    REQUIRE(get_code_point_pos(U8("AB£ƒCD‹ß∂GHR"), 0) == 0);
    REQUIRE(get_code_point_pos(U8("AB£ƒCD‹ß∂GHR"), 6) == 8);
    REQUIRE(get_code_point_pos(U8("AB£ƒCD‹ß∂GHR"), 11) == 18);
    REQUIRE(get_code_point_pos(U8("AB£ƒCD‹ß∂GHR"), 12) == 19);
    REQUIRE(get_code_point_pos(U8("AB£ƒCD‹ß∂GHR"), 13) == std::string_view::npos);
    REQUIRE(get_code_point_pos(U8("AB£ƒCD‹ß∂GHR"), -1) == 18);
    REQUIRE(get_code_point_pos(U8("AB£ƒCD‹ß∂GHR"), -4) == 13);
    REQUIRE(get_code_point_pos(U8("AB£ƒCD‹ß∂GHR"), -12) == 0);
    REQUIRE(get_code_point_pos(U8("AB£ƒCD‹ß∂GHR"), -13) == std::string_view::npos);
}

TEST_CASE("Test get_code_point_substring")
{
    REQUIRE(get_code_point_substring(U8("ABCDÆØÅæøå€µ"), 0, 0).empty());
    REQUIRE(get_code_point_substring(U8("ABCDÆØÅæøå€µ"), 0, 5) == U8("ABCDÆ"));
    REQUIRE(get_code_point_substring(U8("ABCDÆØÅæøå€µ"), 8) == U8("øå€µ"));
    REQUIRE(get_code_point_substring(U8("ABCDÆØÅæøå€µ"), 8, 12) == U8("øå€µ"));
    REQUIRE(get_code_point_substring(U8("ABCDÆØÅæøå€µ"), 8, 13) == U8("øå€µ"));
    REQUIRE(get_code_point_substring(U8("ABCDÆØÅæøå€µ"), 8, 7).empty());
    REQUIRE(get_code_point_substring(U8("ABCDÆØÅæøå€µ"), 12, 13).empty());
    REQUIRE(get_code_point_substring(U8("ABCDÆØÅæøå€µ"), 13, 14).empty());
    REQUIRE(get_code_point_substring(U8("ABCDÆØÅæøå€µ"), -4) == U8("øå€µ"));
    REQUIRE(get_code_point_substring(U8("ABCDÆØÅæøå€µ"), -100, 5) == U8("ABCDÆ"));
    REQUIRE(get_code_point_substring(U8("ABCDÆØÅæøå€µ"), -4, -1) == U8("øå€"));
    REQUIRE(get_code_point_substring(U8("ABCDÆØÅæøå€µ"), 2, -2) == U8("CDÆØÅæøå"));
}

TEST_CASE("Test getNextCharacter")
{
    REQUIRE(get_next_character_range("A†µ", 1) == Subrange(1, 3));
    REQUIRE(get_next_character_range("P\u0310s", 0) == Subrange(0, 3));
    REQUIRE(get_next_character_range("P\u0310s", 3) == Subrange(3, 1));
}

TEST_CASE("Test getPrevCharacter")
{
    REQUIRE(get_prev_character_range("A†µ", 4) == Subrange(1, 3));
    REQUIRE(get_prev_character_range("P\u0310s", 4) == Subrange(3, 1));
    REQUIRE(get_prev_character_range("P\u0310s", 3) == Subrange(0, 3));
    REQUIRE_THROWS(get_prev_character_range("P\u0310s", 2));
}

TEST_CASE("Test insert_character")
{
    REQUIRE(insert_character(U8("PΩ\u0310sÅ"), 0, U'Ø') == U8("ØPΩ\u0310sÅ"));
    REQUIRE(insert_character(U8("PΩ\u0310sÅ"), 3, U'Ø') == U8("PΩ\u0310sØÅ"));
    REQUIRE(insert_character(U8("PΩ\u0310sÅ"), 4, U'Ø') == U8("PΩ\u0310sÅØ"));
    REQUIRE_THROWS(insert_character(U8("PΩ\u0310sÅ"), 5, U'Ø'));
    REQUIRE(insert_character(U8("PΩ\u0310sÅ"), -3, U'Ø') == U8("PØΩ\u0310sÅ"));
}

TEST_CASE("Test insert_characters")
{
    REQUIRE(insert_characters(U8("PΩ\u0310sÅ"), 0, U8("æØå")) == U8("æØåPΩ\u0310sÅ"));
    REQUIRE(insert_characters(U8("PΩ\u0310sÅ"), 3, U8("æØå")) == U8("PΩ\u0310sæØåÅ"));
    REQUIRE(insert_characters(U8("PΩ\u0310sÅ"), 4, U8("æØå")) == U8("PΩ\u0310sÅæØå"));
    REQUIRE_THROWS(insert_characters(U8("PΩ\u0310sÅ"), 5, U8("æØå")));
    REQUIRE(insert_characters(U8("PΩ\u0310sÅ"), -3, U8("æØå")) == U8("PæØåΩ\u0310sÅ"));
}

TEST_CASE("Test insert_code_point")
{
    REQUIRE(insert_code_point(U8("AB£ƒCD‹ß"), 0, U'Å') == U8("ÅAB£ƒCD‹ß"));
    REQUIRE(insert_code_point(U8("AB£ƒCD‹ß"), 3, U'Å') == U8("AB£ÅƒCD‹ß"));
    REQUIRE(insert_code_point(U8("AB£ƒCD‹ß"), 8, U'Å') == U8("AB£ƒCD‹ßÅ"));
    REQUIRE_THROWS(insert_code_point(U8("AB£ƒCD‹ß"), 9, U'Å'));
    REQUIRE(insert_code_point(U8("AB£ƒCD‹ß"), -1, U'Å') == U8("AB£ƒCD‹Åß"));
}

TEST_CASE("Test insert_code_points")
{
    REQUIRE(insert_code_points(U8("AB£ƒCD‹ß"), 0, U8("ØÆÅ")) == U8("ØÆÅAB£ƒCD‹ß"));
    REQUIRE(insert_code_points(U8("AB£ƒCD‹ß"), 3, U8("ØÆÅ")) == U8("AB£ØÆÅƒCD‹ß"));
    REQUIRE(insert_code_points(U8("AB£ƒCD‹ß"), 8, U8("ØÆÅ")) == U8("AB£ƒCD‹ßØÆÅ"));
    REQUIRE_THROWS(insert_code_points(U8("AB£ƒCD‹ß"), 9, U8("ØÆÅ")));
    REQUIRE(insert_code_points(U8("AB£ƒCD‹ß"), -1, U8("ØÆÅ")) == U8("AB£ƒCD‹ØÆÅß"));
}

TEST_CASE("Test is_valid_utf8")
{
    REQUIRE(is_valid_utf8(U8("AB£ƒCD‹ß")));
    REQUIRE(!is_valid_utf8("Q\xF0\xCA\xCAZ"));
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
    REQUIRE(replace("abc de fgh de i", "de", U8("øå")) == U8("abc øå fgh øå i"));
    REQUIRE(replace("abc de fgh de i", "de", U8("øå"), 1) == U8("abc øå fgh de i"));
    REQUIRE(replace("abc de fgh de i", "de", U8("øå"), -2) == U8("abc øå fgh øå i"));
}

TEST_CASE("Test replace_characters")
{
    REQUIRE(replace_characters(U8("AB\u0310ÆØÅäö•"), 3, 6, "√ƒ") == U8("AB\u0310Æ√ƒö•"));
    REQUIRE(replace_characters(U8("AB\u0310ÆØÅäö•"), -12, -1, "√ƒ") == U8("√ƒ•"));
    REQUIRE(replace_characters(U8("AB\u0310ÆØÅäö•"), 3, 100, "√ƒ") == U8("AB\u0310Æ√ƒ"));
}

TEST_CASE("Test replace_code_points")
{
    REQUIRE(replace_code_points(U8("ABÆØÅäö•"), 3, 6, "√ƒ") == U8("ABÆ√ƒö•"));
    REQUIRE(replace_code_points(U8("ABÆØÅäö•"), -12, -1, "√ƒ") == U8("√ƒ•"));
    REQUIRE(replace_code_points(U8("ABÆØÅäö•"), 3, 100, "√ƒ") == U8("ABÆ√ƒ"));
}

TEST_CASE("Test replace_code_point")
{
    REQUIRE(replace_code_point(U8("Øbkøaøhjød"), U'ø', U'ß') == U8("Øbkßaßhjßd"));
    REQUIRE(replace_code_point(U8("Øbkøaøhjød"), U'ø', U'ß', 2) == U8("Øbkßaßhjød"));
    REQUIRE(replace_code_point(U8("Øbkøaøhjød"), U'ø', U'ß', -2) == U8("Øbkøaßhjßd"));
}

TEST_CASE("Test replace_invalid_utf8")
{
    REQUIRE(replace_invalid_utf8(U8(""), U'ƒ').empty());
    REQUIRE(replace_invalid_utf8(U8("Øbkæø"), U'ƒ') == U8("Øbkæø"));
    REQUIRE(replace_invalid_utf8(U8("Øbk" "\xE0\xAF" u8"æø"), U'ƒ') == U8("Øbkƒæø"));
}

TEST_CASE("Test replace_invalid_utf8, mutable string")
{
    std::string s(U8("Øbk" "\xE0\xAF" u8"æø"));
    REQUIRE(replace_invalid_utf8(s, U'™') == U8("Øbk™æø"));
    REQUIRE(replace_invalid_utf8(s, U'™') == U8("Øbk™æø"));
    REQUIRE(s == U8("Øbk™æø"));
}

TEST_CASE("Test reverse")
{
    REQUIRE(reverse("P\u0310s") == "sP\u0310");
}

std::vector<std::string_view> sv(std::vector<std::string_view> strs)
{
    return strs;
}

TEST_CASE("Test split on characters")
{
    char32_t chars[] = {U'Æ', U'Ø', U'Å', U'Q'};
    REQUIRE(split(U8("ÅABØQCDÆ"), Char32Span(chars)) == sv({"", "AB", "", "CD", ""}));
    REQUIRE(split(U8("ÅABØQCDÆ"), Char32Span(chars), IGNORE_EMPTY_SPLIT) == sv({"AB", "CD"}));
    REQUIRE(split(U8("ÅABØQCDÆ"), Char32Span(chars), {2}) == sv({"", "AB", U8("QCDÆ")}));
    REQUIRE(split(U8("ÅABØQCDÆ"), Char32Span(chars), {1, true}) == sv({"AB", U8("QCDÆ")}));
    REQUIRE(split(U8("ÅABØQCDÆ"), Char32Span(chars), {2, true}) == sv({"AB", "CD"}));
}

TEST_CASE("Test split on substring")
{
    REQUIRE(split(U8("BØABC BØBØ cfgå BØ"), U8("BØ")) == sv({"", "ABC ", "", U8(" cfgå "), ""}));
    REQUIRE(split(U8("BØABC BØBØ cfgå BØ"), U8("BØ"), IGNORE_EMPTY_SPLIT) == sv({"ABC ", U8(" cfgå ")}));
    REQUIRE(split(U8("BØABC BØBØ cfgå BØ"), U8("BØ"), {3}) == sv({"", "ABC ", "", U8(" cfgå BØ")}));
}

TEST_CASE("Test case-insensitive split on substring")
{
    REQUIRE(split(U8("BØABC BøbØ cfgå bØ"), U8("Bø"), CASE_INSENSITIVE_SPLIT) == sv({"", "ABC ", "", U8(" cfgå "), ""}));
}

TEST_CASE("Test split_lines")
{
    REQUIRE(split_lines(U8("\nABC\r\n\rcfgå\n")) == sv({"", "ABC", "", U8("cfgå"), ""}));
    REQUIRE(split_lines(U8("\nABC\r\n\rcfgå\n"), IGNORE_EMPTY_SPLIT) == sv({"ABC", U8("cfgå")}));
    REQUIRE(split_lines(U8("\nABC\r\n\rcfgå\n"), {2}) == sv({"", "ABC", U8("\rcfgå\n")}));
    REQUIRE(split_lines(U8("\nABC\r\n\rcfgå\n"), {1, true}) == sv({"ABC", U8("\rcfgå\n")}));
}

TEST_CASE("Test starts_with")
{
    REQUIRE(starts_with(U8("BØABC BØBØ cfgå BØ"), U8("BØA")));
    REQUIRE(!starts_with(U8("BØABC BØBØ cfgå BØ"), U8("BØAD")));
    REQUIRE(starts_with(U8("BØABC"), U8("BØABC")));
    REQUIRE(!starts_with(U8("BØABC"), U8("BØABCE")));
}

TEST_CASE("Test trim")
{
    char32_t CHARS[] = {' ', U'Ø', U'ø'};
    Char32Span CHAR_SPAN(CHARS);
    REQUIRE(trim(U8(""), CHAR_SPAN).empty());
    REQUIRE(trim(U8("f oøo"), CHAR_SPAN) == U8("f oøo"));
    REQUIRE(trim(U8(" øf oøo"), CHAR_SPAN) == U8("f oøo"));
    REQUIRE(trim(U8("f oøo Ø"), CHAR_SPAN) == U8("f oøo"));
    REQUIRE(trim(U8(" øf oøo Ø"), CHAR_SPAN) == U8("f oøo"));
}

TEST_CASE("Test trim_end")
{
    char32_t CHARS[] = {' ', U'Ø', U'ø'};
    Char32Span CHAR_SPAN(CHARS);
    REQUIRE(trim_end(U8(""), CHAR_SPAN).empty());
    REQUIRE(trim_end(U8("f oøo"), CHAR_SPAN) == U8("f oøo"));
    REQUIRE(trim_end(U8(" øf oøo"), CHAR_SPAN) == U8(" øf oøo"));
    REQUIRE(trim_end(U8("f oøo Ø"), CHAR_SPAN) == U8("f oøo"));
    REQUIRE(trim_end(U8(" øf oøo Ø"), CHAR_SPAN) == U8(" øf oøo"));
}

TEST_CASE("Test trim_start")
{
    char32_t CHARS[] = {' ', U'Ø', U'ø'};
    Char32Span CHAR_SPAN(CHARS);
    REQUIRE(trim_start(U8(""), CHAR_SPAN).empty());
    REQUIRE(trim_start(U8("f oøo"), CHAR_SPAN) == U8("f oøo"));
    REQUIRE(trim_start(U8(" øf oøo"), CHAR_SPAN) == U8("f oøo"));
    REQUIRE(trim_start(U8("f oøo Ø"), CHAR_SPAN) == U8("f oøo Ø"));
    REQUIRE(trim_start(U8(" øf oøo Ø"), CHAR_SPAN) == U8("f oøo Ø"));
}
