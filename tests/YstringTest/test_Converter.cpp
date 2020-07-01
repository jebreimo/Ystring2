//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-06-26.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Converter.hpp"

#include <catch2/catch.hpp>

using namespace Ystring;

TEST_CASE("Test UTF-8 -> UTF-8")
{
    Converter converter(Encoding::UTF_8, Encoding::UTF_8);
    std::string s(u8"AäöØ∂ƒ‹‘");
    REQUIRE(converter.getEncodedSize(s.data(), s.size()) == 21);
    std::string t(21, '\0');
    auto [m, n] = converter.convert(s.data(), s.size(), t.data(), t.size());
    REQUIRE(m == 21);
    REQUIRE(n == 21);
    REQUIRE(t == s);
}

TEST_CASE("Test UTF-16 -> UTF-8")
{
    Converter converter(Encoding::UTF_16_NATIVE, Encoding::UTF_8);
    std::u16string s(u"AäöØ∂ƒ‹‘");
    REQUIRE(converter.getEncodedSize(s.data(), s.size() * 2) == 21);
    std::string t(21, '\0');
    auto[m, n] = converter.convert(s.data(), s.size() * 2, t.data(), t.size());
    REQUIRE(m == s.size() * 2);
    REQUIRE(n == 21);
    REQUIRE(t == u8"AäöØ∂ƒ‹‘");
}

TEST_CASE("Test UTF-16BE -> UTF-16LE")
{
    auto BE = [](uint16_t c){return getBigEndian(c);};
    auto LE = [](uint16_t c) {return getLittleEndian(c);};

    Converter converter(Encoding::UTF_16_BE, Encoding::UTF_16_LE);
    std::vector<uint16_t> s{BE('A'), BE(0xD900), BE(0xDD00)};
    REQUIRE(converter.getEncodedSize(s.data(), s.size() * 2) == 6);
    std::vector<uint16_t> t(s.size());
    auto[m, n] = converter.convert(s.data(), s.size() * 2, t.data(), t.size() * 2);
    REQUIRE(m == s.size() * 2);
    REQUIRE(n == t.size() * 2);
    REQUIRE(t[1] == LE(0xD900));
}
