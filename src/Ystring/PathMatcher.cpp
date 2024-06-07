//****************************************************************************
// Copyright © 2024 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2024-05-24.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/PathMatcher.hpp"

#include <variant>
#include <vector>
#include <span>
#include "Ystring/Algorithms.hpp"
#include "Ystring/GlobMatcher.hpp"

namespace ystring
{
    namespace
    {
        struct AnyPath
        {};

        using PathElement = std::variant<std::string, AnyPath, GlobMatcher>;
    }

    class PathMatcher::PathMatcherImpl
    {
    public:
        explicit PathMatcherImpl(std::filesystem::path pattern,
                                 const GlobOptions& options)
            : case_sensitive_(options.case_sensitive)
        {
            pattern = pattern.lexically_normal();
            while (true)
            {
                auto filename_u8 = pattern.filename().u8string();
                auto filename_view = to_string_view(filename_u8);
                if (filename_view == "**")
                    elements_.emplace_back(AnyPath{});
                else if (is_glob_pattern(filename_view))
                    elements_.emplace_back(GlobMatcher(filename_view, options));
                else if (!filename_view.empty())
                    elements_.emplace_back(std::string(filename_view));
                auto parent_path = pattern.parent_path();
                if (parent_path == pattern)
                {
                    if (!parent_path.empty())
                        elements_.emplace_back(parent_path);
                    break;
                }
                pattern = parent_path;
            }
        }

        [[nodiscard]]
        bool match(const std::filesystem::path& path) const
        {
            enum class State {Matching, Searching};
            State state = State::Matching;
            return match(std::span<const PathElement>(elements_.data(), elements_.size()), path);
        }
    private:
        static bool match(std::span<const PathElement> elements,
                          std::filesystem::path path)
        {
            for (size_t i = 0; i < elements.size(); ++i)
            {
                const auto& element = elements[i];
                auto filename = path.filename();
                if (filename.empty())
                    filename = path;
                auto parent = path.parent_path();
                if (std::holds_alternative<std::string>(element))
                {
                    auto str = std::get<std::string>(element);
                    if (str != ystring::to_string_view(filename.u8string()))
                        return false;
                }
                else if (std::holds_alternative<AnyPath>(element))
                {
                    return search(elements.subspan(i + 1), path);
                }
                else if (std::holds_alternative<GlobMatcher>(element))
                {
                    auto& matcher = std::get<GlobMatcher>(element);
                    if (!matcher.match(ystring::to_string_view(filename.u8string())))
                        return false;
                }
                if (parent == path)
                    return i == elements.size() - 1;
                path = std::move(parent);
            }
            return path.empty() || path == ".";
        }

        static bool search(std::span<const PathElement> elements,
                           std::filesystem::path path)
        {
            if (elements.empty())
                return true;

            while (true)
            {
                if (match(elements, path))
                    return true;
                auto parent = path.parent_path();
                if (parent == path)
                    return false;
                path = std::move(parent);
            }
        }

        std::vector<PathElement> elements_;
        bool case_sensitive_ = true;
    };

    PathMatcher::PathMatcher(std::string_view pattern, const GlobOptions& options)
        : impl_(std::make_unique<PathMatcherImpl>(pattern, options))
    {}

    PathMatcher::PathMatcher(const std::filesystem::path& pattern)
        : impl_(std::make_unique<PathMatcherImpl>(pattern, GlobOptions{}))
    {}

    PathMatcher::PathMatcher(const PathMatcher& rhs)
    {
        impl_ = std::make_unique<PathMatcherImpl>(*rhs.impl_);
    }

    PathMatcher::PathMatcher(PathMatcher&& rhs) noexcept
    {
        impl_ = std::move(rhs.impl_);
    }

    PathMatcher::~PathMatcher() = default;

    PathMatcher& PathMatcher::operator=(const PathMatcher& rhs)
    {
        if (this != &rhs)
            impl_ = std::make_unique<PathMatcherImpl>(*rhs.impl_);
        return *this;
    }

    PathMatcher& PathMatcher::operator=(PathMatcher&& rhs) noexcept
    {
        impl_ = std::move(rhs.impl_);
        return *this;
    }

    bool PathMatcher::match(std::string_view str) const
    {
        return false;
    }

    bool PathMatcher::match(const std::filesystem::path& str) const
    {
        return impl_->match(str);
    }
}
