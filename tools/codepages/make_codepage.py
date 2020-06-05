#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
# ===========================================================================
# Copyright © 2020 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2020-05-27.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.
# ===========================================================================
import codegen
import os
import sys


Template = """\
//****************************************************************************
// Copyright © [[[year]]] Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on [[[date]]]
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************

namespace Ystring
{
    [[[arrays]]]
    
    std::tuple<CodePageId, const uint32_t*, size_t> CODE_PAGE_CHARS_MAP[] = {
        {Encoding, WIN_CP1252_CHARS, sizeof(WIN_CP1252_CHARS) / sizeof(uint32_t)}
    };
    
    std::pair<const uint32_t*, size_t> getCodePageChars(int codepageId)
    {
        
    }
}
"""


def encode_char(index, value):
    return index << 24 | value


def append_chars(chars, first_index, first_value, count):
    if count:
        chars.append(encode_char(first_index, first_value))
    if count > 1:
        chars.append(count - 1)


def get_mapping(line):
    line = line.lstrip()
    if not line or line[0] == "#":
        return None
    parts = line.split("\t", maxsplit=2)
    if len(parts) < 2:
        return None
    index = int(parts[0], 0)
    assert index <= 0xFF
    if parts[1] and not parts[1].isspace():
        unicode = int(parts[1], 0)
        assert unicode <= 0xFFFFF
    else:
        unicode = index
    return index, unicode


def get_deviating_char_values(file_name):
    chars = []
    first_value = 0
    count = 0
    for line in open(file_name):
        mapping = get_mapping(line)
        if not mapping:
            continue
        index, value = mapping
        if value == first_value + count:
            count += 1
        else:
            if first_value + count != index:
                append_chars(chars, index - count, first_value, count)
            first_value = value
            count = 1
    if first_value + count != 256:
        append_chars(chars, 256 - count, first_value, count)
    return chars


def read_config_file(file_name):
    root = os.path.dirname(os.path.abspath(file_name))
    config = []
    for line in open(file_name):
        line = line.strip()
        if not line:
            continue
        parts = line.split(";")
        assert len(parts) == 2
        path = os.path.join(root, parts[1])
        config.append((parts[0], path))
    return config


def main(args):
    if len(args) != 1:
        print("usage: %s <configuration file>" % sys.argv[0])
        return 1
    config = read_config_file(args[0])
    arrays = {}
    enums = {}
    counters = {}
    for label, path in config:
        print(f"{label} {path}")
        name = os.path.basename(path)
        name = os.path.splitext(name)[0].replace("-", "_")
        counter = counters.get(label, (len(counters) + 1) * 100 - 1) + 1
        counters[label] = counter
        enums.setdefault(label, [])\
            .append(f"constexpr int {label}_{name} = {counter};")
        chars = get_deviating_char_values(path)
        if not chars:
            continue
        lines = [f"constexpr uint32_t {label}_{name}_CHARS[] = {{"]
        strs = []
        for c in chars:
            if c > 255:
                strs.append("0x%08X" % c)
            else:
                strs.append("%d" % c)
        value_lines = codegen.join(strs, line_width=70, sep=", ", newline_sep=",")
        lines.extend("    " + s for s in value_lines)
        lines.append("};")
        arrays.setdefault(label, []).append(lines)
    lines = []
    for key in arrays:
        lines.append(f"#ifdef YSTRING_ENABLE_{key}_CHARS")
        for arr_lines in arrays[key]:
            lines.append("")
            lines.extend("    " + s for s in arr_lines)
        lines.append("")
        lines.append("#endif")
    # print("\n".join(lines))
    print("\n".join(enums))
    return 0


sys.exit(main(sys.argv[1:]))
