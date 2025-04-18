//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-02-02.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Escape.hpp"
#include <catch2/catch_test_macros.hpp>
#include "U8Adapter.hpp"

using namespace ystring;

TEST_CASE("Test escape_json")
{
    REQUIRE(escape_json("AB\t \\ \17ty") == "AB\\t \\\\ \\u000Fty");
}
