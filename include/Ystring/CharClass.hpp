//****************************************************************************
// Copyright © 2013 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2013-07-01
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cstdint>
#include "Ystring/YstringDefinitions.hpp"

/** @file
  * @brief Defines the CharClass enum and the get_char_class function.
  */

namespace ystring
{
    /** @brief CharClass provides a "namespace" for the unicode character
      *     class values.
      */
    struct CharClass
    {
        /** @brief Constants for all unicode character classes.
          */
        enum Type : uint32_t
        {
            /// An uppercase letter (Lu)
            UPPERCASE_LETTER =      0x00000001ul,
            /// A lowercase letter (Ll)
            LOWERCASE_LETTER =      0x00000002ul,
            /// A digraphic character, with first part uppercase (Lt)
            TITLECASE_LETTER =      0x00000004ul,
            /// Lu | Ll | Lt
            CASED_LETTER =          0x00000007ul,
            /// A modifier letter (Lm)
            MODIFIER_LETTER =       0x00000008ul,
            /// Other letters, including syllables and ideographs (Lo)
            OTHER_LETTER =          0x00000010ul,
            /// Lu | Ll | Lt | Lm | Lo
            LETTER =                0x0000001Ful,
            /// A nonspacing combining mark (zero advance width) (Mn)
            NONSPACING_MARK =       0x00000020ul,
            /// A spacing combining mark (positive advance width) (Mc)
            SPACING_MARK =          0x00000040ul,
            /// An enclosing combining mark (Me)
            ENCLOSING_MARK =        0x00000080ul,
            /// Mn | Mc | Me
            MARK =                  0x000000E0ul,
            /// A decimal digit (Nd)
            DECIMAL_NUMBER =        0x00000100ul,
            /// A letterlike numeric character (Nl)
            LETTER_NUMBER =         0x00000200ul,
            /// a numeric character of other type (No)
            OTHER_NUMBER =          0x00000400ul,
            /// Nd | Nl | No
            NUMBER =                0x00000700ul,
            /// a connecting punctuation mark, like a tie (Pc)
            CONNECTOR_PUNCTUATION = 0x00000800ul,
            /// a dash or hyphen punctuation mark (Pd)
            DASH_PUNCTUATION =      0x00001000ul,
            /// an opening punctuation mark (of a pair) (Ps)
            OPEN_PUNCTUATION =      0x00002000ul,
            /// a closing punctuation mark (of a pair) (Pe)
            CLOSE_PUNCTUATION =     0x00004000ul,
            /// an initial quotation mark (Pi)
            INITIAL_PUNCTUATION =   0x00008000ul,
            /// a final quotation mark (Pf)
            FINAL_PUNCTUATION =     0x00010000ul,
            /// a punctuation mark of other type (Po)
            OTHER_PUNCTUATION =     0x00020000ul,
            /// Pc | Pd | Ps | Pe | Pi | Pf | Po
            PUNCTUATION =           0x0003F800ul,
            /// a symbol of mathematical use (Sm)
            MATH_SYMBOL =           0x00040000ul,
            /// a currency sign (Sc)
            CURRENCY_SYMBOL =       0x00080000ul,
            /// a non-letterlike modifier symbol (Sk)
            MODIFIER_SYMBOL =       0x00100000ul,
            /// a symbol of other type (So)
            OTHER_SYMBOL =          0x00200000ul,
            /// Sm | Sc | Sk | So
            SYMBOL =                0x003C0000ul,
            /// a space character (of various non-zero widths) (Zs)
            SPACE_SEPARATOR =       0x00400000ul,
            /// U+2028 LINE SEPARATOR only (Zl)
            LINE_SEPARATOR =        0x00800000ul,
            /// U+2029 PARAGRAPH SEPARATOR only (Zp)
            PARAGRAPH_SEPARATOR =   0x01000000ul,
            /// Zs | Zl | Zp
            SEPARATOR =             0x01C00000ul,
            /// a C0 or C1 control code (Cc)
            CONTROL =               0x02000000ul,
            /// a format control character (Cf)
            FORMAT =                0x04000000ul,
            /// a surrogate code point (Cs)
            SURROGATE =             0x08000000ul,
            /// a private-use character (Co)
            PRIVATE_USE =           0x10000000ul,
            /// a reserved unassigned code point or a noncharacter (Cn)
            UNASSIGNED =            0x20000000ul,
            /// Cc | Cf | Cs | Co | Cn
            OTHER =                 0x3E000000ul
        };
    };

    /** @brief A shorter alias for CharClass::Type.
      */
    typedef CharClass::Type CharClass_t;

    /** @brief Returns the unicode character class of character @a code_point.
      */
    YSTRING_API CharClass_t get_char_class(char32_t code_point);
}
