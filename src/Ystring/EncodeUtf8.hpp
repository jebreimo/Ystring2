//****************************************************************************
// Copyright © 2011, Jan Erik Breimo.
// Created 2011-07-14 by Jan Erik Breimo
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cstdint>
#include <cstddef>
#include "Ystring/CodePointConstants.hpp"

/** @file
  * @brief Defines functions for working with UTF-8 encoded strings.
  */

namespace Ystring
{
    namespace Detail
    {
        template <typename OutputIt>
        size_t encodeUtf8(char32_t chr, size_t chrLength, OutputIt& it)
        {
            if (chrLength == 1)
            {
                *it++ = char(chr);
            }
            else if (chrLength != 0)
            {
                size_t shift = (chrLength - 1) * 6;
                *it++ = char((0xFFu << (8 - chrLength)) | (chr >> shift));
                for (size_t i = 1; i < chrLength; i++)
                {
                    shift -= 6;
                    *it++ = char(0x80u | ((chr >> shift) & 0x3Fu));
                }
            }
            return chrLength;
        }
    }

    constexpr size_t getUtf8EncodedLength(char32_t c)
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
     * @brief Adds @a codePoint encoded as UTF-8 to @a it.
     * @return the new iterator position.
     */
    template <typename OutputIt>
    size_t encodeUtf8(char32_t codePoint, OutputIt it)
    {
        return Detail::encodeUtf8(codePoint, getUtf8EncodedLength(codePoint),
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
    inline size_t encodeUtf8(char32_t codePoint, char* dst, size_t dstSize)
    {
        size_t length = getUtf8EncodedLength(codePoint);
        if (length > dstSize)
            length = 0;
        return Detail::encodeUtf8(codePoint, length, dst);
    }
}
