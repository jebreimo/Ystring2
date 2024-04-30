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
    TokenType next_token_type(std::string_view pattern,
                              const GlobParserOptions& options)
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
            if (options.support_sets)
                return TokenType::OPEN_BRACKET;
            break;
        case '{':
            if (options.support_braces)
                return TokenType::OPEN_BRACE;
            break;
        default:
            break;
        }

        if (options.is_subpattern)
        {
            if (pattern[0] == '}')
                return TokenType::END_BRACE;
            if (pattern[0] == ',')
                return TokenType::COMMA;
        }
        return TokenType::CHAR;
    }

    CodepointSet extract_char_set(std::string_view& pattern)
    {
        // Remove '['
        pattern.remove_prefix(1);

        CodepointSet result;
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
            if (ch == U']' && !did_unescape)
            {
                if (state == State::AWAITING_LAST)
                    YSTRING_THROW("Incomplete character range in glob pattern.");
                return result;
            }
            else if (ch == U'-' && !did_unescape)
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

    std::string extract_string(std::string_view& pattern,
                               const GlobParserOptions& options)
    {
        std::string result;
        while (!pattern.empty())
        {
            if (pattern[0] == '\\')
            {
                auto ch = unescape_next(pattern);
                encode_utf8(ch.value(), std::back_inserter(result));
            }
            else if (pattern[0] == '?'
                     || pattern[0] == '*'
                     || (options.support_sets && pattern[0] == '[')
                     || (options.support_braces && pattern[0] == '{')
                     || (options.is_subpattern
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

    // NOLINTBEGIN(misc-no-recursion)
    MultiPattern extract_multi_pattern(std::string_view& pattern,
                                       GlobParserOptions options)
    {
        options.is_subpattern = true;

        MultiPattern result;
        while (!pattern.empty())
        {
            switch (next_token_type(pattern, options))
            {
            case TokenType::OPEN_BRACE:
            case TokenType::COMMA:
                pattern.remove_prefix(1);
                result.patterns.push_back(parse_glob_pattern(pattern, options));
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

    [[nodiscard]]
    bool is_star(const Part& part)
    {
        return part.index() == 1;
    }

    [[nodiscard]]
    bool has_star(const std::vector<Part>& parts);

    [[nodiscard]]
    bool has_star(const Part& part)
    {
        if (auto multi_pattern = std::get_if<MultiPattern>(&part))
        {
            for (const auto& pattern: multi_pattern->patterns)
            {
                if (has_star(pattern->parts))
                    return true;
            }
        }
        return is_star(part);
    }

    [[nodiscard]]
    bool has_star(const std::vector<Part>& parts)
    {
        return std::any_of(parts.begin(), parts.end(),
                           [](auto& p) {return has_star(p);}
        );
    }

    void optimize(GlobPattern& pattern)
    {
        pattern.tail_length = 0;
        for (auto it = pattern.parts.rbegin(); it != pattern.parts.rend(); ++it)
        {
            if (has_star(*it))
                break;
            pattern.tail_length++;
        }
    }

    std::unique_ptr<GlobPattern>
    parse_glob_pattern(std::string_view& pattern,
                       const GlobParserOptions& options)
    {
        auto result = std::make_unique<GlobPattern>();

        bool done = false;
        while (!done)
        {
            switch (next_token_type(pattern, options))
            {
            case TokenType::CHAR:
                result->parts.emplace_back(extract_string(pattern, options));
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
                result->parts.emplace_back(extract_multi_pattern(pattern,
                                                                 options));
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

        if (!options.is_subpattern)
            optimize(*result);

        return result;
    }

    bool starts_with(std::string_view& str, const Part& part,
                     bool case_sensitive)
    {
        struct StartsWithVisitor
        {
            bool operator()(const std::string& s)
            {
                if (case_sensitive ? starts_with(str, s)
                                   : case_insensitive::starts_with(str, s))
                {
                    str.remove_prefix(s.size());
                    return true;
                }
                return false;
            }

            bool operator()(const CodepointSet& s)
            {
                if (auto ch = pop_utf8_codepoint(str))
                {
                    return case_sensitive ? s.contains(*ch)
                                          : s.case_insensitive_contains(*ch);
                }
                return false;
            }

            bool operator()(const MultiPattern& mp)
            {
                for (auto& pattern : mp.patterns)
                {
                    if (match_fwd(std::span(pattern->parts), str,
                                  case_sensitive, true))
                    {
                        return true;
                    }
                }
                return false;
            }

            bool operator()(const Qmark& qm)
            {
                for (size_t i = 0; i < qm.length; ++i)
                {
                    if (!remove_utf8_codepoint(str))
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
            bool case_sensitive;
        };

        return std::visit(StartsWithVisitor{str, case_sensitive}, part);
    }

    bool ends_with(std::string_view& str, const Part& part, bool case_sensitive)
    {
        struct EndsWithVisitor
        {
            bool operator()(const std::string& s)
            {
                if (case_sensitive ? ends_with(str, s)
                                   : case_insensitive::ends_with(str, s))
                {
                    str.remove_suffix(s.size());
                    return true;
                }
                return false;
            }

            bool operator()(const CodepointSet& s)
            {
                if (auto ch = pop_last_utf8_codepoint(str))
                {
                    return case_sensitive ? s.contains(*ch)
                                          : s.case_insensitive_contains(*ch);
                }
                return false;
            }

            bool operator()(const MultiPattern& mp)
            {
                for (auto& pattern : mp.patterns)
                {
                    std::span parts(pattern->parts);
                    if (match_end(parts, str, case_sensitive))
                        return true;
                }
                return false;
            }

            bool operator()(const Qmark& qm)
            {
                for (size_t i = 0; i < qm.length; ++i)
                {
                    if (!remove_last_utf8_codepoint(str))
                        return false;
                }
                return true;
            }

            bool operator()(const Star&)
            {
                return false;
            }

            bool operator()(const Empty&) const
            {
                return true;
            }

            std::string_view& str;
            bool case_sensitive;
        };

        return std::visit(EndsWithVisitor{str, case_sensitive}, part);
    }

    bool match_fwd(std::span<Part> parts, std::string_view& str,
                   bool case_sensitive, bool is_subpattern)
    {
        auto str_copy = str;
        for (size_t i = 0; i < parts.size(); ++i)
        {
            if (is_star(parts[i])
                && search_fwd(parts.subspan(i + 1), str,
                              case_sensitive, is_subpattern))
            {
                return true;
            }
            else if (!starts_with(str, parts[i], case_sensitive))
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
                    bool case_sensitive,
                    bool is_subpattern)
    {
        if (parts.empty())
        {
            str.remove_prefix(str.size());
            return true;
        }

        while (!str.empty())
        {
            if (match_fwd(parts, str, case_sensitive, is_subpattern))
                return true;

            remove_utf8_codepoint(str);
        }

        return false;
    }

    bool match_end(std::span<Part> parts, std::string_view& str, bool case_sensitive)
    {
        auto str_copy = str;
        for (size_t i = parts.size(); i-- > 0;)
        {
            if (!ends_with(str, parts[i], case_sensitive))
            {
                str = str_copy;
                return false;
            }
        }
        return true;
    }

    // NOLINTEND(misc-no-recursion)
}
