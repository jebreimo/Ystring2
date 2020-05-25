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

/** @file
  * @brief Defines functions for working with UTF-8 encoded strings.
  */

namespace Ystring
{
    namespace Detail
    {
        template <typename OutputIt>
        size_t encodeUtf8(OutputIt& it, char32_t c, size_t length)
        {
            if (length == 1)
            {
                *it++ = char(c);
            }
            else if (length != 0)
            {
                size_t shift = (length - 1) * 6;
                *it++ = char((0xFFu << (8 - length)) | (c >> shift));
                for (size_t i = 1; i < length; i++)
                {
                    shift -= 6;
                    *it++ = char(0x80u | ((c >> shift) & 0x3Fu));
                }
            }
            return length;
        }

        constexpr size_t getUtf8EncodedLength(char32_t c)
        {
            if ((c >> 7u) == 0)
                return 1;
            else if ((c >> 11u) == 0)
                return 2;
            else if ((c >> 16u) == 0)
                return 3;
            else if ((c >> 21u) == 0)
                return 4;
            else
                return 0;
        }
    }

    /**
     * @brief Adds @a codePoint encoded as UTF-8 to @a it.
     * @return the new iterator position.
     */
    template <typename OutputIt>
    size_t encodeUtf8(OutputIt it, char32_t codePoint)
    {
        return Detail::encodeUtf8(it, codePoint,
                                  Detail::getUtf8EncodedLength(codePoint));
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
    inline size_t encodeUtf8(char* dst, size_t size, char32_t codePoint)
    {
        size_t length = Detail::getUtf8EncodedLength(codePoint);
        if (length > size)
            length = 0;
        return Detail::encodeUtf8(dst, codePoint, length);
    }
}
