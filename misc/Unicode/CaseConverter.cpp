//****************************************************************************
// Copyright © 2013 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2013-07-01.
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "CaseConverter.hpp"

#include <type_traits>
#include "CharMap.hpp"
#include "StandardLowerCase.hpp"
#include "StandardTitleCase.hpp"
#include "StandardUpperCase.hpp"

namespace Ystring { namespace Unicode
{
    namespace
    {
        CharMap g_LowerMap(
                CompactLowerCaseMappings,
                std::extent<decltype(CompactLowerCaseMappings), 0>::value,
                LowerCaseMappings,
                std::extent<decltype(LowerCaseMappings), 0>::value);
        CharMap g_TitleMap(
                CompactTitleCaseMappings,
                std::extent<decltype(CompactTitleCaseMappings), 0>::value,
                TitleCaseMappings,
                std::extent<decltype(TitleCaseMappings), 0>::value);
        CharMap g_UpperMap(
                CompactUpperCaseMappings,
                std::extent<decltype(CompactUpperCaseMappings), 0>::value,
                UpperCaseMappings,
                std::extent<decltype(UpperCaseMappings), 0>::value);
    }

    char32_t lower(char32_t ch)
    {
        return g_LowerMap.get(ch);
    }

    char32_t title(char32_t ch)
    {
        return g_TitleMap.get(ch);
    }

    char32_t upper(char32_t ch)
    {
        return g_UpperMap.get(ch);
    }
}}
