//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-06-20.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Utf8Encoder.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Test Utf8Encoder to string")
{
    std::u32string str32(U"AÆΩ F");
    Ystring::Utf8Encoder encoder;
    std::string result;
    REQUIRE(encoder.encode(str32.data(), str32.size(), result) == 5);
    REQUIRE(result == u8"AÆΩ F");
}

TEST_CASE("Test Utf8Encoder to char array")
{
    auto s = [](size_t a, size_t b){return std::pair(a, b);};
    std::u32string str32(U"AÆΩ F");
    Ystring::Utf8Encoder encoder;
    char result[7];
    REQUIRE(encoder.encode(str32.data(), str32.size(), result, 2) == s(1, 1));
    REQUIRE(encoder.encode(str32.data(), str32.size(), result, 3) == s(2, 3));
    REQUIRE(encoder.encode(str32.data(), str32.size(), result, 7) == s(5, 7));
}