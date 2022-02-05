//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-08-15.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <optional>
#include <vector>
#include "Ystring/Normalize.hpp"
#include "NormalizationTables.hpp"
#include "Ystring/DecodeUtf8.hpp"
#include "EncodeUtf8.hpp"

namespace ystring
{
    namespace
    {
        std::pair<char32_t, char32_t> find_normalized(char32_t ch)
        {
            if (ch < 0xC1)
                return {ch, 0};
            auto it = std::lower_bound(std::begin(TO_NORMALIZED),
                                       std::end(TO_NORMALIZED),
                                       uint64_t(ch) << 40u);
            if (it == std::end(TO_NORMALIZED) || char32_t(*it >> 40u) != ch)
                return {ch, 0};
            return {char32_t((*it >> 20u) & 0xFFFFFu),
                    char32_t(*it & 0xFFFFFu)};
        }

        char32_t find_denormalized(char32_t ch, char32_t mark)
        {
            if (mark < 0x300)
                return 0;
            auto key = (uint64_t(ch) << 20u) | uint64_t(mark);
            auto it = std::lower_bound(std::begin(TO_DENORMALIZED),
                                       std::end(TO_DENORMALIZED),
                                       key << 20u);
            if (it == std::end(TO_NORMALIZED) || char32_t(*it >> 20u) != key)
                return 0;
            return char32_t(*it & 0xFFFFFu);
        }
    }

    std::string denormalize(std::string_view str)
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
            auto denorm = find_denormalized(ch, mark);
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
                    denorm = find_denormalized(ch, mark);
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

    std::string normalize(std::string_view str)
    {
        std::string result;
        std::vector<char32_t> marks;
        auto from = str.begin(), it = from, to = from;
        char32_t ch;
        while (safe_decode_next(it, str.end(), ch))
        {
            char32_t mark;
            std::tie(ch, mark) = find_normalized(ch);
            if (mark == 0)
            {
                to = it;
            }
            else
            {
                marks.clear();
                while (mark)
                {
                    marks.push_back(mark);
                    std::tie(ch, mark) = find_normalized(ch);
                }
                result.append(from, to);

                auto out = std::back_inserter(result);
                encode_utf8(ch, out);
                for (auto j = marks.rbegin(); j != marks.rend(); ++j)
                    encode_utf8(*j, out);

                from = to = it;
            }
        }
        result.append(from, to);
        return result;
    }
}
