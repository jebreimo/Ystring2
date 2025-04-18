//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-05-31.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Algorithms.hpp"

#include "Ystring/DecodeUtf8.hpp"
#include "EncodeUtf8.hpp"
#include "Ystring/CaseInsensitive.hpp"
#include "Ystring/CodepointPredicates.hpp"
#include "AlgorithmUtilities.hpp"

namespace ystring
{
    namespace
    {
        constexpr char32_t NEWLINES[] = {
            '\n',
            '\v',
            '\f',
            '\r',
            NEXT_LINE,
            LINE_SEPARATOR,
            PARAGRAPH_SEPARATOR
        };
    }

    std::string& append(std::string& str, char32_t chr)
    {
        if (encode_utf8(chr, std::back_inserter(str)))
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
        while (safe_decode_next(it, end, ch))
        {
            if (ch == chr)
                return true;
        }
        return false;
    }

    size_t count_chars(std::string_view str)
    {
        size_t count = 0;
        size_t offset = 0;
        while (offset != str.size())
        {
            auto range = get_next_char_range(str, offset);
            offset = range.end();
            ++count;
        }
        return count;
    }

    size_t count_codepoints(std::string_view str)
    {
        auto it = str.begin();
        auto end = str.end();
        size_t result = 0;
        while (skip_next(it, end))
            ++result;
        return result;
    }

    bool ends_with(std::string_view str, std::string_view cmp)
    {
        return str.size() >= cmp.size()
               && str.substr(str.size() - cmp.size()) == cmp;
    }

    Subrange find_first(std::string_view str,
                        std::string_view cmp,
                        size_t offset)
    {
        auto it = std::search(str.begin() + offset, str.end(),
                              cmp.begin(), cmp.end());
        if (it == str.end())
            return {str.size(), 0};
        return {size_t(it - str.begin()), cmp.size()};
    }

    Subrange find_first_newline(std::string_view str, size_t offset)
    {
        auto [s, c] = find_first_of(str, NEWLINES, offset);
        if (c != '\r')
            return s;
        auto it = str.begin() + s.end();
        char32_t ch;
        if (safe_decode_next(it, str.end(), ch) && ch == '\n')
            return {s.start(), (it - str.begin()) - s.start()};
        return s;
    }

    std::pair<Subrange, char32_t>
    find_first_of(std::string_view str, std::u32string_view chars, size_t offset)
    {
        return find_first_where(str,
                                [&](auto c) {return contains(chars, c);},
                                offset);
    }

    std::pair<Subrange, char32_t>
    find_first_of(std::string_view str, CodepointSet chars, size_t offset)
    {
        return find_first_where(str,
                                [&](auto c) {return chars.contains(c);},
                                offset);
    }

    Subrange find_last(std::string_view str,
                       std::string_view cmp,
                       size_t offset)
    {
        offset = std::min(offset, str.size());
        auto its = search_last(str.begin(), str.begin() + offset,
                               cmp.begin(), cmp.end());
        return {size_t(its.first - str.begin()),
                size_t(its.second - its.first)};
    }

    Subrange find_last_newline(std::string_view str, size_t offset)
    {
        auto[s, c] = find_last_of(str, NEWLINES, offset);
        if (c != '\n')
            return s;
        auto it = str.begin() + s.start();
        char32_t ch;
        if (safe_decode_prev(str.begin(), it, ch) && ch == '\r')
            return {str.begin(), it, str.begin() + s.end()};
        return s;
    }

    std::pair<Subrange, char32_t>
    find_last_of(std::string_view str, std::u32string_view chars, size_t offset)
    {
        return find_last_where(
            str,
            [&](auto c) {return contains(chars, c);},
            offset);
    }

    std::pair<Subrange, char32_t>
    find_last_of(std::string_view str, CodepointSet chars, size_t offset)
    {
        return find_last_where(
            str,
            [&](auto c) {return chars.contains(c);},
            offset);
    }

    size_t get_char_pos(std::string_view str, ptrdiff_t pos)
    {
        size_t offset;
        if (pos >= 0)
        {
            offset = 0;
            while (pos-- > 0)
            {
                auto range = get_next_char_range(str, offset);
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
                auto range = get_prev_char_range(str, offset);
                if (!range)
                    return std::string_view::npos;
                offset = range.start();
            }
        }

        return offset;
    }

