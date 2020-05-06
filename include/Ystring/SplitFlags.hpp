//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-06-18.
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "FindFlags.hpp"

/** @file
  * @brief Defines the SplitFlags enum.
  */

namespace Ystring
{
    /** @brief "Namespace" for the FindFlags enum values.
      */
    struct SplitFlags
    {
        /** @brief The flags that can be passed to the
          *     various split-functions.
          */
        enum Type
        {
            /** @brief Case-sensitive matching of delimiters, no parts
              *     are ignored.
              */
            DEFAULTS = 0,
            /** @brief Case-insensitive matching of delimiters.
              */
            CASE_INSENSITIVE = 1,
            /** @brief Don't include an empty part in the result if the string
              *     starts on a delimiter.
              */
            IGNORE_EMPTY_FRONT = 2,
            /** @brief Don't include an empty part in the result where the
              *     string contains two consecutive separators.
              */
            IGNORE_EMPTY_INTERMEDIATES = 4,
            /** @brief Don't include an empty part in the result if the string
               *    ends after a delimiter.
               */
            IGNORE_EMPTY_BACK = 8,
            /** @brief Don't include empty parts in the result.
              */
            IGNORE_EMPTY = IGNORE_EMPTY_FRONT | IGNORE_EMPTY_INTERMEDIATES |
                           IGNORE_EMPTY_BACK,
            /** @brief Don't include the remainder in the result after the
              *     maximum number of splits have been performed.
              *
              * This flag can improve preformance and memory usage when one is
              * only interested in a few tokens at the start or end of
              * a string.
              */
            IGNORE_REMAINDER = 0x10
        };

        typedef unsigned Flags;

        static bool isCaseInsensitive(Flags flags)
        {
            return (flags & CASE_INSENSITIVE) != 0;
        }

        static bool ignoreEmpty(Flags flags)
        {
            return (flags & IGNORE_EMPTY) == IGNORE_EMPTY;
        }

        static bool ignoreEmptyFront(Flags flags)
        {
            return (flags & IGNORE_EMPTY_FRONT) != 0;
        }

        static bool ignoreEmptyIntermediates(Flags flags)
        {
            return (flags & IGNORE_EMPTY_INTERMEDIATES) != 0;
        }

        static bool ignoreEmptyBack(Flags flags)
        {
            return (flags & IGNORE_EMPTY_BACK) != 0;
        }

        static bool ignoreRemainder(Flags flags)
        {
            return (flags & IGNORE_REMAINDER) != 0;
        }

        static FindFlags_t toFindFlags(Flags flags)
        {
            return isCaseInsensitive(flags)
                    ? FindFlags::CASE_INSENSITIVE
                    : FindFlags::DEFAULTS;
        }
    };

    /** @brief A shorter alias for SplitFlags::Flags.
      */
    typedef SplitFlags::Flags SplitFlags_t;
}
