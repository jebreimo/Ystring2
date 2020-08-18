//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-05-31.
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Algorithms.hpp"

#include "Ystring/DecodeUtf8.hpp"
#include "EncodeUtf8.hpp"
#include "Ystring/CaseInsensitive.hpp"
#include "Ystring/CodePointPredicates.hpp"
#include "AlgorithmUtilities.hpp"

namespace Ystring
{
    namespace
    {
        constexpr char32_t NEWLINES[] = {
            '\n',
            '\v',
            '\f',
            NEXT_LINE,
            LINE_SEPARATOR,
            PARAGRAPH_SEPARATOR,
            '\r'
        };
    }

    std::string& append(std::string& str, char32_t chr)
    {
        if (encodeUtf8(chr, std::back_inserter(str)))
            return str;
        YSTRING_THROW("Invalid code point: " + std::to_string(size_t(chr)));
    }

    std::string append(std::string_view str, char32_t chr)
    {
        std::string result(str);
        append(result, chr);
        return result;
    }

    int32_t caseInsensitiveCompare(std::string_view str, std::string_view cmp)
    {
        auto sI = str.begin();
        auto cI = cmp.begin();
        while (true)
        {
            char32_t sC;
            auto hasS = safeDecodeNext(sI, str.end(), sC);
            char32_t cC;
            auto hasC = safeDecodeNext(cI, cmp.end(), cC);
            if (!hasS || !hasC)
                return hasS == hasC ? 0 : (hasS ? 1 : -1);

            if (auto result = caseInsensitiveCompare(sC, cC))
                return result;
        }
    }

    bool caseInsensitiveEndsWith(std::string_view str, std::string_view cmp)
    {
        auto[itStr, itCmp] = prevCaseInsensitiveMismatch(
            str.begin(), str.end(), cmp.begin(), cmp.end());
        return itCmp == cmp.begin();
    }

    bool caseInsensitiveEqual(std::string_view str, std::string_view cmp)
    {
        auto sI = str.begin();
        auto cI = cmp.begin();
        while (true)
        {
            char32_t sC;
            auto hasS = safeDecodeNext(sI, str.end(), sC);
            char32_t cC;
            auto hasC = safeDecodeNext(cI, cmp.end(), cC);
            if (!hasS || !hasC)
                return hasS == hasC;

            if (!caseInsensitiveEqual(sC, cC))
                return false;
        }
    }

    Subrange caseInsensitiveFindFirst(std::string_view str,
                                      std::string_view cmp,
                                      size_t offset)
    {
        auto itCmp = cmp.begin();
        char32_t chCmp;
        if (!safeDecodeNext(itCmp, cmp.end(), chCmp))
            return {};

        auto itStr = str.begin() + offset;
        auto itStrNext = itStr;
        char32_t chStr;
        while (safeDecodeNext(itStrNext, str.end(), chStr))
        {
            if (caseInsensitiveEqual(chStr, chCmp))
            {
                auto[a, b] = nextCaseInsensitiveMismatch(
                    itStrNext, str.end(), itCmp, cmp.end());
                if (b == cmp.end())
                    return Subrange(str.begin(), itStr, a);
            }
            itStr = itStrNext;
        }
        return {};
    }

    std::pair<Subrange, char32_t>
    caseInsensitiveFindFirstOf(std::string_view str,
                               Char32Span chars,
                               size_t offset)
    {
        return findFirstWhere(
            str,
            [&](auto c) {return caseInsensitiveContains(chars, c);},
            offset);
    }

    Subrange caseInsensitiveFindLast(std::string_view str,
                                     std::string_view cmp)
    {
        return caseInsensitiveFindLast(str, cmp, str.size());
    }

