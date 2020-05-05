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

add_library(Ystring2
    include/Ystring/Unicode/UnicodeChars.hpp
    src/Ystring/Encodings/DecodeUtf8.hpp
    src/Ystring/Encodings/DecodeUtf8-impl.hpp
    src/Ystring/Encodings/EncodeUtf8.cpp
    src/Ystring/Encodings/EncodeUtf8.hpp
    src/Ystring/Encodings/EncodeUtf8-impl.hpp
    )

target_include_directories(Ystring2 BEFORE
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

add_library(Ystring2::Ystring2 ALIAS Ystring2)

add_subdirectory(tests/YstringTest)
