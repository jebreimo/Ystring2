# -*- coding: utf-8 -*-
# ****************************************************************************
# Copyright © 2012 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2012-08-18.
#
# This file is distributed under the Simplified BSD License.
# License text is included with the source distribution.
# ****************************************************************************
import datetime
import os
import sys
import codegen

# Abbr    Long    Description
# Lu  Uppercase_Letter    an uppercase letter
# Ll  Lowercase_Letter    a lowercase letter
# Lt  Titlecase_Letter    a digraphic character, with first part uppercase
# LC  Cased_Letter    Lu | Ll | Lt
# Lm  Modifier_Letter a modifier letter
# Lo  Other_Letter    other letters, including syllables and ideographs
# L   Letter  Lu | Ll | Lt | Lm | Lo
# Mn  Nonspacing_Mark a nonspacing combining mark (zero advance width)
# Mc  Spacing_Mark    a spacing combining mark (positive advance width)
# Me  Enclosing_Mark  an enclosing combining mark
# M   Mark    Mn | Mc | Me
# Nd  Decimal_Number  a decimal digit
# Nl  Letter_Number   a letterlike numeric character
# No  Other_Number    a numeric character of other type
# N   Number  Nd | Nl | No
# Pc  Connector_Punctuation   a connecting punctuation mark, like a tie
# Pd  Dash_Punctuation    a dash or hyphen punctuation mark
# Ps  Open_Punctuation    an opening punctuation mark (of a pair)
# Pe  Close_Punctuation   a closing punctuation mark (of a pair)
# Pi  Initial_Punctuation an initial quotation mark
# Pf  Final_Punctuation   a final quotation mark
# Po  Other_Punctuation   a punctuation mark of other type
# P   Punctuation Pc | Pd | Ps | Pe | Pi | Pf | Po
# Sm  Math_Symbol a symbol of mathematical use
# Sc  Currency_Symbol a currency sign
# Sk  Modifier_Symbol a non-letterlike modifier symbol
# So  Other_Symbol    a symbol of other type
# S   Symbol  Sm | Sc | Sk | So
# Zs  Space_Separator a space character (of various non-zero widths)
# Zl  Line_Separator  U+2028 LINE SEPARATOR only
# Zp  Paragraph_Separator U+2029 PARAGRAPH SEPARATOR only
# Z   Separator   Zs | Zl | Zp
# Cc  Control a C0 or C1 control code
# Cf  Format  a format control character
# Cs  Surrogate   a surrogate code point
# Co  Private_Use a private-use character
# Cn  Unassigned  a reserved unassigned code point or a noncharacter
# C   Other   Cc | Cf | Cs | Co | Cn

Classes = {
    "Lu": 0,
    "Ll": 1,
    "Lt": 2,
    "Lm": 3,
    "Lo": 4,
    "Mn": 5,
    "Mc": 6,
    "Me": 7,
    "Nd": 8,
    "Nl": 9,
    "No": 10,
    "Pc": 11,
    "Pd": 12,
    "Ps": 13,
    "Pe": 14,
    "Pi": 15,
    "Pf": 16,
    "Po": 17,
    "Sm": 18,
    "Sc": 19,
    "Sk": 20,
    "So": 21,
    "Zs": 22,
    "Zl": 23,
    "Zp": 24,
    "Cc": 25,
    "Cf": 26,
    "Cs": 27,
    "Co": 28,
    "Cn": 29
}

