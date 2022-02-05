//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-08-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "AlgorithmUtilities.hpp"

namespace ystring
{
    std::vector<Subrange> find_last_n(
        std::string_view str, std::string_view cmp,
        size_t max_count)
    {
        std::vector<Subrange> result;
        auto begin = str.begin();
        auto it = str.end();
        while (true)
        {
            auto match = search_last(begin, it, cmp.begin(), cmp.end());
            if (match.first == match.second)
                break;
            result.emplace_back(match.first - begin, cmp.size());
            if (--max_count == 0)
                break;
            it = match.first;
        }
        return result;
    }

    size_t get_capped_character_pos(std::string_view str, ptrdiff_t pos)
    {
        if (auto p = get_character_pos(str, pos); p != std::string_view::npos)
            return p;
        return pos > 0 ? str.size() : 0;
    }

    size_t get_capped_code_point_pos(std::string_view str, ptrdiff_t pos)
    {
        if (auto p = get_code_point_pos(str, pos); p != std::string_view::npos)
            return p;
        return pos > 0 ? str.size() : 0;
    }

    Subrange get_character_substring_range(std::string_view str,
                                           ptrdiff_t start_index,
                                           ptrdiff_t end_index)
    {
        if (end_index < start_index && (start_index >= 0) == (end_index >= 0))
            end_index = start_index;
        if (start_index >= 0 && end_index >= 0)
        {
            auto s = get_capped_character_pos(str, start_index);
            auto e = get_capped_character_pos(str.substr(s), end_index - start_index);
            return {s, e};
        }
        else if (start_index < 0 && end_index < 0)
        {
            auto e = get_capped_character_pos(str, end_index);
            auto s = get_capped_character_pos(str.substr(0, e), start_index - end_index);
            return {s, e - s};
        }
        else if (start_index >= 0 && end_index < 0)
        {
            auto s = get_capped_character_pos(str, start_index);
            auto e = get_capped_character_pos(str.substr(s), end_index);
            return {s, e - s};
        }
        else
        {
            auto s = get_capped_character_pos(str, start_index);
            auto e = get_capped_character_pos(str, end_index);
            return {s, e > s ? e - s : 0};
        }
    }

    Subrange get_code_point_substring_range(std::string_view str,
                                            ptrdiff_t start_index,
                                            ptrdiff_t end_index)
    {
        if (end_index < start_index && (start_index >= 0) == (end_index >= 0))
            end_index = start_index;
        if (start_index >= 0 && end_index >= 0)
        {
            auto s = get_capped_code_point_pos(str, start_index);
            auto e = get_capped_code_point_pos(str.substr(s), end_index - start_index);
            return {s, e};
        }
        else if (start_index < 0 && end_index < 0)
        {
            auto e = get_capped_code_point_pos(str, end_index);
            auto s = get_capped_code_point_pos(str.substr(0, e), start_index - end_index);
            return {s, e - s};
        }
        else if (start_index >= 0 && end_index < 0)
        {
            auto s = get_capped_code_point_pos(str, start_index);
            auto e = get_capped_code_point_pos(str.substr(s), end_index);
            return {s, e};
        }
        else
        {
            auto s = get_capped_code_point_pos(str, start_index);
            auto e = get_capped_code_point_pos(str, end_index);
            return {s, std::max(s, e)};
        }
    }

    std::string insert_at_offset(std::string_view str, size_t offset, std::string_view code_points)
    {
        if (code_points.empty())
            return std::string(str);
        if (offset == std::string_view::npos)
            YSTRING_THROW("string position is out of bounds");
        std::string result(str.substr(0, offset));
        result.append(code_points);
        result.append(str.substr(offset));
        return result;
    }

    std::string
    insert_at_offset(std::string_view str, size_t offset, char32_t code_point)
    {
        if (offset == std::string_view::npos)
            YSTRING_THROW("string position is out of bounds");
        std::string result(str.substr(0, offset));
        append(result, code_point);
        result.append(str.substr(offset));
        return result;
    }

    std::string
    replace_subrange(std::string_view str, Subrange range, std::string_view repl)
    {
        std::string result;
        result.append(str.substr(0, range.start()));
        result.append(repl);
        result.append(str.substr(range.end()));
        return result;
    }
}
