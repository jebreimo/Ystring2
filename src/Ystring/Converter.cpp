//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-10-23.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Converter.hpp"

#include <vector>
//#include "Ystring/EncodingInfo.hpp"
//#include "Ystring/PrivatePlatformDetails.hpp"
//#include "Ystring/Generic/GenericConvert.hpp"
//#include "Ystring/Conversion/CodePageDecoder.hpp"
//#include "Ystring/Conversion/CodePageEncoder.hpp"
//#include "Ystring/Utf8Decoder.hpp"
//#include "Ystring/Utf8Encoder.hpp"
//#include "Ystring/Conversion/Utf16Decoder.hpp"
//#include "Ystring/Conversion/Utf16Encoder.hpp"
//#include "Ystring/Conversion/Utf32Decoder.hpp"
//#include "Ystring/Conversion/Utf32Encoder.hpp"

namespace Ystring {

    namespace {

        const size_t DEFAULT_BUFFER_SIZE = 256;

        //std::unique_ptr<DecoderBase> makeDecoder(Encoding encoding);
        //
        //std::unique_ptr<EncoderBase> makeEncoder(Encoding encoding);

        //template<typename CharT, typename StringT>
        //size_t copy(const CharT* src,
        //            size_t srcLength,
        //            StringT& dst,
        //            size_t charSize);
        //
        //template <typename T>
        //void swapEndianness(T* str, size_t length);

    }

    Converter::Converter(Encoding srcEncoding, Encoding dstEncoding)
    //    : m_Decoder(makeDecoder(srcEncoding)),
    //      m_Encoder(makeEncoder(dstEncoding)),
    //      m_ConversionType(getConversionType(srcEncoding, dstEncoding)),
    //      m_BufferSize(DEFAULT_BUFFER_SIZE)
    {}

    Converter::Converter(Converter&&) noexcept = default;

    Converter::~Converter() = default;

    Converter& Converter::operator=(Converter&&) noexcept = default;

