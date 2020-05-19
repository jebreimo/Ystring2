//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-05-31.
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Utf/Utf8String.hpp"

#include "Ystring/Encodings/DecodeUtf8.hpp"
#include "Ystring/Encodings/EncodeUtf8.hpp"

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

        template <typename BiIt, typename FwdIt>
        std::pair<BiIt, BiIt> searchLast(BiIt beg, BiIt end,
                                         FwdIt cmpBeg, FwdIt cmpEnd)
        {
            auto n = std::distance(cmpBeg, cmpEnd);
            if (n == 0 || std::distance(beg, end) < n)
                return std::make_pair(beg, beg);
            BiIt it = std::prev(end, n);
            do
            {
                auto its = std::mismatch(it, end, cmpBeg, cmpEnd);
                if (its.second == cmpEnd)
                    return std::make_pair(it, its.first);
            } while (it-- != beg);
            return std::make_pair(beg, beg);
        }

        std::vector<Subrange> findLastN(
            std::string_view str, std::string_view cmp,
            size_t maxCount)
        {
            std::vector<Subrange> result;
            auto begin = str.begin();
            auto it = str.end();
            while (true)
            {
                auto match = searchLast(begin, it, cmp.begin(), cmp.end());
                if (match.first == match.second)
                    break;
                result.emplace_back(match.first - begin, cmp.size());
                if (--maxCount == 0)
                    break;
                it = match.first;
            }
            return result;
        }
    }

    std::string& append(std::string& str, char32_t chr)
    {
        if (encodeUtf8(std::back_inserter(str), chr))
            return str;
        YSTRING_THROW("Invalid code point: " + std::to_string(size_t(chr)));
    }

    std::string append(std::string_view str, char32_t chr)
    {
        std::string result(str);
        append(result, chr);
        return result;
    }

    bool contains(std::string_view str, char32_t chr)
    {
        auto it = str.begin(), end = str.end();
        char32_t ch;
        while (safeNextUtf8Value(it, end, ch))
        {
            if (ch == chr)
                return true;
        }
        return false;
    }

    size_t countCodePoints(std::string_view str)
    {
        auto it = str.begin();
        auto end = str.end();
        size_t result = 0;
        while (skipNextUtf8Value(it, end))
            ++result;
        return result;
    }

    bool endsWith(std::string_view str, std::string_view cmp)
    {
        return str.size() >= cmp.size()
               && str.substr(str.size() - cmp.size()) == cmp;
    }

    Subrange findFirst(std::string_view str, std::string_view cmp)
    {
        auto it = std::search(str.begin(), str.end(), cmp.begin(), cmp.end());
        if (it == str.end())
            return {};
        return {size_t(it - str.begin()), cmp.size()};
    }

    Subrange findFirstNewline(std::string_view str)
    {
        auto [s, c] = findFirstOf(str, NEWLINES, sizeof(NEWLINES) / 4);
        if (c != '\r')
            return s;
        auto it = str.begin() + s.end();
        char32_t ch;
        if (safeNextUtf8Value(it, str.end(), ch) && ch == '\n')
            return {s.start(), (it - str.begin()) - s.start()};
        return s;
    }

    std::pair<Subrange, char32_t>
    findFirstOf(std::string_view str, const char32_t* chars, size_t numChars)
    {
        auto it = str.begin(), end = str.end(), prev = str.begin();
        char32_t ch;
        auto charsEnd = chars + numChars;
        while (safeNextUtf8Value(it, end, ch))
        {
            if (std::find(chars, charsEnd, ch) != charsEnd)
                return {{size_t(prev - str.begin()), size_t(it - prev)}, ch};
            prev = it;
        }
        return {{}, INVALID};
    }

    Subrange findLast(std::string_view str, std::string_view cmp)
    {
        auto its = searchLast(str.begin(), str.end(), cmp.begin(), cmp.end());
        return {size_t(its.first - str.begin()),
                size_t(its.second - its.first)};
    }

    Subrange findLastNewline(std::string_view str)
    {
        auto[s, c] = findLastOf(str, NEWLINES, sizeof(NEWLINES) / 4);
        if (c != '\n')
            return s;
        auto it = str.begin() + s.start();
        char32_t ch;
        if (safePrevUtf8Value(str.begin(), it, ch) && ch == '\r')
            return {size_t(it - str.begin()), s.end() - size_t(it - str.begin())};
        return s;
    }

    std::pair<Subrange, char32_t>
    findLastOf(std::string_view str, const char32_t* chars, size_t numChars)
    {
        auto begin = str.begin(), it = str.end(), next = str.end();
        char32_t ch;
        auto charsEnd = chars + numChars;
        while (safePrevUtf8Value(begin, it, ch))
        {
            if (std::find(chars, charsEnd, ch) != charsEnd)
                return {{size_t(it - str.begin()), size_t(next - it)}, ch};
            next = it;
        }
        return {{}, INVALID};
    }

    std::pair<Subrange, char32_t> getCodePoint(std::string_view str, ptrdiff_t pos)
    {
        if (pos >= 0)
        {
            auto it = str.begin();
            while (pos > 0 && skipNextUtf8Value(it, str.end()))
                --pos;
            char32_t ch;
            auto prev = it;
            if (safeNextUtf8Value(it, str.end(), ch))
                return {{size_t(prev - str.begin()), size_t(it - prev)}, ch};
        }
        else
        {
            auto it = str.end();
            while (pos < -1 && skipPrevUtf8Value(str.begin(), it))
                ++pos;
            char32_t ch;
            auto next = it;
            if (safePrevUtf8Value(str.begin(), it, ch))
                return {{size_t(it - str.begin()), size_t(next - it)}, ch};
        }
        return {{}, INVALID};
    }

    size_t getCodePointPos(std::string_view str, ptrdiff_t pos)
    {
        if (pos >= 0)
        {
            auto it = str.begin();
            while (pos > 0 && skipNextUtf8Value(it, str.end()))
                --pos;
            if (pos == 0)
                return size_t(it - str.begin());
        }
        else
        {
            auto it = str.end();
            while (pos < 0 && skipPrevUtf8Value(str.begin(), it))
                ++pos;
            if (pos == 0)
                return size_t(it - str.begin());
        }
        return std::string_view::npos;
    }

    size_t getClampedCodePointPos(std::string_view str, ptrdiff_t pos)
    {
        if (auto p = getCodePointPos(str, pos); p != std::string_view::npos)
            return p;
        return pos > 0 ? str.size() : 0;
    }

    std::string insertCodePoint(std::string_view str, ptrdiff_t pos, char32_t codePoint)
    {
        auto strpos = getCodePointPos(str, pos);
        if (strpos == std::string_view::npos)
            YSTRING_THROW("string pos is out of bounds: "
                          + std::to_string(pos));
        std::string result(str.substr(0, strpos));
        encodeUtf8(std::back_inserter(result), codePoint);
        result.append(str.substr(strpos));
        return result;
    }

    std::string insertCodePoints(std::string_view str, ptrdiff_t pos, std::string_view codePoints)
    {
        if (codePoints.empty())
            return std::string(str);
        auto strpos = getCodePointPos(str, pos);
        if (strpos == std::string_view::npos)
            YSTRING_THROW("string pos is out of bounds: "
                          + std::to_string(pos));
        std::string result(str.substr(0, strpos));
        result.append(codePoints);
        result.append(str.substr(strpos));
        return result;
    }

    bool isValidUtf8(std::string_view str)
    {
        auto it = str.begin(), end = str.end();
        while (it != end)
        {
            if (nextUtf8Value(it, end) == INVALID)
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
            auto prev = 0;
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
    replaceCodePoints(std::string_view str,
                      ptrdiff_t start,
                      ptrdiff_t end,
                      std::string_view repl)
    {
        auto s = getClampedCodePointPos(str, start);
        auto e = getClampedCodePointPos(str, end);
        std::string result;
        result.append(str.substr(0, s));
        result.append(repl);
        result.append(str.substr(e));
        return result;
    }

    std::string
    replaceCodePoint(std::string_view str, char32_t from, char32_t to,
                     ptrdiff_t maxReplacements)
    {
        char f[4], t[4];
        auto fSize = encodeUtf8(f, 4, from);
        auto tSize = encodeUtf8(t, 4, to);
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
        auto replSize = encodeUtf8(repl, 4, chr);
        std::string result;
        auto it = str.begin();
        auto prev = it;
        auto end = str.end();
        while (it != end)
        {
            if (nextUtf8Value(it, end) != INVALID)
                continue;
            result.append(prev, it);
            result.append(repl, replSize);
            skipNextUtf8Value(it, end);
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
}
