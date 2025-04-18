//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-07-29.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <algorithm>
#include "Ystring/Algorithms.hpp"
#include "Ystring/CodepointPredicates.hpp"
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
                                char32_t codepoint)
        {
            const auto* set = std::lower_bound(
                mapping,
                mapping + mapping_size,
                CompactCharMapping{codepoint & SEGMENT_MASK},
                [](auto& a, auto& b){return a.segment < b.segment;});

            if (set == mapping + mapping_size ||
                set->segment != (codepoint & SEGMENT_MASK))
                return {};

            return set->get(codepoint);
        }

        std::optional<char32_t>
        find_in_mapping(const CharMapping* mapping,
                        size_t mapping_size,
                        char32_t codepoint)
        {
            const auto* it = std::lower_bound(
                mapping,
                mapping + mapping_size,
                CharMapping{codepoint},
                [](const auto& a, const auto& b)
                {return a.from_codepoint < b.from_codepoint;});
            if (it == mapping + mapping_size || it->from_codepoint != codepoint)
                return {};

            return it->to_codepoint;
        }

        char32_t convert(const CompactCharMapping* compact,
                         size_t compact_size,
                         const CharMapping* mapping,
                         size_t mapping_size,
                         char32_t codepoint)
        {
            if (auto c = find_in_compact_mapping(compact, compact_size, codepoint))
                return *c;
            if (auto c = find_in_mapping(mapping, mapping_size, codepoint))
                return *c;
            return codepoint;
        }
    }

    char32_t to_lower(char32_t codepoint)
    {
        if (codepoint < 128)
        {
            if ('A' <= codepoint && codepoint <= 'Z')
                return codepoint + 32;
            return codepoint;
        }
        return convert(COMPACT_LOWER_CASE,
                       std::size(COMPACT_LOWER_CASE),
                       LOWER_CASE,
                       std::size(LOWER_CASE),
                       codepoint);
    }

    char32_t to_title(char32_t codepoint)
    {
        if (codepoint < 128)
        {
            if ('a' <= codepoint && codepoint <= 'z')
                return codepoint - 32;
            return codepoint;
        }
        return convert(COMPACT_TITLE_CASE,
                       std::size(COMPACT_TITLE_CASE),
                       TITLE_CASE,
                       std::size(TITLE_CASE),
                       codepoint);
    }

    char32_t to_upper(char32_t codepoint)
    {
        if (codepoint < 128)
        {
            if ('a' <= codepoint && codepoint <= 'z')
                return codepoint - 32;
            return codepoint;
        }
        return convert(COMPACT_UPPER_CASE,
                       std::size(COMPACT_UPPER_CASE),
                       UPPER_CASE,
                       std::size(UPPER_CASE),
                       codepoint);
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