    Subrange caseInsensitiveFindLast(std::string_view str,
                                     std::string_view cmp,
                                     size_t offset)
    {
        auto itCmp = cmp.end();
        char32_t chCmp;
        if (!safeDecodePrev(cmp.begin(), itCmp, chCmp))
            return {};

        auto itStr = str.begin() + offset;
        auto itStrPrev = itStr;
        char32_t chStr;
        while (safeDecodePrev(str.begin(), itStrPrev, chStr))
        {
            if (caseInsensitiveEqual(chStr, chCmp))
            {
                auto[a, b] = prevCaseInsensitiveMismatch(
                    str.begin(), itStrPrev, cmp.begin(), itCmp);
                if (b == cmp.begin())
                    return Subrange(str.begin(), a, itStr);
            }
            itStr = itStrPrev;
        }
        return {};
    }

    std::pair<Subrange, char32_t>
    caseInsensitiveFindLastOf(std::string_view str, Char32Span chars)
    {
        return caseInsensitiveFindLastOf(str, chars, str.size());
    }

    std::pair<Subrange, char32_t>
    caseInsensitiveFindLastOf(std::string_view str,
                              Char32Span chars,
                              size_t offset)
    {
        return findLastWhere(
            str,
            [&](auto c) {return caseInsensitiveContains(chars, c);},
            offset);
    }

    bool caseInsensitiveLess(std::string_view str, std::string_view cmp)
    {
        auto sI = str.begin();
        auto cI = cmp.begin();
        while (true)
        {
            char32_t sC;
            auto hasS = safeDecodeNext(sI, str.end(), sC);
            char32_t cC;
            auto hasC = safeDecodeNext(cI, cmp.end(), cC);
            if (!hasS || !hasC)
                return !hasS && hasC;

            if (auto result = caseInsensitiveCompare(sC, cC))
                return result < 0;
        }
    }

    std::string caseInsensitiveReplace(std::string_view str,
                                       std::string_view from,
                                       std::string_view to,
                                       ptrdiff_t maxReplacements)
    {
        std::string result;
        if (maxReplacements >= 0)
        {
            while (maxReplacements-- > 0)
            {
                auto match = caseInsensitiveFindFirst(str, from);
                if (!match)
                    break;
                result.append(str.substr(0, match.start()));
                result.append(to);
                str = str.substr(match.end());
            }

            if (!str.empty())
                result.append(str);
        }
        else
        {
            std::vector<Subrange> matches;
            auto tmpStr = str;
            while (maxReplacements++ < 0)
            {
                auto match = caseInsensitiveFindLast(tmpStr, from);
                if (!match)
                    break;
                matches.push_back(match);
                tmpStr = tmpStr.substr(0, match.start());
            }

            size_t start = 0;
            for (auto it = matches.rbegin(); it != matches.rend(); ++it)
            {
                result.append(str.substr(start, it->start() - start));
                result.append(to);
                start = it->end();
            }
            if (start != str.size())
                result.append(str.substr(start));
        }

        return result;
    }

    bool caseInsensitiveStartsWith(std::string_view str, std::string_view cmp)
    {
        auto[itStr, itCmp] = nextCaseInsensitiveMismatch(
            str.begin(), str.end(), cmp.begin(), cmp.end());
        return itCmp == cmp.end();
    }

    bool contains(std::string_view str, char32_t chr)
    {
        auto it = str.begin(), end = str.end();
        char32_t ch;
        while (safeDecodeNext(it, end, ch))
        {
            if (ch == chr)
                return true;
        }
        return false;
    }

    size_t countCodeCharacters(std::string_view str)
    {
        size_t count = 0;
        size_t offset = 0;
        while (offset != str.size())
        {
            auto range = getNextCharacterRange(str, offset);
            offset = range.end();
            ++count;
        }
        return count;
    }

    size_t countCodePoints(std::string_view str)
    {
        auto it = str.begin();
        auto end = str.end();
        size_t result = 0;
        while (skipNext(it, end))
            ++result;
        return result;
    }

    bool endsWith(std::string_view str, std::string_view cmp)
    {
        return str.size() >= cmp.size()
               && str.substr(str.size() - cmp.size()) == cmp;
    }

