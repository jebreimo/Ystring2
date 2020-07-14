//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-10-27
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "EncoderBase.hpp"

#include "Ystring/YstringException.hpp"

namespace Ystring
{

    EncoderBase::EncoderBase(Encoding encoding)
        : m_Encoding(encoding),
          m_ErrorHandlingPolicy(),
          m_ReplacementCharacter(REPLACEMENT_CHARACTER)
    {}

    EncoderBase::~EncoderBase()
    {}

    Encoding EncoderBase::encoding() const
    {
        return m_Encoding;
    }

    ErrorHandlingPolicy EncoderBase::errorHandlingPolicy() const
    {
        return m_ErrorHandlingPolicy;
    }

    void EncoderBase::setErrorHandlingPolicy(ErrorHandlingPolicy value)
    {
        m_ErrorHandlingPolicy = value;
    }

    char32_t EncoderBase::replacementCharacter() const
    {
        return m_ReplacementCharacter;
    }

    void EncoderBase::setReplacementCharacter(char32_t value)
    {
        m_ReplacementCharacter = value;
    }
}
