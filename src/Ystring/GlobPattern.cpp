//****************************************************************************
// Copyright © 2024 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2024-03-30.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "GlobPattern.hpp"
#include "Ystring/Algorithms.hpp"
#include "Ystring/DecodeUtf8.hpp"
#include "Ystring/Unescape.hpp"
#include "EncodeUtf8.hpp"

namespace ystring
{
    //size_t get_min_length(const Part& part)
    //{
    //    struct LengthGetter
    //    {
    //        size_t operator()(const std::string& str) const
    //        {
    //            return str.size();
    //        }
    //
    //        size_t operator()(const CharSet& char_set) const
    //        {
    //            return 1;
    //        }
    //
    //        size_t operator()(const MultiPattern& multi_pattern) const
    //        {
    //            size_t result = SIZE_MAX;
    //            for (const auto& pattern : multi_pattern.patterns)
    //                result = std::min(result, pattern->length);
    //            return result;
    //        }
    //
    //        size_t operator()(const Wildcard& wildcard) const
    //        {
    //            return wildcard.length;
    //        }
    //
    //        size_t operator()(Empty) const
    //        {
    //            return 0;
    //        }
    //    };
    //    return 0;
    //}

    TokenType next_token_type(std::string_view pattern, bool is_subpattern)
    {
        if (pattern.empty())
            return TokenType::NONE;

        switch (pattern[0])
        {
        case '?':
            return TokenType::QUESTION_MARK;
        case '*':
            return TokenType::STAR;
        case '[':
            return TokenType::OPEN_BRACKET;
        case '{':
            return TokenType::OPEN_BRACE;
        default:
            if (is_subpattern)
            {
                if (pattern[0] == '}')
                    return TokenType::END_BRACE;
                if (pattern[0] == ',')
                    return TokenType::COMMA;
            }
            return TokenType::CHAR;
        }
    }

    CharSet extract_char_set(std::string_view& pattern)
    {
        // Remove '['
        pattern.remove_prefix(1);

        CharSet result;
        if (!pattern.empty() && pattern[0] == '^')
        {
            result.negated = true;
            pattern.remove_prefix(1);
        }

        enum class State
        {
            AWAITING_FIRST,
            HAS_FIRST,
            AWAITING_LAST
        };
        State state = State::AWAITING_FIRST;
        bool did_unescape = false;
        while (auto ch = unescape_next(pattern, &did_unescape))
        {
            if (ch == ']' && !did_unescape)
            {
                if (state == State::AWAITING_LAST)
                    YSTRING_THROW("Incomplete character range in glob pattern.");
                return result;
            }
            else if (ch == '-' && !did_unescape)
            {
                if (state == State::HAS_FIRST)
                    state = State::AWAITING_LAST;
                else if (state == State::AWAITING_FIRST && result.ranges.empty())
                    result.ranges.emplace_back('-', '-');
                else
                    YSTRING_THROW("Invalid character range in glob pattern.");
            }
            else if (state != State::AWAITING_LAST)
            {
                result.ranges.emplace_back(ch.value(), ch.value());
                state = State::HAS_FIRST;
            }
            else if (result.ranges.back().second < ch.value())
            {
                result.ranges.back().second = ch.value();
                state = State::AWAITING_FIRST;
            }
            else
            {
                YSTRING_THROW("Invalid character range in glob pattern.");
            }
        }
        YSTRING_THROW("Unmatched '[' in glob pattern.");
    }

    std::string extract_string(std::string_view& pattern, bool is_subpattern)
    {
        std::string result;
        while (!pattern.empty())
        {
            if (pattern[0] == '\\')
            {
                auto ch = unescape_next(pattern);
                if (!ch)
                    YSTRING_THROW("Invalid escape sequence in glob pattern.");
                encode_utf8(ch.value(), std::back_inserter(result));
            }
            else if (pattern[0] == '[' || pattern[0] == '?'
                     || pattern[0] == '*' || pattern[0] == '{'
                     || (is_subpattern
                         && (pattern[0] == '}' || pattern[0] == ',')))
            {
                break;
            }
            else
            {
                result.push_back(pattern[0]);
                pattern.remove_prefix(1);
            }
        }
        return result;
    }

    Star extract_stars(std::string_view& pattern)
    {
        Star result;
        while (!pattern.empty() && pattern[0] == '*')
        {
            pattern.remove_prefix(1);
        }
        return result;
    }

    Qmark extract_qmarks(std::string_view& pattern)
    {
        Qmark result;
        while (!pattern.empty() && pattern[0] == '?')
        {
            result.length++;
            pattern.remove_prefix(1);
        }
        return result;
    }

    MultiPattern extract_multi_pattern(std::string_view& pattern)
    {
        MultiPattern result;
        while (!pattern.empty())
        {
            switch (next_token_type(pattern, true))
            {
            case TokenType::OPEN_BRACE:
            case TokenType::COMMA:
                pattern.remove_prefix(1);
                result.patterns.push_back(parse_glob_pattern(pattern, true));
                break;
            case TokenType::END_BRACE:
                if (result.patterns.empty())
                    YSTRING_THROW("Empty subpattern in glob pattern. Did you mean to use '\\{\\}'?");
                pattern.remove_prefix(1);
                return result;
            default:
                YSTRING_THROW("Unmatched '{' in glob pattern.");
            }
        }
        YSTRING_THROW("Unmatched '{' in glob pattern.");
    }

