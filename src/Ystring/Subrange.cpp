//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-05-07.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Subrange.hpp"
#include <ostream>

namespace ystring
{
    std::ostream& operator<<(std::ostream& os, const Subrange& s)
    {
        return os << "{offset: " << s.offset
                  << ", length: " << s.length << '}';
    }
}
