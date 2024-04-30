//****************************************************************************
// Copyright © 2024 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2024-03-30.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include "Ystring/CodepointSet.hpp"

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

    using Part = std::variant<Empty, Star, Qmark, CodepointSet, std::string, MultiPattern>;

    struct GlobPattern
    {
        std::vector<Part> parts;
        size_t tail_length = 0;
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

    struct GlobParserOptions
    {
        bool support_braces = true;
        bool support_sets = true;
        bool is_subpattern = false;
    };

    std::unique_ptr<GlobPattern>
    parse_glob_pattern(std::string_view& pattern,
                       const GlobParserOptions& options);

    bool match_fwd(std::span<Part> parts, std::string_view& str,
                   bool case_sensitive,
                   bool is_subpattern);

    bool search_fwd(std::span<Part> parts, std::string_view& str,
                    bool case_sensitive,
                    bool is_subpattern);

    bool match_end(std::span<Part> parts, std::string_view& str,
                   bool case_sensitive);

    TokenType next_token_type(std::string_view pattern,
                              const GlobParserOptions& options);

    CodepointSet extract_char_set(std::string_view& pattern);

    std::string extract_string(std::string_view& pattern,
                               const GlobParserOptions& options);

    Star extract_stars(std::string_view& pattern);

    Qmark extract_qmarks(std::string_view& pattern);

    MultiPattern extract_multi_pattern(std::string_view& pattern,
                                       GlobParserOptions options);
}