    //void move_tail_parts(GlobPattern& pattern)
    //{
    //    struct TailMoverVisitor
    //    {
    //        bool operator()(const std::string&) const
    //        {
    //            return true;
    //        }
    //
    //        bool operator()(const CharSet&) const
    //        {
    //            return true;
    //        }
    //
    //        bool operator()(const MultiPattern&) const
    //        {
    //            return true;
    //        }
    //
    //        bool operator()(const Star&) const
    //        {
    //            return false;
    //        }
    //
    //        bool operator()(const Qmark&) const
    //        {
    //            return true;
    //        }
    //
    //        bool operator()(const Empty)
    //        {
    //            return false;
    //        }
    //    };
    //
    //    TailMoverVisitor visitor;
    //
    //    auto it = pattern.parts.rbegin();
    //    for (; it != pattern.parts.rend(); ++it)
    //    {
    //        if (!std::visit(visitor, *it))
    //            break;
    //        pattern.tail_parts.push_back(std::move(*it));
    //    }
    //    pattern.parts.erase(it.base(), pattern.parts.end());
    //}

    std::unique_ptr<GlobPattern>
    parse_glob_pattern(std::string_view& pattern, bool is_subpattern)
    {
        auto result = std::make_unique<GlobPattern>();

        bool done = false;
        while (!done)
        {
            switch (next_token_type(pattern, is_subpattern))
            {
            case TokenType::CHAR:
                result->parts.emplace_back(extract_string(pattern, is_subpattern));
                break;
            case TokenType::QUESTION_MARK:
                result->parts.emplace_back(extract_qmarks(pattern));
                break;
            case TokenType::STAR:
                result->parts.emplace_back(extract_stars(pattern));
                break;
            case TokenType::OPEN_BRACKET:
                result->parts.emplace_back(extract_char_set(pattern));
                break;
            case TokenType::OPEN_BRACE:
                result->parts.emplace_back(extract_multi_pattern(pattern));
                break;
            case TokenType::COMMA:
            case TokenType::END_BRACE:
            default:
                if (result->parts.empty())
                    result->parts.emplace_back(Empty());
                done = true;
                break;
            }
        }

        return result;
    }

    std::unique_ptr<GlobPattern> parse_glob_pattern(std::string_view pattern)
    {
        return parse_glob_pattern(pattern, false);
    }

    bool contains(const CharSet& char_set, char32_t ch)
    {
        return std::any_of(char_set.ranges.begin(), char_set.ranges.end(),
                           [&](auto& r){return r.first <= ch && ch <= r.second;})
               != char_set.negated;
    }

    bool starts_with(std::string_view& str, const Part& part)
    {
        struct StartsWithVisitor
        {
            bool operator()(const std::string& s)
            {
                if (starts_with(str, std::string_view(s)))
                {
                    str.remove_prefix(s.size());
                    return true;
                }
                return false;
            }

            bool operator()(const CharSet& s)
            {
                if (auto ch = next_utf8_char(str))
                    return contains(s, *ch);
                return false;
            }

            bool operator()(const MultiPattern& mp)
            {
                for (auto& pattern : mp.patterns)
                {
                    if (match_fwd(std::span(pattern->parts), str, true))
                        return true;
                }
                return false;
            }

            bool operator()(const Qmark& qm)
            {
                for (size_t i = 0; i < qm.length; ++i)
                {
                    if (!skip_next_utf8_char(str))
                        return false;
                }
                return true;
            }

            bool operator()(const Star&)
            {
                str.remove_prefix(str.size());
                return true;
            }

            bool operator()(const Empty&) const
            {
                return true;
            }

            std::string_view& str;
        };

        return std::visit(StartsWithVisitor{str}, part);
    }

    [[nodiscard]]
    bool is_star(const Part& part)
    {
        return part.index() == 5;
    }

    bool match_fwd(std::span<Part> parts, std::string_view& str,
                   bool is_subpattern)
    {
        auto str_copy = str;
        for (size_t i = 0; i < parts.size(); ++i)
        {
            if (is_star(parts[i])
                && search_fwd(parts.subspan(i + 1), str, is_subpattern))
            {
                return true;
            }
            else if (!starts_with(str, parts[i]))
            {
                str = str_copy;
                return false;
            }
        }

        if (str.empty() || is_subpattern)
            return true;

        str = str_copy;
        return false;
    }

    bool search_fwd(std::span<Part> parts, std::string_view& str,
                    bool is_subpattern)
    {
        if (parts.empty())
        {
            str.remove_prefix(str.size());
            return true;
        }

        while (!str.empty())
        {
            if (match_fwd(parts, str, is_subpattern))
                return true;

            skip_next_utf8_char(str);
        }

        return false;
    }
}
