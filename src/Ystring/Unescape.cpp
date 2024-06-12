//****************************************************************************
// Copyright © 2024 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2024-03-29.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Unescape.hpp"

#include <algorithm>
#include <charconv>
#include <string>
#include "Ystring/DecodeUtf8.hpp"
#include "Ystring/Subrange.hpp"
#include "Ystring/YstringException.hpp"
#include "EncodeUtf8.hpp"

namespace ystring
{
    namespace
    {
        bool is_hex_digit(char c)
        {
            return ('0' <= c && c <= '9')
                   || ('A' <= (c & 0xDFu) && (c & 0xDFu) <= 'F');
        }

        template <typename Pred>
        size_t find_first_where(std::string_view str, size_t start, size_t end,
                                Pred pred)
        {
            for (size_t i = start; i < std::min(str.size(), end); ++i)
            {
                if (pred(str[i]))
                    return i;
            }
            return end;
        }

        size_t
        find_end_of_hex_sequence(std::string_view str, size_t start, size_t end)
        {
            return find_first_where(str, start, end, [](auto c)
            {
                return !is_hex_digit(c);
            });
        }

        size_t
        find_end_of_oct_sequence(std::string_view str, size_t start, size_t end)
        {
            return find_first_where(str, start, end, [](auto c)
            {
                return !('0' <= c && c <= '7');
            });
        }

        char32_t parse_escaped_hex_char(std::string_view str)
        {
            uint32_t value;
            auto res = std::from_chars(str.data() + 2,
                                       str.data() + str.size(),
                                       value, 16);
            if (res.ptr == str.data() + 2)
            {
                YSTRING_THROW("Cannot parse escaped char: "
                              + std::string(str));
            }
            return char32_t(value);
        }

        char32_t parse_escaped_oct_char(std::string_view str)
        {
            uint32_t value;
            auto res = std::from_chars(str.data() + 1,
                                       str.data() + str.size(),
                                       value, 8);
            if (res.ptr == str.data() + 1)
            {
                YSTRING_THROW("Cannot parse escaped char: "
                              + std::string(str));
            }
            return char32_t(value);
        }

        char32_t combine_surrogate_pair(char32_t ch1, char32_t ch2)
        {
            return (((ch1 & 0x3FFu) << 10u) | (ch2 & 0x3FFu)) + 0x10000;
        }

        std::pair<char32_t, Subrange>
        parse_utf16(std::string_view str, Subrange sub)
        {
            auto ch = parse_escaped_hex_char(str.substr(sub.offset, sub.length));
            if (ch < 0xD800 || 0xDBFF < ch)
                return {char32_t(ch), sub};

            // Look for the second part of a surrogate pair.
            auto str2 = str.substr(sub.end(), 6);
            auto sub2 = find_first_escaped_char(str2);
            if (!sub2 || sub2.offset != 0 || sub2.length != 6)
                return {ch, sub};

            if (str[sub.end() + 1] != 'u')
                return {ch, sub};

            auto ch2 = parse_escaped_hex_char(str2);
            if (ch2 < 0xDC00 || 0xDFFF < ch2)
                return {char32_t(ch), sub};

            return {combine_surrogate_pair(ch, ch2), {sub.offset, 12}};
        }
    }

    Subrange find_first_escaped_char(std::string_view str, size_t offset)
    {
        auto it = std::find(str.begin() + ptrdiff_t(offset), str.end(), '\\');
        if (it == str.end())
            return Subrange(str.size());
        auto start = std::distance(str.begin(), it);
        if (++it == str.end())
        {
            YSTRING_THROW("Incomplete escape sequence at offset "
                          + std::to_string(start));
        }

        size_t end;
        unsigned min_length;
        switch (*it)
        {
        case 'X':
        case 'x':
            end = find_end_of_hex_sequence(str, start + 2, str.size());
            min_length = 4;
            break;
        case 'u':
            end = find_end_of_hex_sequence(str, start + 2, start + 6);
            min_length = 6;
            break;
        case 'U':
            end = find_end_of_hex_sequence(str, start + 2, start + 10);
            min_length = 10;
            break;
        default:
            if ('0' <= *it && *it <= '7')
            {
                end = find_end_of_oct_sequence(str, start + 1, start + 4);
                min_length = 2;
            }
            else if ((*it & 0x80u) == 0)
            {
                return {size_t(start), 2};
            }
            else
            {
                YSTRING_THROW("Invalid escape sequence at offset "
                              + std::to_string(start));
            }
        }
        if (end - start < min_length)
        {
            YSTRING_THROW("Invalid escape sequence at offset "
                          + std::to_string(start));
        }

        return {size_t(start), end - start};
    }

    bool has_escaped_chars(std::string_view str)
    {
        return bool(find_first_escaped_char(str));
    }

