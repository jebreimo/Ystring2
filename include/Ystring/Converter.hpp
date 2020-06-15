//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-10-17.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <memory>
#include <string>
#include "Ystring/Encoding.hpp"
//#include "Ystring/PlatformDetails.hpp"
#include "Ystring/YstringDefinitions.hpp"
#include "ErrorHandlingPolicy.hpp"

/** @file
  * @brief Defines the Converter class.
  */

namespace Ystring
{

    class DecoderBase;
    class AbstractEncoder;

    /** @brief Converts strings from one encoding to another.
      */
    class YSTRING_API Converter
    {
    public:
        /** @brief Constructs a converter from @a sourceEncoding to
          *     @a destinationEncoding.
          * @throw YstringException if either of the encodings
          *     are unsupported.
          */
        Converter(Encoding sourceEncoding, Encoding destinationEncoding);

        Converter(Converter&&) noexcept;

        Converter(const Converter&) = delete;

        ~Converter();

        Converter& operator=(Converter&&) noexcept;

        Converter& operator=(const Converter&) = delete;
    //
    //    /** @brief Returns the size (in 32-bit code points) of the internal
    //      *     buffer used during conversion.
    //      */
    //    size_t bufferSize() const;
    //
    //    /** @brief Sets the size (in 32-bit code points) of the internal
    //      *     buffer used during conversion.
    //      *
    //      * A large buffer may improve performance when long texts are
    //      * converted, but should normally be left alone.
    //      * The buffer isn't used at all for some trivial conversions, e.g.
    //      * bid-endian to little-endian versions of the same encoding.
    //      *
    //      * The default size is 256 code points (i.e. 1024 bytes).
    //      */
    //    void setBufferSize(size_t value);
    //
    //    /** @brief Sets the error handling policies for both the decoder
    //      *     and encoder at once.
    //      */
    //    void setErrorHandlingPolicy(ErrorHandlingPolicy value);
    //
    //    /** @brief Returns the error handling policy for the decoder.
    //      *
    //      * The decoder is responsible for converting the source string from
    //      * the source encoding to the "natural" unicode representation,
    //      * i.e. UTF-32.
    //      */
    //    ErrorHandlingPolicy decoderErrorHandlingPolicy() const;
    //
    //    /** @brief Sets the error handling policy for the decoder.
    //      *
    //      * The decoder is responsible for converting the source string from
    //      * the source encoding to the "natural" unicode representation,
    //      * i.e. UTF-32.
    //      */
    //    void setDecoderErrorHandlingPolicy(ErrorHandlingPolicy value);
    //
    //    /** @brief Returns the error handling policy for the encoder.
    //      */
    //    ErrorHandlingPolicy encoderErrorHandlingPolicy() const;
    //
    //    /** @brief Sets the error handling policy for the encoder.
    //      *
    //      * The encoder is responsible for converting the decoded
    //      * character from UTF-32 to the destination encoding.
    //      */
    //    void setEncoderErrorHandlingPolicy(ErrorHandlingPolicy value);
    //
    //    /** @brief Set the replacement character used by both the decoder
    //      *     and encoder.
    //      *
    //      * The replacement character is used when the error handling policy
    //      * is REPLACE.
    //      */
    //    void setReplacementCharacter(char32_t value);
    //
    //    /** @brief Returns the replacement character used by the decoder.
    //      *
    //      * The default value is the unicode "REPLACEMENT CHARACTER".
    //      */
    //    char32_t decoderReplacementCharacter() const;
    //
    //    /** @brief Set the replacement character used by the decoder.
    //      *
    //      * The replacement character is used when the error handling policy
    //      * is REPLACE.
    //      */
    //    void setDecoderReplacementCharacter(char32_t value);
    //
    //    /** @brief Returnes the replacement character used by the encoder.
    //      *
    //      * The default value is the unicode "REPLACEMENT CHARACTER" if the
    //      * destination encoding supports this character, otherwise it will
    //      * be the question mark character.
    //      */
    //    char32_t encoderReplacementCharacter() const;
    //
    //    /** @brief Set the replacement character used by the encoder.
    //      *
    //      * The replacement character is used when the error handling policy
    //      * is REPLACE.
    //      */
    //    void setEncoderReplacementCharacter(char32_t value);
    //
    //    /** @brief Returns the source encoding.
    //      */
    //    Encoding sourceEncoding() const;
    //
    //    /** @brief Returns the destination encoding.
    //      */
    //    Encoding destinationEncoding() const;
    //
    //    size_t convert(const void* source,
    //                   size_t sourceLength,
    //                   std::string& destination,
    //                   bool sourceIsIncomplete = false);
    //
    //    //size_t convert(const void* source,
    //    //               size_t sourceLength,
    //    //               std::u16string& destination,
    //    //               bool sourceIsIncomplete = false);
    //    //
    //    //size_t convert(const void* source,
    //    //               size_t sourceLength,
    //    //               std::u32string& destination,
    //    //               bool sourceIsIncomplete = false);
    //    //
    //    //size_t convert(const void* source,
    //    //               size_t sourceLength,
    //    //               std::wstring& destination,
    //    //               bool sourceIsIncomplete = false);
    //
    //    template <typename Char1T, typename Char2T>
    //    typename std::basic_string<Char1T>::const_iterator
    //    convert(const std::basic_string<Char1T>& source,
    //            std::basic_string<Char2T>& destination,
    //            bool sourceIsIncomplete = false)
    //    {
    //        return source.begin() + convert(source.data(), source.size(),
    //                                        destination, sourceIsIncomplete);
    //    }
    //
    //private:
    //    enum ConversionType
    //    {
    //        CONVERT,
    //        COPY,
    //        SWAP_ENDIANNESS
    //    };
    //
    //    static ConversionType getConversionType(Encoding srcEncoding,
    //                                            Encoding dstEncoding);
    //
    //    template <typename CharT, typename StringT>
    //    size_t convertImpl(const CharT* src,
    //                       size_t srcLength,
    //                       StringT& dst,
    //                       bool sourceIsIncomplete);
    //
    //    template<typename CharT, typename StringT>
    //    size_t doConvert(const CharT* src,
    //                     size_t srcLength,
    //                     StringT& dst,
    //                     bool sourceIsIncomplete);
    //
    //    template<typename CharT, typename StringT>
    //    size_t doCopy(const CharT* src,
    //                  size_t srcLength,
    //                  StringT& dst,
    //                  bool sourceIsIncomplete);
    //
    //    template<typename CharT, typename StringT>
    //    size_t doCopyAndSwapBytes(const CharT* src,
    //                              size_t srcLength,
    //                              StringT& dst,
    //                              bool sourceIsIncomplete);
    //
    //    std::unique_ptr<DecoderBase> m_Decoder;
    //    std::unique_ptr<AbstractEncoder> m_Encoder;
    //    ConversionType m_ConversionType;
    //    size_t m_BufferSize;
    };
}
