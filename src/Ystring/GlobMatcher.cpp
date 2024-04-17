//****************************************************************************
// Copyright © 2024 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2024-03-25.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "GlobMatcher.hpp"
#include "GlobPattern.hpp"

namespace ystring
{
    GlobMatcher::GlobMatcher() = default;

    GlobMatcher::GlobMatcher(std::string_view pattern)
        : pattern_(parse_glob_pattern(pattern))
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
        }
        return *this;
    }

    GlobMatcher& GlobMatcher::operator=(GlobMatcher&& rhs) noexcept
    {
        pattern_ = std::move(rhs.pattern_);
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
        return match_end(tail, str) && match_fwd(parts, str, false);
    }
}