    Subrange findFirst(std::string_view str,
                       std::string_view cmp,
                       size_t offset)
    {
        auto it = std::search(str.begin() + offset, str.end(), cmp.begin(), cmp.end());
        if (it == str.end())
            return {str.size(), 0};
        return {size_t(it - str.begin()), cmp.size()};
    }

    Subrange findFirstNewline(std::string_view str, size_t offset)
    {
        auto [s, c] = findFirstOf(str, NEWLINES, offset);
        if (c != '\r')
            return s;
        auto it = str.begin() + s.end();
        char32_t ch;
        if (safeDecodeNext(it, str.end(), ch) && ch == '\n')
            return {s.start(), (it - str.begin()) - s.start()};
        return s;
    }

    std::pair<Subrange, char32_t>
    findFirstOf(std::string_view str, Char32Span chars, size_t offset)
    {
        return findFirstWhere(str,
                              [&](auto c) {return contains(chars, c);},
                              offset);
    }

    Subrange findLast(std::string_view str, std::string_view cmp)
    {
        return findLast(str, cmp, str.size());
    }

    Subrange findLast(std::string_view str,
                      std::string_view cmp,
                      size_t offset)
    {
        auto its = searchLast(str.begin(), str.begin() + offset,
                              cmp.begin(), cmp.end());
        return {size_t(its.first - str.begin()),
                size_t(its.second - its.first)};
    }

    Subrange findLastNewline(std::string_view str)
    {
        return findLastNewline(str, str.size());
    }

    Subrange findLastNewline(std::string_view str, size_t offset)
    {
        auto[s, c] = findLastOf(str, NEWLINES, offset);
        if (c != '\n')
            return s;
        auto it = str.begin() + s.start();
        char32_t ch;
        if (safeDecodePrev(str.begin(), it, ch) && ch == '\r')
            return {str.begin(), it, str.begin() + s.end()};
        return s;
    }

    std::pair<Subrange, char32_t> findLastOf(std::string_view str,
                                             Char32Span chars)
    {
        return findLastOf(str, chars, str.size());
    }

    std::pair<Subrange, char32_t>
    findLastOf(std::string_view str, Char32Span chars, size_t offset)
    {
        return findLastWhere(
            str,
            [&](auto c) {return contains(chars, c);},
            offset);
    }

    size_t getCharacterPos(std::string_view str, ptrdiff_t pos)
    {
        size_t offset;
        if (pos >= 0)
        {
            offset = 0;
            while (pos-- > 0)
            {
                auto range = getNextCharacterRange(str, offset);
                if (!range)
                    return std::string_view::npos;
                offset = range.end();
            }
        }
        else
        {
            offset = str.size();
            while (pos++ < 0)
            {
                auto range = getPrevCharacterRange(str, offset);
                if (!range)
                    return std::string_view::npos;
                offset = range.start();
            }
        }

        return offset;
    }

    Subrange getCharacterRange(std::string_view str, ptrdiff_t pos)
    {
        size_t offset;
        if (pos >= 0)
        {
            offset = getCharacterPos(str, pos);
            if (offset == std::string_view::npos)
                return {str.size(), 0};
            return getNextCharacterRange(str, offset);
        }
        else
        {
            if (pos < -1)
            {
                offset = getCharacterPos(str, pos + 1);
                if (offset == std::string_view::npos)
                    return {0, 0};
            }
            else
            {
                offset = str.size();
            }
            return getPrevCharacterRange(str, offset);
        }
    }

    std::string_view getCharacterSubstring(std::string_view str,
                                           ptrdiff_t startIndex,
                                           ptrdiff_t endIndex)
    {
        auto range = getCharacterSubstringRange(str, startIndex, endIndex);
        return str.substr(range.offset, range.length);
    }

