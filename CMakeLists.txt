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
    include/Ystring/CaseInsensitive.hpp
    include/Ystring/Char32Span.hpp
    include/Ystring/CharClass.hpp
    include/Ystring/ConvertCase.hpp
    include/Ystring/DecodeUtf8.hpp
    include/Ystring/EncodeUtf8.hpp
    include/Ystring/Subrange.hpp
    include/Ystring/TokenIterator.hpp
    include/Ystring/Algorithms.hpp
    include/Ystring/YstringDefinitions.hpp
    src/Ystring/CharClass.cpp
    src/Ystring/CharClassTables.hpp
    include/Ystring/CodePointPredicates.hpp
    src/Ystring/ConvertCase.cpp
    src/Ystring/LowerCaseTables.hpp
    src/Ystring/TitleCaseTables.hpp
    src/Ystring/UpperCaseTables.hpp
    src/Ystring/Subrange.cpp
    src/Ystring/Algorithms.cpp
    include/Ystring/CodePointConstants.hpp include/Ystring/Ystring.hpp src/Ystring/Normalize.cpp include/Ystring/Normalize.hpp src/Ystring/Utf32.cpp src/Ystring/Utf32.hpp)

target_include_directories(Ystring2 BEFORE
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

add_library(Ystring2::Ystring2 ALIAS Ystring2)

add_subdirectory(tests/YstringTest)
