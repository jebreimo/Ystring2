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

namespace ystring
{
    template <typename BiIt, typename FwdIt>
    [[nodiscard]]
    std::pair<BiIt, BiIt> search_last(BiIt beg, BiIt end,
                                      FwdIt cmpBeg, FwdIt cmpEnd)
    {
        auto n = std::distance(cmpBeg, cmpEnd);
        if (n == 0 || std::distance(beg, end) < n)
          return {beg, beg};
        BiIt it = std::prev(end, n);
        while (true)
        {
            auto its = std::mismatch(it, end, cmpBeg, cmpEnd);
            if (its.second == cmpEnd)
                return {it, its.first};
            if (it == beg)
                break;
            --it;
        }
        return {beg, beg};
    }

    [[nodiscard]]
    std::vector<Subrange> find_last_n(
        std::string_view str, std::string_view cmp,
        size_t max_count);

    template <typename It>
    [[nodiscard]]
    std::pair<It, It>
    next_case_insensitive_mismatch(It beg1, It end1, It beg2, It end2)
    {
        while (true)
        {
            char32_t ch1;
            auto it1 = beg1;
            auto has1 = safe_decode_next(it1, end1, ch1);
            if (!has1)
                return {beg1, beg2};

            char32_t ch2;
            auto it2 = beg2;
            auto has2 = safe_decode_next(it2, end2, ch2);
            if (!has2 || !case_insensitive_equal(ch1, ch2))
                return {beg1, beg2};

            beg1 = it1;
            beg2 = it2;
        }
    }

    template <typename It>
    [[nodiscard]]
    std::pair<It, It>
    prev_case_insensitive_mismatch(It beg1, It end1, It beg2, It end2)
    {
        while (true)
        {
            char32_t ch1;
            auto it1 = end1;
            auto has1 = safe_decode_prev(beg1, it1, ch1);
            if (!has1)
                return {end1, end2};

            char32_t ch2;
            auto it2 = end2;
            auto has2 = safe_decode_prev(beg2, it2, ch2);
            if (!has2 || !case_insensitive_equal(ch1, ch2))
                return {end1, end2};

            end1 = it1;
            end2 = it2;
        }
    }

    [[nodiscard]]
    size_t get_capped_character_pos(std::string_view str, ptrdiff_t pos);

    [[nodiscard]]
    size_t get_capped_code_point_pos(std::string_view str, ptrdiff_t pos);

    [[nodiscard]]
    Subrange get_character_substring_range(std::string_view str,
                                           ptrdiff_t start_index,
                                           ptrdiff_t end_index);

    [[nodiscard]]
    Subrange get_code_point_substring_range(std::string_view str,
                                            ptrdiff_t start_index,
                                            ptrdiff_t end_index);

    [[nodiscard]]
    std::string insert_at_offset(std::string_view str, size_t offset,
                                 std::string_view code_points);

    [[nodiscard]]
    std::string
    insert_at_offset(std::string_view str, size_t offset, char32_t code_point);

    [[nodiscard]]
    std::string replace_subrange(std::string_view str, Subrange range,
                                 std::string_view repl);
}
