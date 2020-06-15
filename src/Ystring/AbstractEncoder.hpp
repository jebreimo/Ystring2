//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-10-27
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <string>
#include "Ystring/Encoding.hpp"
#include "Ystring/ErrorHandlingPolicy.hpp"

namespace Ystring
{
    class AbstractEncoder
    {
    public:
        virtual ~AbstractEncoder();

        Encoding encoding() const;

        const std::string& encodingName() const;

        ErrorHandlingPolicy errorHandlingPolicy() const;

        void setErrorHandlingPolicy(ErrorHandlingPolicy value);

        virtual char32_t replacementCharacter() const;

        virtual void setReplacementCharacter(char32_t value);

        virtual std::pair<size_t, size_t>
        encode(const char32_t*& src, size_t srcSize,
               void* dst, size_t dstSize) = 0;

        virtual bool encode(const char32_t*& src, size_t srcSize,
                            std::string& dst) = 0;
    protected:
        AbstractEncoder(Encoding encoding);
    private:
        Encoding m_Encoding;
        ErrorHandlingPolicy m_ErrorHandlingPolicy;
        char32_t m_ReplacementCharacter;
    };
}
