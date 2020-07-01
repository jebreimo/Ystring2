//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-06-25.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Utf16Decoder.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Test UTF-16 decoder")
{
    std::string s("\x00A\x00B\x00C\x00D\x00E\x00F\x00G", 14);
    Ystring::Utf16Decoder<Ystring::IsLittleEndian> decoder;
    std::vector<char32_t> u(7);
    auto [m, n] = decoder.decode(s.data(), s.size(), u.data(), u.size());
    REQUIRE(m == 14);
    REQUIRE(n == 7);
}
