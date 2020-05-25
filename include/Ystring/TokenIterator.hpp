//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-05-23.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <string_view>
#include "Subrange.hpp"

namespace Ystring
{
    template <typename TokenFinder>
    class TokenIterator
    {
    public:
        TokenIterator(std::string_view str, TokenFinder finder)
            : m_Str(str),
              m_Token(0, 0),
              m_Finder(std::move(finder))
        {}

        constexpr operator bool() const
        {
            return bool(m_Token);
        }

        bool next()
        {
            if (m_Token)
                m_Str = m_Str.substr(m_Token.end());
            else if (m_Token.start() != 0)
                return false;
            m_Token = m_Finder(m_Str);
            if (!m_Token)
                m_Token.offset = SIZE_MAX;
            return true;
        }

        [[nodiscard]] std::string_view part() const
        {
            if (m_Token)
                return m_Str.substr(0, m_Token.start());
            else if (m_Token.start() != 0)
                return m_Str;
            else
                return {};
        }

        [[nodiscard]] std::string_view token() const
        {
            if (!m_Token)
                return {};
            return m_Str.substr(m_Token.start(), m_Token.end());
        }

        [[nodiscard]] std::string_view remainder() const
        {
            if (m_Token || m_Token.start() == 0)
                return m_Str.substr(m_Token.end());
            return {};
        }
    private:
        std::string_view m_Str;
        Subrange m_Token;
        TokenFinder m_Finder;
    };
}
