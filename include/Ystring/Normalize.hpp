//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-08-15.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <string>
#include <string_view>
#include "Ystring/YstringDefinitions.hpp"

namespace ystring
{
    [[nodiscard]]
    YSTRING_API std::string denormalize(std::string_view str);

    [[nodiscard]]
    YSTRING_API std::string normalize(std::string_view str);

}
