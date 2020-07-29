//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-07-29.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Ystring/YstringDefinitions.hpp"

namespace Ystring
{
    YSTRING_API char32_t lower(char32_t codePoint);

    YSTRING_API char32_t title(char32_t codePoint);

    YSTRING_API char32_t upper(char32_t codePoint);
}
