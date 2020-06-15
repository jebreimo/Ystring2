//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-10-26
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "DecoderBase.hpp"

#include "Ystring/YstringException.hpp"
#include "Ystring/UnicodeChars.hpp"
#include "YstringThrow.hpp"

namespace Ystring
{
    DecoderBase::DecoderBase(Encoding encoding)
        : m_Encoding(encoding),
          m_ReplacementCharacter(REPLACEMENT_CHARACTER),
          m_ErrorHandlingPolicy()
    {}

    Encoding DecoderBase::encoding() const
    {
        return m_Encoding;
    }

    ErrorHandlingPolicy DecoderBase::errorHandlingPolicy() const
    {
        return m_ErrorHandlingPolicy;
    }

    void DecoderBase::setErrorHandlingPolicy(ErrorHandlingPolicy value)
    {
        m_ErrorHandlingPolicy = value;
    }

    char32_t DecoderBase::replacementCharacter() const
    {
        return m_ReplacementCharacter;
    }

    void DecoderBase::setReplacementCharacter(char32_t value)
    {
        m_ReplacementCharacter = value;
    }

    std::pair<size_t, size_t>
    DecoderBase::decode(const char* src, size_t srcSize,
                        char32_t* dst, size_t dstSize) const
    {
        size_t iSrc = 0, iDst = 0;
        while (true)
        {
            auto size = doDecode(src + iSrc, srcSize - iSrc,
                                 dst + iDst, dstSize - iDst);
            iSrc += size.first;
            iDst += size.second;
            if (iSrc == srcSize || iDst == dstSize)
                return {iSrc, iDst};

            switch (m_ErrorHandlingPolicy)
            {
            case ErrorHandlingPolicy::REPLACE:
                dst[iDst++] = m_ReplacementCharacter;
                iSrc += skipCharacter(src + iSrc, srcSize - iSrc);
                break;
            case ErrorHandlingPolicy::STOP:
                return {iSrc, iDst};
            case ErrorHandlingPolicy::THROW:
                YSTRING_THROW("Invalid character starting at index "
                              + std::to_string(iSrc));
            case ErrorHandlingPolicy::SKIP:
                iSrc += skipCharacter(src, srcSize);
                break;
            }
        }
    }
}
