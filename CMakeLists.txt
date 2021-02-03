##****************************************************************************
## Copyright © 2020 Jan Erik Breimo. All rights reserved.
## Created by Jan Erik Breimo on 2020-05-04.
##
## This file is distributed under the Simplified BSD License.
## License text is included with the source distribution.
##****************************************************************************
cmake_minimum_required(VERSION 3.13)
project(Ystring VERSION 2.0.2)

set(CMAKE_CXX_STANDARD 17)

# Set YSTRING_MASTER_PROJECT to ON if Ystring is the root level project.
if (NOT DEFINED YSTRING_MASTER_PROJECT)
    if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(YSTRING_MASTER_PROJECT ON)
    else()
        set(YSTRING_MASTER_PROJECT OFF)
    endif()
endif ()

# Test option
option(YSTRING_BUILD_TESTS "Build tests" ${YSTRING_MASTER_PROJECT})

# Install option
option(YSTRING_INSTALL "Generate the install target" ${YSTRING_MASTER_PROJECT})

function(ystring_enable_all_warnings target)
    target_compile_options(${target}
        PRIVATE
            $<$<CXX_COMPILER_ID:Clang>:-Wall>
            $<$<CXX_COMPILER_ID:GNU>:-Wall>
            $<$<CXX_COMPILER_ID:AppleClang>:-Wall>
            $<$<CXX_COMPILER_ID:MSVC>:/W4>)
endfunction()

include(GNUInstallDirs)

configure_file(include/Ystring/YstringVersion.hpp.in YstringVersion.hpp @ONLY)

add_library(Ystring
    include/Ystring/Algorithms.hpp
    include/Ystring/CaseInsensitive.hpp
    include/Ystring/Char32Span.hpp
    include/Ystring/CharClass.hpp
    include/Ystring/CodePointConstants.hpp
    include/Ystring/CodePointPredicates.hpp
    include/Ystring/ConvertCase.hpp
    include/Ystring/DecodeUtf8.hpp
    include/Ystring/Escape.hpp
    include/Ystring/Normalize.hpp
    include/Ystring/Subrange.hpp
    include/Ystring/TokenIterator.hpp
    include/Ystring/Utf32.hpp
    include/Ystring/Ystring.hpp
    include/Ystring/YstringDefinitions.hpp
    src/Ystring/Algorithms.cpp
    src/Ystring/AlgorithmUtilities.hpp
    src/Ystring/AlgorithmUtilities.cpp
    src/Ystring/CharClass.cpp
    src/Ystring/CharClassTables.hpp
    src/Ystring/ConvertCase.cpp
    src/Ystring/EncodeUtf8.hpp
    src/Ystring/Escape.cpp
    src/Ystring/LowerCaseTables.hpp
    src/Ystring/Normalize.cpp
    src/Ystring/Subrange.cpp
    src/Ystring/TitleCaseTables.hpp
    src/Ystring/UpperCaseTables.hpp
    src/Ystring/Utf32.cpp
    )

target_include_directories(Ystring BEFORE
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

target_compile_definitions(Ystring
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:YSTRING_EXPORTS>
    )

target_compile_options(Ystring
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
    )

ystring_enable_all_warnings(Ystring)

add_library(Ystring2::Ystring ALIAS Ystring)

if (YSTRING_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/YstringTest)
endif()


export(TARGETS Ystring
    NAMESPACE Ystring2::
    FILE YstringConfig.cmake)

if (YSTRING_INSTALL)
    install(TARGETS Ystring
        EXPORT YstringConfig
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )

    install(EXPORT YstringConfig
        FILE
            YstringConfig.cmake
        NAMESPACE
            Ystring2::
        DESTINATION
            ${CMAKE_INSTALL_LIBDIR}/cmake/Ystring
        )

    file(GLOB_RECURSE PUBLIC_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
    install(
        FILES
            ${PUBLIC_INCLUDES}
            ${CMAKE_CURRENT_BINARY_DIR}/YstringVersion.hpp
        DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR}/Ystring
        )

    write_basic_package_version_file(YstringConfigVersion.cmake
        COMPATIBILITY SameMajorVersion
        )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/YstringConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Ystring
        )
endif()
