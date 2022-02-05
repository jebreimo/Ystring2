//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-07-29.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <algorithm>
#include "Ystring/Algorithms.hpp"
#include "Ystring/CodePointPredicates.hpp"
#include "Ystring/ConvertCase.hpp"
#include "LowerCaseTables.hpp"
#include "TitleCaseTables.hpp"
#include "UpperCaseTables.hpp"

namespace ystring
{
    namespace
    {
        constexpr char32_t SEGMENT_MASK = ~(char32_t)0x1F;

        std::optional<char32_t>
        find_in_compact_mapping(const CompactCharMapping* mapping,
                                size_t mapping_size,
                                char32_t code_point)
        {
            const auto* set = std::lower_bound(
                mapping,
                mapping + mapping_size,
                CompactCharMapping{code_point & SEGMENT_MASK},
                [](auto& a, auto& b){return a.segment < b.segment;});

            if (set == mapping + mapping_size ||
                set->segment != (code_point & SEGMENT_MASK))
                return {};

            return set->get(code_point);
        }

        std::optional<char32_t>
        find_in_mapping(const CharMapping* mapping,
                        size_t mapping_size,
                        char32_t code_point)
        {
            const auto* it = std::lower_bound(
                mapping,
                mapping + mapping_size,
                CharMapping{code_point},
                [](const auto& a, const auto& b)
                {return a.from_code_point < b.from_code_point;});
            if (it == mapping + mapping_size || it->from_code_point != code_point)
                return {};

            return it->to_code_point;
        }

        char32_t convert(const CompactCharMapping* compact,
                         size_t compact_size,
                         const CharMapping* mapping,
                         size_t mapping_size,
                         char32_t code_point)
        {
            if (auto c = find_in_compact_mapping(compact, compact_size, code_point))
                return *c;
            if (auto c = find_in_mapping(mapping, mapping_size, code_point))
                return *c;
            return code_point;
        }
    }

    char32_t to_lower(char32_t code_point)
    {
        if (code_point < 128)
        {
            if ('A' <= code_point && code_point <= 'Z')
                return code_point + 32;
            return code_point;
        }
        return convert(COMPACT_LOWER_CASE,
                       std::size(COMPACT_LOWER_CASE),
                       LOWER_CASE,
                       std::size(LOWER_CASE),
                       code_point);
    }

    char32_t to_title(char32_t code_point)
    {
        if (code_point < 128)
        {
            if ('a' <= code_point && code_point <= 'z')
                return code_point - 32;
            return code_point;
        }
        return convert(COMPACT_TITLE_CASE,
                       std::size(COMPACT_TITLE_CASE),
                       TITLE_CASE,
                       std::size(TITLE_CASE),
                       code_point);
    }

    char32_t to_upper(char32_t code_point)
    {
        if (code_point < 128)
        {
            if ('a' <= code_point && code_point <= 'z')
                return code_point - 32;
            return code_point;
        }
        return convert(COMPACT_UPPER_CASE,
                       std::size(COMPACT_UPPER_CASE),
                       UPPER_CASE,
                       std::size(UPPER_CASE),
                       code_point);
    }

    std::string to_lower(std::string_view str)
    {
        std::string result;
        result.reserve(str.size());
        auto it = str.begin();
        char32_t ch;
        while (safe_decode_next(it, str.end(), ch))
            append(result, to_lower(ch));
        return result;
    }

    std::string to_title(std::string_view str)
    {
        std::string result;
        result.reserve(str.size());
        auto it = str.begin();
        char32_t ch;
        bool preceded_by_letter = false;
        while (safe_decode_next(it, str.end(), ch))
        {
            if (!is_letter(ch))
            {
                append(result, ch);
                preceded_by_letter = false;
            }
            else if (preceded_by_letter)
            {
                append(result, to_lower(ch));
            }
            else if (ch != U'ß')
            {
                append(result, to_title(ch));
                preceded_by_letter = true;
            }
            else
            {
                result.append("Ss");
                preceded_by_letter = true;
            }
        }
        return result;
    }

    std::string to_upper(std::string_view str)
    {
        std::string result;
        result.reserve(str.size());
        auto it = str.begin();
        char32_t ch;
        while (safe_decode_next(it, str.end(), ch))
        {
            if (ch != U'ß')
                append(result, to_upper(ch));
            else
                result.append("SS");
        }
        return result;
    }
}
