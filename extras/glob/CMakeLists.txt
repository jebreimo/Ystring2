# ===========================================================================
# Copyright © 2024 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2024-04-26.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.
# ===========================================================================
cmake_minimum_required(VERSION 3.16)
project(glob)

add_executable(glob
    Glob.cpp
    )

target_link_libraries(glob
    Ystring2::Ystring
    )
