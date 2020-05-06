//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-07-15.
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

/** @file
  * @brief Defines the EscapeType enum.
  */

namespace Ystring
{
    /** @brief "Namespace" for the EscapeType enum values.
      */
    struct EscapeType
    {
        /** @brief The EscapeType enum values used by the escape and unescape
          *     functions.
          */
        enum Type
        {
            /** @brief Escape control characters in the ASCII range with
              *     backslashes.
              */
            BACKSLASH,
            /** @brief Escape control characters and non-ASCII characters with
              *     backslashes.
              */
            BACKSLASH_ASCII,
            BACKSLASH_ASCII_SMART,
            /**
              * The difference between JSON and BACKSLASH is that the former
              * escapes characters without single letter escape as
              * 16-bit unicode code points (e.g. \\u0001) no matter what the
              * string's encoding is.
              */
            JSON,
            JSON_ASCII,
            /** @brief Escape the characters that are illegal in an URL query
              *     with percentage characters.
              */
            URL_QUERY,
            /** @brief Escape characters that are illegal in XML text
              *     with ampersands.
              */
            XML_TEXT,
            /** @brief Escape characters that are illegal in XML attributes
              *     with ampersands.
              */
            XML_ATTRIBUTE
        };
    };

    /** @brief A shorter alias for EscapeType::Type.
      */
    typedef EscapeType::Type EscapeType_t;
}
