//****************************************************************************
// Copyright © 2013 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2013-07-01.
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <functional>
#include "Ystring/YstringDefinitions.hpp"

namespace Ystring { namespace Unicode
{
    YSTRING_API char32_t lower(char32_t ch);
    YSTRING_API char32_t title(char32_t ch);
    YSTRING_API char32_t upper(char32_t ch);
}}
