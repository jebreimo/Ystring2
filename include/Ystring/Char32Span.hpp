//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-05-25.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <algorithm>
#include "CaseInsensitive.hpp"

namespace ystring
{
    class Char32Span
    {
    public:
        constexpr Char32Span() noexcept
            : m_first(), m_count()
        {}

        constexpr Char32Span(const char32_t* first, size_t count) noexcept
            : m_first(first), m_count(count)
        {}

        template <size_t N>
        constexpr Char32Span(const char32_t (& arr)[N]) noexcept
            : m_first(arr), m_count(N)
        {}

        [[nodiscard]] constexpr bool empty() const noexcept
        {
            return m_count == 0;
        }

        [[nodiscard]] constexpr size_t size() const noexcept
        {
            return m_count;
        }

        [[nodiscard]] constexpr const char32_t* begin() const noexcept
        {
            return m_first;
        }

        [[nodiscard]] constexpr const char32_t* end() const noexcept
        {
            return m_first + m_count;
        }
    private:
        const char32_t* m_first;
        size_t m_count;
    };

    [[nodiscard]]
    inline bool contains(const Char32Span& span, char32_t ch)
    {
        return std::find(span.begin(), span.end(), ch) != span.end();
    }

    [[nodiscard]]
    inline bool case_insensitive_contains(const Char32Span& span, char32_t ch)
    {
        return std::find_if(
            span.begin(), span.end(),
            [&](auto c) {return case_insensitive_equal(ch, c);}) != span.end();
    }

}
