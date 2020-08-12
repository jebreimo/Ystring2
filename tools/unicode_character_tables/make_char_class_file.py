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
    "Lu": 1,
    "Ll": 2,
    "Lt": 3,
    "Lm": 4,
    "Lo": 5,
    "Mn": 6,
    "Mc": 7,
    "Me": 8,
    "Nd": 9,
    "Nl": 10,
    "No": 11,
    "Pc": 12,
    "Pd": 13,
    "Ps": 14,
    "Pe": 15,
    "Pi": 16,
    "Pf": 17,
    "Po": 18,
    "Sm": 19,
    "Sc": 20,
    "Sk": 21,
    "So": 22,
    "Zs": 23,
    "Zl": 24,
    "Zp": 25,
    "Cc": 26,
    "Cf": 27,
    "Cs": 28,
    "Co": 29,
    "Cn": 30
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
    "C":   "Other",
    "XLl":  "Alternating upper and lower case, ends on lower",
    "XLu":  "Alternating upper and lower case, ends on upper"
}

KnownRanges = {0x4DB5, 0x9FCC, 0xD7A3, 0xDB7F, 0xDBFF, 0xDFFF, 0xF8FF,
               0x2A6D6, 0x2B734, 0x2B81D, 0xFFFFD, 0x10FFFD}


Template = """\
//****************************************************************************
// Copyright © [[[year]]] Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on [[[date]]]
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <cstdint>

namespace Ystring
{
    constexpr uint8_t ASCII_CHAR_CLASSES[] =
    {
        [[[asciiClasses]]]
    };

    constexpr uint32_t COMPLETE_CHAR_CLASSES[] =
    {
        [[[allClasses]]]
    };
}
"""


def encode_value(index, cls):
    if len(cls) == 2:
        cls_val = Classes[cls]
    elif len(cls) == 4:
        cls_val = (Classes[cls[:2]] << 5) | Classes[cls[2:]]
    else:
        cls_val = 0x400 | Classes[cls[1:]]
    return (index << 11) + cls_val


def get_class_ranges(file_name):
    ranges = []
    range_class = None
    range_start = -1
    range_end = -1
    for line in open(file_name):
        parts = line.split(";")
        char_class = parts[2]
        char_id = int(parts[0], 16)
        if (char_id == range_end + 1 or char_id in KnownRanges) and char_class == range_class:
            range_end = char_id
        else:
            if range_class:
                ranges.append((range_start, range_end, range_class))
            if char_id != range_end + 1:
                ranges.append((range_end + 1, char_id - 1, "Cn"))
            range_class = char_class
            range_start = char_id
            range_end = char_id
    if range_class:
        ranges.append((range_start, range_end, range_class))
    return ranges


def optimize_alternating_case(ranges):
    new_ranges = [ranges[0]]
    for r in ranges[1:]:
        if r[0] == r[1] and r[2] == "Lu":
            if new_ranges[-1][1] + 1 == r[0] and (new_ranges[-1][2] == "XLl" or (new_ranges[-1][2] == "Ll" and new_ranges[-1][0] == new_ranges[-1][1])):
                new_ranges[-1] = (new_ranges[-1][0], r[1], "XLu")
            else:
                new_ranges.append(r)
        elif r[0] == r[1] and r[2] == "Ll":
            if new_ranges[-1][1] + 1 == r[0] and (new_ranges[-1][2] == "XLu" or (new_ranges[-1][2] == "Lu" and new_ranges[-1][0] == new_ranges[-1][1])):
                new_ranges[-1] = (new_ranges[-1][0], r[1], "XLl")
            else:
                new_ranges.append(r)
        else:
            new_ranges.append(r)
    return new_ranges


def optimize_unassigned(ranges):
    new_ranges = [ranges[0]]
    for r in ranges[1:]:
        q = new_ranges[-1]
        if r[0] == r[1] and len(q[2]) == 2 and q[2] not in ("XLl", "XLu"):
            new_ranges[-1] = (q[0], r[1], q[2] + r[2])
        else:
            new_ranges.append(r)
    return new_ranges


def make_table_rows(ranges):
    result = []
    n = 5
    remainder = len(ranges) % n or n
    for i in range(0, len(ranges) - remainder, n):
        values = (encode_value(r[1], r[2]) for r in ranges[i:i + n])
        result.append(", ".join("0x%08X" % v for v in values) + ",")
    values = (encode_value(r[1], r[2]) for r in ranges[len(ranges) - remainder:])
    result.append(", ".join("0x%08X" % v for v in values))
    return result


def get_ascii_table(file_name):
    result = []
    output_line = []
    for line in open(file_name):
        parts = line.split(";")
        char_class = parts[2]
        char_id = int(parts[0], 16)
        if char_id >= 128:
            break
        if len(output_line) == 8:
            result.append(", ".join(output_line) + ",")
            output_line = []
        output_line.append("0x%02X" % (Classes[char_class] - 1))
    result.append(", ".join(output_line))
    return result


def write_cpp(ascii_classes, all_classes):
    date = datetime.date.today()
    codegen_params = {'year': date.year,
                      'date': "%d-%02d-%02d" % (date.year, date.month, date.day),
                      'asciiClasses': ascii_classes,
                      'allClasses': all_classes}
    print(codegen.make_text(Template, codegen.DictExpander(codegen_params)))


def main(args):
    if len(args) != 1:
        print("usage: %s <unicode data file>" % os.path.basename(sys.argv[0]))
        return 1
    ranges = get_class_ranges(args[0])
    ranges = optimize_alternating_case(ranges)
    ranges = optimize_unassigned(ranges)
    # for r in ranges:
    #     print("%X, %X, %s" % r)
    rows = make_table_rows(ranges)
    write_cpp(get_ascii_table(args[0]), rows)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
