##****************************************************************************
## Copyright © 2020 Jan Erik Breimo. All rights reserved.
## Created by Jan Erik Breimo on 2020-05-04.
##
## This file is distributed under the Simplified BSD License.
## License text is included with the source distribution.
##****************************************************************************
cmake_minimum_required(VERSION 3.13)
project(Ystring2 VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 17)

add_library(Ystring2_CodePoints
    include/Ystring/Char32Span.hpp
    include/Ystring/DecodeUtf8.hpp
    include/Ystring/DecodeUtf16.hpp
    include/Ystring/EncodeUtf8.hpp
    include/Ystring/EncodeUtf16.hpp
    include/Ystring/Subrange.hpp
    include/Ystring/TokenIterator.hpp
    include/Ystring/UnicodeChars.hpp
    include/Ystring/Utf8String.hpp
    include/Ystring/YstringDefinitions.hpp
    include/Ystring/YstringException.hpp
    src/Ystring/CodePointPredicates.hpp
    src/Ystring/Subrange.cpp
    src/Ystring/Utf8String.cpp
    src/Ystring/YstringThrow.hpp
    )

target_include_directories(Ystring2_CodePoints BEFORE
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

add_library(Ystring2::CodePoints ALIAS Ystring2_CodePoints)

#add_library(Ystring2_Conversion
#    include/Ystring/Encodings.hpp)

add_subdirectory(tests/YstringTest)
