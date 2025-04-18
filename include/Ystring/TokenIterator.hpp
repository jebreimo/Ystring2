//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-05-23.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <string_view>
#include "Subrange.hpp"

namespace ystring
{
    template <typename TokenFinder>
    class TokenIterator
    {
    public:
        TokenIterator(std::string_view str, TokenFinder finder)
            : m_str(str),
              m_token(0, 0),
              m_finder(std::move(finder))
        {}

        explicit constexpr operator bool() const
        {
            return bool(m_token);
        }

        bool next()
        {
            if (m_token)
                m_str = m_str.substr(m_token.end());
            else if (m_token.start() != 0)
                return false;
            m_token = m_finder(m_str);
            if (!m_token)
                m_token.offset = SIZE_MAX;
            return true;
        }

        [[nodiscard]] std::string_view part() const
        {
            if (m_token)
                return m_str.substr(0, m_token.start());
            else if (m_token.start() != 0)
                return m_str;
            else
                return {};
        }

        [[nodiscard]] std::string_view token() const
        {
            if (!m_token)
                return {};
            return m_str.substr(m_token.start(), m_token.end());
        }

        [[nodiscard]] std::string_view remainder() const
        {
            if (m_token || m_token.start() == 0)
                return m_str.substr(m_token.end());
            return {};
        }
    private:
        std::string_view m_str;
        Subrange m_token;
        TokenFinder m_finder;
    };
}