    std::pair<Subrange, char32_t>
    getCodePoint(std::string_view str, ptrdiff_t pos)
    {
        if (pos >= 0)
        {
            auto it = str.begin();
            while (pos > 0 && skipNext(it, str.end()))
                --pos;
            char32_t ch;
            auto prev = it;
            if (safeDecodeNext(it, str.end(), ch))
                return {{str.begin(), prev, it}, ch};
            return {{str.size(), 0}, INVALID_CHAR};
        }
        else
        {
            auto it = str.end();
            while (pos < -1 && skipPrev(str.begin(), it))
                ++pos;
            char32_t ch;
            auto next = it;
            if (safeDecodePrev(str.begin(), it, ch))
                return {{str.begin(), it, next}, ch};
            return {{0, 0}, INVALID_CHAR};
        }
    }

    size_t getCodePointPos(std::string_view str, ptrdiff_t pos)
    {
        if (std::abs(pos) > ptrdiff_t(str.size()))
            return std::string_view::npos;
        if (pos >= 0)
        {
            auto it = str.begin();
            while (pos > 0 && skipNext(it, str.end()))
                --pos;
            if (pos == 0)
                return size_t(it - str.begin());
        }
        else
        {
            auto it = str.end();
            while (pos < 0 && skipPrev(str.begin(), it))
                ++pos;
            if (pos == 0)
                return size_t(it - str.begin());
        }
        return std::string_view::npos;
    }

    std::string_view getCodePointSubstring(std::string_view str,
                                           ptrdiff_t startIndex,
                                           ptrdiff_t endIndex)
    {
        auto range = getCodePointSubstringRange(str, startIndex, endIndex);
        return str.substr(range.offset, range.length);
    }

    Subrange getNextCharacterRange(std::string_view str, size_t offset)
    {
        auto it = str.begin() + offset;
        char32_t codePoint;
        if (!safeDecodeNext(it, str.end(), codePoint))
            return Subrange(offset, 0);

        auto end = it;
        while (safeDecodeNext(it, str.end(), codePoint)
               && isMark(codePoint))
        {
            end = it;
        }

        return Subrange(offset, (end - str.begin()) - offset);
    }

    Subrange getPrevCharacterRange(std::string_view str, size_t offset)
    {
        auto it = str.begin() + offset;
        char32_t codePoint;
        while (safeDecodePrev(str.begin(), it, codePoint)
               && isMark(codePoint))
        {
        }

        auto start = it - str.begin();
        return Subrange(start, offset - start);
    }

    std::string
    insertCharacter(std::string_view str, ptrdiff_t pos, char32_t chr)
    {
        return insertAtOffset(str, getCharacterPos(str, pos), chr);
    }

    std::string
    insertCharacters(std::string_view str, ptrdiff_t pos, std::string_view sub)
    {
        return insertAtOffset(str, getCharacterPos(str, pos), sub);
    }

    std::string
    insertCodePoint(std::string_view str, ptrdiff_t pos, char32_t codePoint)
    {
        return insertAtOffset(str, getCodePointPos(str, pos), codePoint);
    }

    std::string
    insertCodePoints(std::string_view str, ptrdiff_t pos,
                     std::string_view codePoints)
    {
        return insertAtOffset(str, getCodePointPos(str, pos), codePoints);
    }

    bool isValidUtf8(std::string_view str)
    {
        auto it = str.begin(), end = str.end();
        while (it != end)
        {
            if (decodeNext(it, end) == INVALID_CHAR)
                return false;
        }
        return true;
    }

    std::string replace(std::string_view str,
                        std::string_view from,
                        std::string_view to,
                        ptrdiff_t maxReplacements)
    {
        std::string result;
        if (maxReplacements >= 0)
        {
            auto it = str.begin();
            while (maxReplacements-- > 0)
            {
                auto match = std::search(it, str.end(), from.begin(), from.end());
                if (match == str.end())
                    break;
                result.append(it, match);
                result.append(to);
                it = match + from.size();
            }
            if (it != str.end())
                result.append(it, str.end());
        }
        else
        {
            size_t prev = 0;
            auto matches = findLastN(str, from, -maxReplacements);
            for (auto it = matches.rbegin(); it != matches.rend(); ++it)
            {
                if (it->offset != prev)
                    result.append(str.substr(prev, it->offset - prev));
                result.append(to);
                prev = it->end();
            }
            if (prev != str.size())
                result.append(str.substr(prev, str.size()));
        }
        return result;
    }

