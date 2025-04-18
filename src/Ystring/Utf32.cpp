//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-08-17.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Utf32.hpp"
#include "Ystring/DecodeUtf8.hpp"
#include "EncodeUtf8.hpp"

namespace ystring
{
    std::string from_utf32(char32_t ch)
    {
        std::string result;
        encode_utf8(ch, std::back_inserter(result));
        return result;
    }

    std::string from_utf32(std::u32string_view str)
    {
        std::string result;
        auto out = std::back_inserter(result);
        for (auto ch : str)
            encode_utf8(ch, out);
        return result;
    }

    std::u32string to_utf32(std::string_view str)
    {
        std::u32string result;
        auto it = str.begin();
        char32_t ch;
        while (safe_decode_next(it, str.end(), ch))
            result.push_back(ch);
        return result;
    }
}
