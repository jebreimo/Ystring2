//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-01-30.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Escape.hpp"

#include <charconv>
#include "Ystring/Subrange.hpp"
#include "Ystring/YstringException.hpp"
#include "EncodeUtf8.hpp"

namespace ystring
{
    namespace
    {
        char to_digit(uint8_t c)
        {
            if (c < 10)
                return char('0' + c);
            if (c < 36)
                return char('A' + (c - 10));
            return '?';
        }
    }

    Subrange find_first_escapable_character(std::string_view str,
                                            size_t offset)
    {
        auto it = std::find_if(str.begin() + offset, str.end(), [](auto c)
        {
            return static_cast<uint8_t>(c) < 32 || c == '"' || c == '\\';
        });
        return Subrange(it - str.begin(), it == str.end() ? 0 : 1);
    }

    bool has_escapable_characters(std::string_view str)
    {
        return bool(find_first_escapable_character(str));
    }

    std::string escape_json(std::string_view str)
    {
        std::string result;
        size_t offset = 0;
        while (auto sub = find_first_escapable_character(str, offset))
        {
            result.append(str.begin() + offset, str.begin() + sub.offset);
            result.push_back('\\');
            switch(str[sub.offset])
            {
            case '\b':
                result.push_back('b');
                break;
            case '\f':
                result.push_back('f');
                break;
            case '\n':
                result.push_back('n');
                break;
            case '\r':
                result.push_back('r');
                break;
            case '\t':
                result.push_back('t');
                break;
            case '\"':
                result.push_back('"');
                break;
            case '\\':
                result.push_back('\\');
                break;
            default:
                result.append("u00");
                result.push_back(to_digit((str[sub.offset] & 0xF0u) >> 4u));
                result.push_back(to_digit(str[sub.offset] & 0xFu));
                break;
            }
            offset = sub.end();
        }
        result.append(str.begin() + offset, str.end());
        return result;
    }

    namespace
    {
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
                return !('0' <= c && c <= '9')
                       && !('A' <= (c & 0xDFu) && (c & 0xDFu) <= 'F');
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
                YSTRING_THROW("Cannot parse escaped character: "
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
                YSTRING_THROW("Cannot parse escaped character: "
                              + std::string(str));
            }
            return char32_t(value);
        }

        std::pair<char32_t, Subrange>
        parse_utf16(std::string_view str, Subrange sub)
        {
            auto ch = parse_escaped_hex_char(str.substr(sub.offset, sub.length));
            if (ch < 0xD800 || 0xDBFF < ch)
                return {char32_t(ch), sub};

            // Look for the second part of a surrogate pair.
            auto str2 = str.substr(sub.end(), 6);
            auto sub2 = find_first_escaped_character(str2);
            if (!sub2 || sub2.offset != 0 || sub2.length != 6)
                return {ch, sub};

            if (str[sub.end() + 1] != 'u')
                return {ch, sub};

            auto ch2 = parse_escaped_hex_char(str2);
            if (ch2 < 0xDC00 || 0xDFFF < ch2)
                return {char32_t(ch), sub};

            return {(((ch & 0x3FFu) << 10u) | (ch2 & 0x3FFu)) + 0x10000,
                    {sub.offset, 12}};
        }
    }

    Subrange find_first_escaped_character(std::string_view str, size_t offset)
    {
        auto it = std::find(str.begin() + offset, str.end(), '\\');
        if (it == str.end())
            return Subrange(str.size());
        auto start = std::distance(str.begin(), it);
        if (++it == str.end())
            YSTRING_THROW("Incomplete escape sequence at offset "
                          + std::to_string(start));
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
                return Subrange(start, 2);
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

        return Subrange(start, end - start);
    }

    bool has_escaped_characters(std::string_view str)
    {
        return bool(find_first_escaped_character(str));
    }

    size_t unescape(char* str, size_t length)
    {
        size_t from = 0;
        size_t to = 0;
        std::string_view sv(str, length);
        while (true)
        {
            auto sub = find_first_escaped_character(sv, from);
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
}
