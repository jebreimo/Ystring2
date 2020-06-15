//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-10-26
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cstdint>
#include <string>
#include "Ystring/Encoding.hpp"
#include "Ystring/ErrorHandlingPolicy.hpp"

namespace Ystring
{
    class DecoderBase
    {
    public:
        virtual ~DecoderBase() = default;

        [[nodiscard]]
        Encoding encoding() const;

        [[nodiscard]]
        ErrorHandlingPolicy errorHandlingPolicy() const;

        void setErrorHandlingPolicy(ErrorHandlingPolicy value);

        [[nodiscard]]
        virtual char32_t replacementCharacter() const;

        virtual void setReplacementCharacter(char32_t value);

        std::pair<size_t, size_t>
        decode(const char* src, size_t srcSize,
               char32_t* dst, size_t dstSize) const;
    protected:
        explicit DecoderBase(Encoding encoding);

        virtual size_t
        skipCharacter(const char* src, size_t srcSize) const = 0;

        virtual std::pair<size_t, size_t>
        doDecode(const char* src, size_t srcSize,
                 char32_t* dst, size_t dstSize) const = 0;
    private:
        Encoding m_Encoding;
        char32_t m_ReplacementCharacter;
        ErrorHandlingPolicy m_ErrorHandlingPolicy;
    };
}
