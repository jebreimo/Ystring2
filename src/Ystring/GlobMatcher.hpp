//****************************************************************************
// Copyright © 2024 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2024-03-25.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace ystring
{
    struct GlobPattern;

    class GlobMatcher
    {
    public:
        GlobMatcher();

        explicit GlobMatcher(std::string_view pattern);

        GlobMatcher(const GlobMatcher& rhs);

        GlobMatcher(GlobMatcher&& rhs) noexcept;

        ~GlobMatcher();

        GlobMatcher& operator=(const GlobMatcher& rhs);

        GlobMatcher& operator=(GlobMatcher&& rhs) noexcept;

        [[nodiscard]]
        bool match(std::string_view str) const;
    private:
        std::unique_ptr<GlobPattern> pattern_;
    };
}