    size_t unescape(char* str, size_t length)
    {
        size_t from = 0;
        size_t to = 0;
        std::string_view sv(str, length);
        while (true)
        {
            auto sub = find_first_escaped_char(sv, from);
            if (from != to)
                std::copy(str + from, str + sub.start(), str + to);
            to += sub.start() - from;

            if (!sub)
                break;

            char32_t ch = 0;
            switch (sv[sub.start() + 1])
            {
            case 'x':
            case 'X':
            case 'U':
                ch = parse_escaped_hex_char(sv.substr(sub.start(), sub.length));
                break;
            case 'u':
                std::tie(ch, sub) = parse_utf16(sv, sub);
                break;
            case 'a':
                ch = 0x07;
                break;
            case 'b':
                ch = 0x08;
                break;
            case 'f':
                ch = 0x0C;
                break;
            case 'n':
                ch = 0x0A;
                break;
            case 'r':
                ch = 0x0D;
                break;
            case 't':
                ch = 0x09;
                break;
            case 'v':
                ch = 0x0B;
                break;
            default:
                if ('0' <= sv[sub.start() + 1] && sv[sub.start() + 1] <= '7')
                    ch = parse_escaped_oct_char(sv.substr(sub.start(), sub.length));
                else
                    ch = char32_t(sv[sub.start() + 1]);
                break;
            }
            to += encode_utf8(ch, str + to);
            from = sub.end();
        }
        return to;
    }

    std::string unescape(std::string str)
    {
        str.resize(unescape(str.data(), str.size()));
        return str;
    }

    char32_t extract_hex_char(std::string_view& str,
                              size_t min_length,
                              size_t max_length)
    {
        if (max_length > str.size())
            max_length = str.size();

        uint32_t value;
        auto res = std::from_chars(str.data(), str.data() + max_length,
                                   value, 16);
        auto length = size_t(res.ptr - str.data());
        if (length < min_length)
        {
            YSTRING_THROW("Too few hex digits in escape sequence."
                          " Expected at least " + std::to_string(min_length)
                          + ", got " + std::to_string(length));
        }

        str.remove_prefix(length);
        return char32_t(value);
    }

    char32_t extract_utf16_char(std::string_view& str)
    {
        auto ch = extract_hex_char(str, 4, 4);
        if (ch < 0xD800 || 0xDBFF < ch)
            return ch;

        // Look for the second part of a surrogate pair.
        auto str2 = str.substr(0, 6);
        if (str2.size() < 6 || str2[0] != '\\' || str2[1] != 'u')
            return ch;

        str2.remove_prefix(2);
        auto ch2 = extract_hex_char(str2, 4, 4);
        if (ch2 < 0xDC00 || 0xDFFF < ch2)
            return ch;

        str.remove_prefix(6);
        return combine_surrogate_pair(ch, ch2);
    }

    char32_t extract_oct_char(std::string_view& str)
    {
        auto max_length = std::min(str.size(), size_t(3));
        uint32_t value;
        auto res = std::from_chars(str.data(),
                                   str.data() + max_length,
                                   value, 8);
        auto length = res.ptr - str.data();
        if (length == 0)
        {
            YSTRING_THROW("Invalid octal escape sequence.");
        }

        str.remove_prefix(length);
        return char32_t(value);
    }

    std::optional<char32_t>
    unescape_next(std::string_view& str, bool* did_unescape)
    {
        while (str.size() > 1 && str[0] == '\\' && str[1] == '\n')
            str.remove_prefix(2);

        if (str.empty())
        {
            if (did_unescape)
                *did_unescape = false;
            return {};
        }

        if (str[0] != '\\')
        {
            if (did_unescape)
                *did_unescape = false;
            return pop_utf8_codepoint(str);
        }

        if (str.size() == 1)
            YSTRING_THROW("Incomplete escape sequence.");

        auto backup = str;
        char type = str[1];
        str.remove_prefix(2);
        if (did_unescape)
            *did_unescape = true;

        try
        {
            switch (type)
            {
            case 'a':
                return char32_t(0x07);
            case 'b':
                return char32_t(0x08);
            case 'f':
                return char32_t(0x0C);
            case 'n':
                return char32_t(0x0A);
            case 'r':
                return char32_t(0x0D);
            case 't':
                return char32_t(0x09);
            case 'v':
                return char32_t(0x0B);
            case 'x':
            case 'X':
                return extract_hex_char(str, 1, 8);
            case 'U':
                return extract_hex_char(str, 8, 8);
            case 'u':
                return extract_utf16_char(str);
            default:
                str = backup.substr(1);
                if ('0' <= type && type <= '7')
                    return extract_oct_char(str);
                return pop_utf8_codepoint(str);
            }
        }
        catch (YstringException&)
        {
            str = backup;
            throw;
        }
    }
}
