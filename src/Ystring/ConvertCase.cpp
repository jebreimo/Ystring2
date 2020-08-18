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

namespace Ystring
{
    namespace
    {
        constexpr char32_t SEGMENT_MASK = ~(char32_t)0x1F;

        std::optional<char32_t>
        findInCompactMapping(const CompactCharMapping* mapping,
                             size_t mappingSize,
                             char32_t codePoint)
        {
            const auto* set = std::lower_bound(
                mapping,
                mapping + mappingSize,
                CompactCharMapping{codePoint & SEGMENT_MASK},
                [](auto& a, auto& b){return a.segment < b.segment;});

            if (set == mapping + mappingSize ||
                set->segment != (codePoint & SEGMENT_MASK))
                return {};

            return set->get(codePoint);
        }

        std::optional<char32_t>
        findInMapping(const CharMapping* mapping,
                      size_t mappingSize,
                      char32_t codePoint)
        {
            const auto* it = std::lower_bound(
                mapping,
                mapping + mappingSize,
                CharMapping{codePoint},
                [](const auto& a, const auto& b)
                {return a.fromCodePoint < b.fromCodePoint;});
            if (it == mapping + mappingSize || it->fromCodePoint != codePoint)
                return {};

            return it->toCodePoint;
        }

        char32_t convert(const CompactCharMapping* compact,
                         size_t compactSize,
                         const CharMapping* mapping,
                         size_t mappingSize,
                         char32_t codePoint)
        {
            if (auto c = findInCompactMapping(compact, compactSize, codePoint))
                return *c;
            if (auto c = findInMapping(mapping, mappingSize, codePoint))
                return *c;
            return codePoint;
        }
    }

    char32_t toLower(char32_t codePoint)
    {
        if (codePoint < 128)
        {
            if ('A' <= codePoint && codePoint <= 'Z')
                return codePoint + 32;
            return codePoint;
        }
        return convert(COMPACT_LOWER_CASE,
                       std::size(COMPACT_LOWER_CASE),
                       LOWER_CASE,
                       std::size(LOWER_CASE),
                       codePoint);
    }

    char32_t toTitle(char32_t codePoint)
    {
        if (codePoint < 128)
        {
            if ('a' <= codePoint && codePoint <= 'z')
                return codePoint - 32;
            return codePoint;
        }
        return convert(COMPACT_TITLE_CASE,
                       std::size(COMPACT_TITLE_CASE),
                       TITLE_CASE,
                       std::size(TITLE_CASE),
                       codePoint);
    }

    char32_t toUpper(char32_t codePoint)
    {
        if (codePoint < 128)
        {
            if ('a' <= codePoint && codePoint <= 'z')
                return codePoint - 32;
            return codePoint;
        }
        return convert(COMPACT_UPPER_CASE,
                       std::size(COMPACT_UPPER_CASE),
                       UPPER_CASE,
                       std::size(UPPER_CASE),
                       codePoint);
    }

    std::string toLower(std::string_view str)
    {
        std::string result;
        result.reserve(str.size());
        auto it = str.begin();
        char32_t ch;
        while (safeDecodeNext(it, str.end(), ch))
            append(result, toLower(ch));
        return result;
    }

    std::string toTitle(std::string_view str)
    {
        std::string result;
        result.reserve(str.size());
        auto it = str.begin();
        char32_t ch;
        bool precededByLetter = false;
        while (safeDecodeNext(it, str.end(), ch))
        {
            if (!isLetter(ch))
            {
                append(result, ch);
                precededByLetter = false;
            }
            else if (precededByLetter)
            {
                append(result, toLower(ch));
            }
            else if (ch != U'ß')
            {
                append(result, toTitle(ch));
                precededByLetter = true;
            }
            else
            {
                result.append("Ss");
                precededByLetter = true;
            }
        }
        return result;
    }

    std::string toUpper(std::string_view str)
    {
        std::string result;
        result.reserve(str.size());
        auto it = str.begin();
        char32_t ch;
        while (safeDecodeNext(it, str.end(), ch))
        {
            if (ch != U'ß')
                append(result, toUpper(ch));
            else
                result.append("SS");
        }
        return result;
    }
}
