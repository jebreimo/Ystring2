//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-01-30.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Escape.hpp"

#include <algorithm>
#include "Ystring/Subrange.hpp"
#include "Ystring/YstringException.hpp"

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

    Subrange find_first_escapable_char(std::string_view str,
                                            size_t offset)
    {
        auto it = std::find_if(str.begin() + offset, str.end(), [](auto c)
        {
            return static_cast<uint8_t>(c) < 32 || c == '"' || c == '\\';
        });
        return {size_t(it - str.begin()), size_t(it == str.end() ? 0 : 1)};
    }

    bool has_escapable_chars(std::string_view str)
    {
        return bool(find_first_escapable_char(str));
    }

    std::string escape_json(std::string_view str)
    {
        std::string result;
        size_t offset = 0;
        while (auto sub = find_first_escapable_char(str, offset))
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
}
