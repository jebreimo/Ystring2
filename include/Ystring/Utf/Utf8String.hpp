//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-05-31.
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <vector>
//#include "Ystring/Encoding.hpp"
//#include "Ystring/EscapeType.hpp"
//#include "Ystring/FindFlags.hpp"
//#include "Ystring/PlatformDetails.hpp"
#include "Ystring/SplitFlags.hpp"
#include "Ystring/Subrange.hpp"
#include "Ystring/Unicode/UnicodeChars.hpp"
#include "Ystring/YstringDefinitions.hpp"
//#include "Ystring/YstringException.hpp"

/** @file
  * @brief The function library for UTF-8 encoded strings.
  */

namespace Ystring
{
    /** @brief Adds @a codePoint encoded as UTF-8 to the end of @a str.
      */
    YSTRING_API std::string& append(std::string& str, char32_t chr);

    /** @brief Adds @a codePoint encoded as UTF-8 to the end of @a str.
      */
    YSTRING_API [[nodiscard]]
    std::string append(std::string_view str, char32_t chr);

    /** @brief Returns true if @a str contains code point @a chr.
      * @throw YstringException if str contains an invalid UTF-8 code point.
      */
    YSTRING_API [[nodiscard]] bool contains(std::string_view str, char32_t chr);

    /** @brief Returns the number of code points in @a str.
      *
      * @note A composed character can consist of multiple code points.
      * @return the number of code points.
      * @throw YstringException if str contains an invalid UTF-8 code point.
      */
    YSTRING_API [[nodiscard]] size_t countCodePoints(std::string_view str);

    /** @brief Returns true if @a str ends with @a cmp.
      * @note Composed and decomposed versions of the same characters are
      *     treated as different characters.
      */
    YSTRING_API [[nodiscard]]
    bool endsWith(std::string_view str, std::string_view cmp);

    /** @brief Returns the first substring in @a str that matches @a cmp.
      * @note Composed and decomposed versions of the same characters are
      *     treated as different characters.
      * @return A pair of iterators where first points to the start and
      *     second points to the end of the substring within @a str.
      *     If the substring can't be found both point to @a str.end().
      */
    YSTRING_API [[nodiscard]] Subrange findFirst(
            std::string_view str,
            std::string_view cmp);

    /** @brief Returns the first substring in @a str that constitutes
      *     a newline.
      *
      * The following characters are treated as newline characters:
      *   - \\n Unix
      *   - \\r Old MacOS
      *   - \\r\\n Windows
      *   - \\f Form feed
      *   - \\v Vertical tab
      *   - NEXT LINE (code point 133)
      *   - LINE SEPARATOR (code point 8232)
      *   - PARAGRAPH SEPARATOR (code point 8233)
      */
    YSTRING_API [[nodiscard]] Subrange findFirstNewline(
            std::string_view str);

    YSTRING_API [[nodiscard]] std::pair<Subrange, char32_t>
    findFirstOf(std::string_view str, const char32_t* chars, size_t numChars);

    /** @brief Returns the last substring in @a str that matches @a cmp.
      * @note Composed and decomposed versions of the same characters are
      *     treated as different characters.
      * @return A pair of iterators where first points to the start and
      *     second points to the end of the substring within @a str.
      *     If the substring can't be found both point to @a str.begin().
      */
    YSTRING_API [[nodiscard]]
    Subrange findLast(std::string_view str, std::string_view cmp);

    /** @brief Returns the last substring in @a str that constitutes
      *     a newline.
      *
      * The following characters are treated as newline characters:
      *   - \\n Unix
      *   - \\r Old MacOS
      *   - \\r\\n Windows
      *   - \\f Form feed
      *   - \\v Vertical tab
      *   - NEXT LINE (code point 133)
      *   - LINE SEPARATOR (code point 8232)
      *   - PARAGRAPH SEPARATOR (code point 8233)
      */
    YSTRING_API [[nodiscard]] Subrange findLastNewline(std::string_view str);

    YSTRING_API [[nodiscard]] std::pair<Subrange, char32_t>
    findLastOf(std::string_view str, const char32_t* chars, size_t numChars);

    /** @brief Return code point at position @a pos in @a str.
      *
      * The first code point has position 0.
      * If @a pos is negative, code points are counted from the end of @a str
      *  where the last code point in @a str is at position -1.
      */
    YSTRING_API [[nodiscard]] std::pair<Subrange, char32_t>
    getCodePoint(std::string_view str, ptrdiff_t pos);