    Subrange get_char_range(std::string_view str, ptrdiff_t pos)
    {
        size_t offset;
        if (pos >= 0)
        {
            offset = get_char_pos(str, pos);
            if (offset == std::string_view::npos)
                return {str.size(), 0};
            return get_next_char_range(str, offset);
        }
        else
        {
            if (pos < -1)
            {
                offset = get_char_pos(str, pos + 1);
                if (offset == std::string_view::npos)
                    return {0, 0};
            }
            else
            {
                offset = str.size();
            }
            return get_prev_char_range(str, offset);
        }
    }

    std::string_view get_char_substring(std::string_view str,
                                        ptrdiff_t start_index,
                                        ptrdiff_t end_index)
    {
        auto range = get_char_substring_range(str, start_index, end_index);
        return str.substr(range.offset, range.length);
    }

    std::pair<Subrange, char32_t>
    get_codepoint(std::string_view str, ptrdiff_t pos)
    {
        if (pos >= 0)
        {
            auto it = str.begin();
            while (pos > 0 && skip_next(it, str.end()))
                --pos;
            char32_t ch;
            auto prev = it;
            if (safe_decode_next(it, str.end(), ch))
                return {{str.begin(), prev, it}, ch};
            return {{str.size(), 0}, INVALID_CHAR};
        }
        else
        {
            auto it = str.end();
            while (pos < -1 && skip_prev(str.begin(), it))
                ++pos;
            char32_t ch;
            auto next = it;
            if (safe_decode_prev(str.begin(), it, ch))
                return {{str.begin(), it, next}, ch};
            return {{0, 0}, INVALID_CHAR};
        }
    }

    size_t get_codepoint_pos(std::string_view str, ptrdiff_t pos)
    {
        if (std::abs(pos) > ptrdiff_t(str.size()))
            return std::string_view::npos;
        if (pos >= 0)
        {
            auto it = str.begin();
            while (pos > 0 && skip_next(it, str.end()))
                --pos;
            if (pos == 0)
                return size_t(it - str.begin());
        }
        else
        {
            auto it = str.end();
            while (pos < 0 && skip_prev(str.begin(), it))
                ++pos;
            if (pos == 0)
                return size_t(it - str.begin());
        }
        return std::string_view::npos;
    }

    std::string_view get_codepoint_substring(std::string_view str,
                                             ptrdiff_t start_index,
                                             ptrdiff_t end_index)
    {
        auto range = get_codepoint_substring_range(str, start_index, end_index);
        return str.substr(range.offset, range.length);
    }

    Subrange get_next_char_range(std::string_view str, size_t offset)
    {
        auto it = str.begin() + offset;
        char32_t codepoint;
        if (!safe_decode_next(it, str.end(), codepoint))
            return {offset, 0};

        auto end = it;
        while (safe_decode_next(it, str.end(), codepoint)
               && is_mark(codepoint))
        {
            end = it;
        }

        return {offset, (end - str.begin()) - offset};
    }

    Subrange get_prev_char_range(std::string_view str, size_t offset)
    {
        auto it = str.begin() + offset;
        char32_t codepoint;
        while (safe_decode_prev(str.begin(), it, codepoint)
               && is_mark(codepoint))
        {
        }

        auto start = it - str.begin();
        return {size_t(start), offset - start};
    }

    std::string
    insert_char(std::string_view str, ptrdiff_t pos, char32_t chr)
    {
        return insert_at_offset(str, get_char_pos(str, pos), chr);
    }

    std::string
    insert_chars(std::string_view str, ptrdiff_t pos, std::string_view sub)
    {
        return insert_at_offset(str, get_char_pos(str, pos), sub);
    }

    std::string
    insert_codepoint(std::string_view str, ptrdiff_t pos, char32_t codepoint)
    {
        return insert_at_offset(str, get_codepoint_pos(str, pos), codepoint);
    }

    std::string
    insert_codepoints(std::string_view str, ptrdiff_t pos,
                      std::string_view codepoints)
    {
        return insert_at_offset(str, get_codepoint_pos(str, pos), codepoints);
    }

    bool is_valid_utf8(std::string_view str)
    {
        auto it = str.begin(), end = str.end();
        while (it != end)
        {
            if (decode_next(it, end) == INVALID_CHAR)
                return false;
        }
        return true;
    }

