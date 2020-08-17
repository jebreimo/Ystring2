//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-08-17.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <string_view>
#include "Ystring/Algorithms.hpp"

namespace Ystring
{
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

    inline std::vector<Subrange> findLastN(
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

    template <typename It>
    std::pair<It, It>
    nextCaseInsensitiveMismatch(It beg1, It end1, It beg2, It end2)
    {
        while (true)
        {
            char32_t ch1;
            auto it1 = beg1;
            auto has1 = safeDecodeNext(it1, end1, ch1);
            if (!has1)
                return {beg1, beg2};

            char32_t ch2;
            auto it2 = beg2;
            auto has2 = safeDecodeNext(it2, end2, ch2);
            if (!has2 || !caseInsensitiveEqual(ch1, ch2))
                return {beg1, beg2};

            beg1 = it1;
            beg2 = it2;
        }
    }

    template <typename It>
    std::pair<It, It>
    prevCaseInsensitiveMismatch(It beg1, It end1, It beg2, It end2)
    {
        while (true)
        {
            char32_t ch1;
            auto it1 = end1;
            auto has1 = safeDecodePrev(beg1, it1, ch1);
            if (!has1)
                return {end1, end2};

            char32_t ch2;
            auto it2 = end2;
            auto has2 = safeDecodePrev(beg2, it2, ch2);
            if (!has2 || !caseInsensitiveEqual(ch1, ch2))
                return {end1, end2};

            end1 = it1;
            end2 = it2;
        }
    }

    [[nodiscard]]
    inline size_t
    getClampedCodePointPos(std::string_view str, ptrdiff_t pos)
    {
        if (auto p = getCodePointPos(str, pos); p != std::string_view::npos)
            return p;
        return pos > 0 ? str.size() : 0;
    }
}