    YSTRING_API [[nodiscard]] size_t
    getCodePointPos(std::string_view str, ptrdiff_t pos);

    YSTRING_API [[nodiscard]] size_t
    getClampedCodePointPos(std::string_view str, ptrdiff_t pos);

    /**
     * @brief Inserts character @a chr into @a str at position @a pos.
     *
     * @param pos The insert position in code points (which are not the same
     *     as characters if the string contains decomposed characters)
     *     from the start of the string. If @a pos is negative it's from the
     *     end of the string instead.
     * @throws YstringException if @a str isn't a valid UTF-8 string.
     */
    YSTRING_API [[nodiscard]] std::string
    insertCodePoint(std::string_view str, ptrdiff_t pos, char32_t codePoint);

    /**
     * @brief Inserts string @a sub into @a str at position @a pos.
     *
     * @param pos The insert position in code points (which are not the same
     *     as characters if the string contains decomposed characters) from
     *     the start of the string. If @a pos is negative it's from the end
     *     of the string instead.
     * @throws YstringException if @a str isn't a valid UTF-8 string.
     */
    YSTRING_API [[nodiscard]] std::string
    insertCodePoints(std::string_view str, ptrdiff_t pos,
                     std::string_view codePoints);

    /** @brief Returns true if all characters in @a str are valid UTF-8.
      */
    YSTRING_API [[nodiscard]] bool isValidUtf8(std::string_view str);

    /** @brief Returns the concatenation of the strings in @a strings
      *     delimited by @a delimiter.
      */
    template <typename It>
    [[nodiscard]] std::string join(It begin, It end,
                                   std::string_view delimiter = {})
    {
        if (begin == end)
            return {};
        std::string result(*begin);
        while (++begin != end)
        {
            result.append(delimiter);
            result.append(*begin);
        }
        return result;
    }

    /** @brief Returns a copy of @a str where instances of @a from are
      *     replaced with @a to.
      *
      * @param maxReplacements The maximum number of replacements that will be
      *     performed. All instances of @a from are replaced if the
      *     value is 0. If it is negative at most abs(maxReplacements) will be
      *     made, starting at the end of the string.
      */
    YSTRING_API [[nodiscard]] std::string replace(
            std::string_view str,
            std::string_view from,
            std::string_view to,
            ptrdiff_t maxReplacements = PTRDIFF_MAX);

    /**
     * @brief Returns a copy of @a str where the substring between code
     *  points @a start and @a end has been replaced with @a repl.
     */
    YSTRING_API [[nodiscard]] std::string replaceCodePoints(
            std::string_view str,
            ptrdiff_t start,
            ptrdiff_t end,
            std::string_view repl);

    /** @brief Returns a copy of @a str with instances of @a from replaced
      *     with @a to.
      *
      * @param str The string operated on.
      * @param from The character to replace.
      * @param to The replacement.
      * @param maxReplacements The maximum number of replacements that will be
      *     performed. All instances of @a from are replaced if the
      *     value is 0. If it is negative at most abs(maxReplacements) will be
      *     made, starting at the end of the string.
      */
    YSTRING_API std::string replaceCodePoint(
            std::string_view str,
            char32_t from,
            char32_t to,
            ptrdiff_t maxReplacements = 0);

    /** @brief Returns a copy of @a str where all invalid code points have
      *     been replaced with @a chr.
      */
    YSTRING_API std::string replaceInvalidUtf8(
            std::string_view str,
            char32_t chr = REPLACEMENT_CHARACTER);

    /** @brief Replaces all invalid code points in @a str with @a chr.
      */
    YSTRING_API std::string& replaceInvalidUtf8InPlace(
            std::string& str,
            char chr = '?');

    /** @brief Returns a reversed copy of @a str.
      *
      * Characters with combining marks are left intact.
      */
    YSTRING_API std::string reverse(std::string_view str);

    /** @brief Splits @a str where it contains whitespace characters and
      *     returns a list of the parts.
      * @param maxSplits The maximum number of times @a str will be split.
      *     If the value is 0 @a str wil be split at every newline character.
      *     If the value is negative the splitting will start from the end of
      *     @a str, the result will have parts in reverse order (i.e. the last
      *     part is first, the second to last is second and so on).
      */
    YSTRING_API std::vector<std::string> split(
            std::string_view str,
            ptrdiff_t maxSplits = 0,
            SplitFlags_t flags = SplitFlags::IGNORE_EMPTY);