ClassNames = {
    "Lu":  "Uppercase_Letter",
    "Ll":  "Lowercase_Letter",
    "Lt":  "Titlecase_Letter",
    "LC":  "Cased_Letter",
    "Lm":  "Modifier_Letter",
    "Lo":  "Other_Letter",
    "L":   "Letter",
    "Mn":  "Nonspacing_Mark",
    "Mc":  "Spacing_Mark",
    "Me":  "Enclosing_Mark",
    "M":   "Mark",
    "Nd":  "Decimal_Number",
    "Nl":  "Letter_Number",
    "No":  "Other_Number",
    "N":   "Number",
    "Pc":  "Connector_Punctuation",
    "Pd":  "Dash_Punctuation",
    "Ps":  "Open_Punctuation",
    "Pe":  "Close_Punctuation",
    "Pi":  "Initial_Punctuation",
    "Pf":  "Final_Punctuation",
    "Po":  "Other_Punctuation",
    "P":   "Punctuation",
    "Sm":  "Math_Symbol",
    "Sc":  "Currency_Symbol",
    "Sk":  "Modifier_Symbol",
    "So":  "Other_Symbol",
    "S":   "Symbol",
    "Zs":  "Space_Separator",
    "Zl":  "Line_Separator",
    "Zp":  "Paragraph_Separator",
    "Z":   "Separator",
    "Cc":  "Control",
    "Cf":  "Format",
    "Cs":  "Surrogate",
    "Co":  "Private_Use",
    "Cn":  "Unassigned",
    "C":   "Other"
}

KnownRanges = {0x4DB5, 0x9FCC, 0xD7A3, 0xDB7F, 0xDBFF, 0xDFFF, 0xF8FF,
               0x2A6D6, 0x2B734, 0x2B81D, 0xFFFFD, 0x10FFFD}


def value(index, cls):
    return (index << 8) + Classes[cls]


Template = """\
//****************************************************************************
// Copyright © [[[year]]] Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on [[[date]]]
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "CharClass.hpp"
#include <algorithm>
#include <iterator>

namespace Ystring { namespace Unicode
{
    static uint8_t AsciiCharClasses[] =
    {
        [[[asciiClasses]]]
    };

    static char32_t CompleteCharClasses[] =
    {
        [[[allClasses]]]
    };

    CharClass_t getCharClass(char32_t ch)
    {
        if (ch < 128)
            return CharClass_t(1 << AsciiCharClasses[ch]);
        else if (ch > 0xFFFFFFul)
            return CharClass::UNASSIGNED;
        char32_t key = ch << 8;
        char32_t keyValue = *std::lower_bound(
                std::begin(CompleteCharClasses),
                std::end(CompleteCharClasses),
                key);
        return CharClass_t(1 << (keyValue & 0xFFul));
    }
}}
"""


def get_class_table(fileName):
    values = []
    range_class = None
    range_end = -1
    for line in open(fileName):
        parts = line.split(";")
        char_class = parts[2]
        char_id = int(parts[0], 16)
        if (char_id == range_end + 1 or char_id in KnownRanges) and char_class == range_class:
            range_end = char_id
        else:
            if range_class:
                values.append(value(range_end, range_class))
            if char_id != range_end + 1:
                values.append(value(char_id - 1, "Cn"))
            range_class = char_class
            range_end = char_id
    if range_class:
        values.append(value(range_end, range_class))
    result = []
    line = []
    for i, v in enumerate(values):
        if i != 0 and i % 5 == 0:
            result.append(", ".join(line) + ",")
            line = []
        line.append("0x%08X" % v)
    result.append(", ".join(line))
    return result


def get_ascii_table(fileName):
    result = []
    output_line = []
    for line in open(fileName):
        parts = line.split(";")
        char_class = parts[2]
        char_id = int(parts[0], 16)
        if char_id >= 128:
            break
        if len(output_line) == 8:
            result.append(", ".join(output_line) + ",")
            output_line = []
        output_line.append("0x%02X" % Classes[char_class])
    result.append(", ".join(output_line))
    return result


def write_cpp(asciiClasses, allClasses):
    date = datetime.date.today()
    codegen_params = {'year': date.year,
                      'date': "%d-%02d-%02d" % (date.year, date.month, date.day),
                      'asciiClasses': asciiClasses,
                      'allClasses': allClasses}
    print(codegen.make_text(Template, codegen.DictExpander(codegen_params)))


def main(args):
    if len(args) != 1:
        print("usage: %s <unicode data file>" % os.path.basename(sys.argv[0]))
        return 1
    write_cpp(get_ascii_table(args[0]), get_class_table(args[0]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
