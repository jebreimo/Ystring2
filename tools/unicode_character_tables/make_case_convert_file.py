# -*- coding: utf-8 -*-
# ****************************************************************************
# Copyright © 2012 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2012-06-28.
#
# This file is distributed under the Simplified BSD License.
# License text is included with the source distribution.
# ****************************************************************************
import datetime
import os
import sys
import codegen


AlternativeIndexes = {
    "Upper": -3,
    "Lower": -2,
    "Title": -1
}


def get_most_prevalent_diff(values):
    diffs = {}
    for v, w in values:
        if not diffs.get(w - v):
            diffs[w - v] = 1
        else:
            diffs[w - v] += 1
    best_diff, max_count = 0, 0
    for diff in diffs:
        if diffs[diff] > max_count:
            best_diff, max_count = diff, diffs[diff]
    return best_diff, max_count


def make_diff_set(major, diff, values):
    bit_set = 0
    nbit_set = 0xFFFFFFFF
    remainder = []
    for v, w in values:
        if w - v == diff:
            bit_set |= 1 << (v - major)
        else:
            remainder.append((v, w))
        nbit_set ^= 1 << (v - major)
    return (major, diff, bit_set, nbit_set), remainder


template = """\
//****************************************************************************
// Copyright © [[[year]]] Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on [[[date]]].
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "CharMappingTypes.hpp"

namespace Ystring
{
    constexpr CompactCharMapping COMPACT_[[[name]]]_CASE[] =
    {
        [[[compact_mappings]]]
    };

    constexpr CharMapping [[[name]]]_CASE[] =
    {
        [[[mappings]]]
    };
}
"""


def write_cpp(diff_sets, mappings, name):
    compact_mappings = []
    for diff_set in diff_sets:
        if diff_set[1] < 0:
            s = "{0x%06X, 0x%08X, 0x%08X, 0x%08X}," % (
                diff_set[0], 2**32 + diff_set[1], diff_set[2], diff_set[3])
        else:
            s = "{0x%06X, 0x%08X, 0x%08X, 0x%08X}," % diff_set
        compact_mappings.append(s)
    expanded_mappings = []
    for ch, alt_ch in mappings:
        expanded_mappings.append("{0x%06X, 0x%06X}," % (ch, alt_ch))
    date = datetime.date.today()
    codegen_params = dict(
        year=date.year,
        date="%d-%02d-%02d" % (date.year, date.month, date.day),
        name=name.upper(),
        compact_mappings=compact_mappings,
        mappings=expanded_mappings)
    print(codegen.make_text(template, codegen.DictExpander(codegen_params)))


def main(args):
    if len(args) != 2:
        print("usage: %s <name> <unicode data file>"
              % os.path.basename(sys.argv[0]))
        print("<name> must be 'upper', 'lower' or 'title'")
        return 1
    name = args[0].title()
    alternative_index = AlternativeIndexes[name]
    chars = {}
    for line in open(args[1]):
        parts = line.split(";")
        parts[alternative_index] = parts[alternative_index].strip()
        if parts[alternative_index]:
            ch = int(parts[0], 16)
            alt_ch = int(parts[alternative_index], 16)
            major = (ch // 32) * 32
            char_list = chars.get(major)
            if not char_list:
                chars[major] = [(ch, alt_ch)]
            else:
                char_list.append((ch, alt_ch))
    diff_sets = []
    mappings = []
    for major in sorted(chars):
        diff, count = get_most_prevalent_diff(chars[major])
        if count > 2:
            diff_set, remainder = make_diff_set(major, diff, chars[major])
            diff_sets.append(diff_set)
            mappings.extend(remainder)
        else:
            mappings.extend(chars[major])
    write_cpp(diff_sets, mappings, name)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