    std::string
    replaceCharacters(std::string_view str, ptrdiff_t start, ptrdiff_t end,
                      std::string_view repl)
    {
        return replaceSubrange(str,
                               getCharacterSubstringRange(str, start, end),
                               repl);
    }

    std::string
    replaceCodePoints(std::string_view str, ptrdiff_t start, ptrdiff_t end,
                      std::string_view repl)
    {
        return replaceSubrange(str,
                               getCodePointSubstringRange(str, start, end),
                               repl);
    }

    std::string
    replaceCodePoint(std::string_view str, char32_t from, char32_t to,
                     ptrdiff_t maxReplacements)
    {
        char f[4], t[4];
        auto fSize = encodeUtf8(from, f, 4);
        auto tSize = encodeUtf8(to, t, 4);
        if (!fSize)
            YSTRING_THROW("Invalid from-code point: "
                          + std::to_string(uint32_t(from)));
        if (!tSize)
            YSTRING_THROW("Invalid to-code point: "
                          + std::to_string(uint32_t(to)));
        return replace(str, std::string_view(f, fSize),
                       std::string_view(t, tSize), maxReplacements);
    }

    std::string replaceInvalidUtf8(std::string_view str, char32_t chr)
    {
        char repl[4];
        auto replSize = encodeUtf8(chr, repl, 4);
        std::string result;
        auto it = str.begin();
        auto prev = it;
        auto end = str.end();
        while (it != end)
        {
            if (decodeNext(it, end) != INVALID_CHAR)
                continue;
            result.append(prev, it);
            result.append(repl, replSize);
            skipNext(it, end);
            prev = it;
        }
        result.append(prev, end);
        return result;
    }

    std::string& replaceInvalidUtf8(std::string& str, char32_t chr)
    {
        if (!isValidUtf8(str))
            str = replaceInvalidUtf8(std::string_view(str), chr);
        return str;
    }

    std::string reverse(std::string_view str)
    {
        std::string result;
        size_t offset = str.size();
        while (offset != 0)
        {
            auto range = getPrevCharacterRange(str, offset);
            offset = range.start();
            result.insert(result.end(),
                          str.begin() + range.start(),
                          str.begin() + range.end());
        }
        return result;
    }

    std::vector<std::string_view>
    split(std::string_view str, Char32Span chars, SplitParams params)
    {
        if (params.caseInsensitive)
        {
            return splitWhere(
                str,
                [&](auto s) {return caseInsensitiveFindFirstOf(s, chars).first;},
                params);
        }
        else
        {
            return splitWhere(
                str,
                [&](auto s) {return findFirstOf(s, chars).first;},
                params);
        }
    }

    std::vector<std::string_view>
    split(std::string_view str, std::string_view sep, SplitParams params)
    {
        if (params.caseInsensitive)
        {
            return splitWhere(
                str,
                [&](auto s) {return caseInsensitiveFindFirst(s, sep);},
                params);
        }
        else
        {
            return splitWhere(
                str,
                [&](auto s) {return findFirst(s, sep);},
                params);
        }
    }

    std::vector<std::string_view>
    splitLines(std::string_view str, SplitParams params)
    {
        return splitWhere(str,
                          [&](auto s) {return findFirstNewline(s);},
                          params);
    }

    bool startsWith(std::string_view str, std::string_view cmp)
    {
        return cmp.size() <= str.size() && str.substr(0, cmp.size()) == cmp;
    }

    std::string_view trim(std::string_view str, Char32Span chars)
    {
        return trimEnd(trimStart(str, chars), chars);
    }

    std::string_view trimEnd(std::string_view str, Char32Span chars)
    {
        return trimEndWhere(str, [&](auto c) {return contains(chars, c);});
    }

    std::string_view trimStart(std::string_view str, Char32Span chars)
    {
        return trimStartWhere(str, [&](auto c) {return contains(chars, c);});
    }
}
