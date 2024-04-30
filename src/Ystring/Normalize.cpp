//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-08-15.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Normalize.hpp"

#include <algorithm>
#include <vector>
#include "NormalizationTables.hpp"
#include "Ystring/DecodeUtf8.hpp"
#include "EncodeUtf8.hpp"

namespace ystring
{
    namespace
    {
        std::pair<char32_t, char32_t> find_decomposed(char32_t ch)
        {
            if (ch < 0xC0)
                return {ch, 0};
            auto it = std::lower_bound(std::begin(TO_DECOMPOSED),
                                       std::end(TO_DECOMPOSED),
                                       uint64_t(ch) << 40u);
            if (it == std::end(TO_DECOMPOSED) || char32_t(*it >> 40u) != ch)
                return {ch, 0};
            return {char32_t((*it >> 20u) & 0xFFFFFu),
                    char32_t(*it & 0xFFFFFu)};
        }

        char32_t find_composed(char32_t ch, char32_t mark)
        {
            if (mark < 0x300)
                return 0;
            auto key = (uint64_t(ch) << 20u) | uint64_t(mark);
            auto it = std::lower_bound(std::begin(TO_COMPOSED),
                                       std::end(TO_COMPOSED),
                                       key << 20u);
            if (it == std::end(TO_DECOMPOSED) || char32_t(*it >> 20u) != key)
                return 0;
            return char32_t(*it & 0xFFFFFu);
        }

        size_t get_decomposed_size(char32_t ch)
        {
            size_t n = 1;
            auto [norm, mark] = find_decomposed(ch);
            while (mark)
            {
                ++n;
                std::tie(norm, mark) = find_decomposed(mark);
            }
            return n;
        }

        std::pair<bool, size_t>
        decompose_char(char32_t ch, char32_t* str, size_t str_size)
        {
            if (!str_size)
                return {false, get_decomposed_size(ch)};

            auto [norm, mark] = find_decomposed(ch);
            if (mark == 0)
            {
                str[0] = norm;
                return {true, 1};
            }

            if (str_size < 2)
                return {false, get_decomposed_size(ch)};

            // Optimize for the common case where the character can only
            // be decomposed once.
            auto mark2 = mark;
            std::tie(norm, mark) = find_decomposed(norm);
            if (mark == 0)
            {
                str[0] = norm;
                str[1] = mark2;
                return {true, 2};
            }

            // The character can be decomposed more than once.
            size_t i = 0;
            str[i++] = mark2;
            while (mark)
            {
                if (i + 1 == str_size)
                    return {false, get_decomposed_size(ch)};
                str[i++] = mark;
                std::tie(norm, mark) = find_decomposed(norm);
            }
            str[i++] = norm;

            std::reverse(str, str + i);

            return {true, i};
        }
    }

    std::string to_composed(std::string_view str)
    {
        auto from = str.begin(), it = from, to = from;
        char32_t ch;

        if (!safe_decode_next(it, str.end(), ch))
            return {};

        auto prev = it;
        std::string result;
        char32_t mark;
        while (safe_decode_next(it, str.end(), mark))
        {
            auto denorm = find_composed(ch, mark);
            if (denorm == 0)
            {
                to = prev;
                prev = it;
            }
            else
            {
                result.append(from, to);

                ch = denorm;
                from = prev = to = it;
                while (safe_decode_next(it, str.end(), mark))
                {
                    denorm = find_composed(ch, mark);
                    if (denorm == 0)
                        break;

                    ch = denorm;
                    from = prev = to = it;
                }
                auto out = std::back_inserter(result);
                encode_utf8(ch, out);
            }
            ch = mark;
        }
        result.append(from, prev);
        return result;
    }

    std::string to_decomposed(std::string_view str)
    {
        std::string result;
        // A buffer for decomposed characters. The size is set to
        // what should cover most cases, and at the same time stay within
        // the upper limit for small string optimizations on most platforms.
        std::u32string buffer(3, char32_t{});
        auto from = str.begin(), it = from, to = from;
        char32_t ch;
        while (safe_decode_next(it, str.end(), ch))
        {
            auto [ok, size] = decompose_char(ch, buffer.data(), buffer.size());
            while (!ok)
            {
                buffer.resize(size);
                std::tie(ok, size) = decompose_char(ch, buffer.data(), buffer.size());
            }

            if (size == 1)
            {
                // There's no point in re-encoding the character. We will
                // just copy it from the input string.
                to = it;
            }
            else
            {
                result.append(from, to);
                auto out = std::back_inserter(result);
                std::for_each(buffer.data(), buffer.data() + size,
                              [&](char32_t m) { encode_utf8(m, out); });
                from = to = it;
            }
        }
        result.append(from, to);
        return result;
    }

    std::u32string decompose(char32_t ch)
    {
        std::u32string str(3, char32_t{});
        auto [ok, n] = decompose_char(ch, str.data(), str.size());
        str.resize(n);
        if (!ok)
            std::tie(ok, n) = decompose_char(ch, str.data(), str.size());
        return str;
    }
}