    /** @brief Splits @a str where it matches @a sep and returns a list of
      *     the parts.
      * @param maxSplits The maximum number of times @a str will be split.
      *     If the value is 0 @a str wil be split at every newline character.
      *     If the value is negative the splitting will start from the end of
      *     @a str, the result will have parts in reverse order (i.e. the last
      *     part is first, the second to last is second and so on).
      */
    YSTRING_API std::vector<std::string> split(
            std::string_view str,
            std::string_view sep,
            ptrdiff_t maxSplits = 0,
            SplitFlags_t flags = SplitFlags::DEFAULTS);

    /** @brief Splits @a str at characters that satisfy predicate and returns
      *     a list of the parts.
      * @param maxSplits The maximum number of times @a str will be split.
      *     If the value is 0 @a str wil be split at every newline character.
      *     If the value is negative the splitting will start from the end of
      *     @a str, the result will have parts in reverse order (i.e. the last
      *     part is first, the second to last is second and so on).
      */
    YSTRING_API std::vector<std::string> splitIf(
            std::string_view str,
            std::function<bool(char32_t)> predicate,
            ptrdiff_t maxSplits = 0,
            SplitFlags_t flags = SplitFlags::DEFAULTS);

    /** @brief Splits @a str at newline characters and returns a list
      *     of the parts.
      * @param maxSplits The maximum number of times @a str will be split.
      *     If the value is 0 @a str wil be split at every newline character.
      *     If the value is negative the splitting will start from the end of
      *     @a str, the result will have parts in reverse order (i.e. the last
      *     part is first, the second to last is second and so on).
      */
    YSTRING_API std::vector<std::string> splitLines(
            std::string_view str,
            ptrdiff_t maxSplits = 0,
            SplitFlags_t flags = SplitFlags::DEFAULTS);

    /** @brief Returns true if @a str starts with substring @a cmp.
      * @throw YstringException if @a str or @a cmp contain any invalid UTF-8
      *     code points.
      */
    YSTRING_API bool startsWith(std::string_view str,
                                std::string_view cmp);

    /** @brief Returns the substring of of @a str that starts at character
      *     number @a startIndex and ends at character number @a endIndex.
      * @param startIndex The start position in complete characters (i.e. not
      *     bytes, not even code points if the string has decomposed
      *     characters) from the start of the string. If @a startIndex is
      *     negative it's from the end of the string instead.
      * @param endIndex The end position in complete characters (i.e. not
      *     bytes, not even code points if the string has decomposed
      *     characters) from the start of the string. If @a startIndex is
      *     negative it's from the end of the string instead.
      * @throw YstringException if str contains an invalid UTF-8 code point.
      */
    YSTRING_API std::string substring(
            std::string_view str,
            ptrdiff_t startIndex,
            ptrdiff_t endIndex = PTRDIFF_MAX);

    /** @brief Returns a UTF-8 encoded string representing @a chr
      */
    YSTRING_API std::string toUtf8(char32_t chr);

    /** @brief Returns a copy of @a str where all whitespace characters at the
      *     start and end of the string have been removed.
      */
    YSTRING_API std::string trim(std::string_view str);

    /** @brief Returns a copy of @a str where all characters satisfying
      *     @a predicate at the start and end of the string have been removed.
      */
    YSTRING_API std::string trim(
            std::string_view str,
            std::function<bool(char32_t)> predicate);

    /** @brief Returns a copy of @a str where all whitespace characters at the
      *     end of the string have been removed.
      */
    YSTRING_API std::string trimEnd(std::string_view str);

    /** @brief Returns a copy of @a str where all characters satisfying
      *     @a predicate at the end of the string have been removed.
      */
    YSTRING_API std::string trimEnd(
            std::string_view str,
            std::function<bool(char32_t)> predicate);

    /** @brief Returns a copy of @a str where all whitespace characters at the
      *     start of the string have been removed.
      */
    YSTRING_API std::string trimStart(std::string_view str);

    /** @brief Returns a copy of @a str where all characters that satisfy
     *      @a predicate at the start of the string have been removed.
      */
    YSTRING_API std::string trimStart(
            std::string_view str,
            std::function<bool(char32_t)> predicate);
}
