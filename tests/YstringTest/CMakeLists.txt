# ===========================================================================
# Copyright © 2020 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2020-05-04.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.
# ===========================================================================
cmake_minimum_required(VERSION 3.13)

include(FetchContent)
FetchContent_Declare(catch
    GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
    GIT_TAG "v2.11.1"
    )
FetchContent_GetProperties(catch)
if (NOT catch_POPULATED)
    FetchContent_Populate(catch)
    add_subdirectory(${catch_SOURCE_DIR} ${catch_BINARY_DIR})
endif ()

add_executable(YstringTest
    Utf8Chars.hpp
    YstringTestMain.cpp
    test_Algorithms.cpp
    test_CharClass.cpp
    test_ConvertCase.cpp
    test_DecodeUtf8.cpp
    test_EncodeUtf8.cpp
    test_Escape.cpp
    test_Normalize.cpp
    test_Utf32.cpp
    )

target_link_libraries(YstringTest
    PRIVATE
        Ystring2::Ystring
        Catch2::Catch2
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
