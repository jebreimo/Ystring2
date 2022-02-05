//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-08-03.
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <stdexcept>
#include <string>

/** @file
  * @brief Defines the exception thrown by Ystring functions.
  */

namespace ystring
{
    /**
     * @brief The exception class used throughout Ystring.
     */
    class YstringException : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };
}

#define _YSTRING_THROW_3(file, line, msg) \
    throw ::ystring::YstringException(file ":" #line ": " msg)

#define _YSTRING_THROW_2(file, line, msg) \
    _YSTRING_THROW_3(file, line, msg)

#define YSTRING_THROW(msg) \
    _YSTRING_THROW_2(__FILE__, __LINE__, msg)
