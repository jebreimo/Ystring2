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

namespace Ystring
{
    class Char32Span
    {
    public:
        constexpr Char32Span() noexcept
            : m_First(), m_Count()
        {}

        constexpr Char32Span(const char32_t* first, size_t count) noexcept
            : m_First(first), m_Count(count)
        {}

        template <size_t N>
        constexpr Char32Span(const char32_t (& arr)[N]) noexcept
            : m_First(arr), m_Count(N)
        {}

        [[nodiscard]] constexpr bool empty() const noexcept
        {
            return m_Count == 0;
        }

        [[nodiscard]] constexpr size_t size() const noexcept
        {
            return m_Count;
        }

        [[nodiscard]] constexpr const char32_t* begin() const noexcept
        {
            return m_First;
        }

        [[nodiscard]] constexpr const char32_t* end() const noexcept
        {
            return m_First + m_Count;
        }
    private:
        const char32_t* m_First;
        size_t m_Count;
    };

    [[nodiscard]]
    inline bool contains(const Char32Span& span, char32_t ch)
    {
        return std::find(span.begin(), span.end(), ch) != span.end();
    }

    [[nodiscard]]
    inline bool caseInsensitiveContains(const Char32Span& span, char32_t ch)
    {
        return std::find_if(
            span.begin(), span.end(),
            [&](auto c) {return caseInsensitiveEqual(ch, c);}) != span.end();
    }

}
