//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-08-17.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/AlgorithmUtilities.hpp"
#include <catch2/catch.hpp>

using namespace Ystring;

TEST_CASE("Test getClampedCodePointPos")
{
    REQUIRE(getCappedCodePointPos(u8"AB£ƒCD‹ß∂GHR", 0) == 0);
    REQUIRE(getCappedCodePointPos(u8"AB£ƒCD‹ß∂GHR", 12) == 19);
    REQUIRE(getCappedCodePointPos(u8"AB£ƒCD‹ß∂GHR", 13) == 19);
    REQUIRE(getCappedCodePointPos(u8"AB£ƒCD‹ß∂GHR", -1) == 18);
    REQUIRE(getCappedCodePointPos(u8"AB£ƒCD‹ß∂GHR", -12) == 0);
    REQUIRE(getCappedCodePointPos(u8"AB£ƒCD‹ß∂GHR", -13) == 0);
}
