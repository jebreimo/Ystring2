//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-05-06.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once


    /** @brief Returns an UTF-8 encoded string equivalent to @a str.
      *
      * @param str The string to convert from.
      * @param encoding The encoding of @a str.
      * @throws YstringException if str contains any characters that aren't
      *     encoded according to @a encoding, or if @a encoding is
      *     unsupported for strings of @a str's type.
      */
    YSTRING_API std::string toUtf8(
            std::string_view str,
            Encoding_t encoding);

    /** @brief Returns an UTF-8 encoded string equivalent to @a str.
      *
      * @param str The string to convert from.
      * @param encoding The encoding of @a str.
      * @throws YstringException if str contains any characters that aren't
      *     encoded according to @a encoding, or if @a encoding is
      *     unsupported for strings of @a str's type.
      */
    YSTRING_API std::string toUtf8(
            const char* str, size_t length,
            Encoding_t encoding);

    /** @brief Returns an UTF-8 encoded string equivalent to @a str.
      *
      * @param str The string to convert from.
      * @param encoding The encoding of @a str.
      * @throws YstringException if str contains any characters that aren't
      *     encoded according to @a encoding, or if @a encoding is
      *     unsupported for strings of @a str's type.
      */
    YSTRING_API std::string toUtf8(
            const wchar_t* str, size_t length,
            Encoding_t encoding);

    /** @brief Returns an UTF-8 encoded string equivalent to @a str.
      *
      * @param str The string to convert from.
      * @param encoding The encoding of @a str.
      * @throws YstringException if str contains any characters that aren't
      *     encoded according to @a encoding, or if @a encoding is
      *     unsupported for strings of @a str's type.
      */
    YSTRING_API std::string toUtf8(
            const char16_t* str, size_t length,
            Encoding_t encoding);

    /** @brief Returns an UTF-8 encoded string equivalent to @a str.
      *
      * @param str The string to convert from.
      * @param encoding The encoding of @a str.
      * @throws YstringException if str contains any characters that aren't
      *     encoded according to @a encoding, or if @a encoding is
      *     unsupported for strings of @a str's type.
      */
    YSTRING_API std::string toUtf8(
            const char32_t* str, size_t length,
            Encoding_t encoding);

    /** @brief Returns an UTF-8 encoded string equivalent to @a str.
      *
      * @param str The string to convert from.
      * @param encoding The encoding of @a str.
      * @throws YstringException if str contains any characters that aren't
      *     encoded according to @a encoding, or if @a encoding is
      *     unsupported for strings of @a str's type.
      */
    YSTRING_API std::string toUtf8(
            const std::u16string& str,
            Encoding_t encoding = Encoding::UTF_16);

    /** @brief Returns an UTF-8 encoded string equivalent to @a str.
      *
      * @param str The string to convert from.
      * @param encoding The encoding of @a str.
      * @throws YstringException if str contains any characters that aren't
      *     encoded according to @a encoding, or if @a encoding is
      *     unsupported for strings of @a str's type.
      */
    YSTRING_API std::string toUtf8(
            const std::u32string& str,
            Encoding_t encoding = Encoding::UTF_32);

    /** @brief Returns an UTF-8 encoded string equivalent to @a str.
      *
      * @param str The string to convert from.
      * @param encoding The encoding of @a str.
      * @throws YstringException if str contains any characters that aren't
      *     encoded according to @a encoding, or if @a encoding is
      *     unsupported for strings of @a str's type.
      */
    YSTRING_API std::string toUtf8(
            const std::wstring& str,
            Encoding_t encoding);
