#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
# ===========================================================================
# Copyright © 2020 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2020-05-08.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.
# ===========================================================================
import sys


def get_ranges(file_name):
    result = []
    mark = False
    for line in open(file_name):
        parts = line.split(";")
        char_class = parts[2]
        char_id = int(parts[0], 16)
        if char_class == "Mn":
            if not mark:
                result.append((char_id, char_id))
            else:
                result[-1] = (result[-1][0], char_id)
            mark = True
        else:
            mark = False
    return result


def main(args):
    if len(args) != 1:
        print("usage: %s <unicode file>" % sys.argv[0])
        return 1
    i = 1
    for b, e in get_ranges(args[0]):
        print(f"{i}: {b:05x} - {e:05x}")
        i += 1
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