    //size_t Converter::bufferSize() const
    //{
    //    return m_BufferSize;
    //}
    //
    //void Converter::setBufferSize(size_t value)
    //{
    //    m_BufferSize = value;
    //}
    //
    //void Converter::setErrorHandlingPolicy(ErrorHandlingPolicy value)
    //{
    //    setDecoderErrorHandlingPolicy(value);
    //    setEncoderErrorHandlingPolicy(value);
    //}
    //
    //ErrorHandlingPolicy Converter::decoderErrorHandlingPolicy() const
    //{
    //    return m_Decoder->errorHandlingPolicy();
    //}
    //
    //void Converter::setDecoderErrorHandlingPolicy(ErrorHandlingPolicy value)
    //{
    //    m_Decoder->setErrorHandlingPolicy(value);
    //}
    //
    //ErrorHandlingPolicy Converter::encoderErrorHandlingPolicy() const
    //{
    //    return m_Encoder->errorHandlingPolicy();
    //}
    //
    //void Converter::setEncoderErrorHandlingPolicy(ErrorHandlingPolicy value)
    //{
    //    m_Encoder->setErrorHandlingPolicy(value);
    //}
    //
    //void Converter::setReplacementCharacter(char32_t value)
    //{
    //    setDecoderReplacementCharacter(value);
    //    setEncoderReplacementCharacter(value);
    //}
    //
    //char32_t Converter::decoderReplacementCharacter() const
    //{
    //    return m_Decoder->replacementCharacter();
    //}
    //
    //void Converter::setDecoderReplacementCharacter(char32_t value)
    //{
    //    m_Decoder->setReplacementCharacter(value);
    //}
    //
    //char32_t Converter::encoderReplacementCharacter() const
    //{
    //    return m_Encoder->replacementCharacter();
    //}
    //
    //void Converter::setEncoderReplacementCharacter(char32_t value)
    //{
    //    m_Encoder->setReplacementCharacter(value);
    //}
    //
    //Encoding Converter::sourceEncoding() const
    //{
    //    if (!m_Decoder)
    //        return Encoding::UNKNOWN;
    //    return m_Decoder->encoding();
    //}
    //
    //Encoding Converter::destinationEncoding() const
    //{
    //    if (!m_Encoder)
    //        return Encoding::UNKNOWN;
    //    return m_Encoder->encoding();
    //}
    //
    //size_t Converter::convert(const void* source,
    //                          size_t sourceLength,
    //                          std::string& destination,
    //                          bool sourceIsIncomplete)
    //{
    //    return convertImpl(internal_char_type_cast(source),
    //                       sourceLength,
    //                       destination,
    //                       sourceIsIncomplete);
    //}
    //
    ////size_t Converter::convert(const char* source,
    ////                          size_t sourceLength,
    ////                          std::u16string& destination,
    ////                          bool sourceIsIncomplete)
    ////{
    ////    return convertImpl(internal_char_type_cast(source),
    ////                       sourceLength,
    ////                       destination,
    ////                       sourceIsIncomplete);
    ////}
    ////
    ////size_t Converter::convert(const char* source,
    ////                          size_t sourceLength,
    ////                          std::u32string& destination,
    ////                          bool sourceIsIncomplete)
    ////{
    ////    return convertImpl(internal_char_type_cast(source),
    ////                       sourceLength,
    ////                       destination,
    ////                       sourceIsIncomplete);
    ////}
    ////
    ////size_t Converter::convert(const char* source,
    ////                          size_t sourceLength,
    ////                          std::wstring& destination,
    ////                          bool sourceIsIncomplete)
    ////{
    ////    return convertImpl(internal_char_type_cast(source),
    ////                       sourceLength,
    ////                       destination,
    ////                       sourceIsIncomplete);
    ////}
    ////
    ////size_t Converter::convert(const char16_t* source,
    ////                          size_t sourceLength,
    ////                          std::string& destination,
    ////                          bool sourceIsIncomplete)
    ////{
    ////    return convertImpl(internal_char_type_cast(source),
    ////                       sourceLength,
    ////                       destination,
    ////                       sourceIsIncomplete);
    ////}
    ////
    ////size_t Converter::convert(const char16_t* source,
    ////                          size_t sourceLength,
    ////                          std::u16string& destination,
    ////                          bool sourceIsIncomplete)
    ////{
    ////    return convertImpl(internal_char_type_cast(source),
    ////                       sourceLength,
    ////                       destination,
    ////                       sourceIsIncomplete);
    ////}
    ////
    ////size_t Converter::convert(const char16_t* source,
    ////                          size_t sourceLength,
    ////                          std::u32string& destination,
    ////                          bool sourceIsIncomplete)
    ////{
    ////    return convertImpl(internal_char_type_cast(source),
    ////                       sourceLength,
    ////                       destination,
    ////                       sourceIsIncomplete);
    ////}
    ////
    ////size_t Converter::convert(const char16_t* source,
    ////                          size_t sourceLength,
    ////                          std::wstring& destination,
    ////                          bool sourceIsIncomplete)
    ////{
    ////    return convertImpl(internal_char_type_cast(source),
    ////                       sourceLength,
    ////                       destination,
    ////                       sourceIsIncomplete);
    ////}
    ////
    ////size_t Converter::convert(const char32_t* source,
    ////                          size_t sourceLength,
    ////                          std::string& destination,
    ////                          bool sourceIsIncomplete)
    ////{
    ////    return convertImpl(internal_char_type_cast(source),
    ////                       sourceLength,
    ////                       destination,
    ////                       sourceIsIncomplete);
    ////}
    ////
    ////size_t Converter::convert(const char32_t* source,
    ////                          size_t sourceLength,
    ////                          std::u16string& destination,
    ////                          bool sourceIsIncomplete)
    ////{
    ////    return convertImpl(internal_char_type_cast(source),
    ////                       sourceLength,
    ////                       destination,
    ////                       sourceIsIncomplete);
    ////}
    ////
    ////size_t Converter::convert(const char32_t* source,
    ////                          size_t sourceLength,
    ////                          std::u32string& destination,
    ////                          bool sourceIsIncomplete)
    ////{
    ////    return convertImpl(internal_char_type_cast(source),
    ////                       sourceLength,
    ////                       destination,
    ////                       sourceIsIncomplete);
    ////}
    ////
    ////size_t Converter::convert(const char32_t* source,
    ////                          size_t sourceLength,
    ////                          std::wstring& destination,
    ////                          bool sourceIsIncomplete)
    ////{
    ////    return convertImpl(internal_char_type_cast(source),
    ////                       sourceLength,
    ////                       destination,
    ////                       sourceIsIncomplete);
    ////}
    ////
    ////size_t Converter::convert(const wchar_t* source,
    ////                          size_t sourceLength,
    ////                          std::string& destination,
    ////                          bool sourceIsIncomplete)
    ////{
    ////    return convertImpl(internal_char_type_cast(source),
    ////                       sourceLength,
    ////                       destination,
    ////                       sourceIsIncomplete);
    ////}
    ////
    ////size_t Converter::convert(const wchar_t* source,
    ////                          size_t sourceLength,
    ////                          std::u16string& destination,
    ////                          bool sourceIsIncomplete)
    ////{
    ////    return convertImpl(internal_char_type_cast(source),
    ////                       sourceLength,
    ////                       destination,
    ////                       sourceIsIncomplete);
    ////}
    ////
    ////size_t Converter::convert(const wchar_t* source,
    ////                          size_t sourceLength,
    ////                          std::u32string& destination,
    ////                          bool sourceIsIncomplete)
    ////{
    ////    return convertImpl(internal_char_type_cast(source),
    ////                       sourceLength,
    ////                       destination,
    ////                       sourceIsIncomplete);
    ////}
    ////
    ////size_t Converter::convert(const wchar_t* source,
    ////                          size_t sourceLength,
    ////                          std::wstring& destination,
    ////                          bool sourceIsIncomplete)
    ////{
    ////    return convertImpl(internal_char_type_cast(source),
    ////                       sourceLength,
    ////                       destination,
    ////                       sourceIsIncomplete);
    ////}
    //
    //Converter::ConversionType Converter::getConversionType(
    //        Encoding src, Encoding dst)
    //{
    //    if (src == dst)
    //        return COPY;
    //    if ((src == Encoding::UTF_16_LE && dst == Encoding::UTF_16_BE)
    //        || (src == Encoding::UTF_16_BE && dst == Encoding::UTF_16_LE)
    //        || (src == Encoding::UTF_32_LE && dst == Encoding::UTF_32_BE)
    //        || (src == Encoding::UTF_32_BE && dst == Encoding::UTF_32_LE))
    //    {
    //        return SWAP_ENDIANNESS;
    //    }
    //    return CONVERT;
    //}
    //
    //template<typename CharT, typename StringT>
    //size_t Converter::convertImpl(const CharT* src,
    //                              size_t srcLength,
    //                              StringT& dst,
    //                              bool sourceIsIncomplete)
    //{
    //    switch (m_ConversionType)
    //    {
    //    case SWAP_ENDIANNESS:
    //        return doCopyAndSwapBytes(src, srcLength, dst, sourceIsIncomplete);
    //    case COPY:
    //        return doCopy(src, srcLength, dst, sourceIsIncomplete);
    //    case CONVERT:
    //        return doConvert(src, srcLength, dst, sourceIsIncomplete);
    //    }
    //    return 0;
    //}
    //
    //template<typename CharT, typename StringT>
    //size_t Converter::doConvert(const CharT* src,
    //                            size_t srcLength,
    //                            StringT& dst,
    //                            bool sourceIsIncomplete)
    //{
    //    auto srcBeg = src;
    //    auto srcEnd = src + srcLength;
    //    auto bufSize = std::min(srcLength, m_BufferSize);
    //    std::vector<char32_t> buffer(bufSize);
    //    auto bufEnd = buffer.data() + bufSize;
    //    while (srcBeg != srcEnd)
    //    {
    //        auto bufIt = buffer.data();
    //        auto result = m_Decoder->decode(srcBeg, srcEnd, bufIt, bufEnd,
    //                                        sourceIsIncomplete);
    //        auto bufBeg = static_cast<const char32_t*>(buffer.data());
    //        m_Encoder->encode(bufBeg, bufIt, dst);
    //        if (result != DecoderResult::OK)
    //            break;
    //    }
    //    return srcBeg - src;
    //}
    //
    //template<typename CharT, typename StringT>
    //size_t Converter::doCopy(const CharT* src,
    //                         size_t srcLength,
    //                         StringT& dst,
    //                         bool sourceIsIncomplete)
    //{
    //    typedef typename StringT::value_type StringChar;
    //    auto unitSize = m_Decoder->characterUnitSize();
    //    auto checkResult = m_Decoder->checkString(src, src + srcLength,
    //                                              sourceIsIncomplete);
    //    if (checkResult.first
    //        && (sizeof(CharT) == 1 || sizeof(CharT) == unitSize)
    //        && (sizeof(StringChar) == 1 || sizeof(StringChar) == unitSize))
    //    {
    //        srcLength = checkResult.second - src;
    //        return copy(src, srcLength, dst, unitSize);
    //    }
    //    else
    //    {
    //        return doConvert(src, srcLength, dst, sourceIsIncomplete);
    //    }
    //}
    //
    //template<typename CharT, typename StringT>
    //size_t Converter::doCopyAndSwapBytes(const CharT* src,
    //                                     size_t srcLength,
    //                                     StringT& dst,
    //                                     bool sourceIsIncomplete)
    //{
    //    typedef typename StringT::value_type StringChar;
    //    auto unitSize = m_Decoder->characterUnitSize();
    //    auto checkResult = m_Decoder->checkString(src, src + srcLength,
    //                                              sourceIsIncomplete);
    //    if (checkResult.first
    //        && (sizeof(CharT) == 1 || sizeof(CharT) == unitSize)
    //        && sizeof(StringChar) == unitSize)
    //    {
    //        auto initialSize = dst.size();
    //        srcLength = checkResult.second - src;
    //        auto result = copy(src, srcLength, dst, unitSize);
    //        swapEndianness(&dst[initialSize], dst.size() - initialSize);
    //        return result;
    //    }
    //    else
    //    {
    //        return doConvert(src, srcLength, dst, sourceIsIncomplete);
    //    }
    //}
    //
    //namespace {
    //
    //    std::unique_ptr<DecoderBase> makeDecoder(Encoding encoding)
    //    {
    //        switch (encoding)
    //        {
    //        case Encoding::UTF_8:
    //            return std::unique_ptr<DecoderBase>(new Utf8Decoder);
    //        case Encoding::ASCII:
    //        case Encoding::ISO_8859_1:
    //        case Encoding::ISO_8859_10:
    //        case Encoding::ISO_8859_15:
    //        case Encoding::WINDOWS_1250:
    //        case Encoding::WINDOWS_1252:
    //        case Encoding::IBM_437:
    //        case Encoding::IBM_850:
    //            return std::unique_ptr<DecoderBase>(new CodePageDecoder(
    //                    encoding));
    //        case Encoding::UTF_16_BE:
    //            return std::unique_ptr<DecoderBase>(new Utf16BEDecoder);
    //        case Encoding::UTF_16_LE:
    //            return std::unique_ptr<DecoderBase>(new Utf16LEDecoder);
    //        case Encoding::UTF_32_BE:
    //            return std::unique_ptr<DecoderBase>(new Utf32BEDecoder);
    //        case Encoding::UTF_32_LE:
    //            return std::unique_ptr<DecoderBase>(new Utf32LEDecoder);
    //        default:
    //            break;
    //        }
    //
    //        auto info = getEncodingInfo(encoding);
    //        auto name = info ? info->name()
    //                         : std::to_string(int64_t(encoding));
    //        YSTRING_THROW("Unsupported source-encoding: " + name);
    //    }
    //
    //    std::unique_ptr<EncoderBase> makeEncoder(Encoding encoding)
    //    {
    //        switch (encoding)
    //        {
    //        case Encoding::ASCII:
    //            break;
    //        case Encoding::UTF_8:
    //            return std::unique_ptr<EncoderBase>(new Utf8Encoder);
    //        case Encoding::ISO_8859_1:
    //        case Encoding::ISO_8859_10:
    //        case Encoding::ISO_8859_15:
    //        case Encoding::WINDOWS_1250:
    //        case Encoding::WINDOWS_1252:
    //        case Encoding::IBM_437:
    //        case Encoding::IBM_850:
    //            return std::unique_ptr<EncoderBase>(new CodePageEncoder(
    //                    encoding));
    //        case Encoding::UTF_16_BE:
    //            return std::unique_ptr<EncoderBase>(new Utf16BEEncoder);
    //        case Encoding::UTF_16_LE:
    //            return std::unique_ptr<EncoderBase>(new Utf16LEEncoder);
    //        case Encoding::UTF_32_BE:
    //            return std::unique_ptr<EncoderBase>(new Utf32BEEncoder);
    //        case Encoding::UTF_32_LE:
    //            return std::unique_ptr<EncoderBase>(new Utf32LEEncoder);
    //        default:
    //            break;
    //        }
    //
    //        auto info = getEncodingInfo(encoding);
    //        auto name = info ? info->name()
    //                         : std::to_string(int64_t(encoding));
    //        YSTRING_THROW("Unsupported source-encoding: " + name);
    //    }
    //
    //    template<typename CharT, typename StringT>
    //    size_t copy(const CharT* src,
    //                size_t srcLength,
    //                StringT& dst,
    //                size_t charSize)
    //    {
    //        typedef typename StringT::value_type DstChar;
    //        auto srcMemLength = srcLength * sizeof(CharT);
    //        srcMemLength -= srcMemLength % charSize;
    //        auto dstLength = srcMemLength / sizeof(DstChar);
    //        auto initialDstSize = dst.size();
    //        dst.resize(initialDstSize + dstLength);
    //        memcpy(&dst[initialDstSize], src, srcMemLength);
    //        return srcMemLength / sizeof(CharT);
    //    }
    //
    //    template <typename T>
    //    void swapEndianness(T* str, size_t length)
    //    {
    //        for (size_t i = 0; i < length; ++i)
    //            str[i] = Utilities::reverseBytes(str[i]);
    //    }
    //
    //}
}