    std::string replace(std::string_view str,
                        std::string_view from,
                        std::string_view to,
                        ptrdiff_t max_replacements)
    {
        std::string result;
        if (max_replacements >= 0)
        {
            auto it = str.begin();
            while (max_replacements-- > 0)
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
            auto matches = find_last_n(str, from, -max_replacements);
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
    replace_chars(std::string_view str, ptrdiff_t start, ptrdiff_t end,
                  std::string_view repl)
    {
        return replace_subrange(
            str, get_char_substring_range(str, start, end), repl);
    }

    std::string
    replace_codepoints(std::string_view str, ptrdiff_t start, ptrdiff_t end,
                       std::string_view repl)
    {
        return replace_subrange(
            str, get_codepoint_substring_range(str, start, end), repl);
    }

    std::string
    replace_codepoint(std::string_view str, char32_t from, char32_t to,
                      ptrdiff_t max_replacements)
    {
        char f[4], t[4];
        auto f_size = encode_utf8(from, f, 4);
        auto t_size = encode_utf8(to, t, 4);
        if (!f_size)
            YSTRING_THROW("Invalid from-code point: "
                          + std::to_string(uint32_t(from)));
        if (!t_size)
            YSTRING_THROW("Invalid to-code point: "
                          + std::to_string(uint32_t(to)));
        return replace(str, std::string_view(f, f_size),
                       std::string_view(t, t_size), max_replacements);
    }

    std::string replace_invalid_utf8(std::string_view str, char32_t chr)
    {
        char repl[4];
        auto repl_size = encode_utf8(chr, repl, 4);
        std::string result;
        auto it = str.begin();
        auto prev = it;
        auto end = str.end();
        while (it != end)
        {
            if (decode_next(it, end) != INVALID_CHAR)
                continue;
            result.append(prev, it);
            result.append(repl, repl_size);
            skip_next(it, end);
            prev = it;
        }
        result.append(prev, end);
        return result;
    }

    std::string& replace_invalid_utf8(std::string& str, char32_t chr)
    {
        if (!is_valid_utf8(str))
            str = replace_invalid_utf8(std::string_view(str), chr);
        return str;
    }

    std::string reverse(std::string_view str)
    {
        std::string result;
        size_t offset = str.size();
        while (offset != 0)
        {
            auto range = get_prev_char_range(str, offset);
            offset = range.start();
            result.insert(result.end(),
                          str.begin() + range.start(),
                          str.begin() + range.end());
        }
        return result;
    }

    std::vector<std::string_view>
    split(std::string_view str, std::u32string_view chars, SplitParams params)
    {
        return split_where(
            str,
            [&](auto s) {return find_first_of(s, chars).first;},
            params);
    }

    std::vector<std::string_view>
    split(std::string_view str, std::string_view sep, SplitParams params)
    {
        return split_where(
            str,
            [&](auto s) {return find_first(s, sep);},
            params);
    }

    std::vector<std::string_view>
    split_lines(std::string_view str, SplitParams params)
    {
        return split_where(str,
                           [&](auto s) {return find_first_newline(s);},
                           params);
    }

    bool starts_with(std::string_view str, std::string_view cmp)
    {
        return cmp.size() <= str.size() && str.substr(0, cmp.size()) == cmp;
    }

    std::string_view trim(std::string_view str, std::u32string_view chars)
    {
        return trim_end(trim_start(str, chars), chars);
    }

    std::string_view trim_end(std::string_view str, std::u32string_view chars)
    {
        return trim_end_where(str, [&](auto c) {return contains(chars, c);});
    }

    std::string_view trim_start(std::string_view str, std::u32string_view chars)
    {
        return trim_start_where(str, [&](auto c) {return contains(chars, c);});
    }

    namespace case_insensitive
    {
        int32_t compare(std::string_view str, std::string_view cmp)
        {
            auto s_it = str.begin();
            auto c_it = cmp.begin();
            while (true)
            {
                char32_t s_ch;
                auto has_s = safe_decode_next(s_it, str.end(), s_ch);
                char32_t c_ch;
                auto has_c = safe_decode_next(c_it, cmp.end(), c_ch);
                if (!has_s || !has_c)
                    return has_s == has_c ? 0 : (has_s ? 1 : -1);

                if (auto result = case_insensitive_compare(s_ch, c_ch))
                    return result;
            }
        }

        bool ends_with(std::string_view str, std::string_view cmp)
        {
            auto [it_str, it_cmp] = prev_case_insensitive_mismatch(
                str.begin(), str.end(), cmp.begin(), cmp.end());
            return it_cmp == cmp.begin();
        }

        bool equal(std::string_view str, std::string_view cmp)
        {
            auto s_it = str.begin();
            auto c_it = cmp.begin();
            while (true)
            {
                char32_t s_ch;
                auto has_s = safe_decode_next(s_it, str.end(), s_ch);
                char32_t c_ch;
                auto has_c = safe_decode_next(c_it, cmp.end(), c_ch);
                if (!has_s || !has_c)
                    return has_s == has_c;

                if (!case_insensitive_equal(s_ch, c_ch))
                    return false;
            }
        }

        Subrange find_first(std::string_view str,
                            std::string_view cmp,
                            size_t offset)
        {
            auto it_cmp = cmp.begin();
            char32_t ch_cmp;
            if (!safe_decode_next(it_cmp, cmp.end(), ch_cmp))
                return {};

            auto it_str = str.begin() + offset;
            auto it_str_next = it_str;
            char32_t ch_str;
            while (safe_decode_next(it_str_next, str.end(), ch_str))
            {
                if (case_insensitive_equal(ch_str, ch_cmp))
                {
                    auto [a, b] = next_case_insensitive_mismatch(
                        it_str_next, str.end(), it_cmp, cmp.end());
                    if (b == cmp.end())
                        return {str.begin(), it_str, a};
                }
                it_str = it_str_next;
            }
            return {};
        }

        std::pair<Subrange, char32_t>
        find_first_of(std::string_view str,
                      std::u32string_view chars,
                      size_t offset)
        {
            return find_first_where(
                str,
                [&](auto c) {return case_insensitive_contains(chars, c);},
                offset);
        }

        std::pair<Subrange, char32_t>
        find_first_of(std::string_view str, CodepointSet chars, size_t offset)
        {
            return find_first_where(
                str,
                [&](auto c) {return chars.case_insensitive_contains(c);},
                offset);
        }

        Subrange find_last(std::string_view str,
                           std::string_view cmp,
                           size_t offset)
        {
            auto it_cmp = cmp.end();
            char32_t ch_cmp;
            if (!safe_decode_prev(cmp.begin(), it_cmp, ch_cmp))
                return {};

            offset = std::min(offset, str.size());

            auto it_str = str.begin() + offset;
            auto it_str_prev = it_str;
            char32_t ch_str;
            while (safe_decode_prev(str.begin(), it_str_prev, ch_str))
            {
                if (case_insensitive_equal(ch_str, ch_cmp))
                {
                    auto [a, b] = prev_case_insensitive_mismatch(
                        str.begin(), it_str_prev, cmp.begin(), it_cmp);
                    if (b == cmp.begin())
                        return {str.begin(), a, it_str};
                }
                it_str = it_str_prev;
            }
            return {};
        }

        std::pair<Subrange, char32_t>
        find_last_of(std::string_view str,
                     std::u32string_view chars,
                     size_t offset)
        {
            return find_last_where(
                str,
                [&](auto c) {return case_insensitive_contains(chars, c);},
                offset);
        }

        std::pair<Subrange, char32_t>
        find_last_of(std::string_view str, CodepointSet chars, size_t offset)
        {
            return find_last_where(
                str,
                [&](auto c) {return chars.case_insensitive_contains(c);},
                offset);
        }

        bool less(std::string_view str, std::string_view cmp)
        {
            auto s_it = str.begin();
            auto c_it = cmp.begin();
            while (true)
            {
                char32_t s_ch;
                auto has_s = safe_decode_next(s_it, str.end(), s_ch);
                char32_t c_ch;
                auto has_c = safe_decode_next(c_it, cmp.end(), c_ch);
                if (!has_s || !has_c)
                    return !has_s && has_c;

                if (auto result = case_insensitive_compare(s_ch, c_ch))
                    return result < 0;
            }
        }

        std::string replace(std::string_view str,
                            std::string_view from,
                            std::string_view to,
                            ptrdiff_t max_replacements)
        {
            std::string result;
            if (max_replacements >= 0)
            {
                while (max_replacements-- > 0)
                {
                    auto match = find_first(str, from);
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
                auto tmp_str = str;
                while (max_replacements++ < 0)
                {
                    auto match = find_last(tmp_str, from);
                    if (!match)
                        break;
                    matches.push_back(match);
                    tmp_str = tmp_str.substr(0, match.start());
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

        std::vector<std::string_view>
        split(std::string_view str, std::u32string_view chars, SplitParams params)
        {
            return split_where(
                str,
                [&](auto s) {return case_insensitive::find_first_of(s, chars).first;},
                params);
        }

        std::vector<std::string_view>
        split(std::string_view str, std::string_view sep, SplitParams params)
        {
            return split_where(
                str,
                [&](auto s) {return case_insensitive::find_first(s, sep);},
                params);
        }

        bool starts_with(std::string_view str, std::string_view cmp)
        {
            auto [it_str, it_cmp] = next_case_insensitive_mismatch(
                str.begin(), str.end(), cmp.begin(), cmp.end());
            return it_cmp == cmp.end();
        }
    }
}

