//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-05-06.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "Ystring/YstringException.hpp"

#define _YSTRING_THROW_3(file, line, msg) \
    throw ::Ystring::YstringException(file ":" #line ": " msg)

#define _YSTRING_THROW_2(file, line, msg) \
    _YSTRING_THROW_3(file, line, msg)

#define YSTRING_THROW(msg) \
    _YSTRING_THROW_2(__FILE__, __LINE__, msg)
