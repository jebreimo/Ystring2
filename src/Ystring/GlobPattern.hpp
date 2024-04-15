//****************************************************************************
// Copyright © 2024 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2024-03-30.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <span>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
//#include "BitmaskOperators.hpp"

namespace ystring
{
    struct GlobPattern;

    struct Qmark
    {
        size_t length = 0;
    };

    struct Star
    {};

    struct Empty
    {};

    using CharRange = std::pair<char32_t, char32_t>;

    struct CharSet
    {
        bool negated = false;
        std::vector<CharRange> ranges;
    };

    struct MultiPattern
    {
        MultiPattern() = default;

        MultiPattern(const MultiPattern& rhs)
            : patterns(rhs.patterns.size())
        {
            for (size_t i = 0; i < rhs.patterns.size(); ++i)
                patterns[i] = std::make_unique<GlobPattern>(*rhs.patterns[i]);
        }

        MultiPattern(MultiPattern&& rhs) noexcept
            : patterns(std::move(rhs.patterns))
        {}

        ~MultiPattern() = default;

        MultiPattern& operator=(const MultiPattern& rhs)
        {
            if (this != &rhs)
            {
                patterns.resize(rhs.patterns.size());
                for (size_t i = 0; i < rhs.patterns.size(); ++i)
                    patterns[i] = std::make_unique<GlobPattern>(*rhs.patterns[i]);
            }
            return *this;
        }

        MultiPattern& operator=(MultiPattern&& rhs) noexcept
        {
            patterns = std::move(rhs.patterns);
            return *this;
        }

        std::vector<std::unique_ptr<GlobPattern>> patterns;
    };

    using Part = std::variant<Empty, Star, Qmark, CharSet, std::string, MultiPattern>;

    size_t get_min_length(const Part& part);

    //enum class FixedPosition
    //{
    //    NONE = 0,
    //    START = 1,
    //    END = 2
    //};
    //
    //YSTRING_ENABLE_BITMASK_OPERATORS(FixedPosition);

    struct GlobPattern
    {
        //size_t length = 0;
        std::vector<Part> parts;
        //std::vector<Part> tail_parts;
    };

    enum class TokenType
    {
        NONE,
        CHAR,
        QUESTION_MARK,
        STAR,
        OPEN_BRACKET,
        OPEN_BRACE,
        COMMA,
        END_BRACE
    };

    TokenType next_token_type(std::string_view pattern, bool is_subpattern);

    CharSet extract_char_set(std::string_view& pattern);

    std::string extract_string(std::string_view& pattern, bool is_subpattern);

    Star extract_stars(std::string_view& pattern);

    Qmark extract_qmarks(std::string_view& pattern);

    MultiPattern extract_multi_pattern(std::string_view& pattern);

    std::unique_ptr<GlobPattern>
    parse_glob_pattern(std::string_view& pattern, bool is_subpattern);

    std::unique_ptr<GlobPattern> parse_glob_pattern(std::string_view pattern);

    bool starts_with(std::string_view& str, const Part& part);

    bool match_fwd(std::span<Part> parts, std::string_view& str,
                   bool is_subpattern);

    bool search_fwd(std::span<Part> parts, std::string_view& str,
                    bool is_subpattern);

    bool match_bwd(std::span<Part>& parts, std::string_view& str);
}
