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

add_library(Ystring2_Conversion
    include/Ystring/Convert.hpp
    include/Ystring/Converter.hpp
    include/Ystring/Encoding.hpp
    include/Ystring/ErrorHandlingPolicy.hpp
    src/Ystring/Convert.cpp
    src/Ystring/Converter.cpp
    src/Ystring/DecoderBase.cpp
    src/Ystring/DecoderBase.hpp
    src/Ystring/EncoderBase.hpp
    src/Ystring/EncoderBase.cpp
    src/Ystring/Utf8Decoder.cpp
    src/Ystring/Utf8Decoder.hpp
    src/Ystring/Utf8Encoder.cpp
    src/Ystring/Utf8Encoder.hpp
    src/Ystring/Encoding.cpp
    src/Ystring/Utf16Decoder.hpp
    src/Ystring/Utf16Encoder.hpp
    src/Ystring/Utf32Encoder.hpp
    )


target_include_directories(Ystring2_Conversion BEFORE
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

add_library(Ystring2::Conversion ALIAS Ystring2_Conversion)

add_subdirectory(tests/YstringTest)
