//****************************************************************************
// Copyright © 2024 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2024-03-25.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/GlobMatcher.hpp"
#include "Ystring/Unescape.hpp"
#include "GlobPattern.hpp"

namespace ystring
{
    GlobMatcher::GlobMatcher() = default;

    GlobMatcher::GlobMatcher(std::string_view pattern,
                             const GlobOptions& options)
        : case_sensitive(options.case_sensitive),
          pattern_(parse_glob_pattern(pattern,
                                      {options.support_braces,
                                       options.support_sets}))

    {}

    GlobMatcher::GlobMatcher(const GlobMatcher& rhs)
        : pattern_(rhs.pattern_ ? std::make_unique<GlobPattern>(*rhs.pattern_)
                                : nullptr)
    {}

    GlobMatcher::GlobMatcher(GlobMatcher&& rhs) noexcept
        : pattern_(std::move(rhs.pattern_))
    {}

    GlobMatcher::~GlobMatcher() = default;

    GlobMatcher& GlobMatcher::operator=(const GlobMatcher& rhs)
    {
        if (this != &rhs)
        {
            pattern_ = rhs.pattern_
                       ? std::make_unique<GlobPattern>(*rhs.pattern_)
                       : nullptr;
            case_sensitive = rhs.case_sensitive;
        }
        return *this;
    }

    GlobMatcher& GlobMatcher::operator=(GlobMatcher&& rhs) noexcept
    {
        pattern_ = std::move(rhs.pattern_);
        case_sensitive = rhs.case_sensitive;
        return *this;
    }

    [[nodiscard]]
    bool GlobMatcher::match(std::string_view str) const
    {
        if (!pattern_)
            return str.empty();

        auto length = pattern_->parts.size() - pattern_->tail_length;
        std::span parts(pattern_->parts.data(), length);
        std::span tail(pattern_->parts.data() + length, pattern_->tail_length);
        return match_end(tail, str, case_sensitive)
               && match_fwd(parts, str, case_sensitive, false);
    }

    std::ostream& operator<<(std::ostream& os, const GlobMatcher& matcher)
    {
        if (matcher.pattern_)
            os << *matcher.pattern_;
        return os;
    }

    bool is_glob_pattern(std::string_view str, const GlobOptions& options)
    {
        GlobParserOptions parser_opts{options.support_braces, options.support_sets};
        while (!str.empty())
        {
            switch (next_token_type(str, parser_opts))
            {
            case TokenType::STAR:
            case TokenType::QUESTION_MARK:
            case TokenType::OPEN_BRACE:
            case TokenType::OPEN_BRACKET:
                return true;
            default:
                break;
            }
            std::ignore = unescape_next(str);
        }
        return false;
    }
}
