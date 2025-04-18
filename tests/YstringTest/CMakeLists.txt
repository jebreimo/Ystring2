# ===========================================================================
# Copyright © 2020 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2020-05-04.
#
# This file is distributed under the Zero-Clause BSD License.
# License text is included with the source distribution.
# ===========================================================================
cmake_minimum_required(VERSION 3.16)

include(FetchContent)
FetchContent_Declare(catch
    GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
    GIT_TAG "v3.4.0"
    )
FetchContent_MakeAvailable(catch)

add_executable(YstringTest
    Utf8Chars.hpp
    test_Algorithms.cpp
    test_CharClass.cpp
    test_ConvertCase.cpp
    test_DecodeUtf8.cpp
    test_EncodeUtf8.cpp
    test_Escape.cpp
    test_Normalize.cpp
    test_Unescape.cpp
    test_Utf32.cpp
    U8Adapter.hpp
)

target_link_libraries(YstringTest
    PRIVATE
        Ystring2::Ystring
        Catch2::Catch2WithMain
    )

target_compile_options(YstringTest
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
    )

target_include_directories(YstringTest
    PRIVATE
        ../../src
    )

ystring_enable_all_warnings(YstringTest)

add_test(NAME YstringTest COMMAND ${CMAKE_CURRENT_BINARY_DIR}/YstringTest)
