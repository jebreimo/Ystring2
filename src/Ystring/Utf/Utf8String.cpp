//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-05-31.
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Utf/Utf8String.hpp"

#include "Ystring/YstringThrow.hpp"
#include "Ystring/Encodings/DecodeUtf8.hpp"
#include "Ystring/Encodings/EncodeUtf8.hpp"

namespace Ystring
{
    namespace
    {
        template <typename It>
        bool safeNextUtf8Value(It& it, It end, char32_t& ch)
        {
            if (it == end)
                return false;
            ch = nextUtf8Value(it, end);
            if (ch == INVALID)
                YSTRING_THROW("Invalid UTF-8 string.");
            return true;
        }

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
        while (it != end)
        {
            skipNextUtf8Value(it, end);
            ++result;
        }
        return result;
    }

    bool endsWith(std::string_view str, std::string_view cmp)
    {
        return str.size() >= cmp.size()
               && str.substr(str.size() - cmp.size()) == cmp;
    }

    std::string_view findFirst(std::string_view str, std::string_view cmp)
    {
        auto it = std::search(str.begin(), str.end(), cmp.begin(), cmp.end());
        if (it == str.end())
            return {};
        return str.substr(it - str.begin(), cmp.size());
    }

    std::string_view findFirstNewline(std::string_view str)
    {
        auto it = str.begin(), end = str.end(), prev = str.begin();
        char32_t ch;
        while (safeNextUtf8Value(it, end, ch))
        {
            switch (ch)
            {
            case '\n':
            case '\v':
            case '\f':
            case NEXT_LINE:
            case LINE_SEPARATOR:
            case PARAGRAPH_SEPARATOR:
                return str.substr(prev - str.begin(), it - prev);
            case '\r':
                {
                    auto tmp = it;
                    if (safeNextUtf8Value(it, end, ch) && ch == '\n')
                        return str.substr(prev - str.begin(), it - prev);
                    else
                        return str.substr(prev - str.begin(), tmp - prev);
                }
            default:
                break;
            }
            prev = it;
        }
        return {};
    }

    std::string_view findLast(std::string_view str, std::string_view cmp)
    {
        auto its = searchLast(str.begin(), str.end(), cmp.begin(), cmp.end());
        return {str.data() + std::distance(str.begin(), its.first),
                size_t(std::distance(its.first, its.second))};
    }
}
