//****************************************************************************
// Copyright © 2011, Jan Erik Breimo.
// Created 2011-07-14 by Jan Erik Breimo
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cstdint>
#include <cstddef>
#include "Ystring/CodepointConstants.hpp"

/** @file
  * @brief Defines functions for working with UTF-8 encoded strings.
  */

namespace ystring
{
    namespace detail
    {
        template <typename OutputIt>
        size_t encode_utf8(char32_t chr, size_t chr_length, OutputIt& it)
        {
            if (chr_length == 1)
            {
                *it++ = char(chr);
            }
            else if (chr_length != 0)
            {
                size_t shift = (chr_length - 1) * 6;
                *it++ = char((0xFFu << (8 - chr_length)) | (chr >> shift));
                for (size_t i = 1; i < chr_length; i++)
                {
                    shift -= 6;
                    *it++ = char(0x80u | ((chr >> shift) & 0x3Fu));
                }
            }
            return chr_length;
        }
    }

    constexpr size_t get_utf8_encoded_length(char32_t c)
    {
        if (c < 0x80u)
            return 1;
        else if (c < 0x800u)
            return 2;
        else if (c < 0x10000u)
            return 3;
        else if (c <= UNICODE_MAX)
            return 4;
        else
            return 0;
    }

    /**
     * @brief Adds @a code_point encoded as UTF-8 to @a it.
     * @return the new iterator position.
     */
    template <typename OutputIt>
    size_t encode_utf8(char32_t code_point, OutputIt it)
    {
        return detail::encode_utf8(code_point, get_utf8_encoded_length(code_point),
                                   it);
    }

    /**
     * @brief Encodes a unicode code point as UTF-8.
     *
     * @note The function does not add a terminating 0 to @a buffer.
     *
     * @param buffer a buffer for the UTF-8 code point, it must be at least
     *     as long as the encoding (i.e. 7 bytes to be error proof).
     * @param chr the code point to be encoded
     * @param bufferSize the size of @a buffer
     * @return the length of the encoded code point, or 0 if @a bufferSize is
     *     too small.
     */
    inline size_t encode_utf8(char32_t code_point, char* dst, size_t dst_size)
    {
        size_t length = get_utf8_encoded_length(code_point);
        if (length > dst_size)
            length = 0;
        return detail::encode_utf8(code_point, length, dst);
    }
}
