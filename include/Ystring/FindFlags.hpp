//****************************************************************************
// Copyright © 2013 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2013-07-01.
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

/** @file
  * @brief Defines the FindFlags enum.
  */

namespace Ystring
{
    /** @brief "Namespace" for the FindFlags enum values.
      */
    struct FindFlags
    {
        /** @brief The flags that can be passed to the various find-functions.
          */
        enum Type
        {
            /// Case-sensitive matching of strings.
            DEFAULTS = 0,
            /// Case-insensitive matching of strings.
            CASE_INSENSITIVE = 1
        };

        typedef unsigned Flags;

        static bool isCaseInsensitive(Flags flags)
        {
            return (flags & CASE_INSENSITIVE) != 0;
        }
    };

    /** @brief A shorter alias for FindFlags::Flags.
      */
    typedef FindFlags::Flags FindFlags_t;
}
