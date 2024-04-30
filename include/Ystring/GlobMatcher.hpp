//****************************************************************************
// Copyright © 2024 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2024-03-25.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <memory>
#include <string_view>
#include "YstringDefinitions.hpp"

namespace ystring
{
    struct GlobPattern;

    struct GlobOptions
    {
        bool case_sensitive = true;
        bool support_braces = true;
        bool support_sets = true;
    };

    class YSTRING_API GlobMatcher
    {
    public:
        GlobMatcher();

        explicit GlobMatcher(std::string_view pattern,
                             const GlobOptions& options = {});

        GlobMatcher(const GlobMatcher& rhs);

        GlobMatcher(GlobMatcher&& rhs) noexcept;

        ~GlobMatcher();

        GlobMatcher& operator=(const GlobMatcher& rhs);

        GlobMatcher& operator=(GlobMatcher&& rhs) noexcept;

        bool case_sensitive = true;

        [[nodiscard]]
        bool match(std::string_view str) const;
    private:
        std::unique_ptr<GlobPattern> pattern_;
    };

    bool is_glob_pattern(std::string_view str,
                         const GlobOptions& options = {});
}
