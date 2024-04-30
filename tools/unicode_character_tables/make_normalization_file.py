#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
# ===========================================================================
# Copyright © 2020 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2020-08-13.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.
# ===========================================================================
import datetime
import os
import sys
import codegen

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
    constexpr uint64_t TO_DECOMPOSED[] =
    {
        [[[to_normalized]]]
    };

    constexpr uint64_t TO_COMPOSED[] =
    {
        [[[to_denormalized]]]
    };
}
"""


def get_rules(file_name):
    result = []
    for line in open(file_name):
        line = line.strip()
        if not line:
            continue
        parts = line.split(";")
        char_id = int(parts[0], 16)
        if not parts[5] or parts[5][0] == '<':
            continue
        values = [int(p, 16) for p in parts[5].split()]
        if len(values) != 2:
            continue
        result.append((char_id, values))
    return result


def encode_decomposed(rule):
    return (rule[0] << 40) | (rule[1][0] << 20) | rule[1][1]


def encode_composed(rule):
    return (rule[1][0] << 40) | (rule[1][1] << 20) | rule[0]


def make_decomposed_table_rows(rules):
    result = []
    n = 3
    remainder = len(rules) % n or n
    for i in range(0, len(rules) - remainder, n):
        values = (encode_decomposed(r) for r in rules[i:i + n])
        result.append(", ".join("0x%016X" % v for v in values) + ",")
    values = (encode_decomposed(r) for r in rules[len(rules) - remainder:])
    result.append(", ".join("0x%016X" % v for v in values))
    return result


def make_composed_table_rows(rules):
    rules = sorted(rules, key=lambda r: (r[1][0], r[1][1]))
    result = []
    n = 3
    remainder = len(rules) % n or n
    for i in range(0, len(rules) - remainder, n):
        values = (encode_composed(r) for r in rules[i:i + n])
        result.append(", ".join("0x%016X" % v for v in values) + ",")
    values = (encode_composed(r) for r in rules[len(rules) - remainder:])
    result.append(", ".join("0x%016X" % v for v in values))
    return result


def write_cpp(decomposed, composed):
    date = datetime.date.today()
    codegen_params = {'year': date.year,
                      'date': "%d-%02d-%02d" % (date.year, date.month, date.day),
                      'to_normalized': decomposed,
                      'to_denormalized': composed}
    print(codegen.make_text(Template, codegen.DictExpander(codegen_params)))


def analyze(rules):
    import math
    for i in range(1, 6):
        f = lambda n: (n >> i, n & ((1 << i) - 1))
        la, lb, lc = set(), set(), set()
        for rule in rules:
            # print("%05X -> %05X %05X" % (rule[0], rule[1][0], rule[1][1]))
            # print("%5X %5X -> %5X" % (rule[1][0], rule[1][1], rule[0]))
            a, b, c = rule[0], rule[1][0], rule[1][1]
            a, b, c = f(a), f(b), f(c)
            la.add(a[0])
            lb.add(b[0])
            lc.add(c[0])
        # print("%04X %02X -> %04X %02X %04X %02X" % (a[0], a[1], b[0], b[1], c[0], c[1]))
        m, n, o = len(la), len(lb), len(lc)
        print("Shift %d: %d %d %d" % (i, m, n, o))
        bits = sum(int(math.ceil(math.log2(v))) for v in (m, n, o)) + 3 * i
        print(f"Bits = {bits}")


def main(args):
    if len(args) != 1:
        print("usage: %s <unicode data file>" % os.path.basename(sys.argv[0]))
        return 1
    rules = get_rules(args[0])
    # analyze(rules)
    write_cpp(make_decomposed_table_rows(rules),
              make_composed_table_rows(rules))


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
