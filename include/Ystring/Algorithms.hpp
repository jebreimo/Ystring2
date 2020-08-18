//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-05-31.
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <vector>
#include "Char32Span.hpp"
#include "CodePointConstants.hpp"
#include "DecodeUtf8.hpp"
#include "Subrange.hpp"
#include "TokenIterator.hpp"
#include "YstringDefinitions.hpp"

/** @file
  * @brief The function library for UTF-8 encoded strings.
  */

namespace Ystring
{
    constexpr char32_t ASCII_WHITESPACE[] = {' ', '\t', '\n', '\r', '\f', '\v'};
    constexpr char32_t COMMON_WHITESPACE[] = {' ', '\t', '\n', '\r'};

    /** @brief Adds @a codePoint encoded as UTF-8 to the end of @a str.
      */
    YSTRING_API std::string& append(std::string& str, char32_t chr);

    /** @brief Adds @a codePoint encoded as UTF-8 to the end of @a str.
      */
    [[nodiscard]]
    YSTRING_API std::string append(std::string_view str, char32_t chr);

    /** @brief Compares @a str and @a cmp, ignoring any differences in
      *     letter casing.
      *
      * @note There is no proper support for decomposed characters; composed
      *     and decomposed versions of the same characters are
      *     treated as different characters (the decomposed character will
      *     typically be the "lesser" one).
      * @returns @arg < 0 if @a str is less than @a cmp
      *          @arg 0 if @a str is equal to @a cmp
      *          @arg > 0 if @a str is greater than @a cmp
      * @throw YstringException if str contains an invalid UTF-8 code point.
      */
    [[nodiscard]]
    YSTRING_API int32_t caseInsensitiveCompare(std::string_view str,
                                               std::string_view cmp);

    /** @brief Returns true if @a str ends with @a cmp.
      * @note Composed and decomposed versions of the same characters are
      *     treated as different characters when flags is CASE_INSENSITIVE.
      */
    [[nodiscard]]
    YSTRING_API bool caseInsensitiveEndsWith(std::string_view str,
                                             std::string_view cmp);

    /** @brief Returns true if the upper case versions of @a str and @a cmp
      *     are equal.
      *
      * @note There is no proper support for decomposed characters; composed
      *     and decomposed versions of the same characters are
      *     treated as different characters (the decomposed character will
      *     typically be the "lesser" one).
      * @throw YstringException if str contains an invalid UTF-16 code point.
      */
    [[nodiscard]]
    YSTRING_API bool caseInsensitiveEqual(std::string_view str,
                                          std::string_view cmp);

    /** @brief Returns the first substring in @a str that matches @a cmp.
         * @note Composed and decomposed versions of the same characters are
         *     treated as different characters.
         * @return A pair of iterators where first points to the start and
         *     second points to the end of the substring within @a str.
         *     If the substring can't be found both point to @a str.end().
         */
    [[nodiscard]]
    YSTRING_API Subrange caseInsensitiveFindFirst(std::string_view str,
                                                  std::string_view cmp,
                                                  size_t offset = 0);

    [[nodiscard]]
    YSTRING_API std::pair<Subrange, char32_t>
    caseInsensitiveFindFirstOf(std::string_view str,
                               Char32Span chars,
                               size_t offset = 0);

    /** @brief Returns the last substring in @a str that matches @a cmp.
      * @note Composed and decomposed versions of the same characters are
      *     treated as different characters.
      * @return A pair of iterators where first points to the start and
      *     second points to the end of the substring within @a str.
      *     If the substring can't be found both point to @a str.begin().
      */
    [[nodiscard]]
    YSTRING_API Subrange
    caseInsensitiveFindLast(std::string_view str,
                            std::string_view cmp);

    /** @brief Returns the last substring in @a str that matches @a cmp.
      * @note Composed and decomposed versions of the same characters are
      *     treated as different characters.
      * @return A pair of iterators where first points to the start and
      *     second points to the end of the substring within @a str.
      *     If the substring can't be found both point to @a str.begin().
      */
    [[nodiscard]]
    YSTRING_API Subrange
    caseInsensitiveFindLast(std::string_view str,
                            std::string_view cmp,
                            size_t offset);

    [[nodiscard]]
    YSTRING_API std::pair<Subrange, char32_t>
    caseInsensitiveFindLastOf(std::string_view str, Char32Span chars);

    [[nodiscard]]
    YSTRING_API std::pair<Subrange, char32_t>
    caseInsensitiveFindLastOf(std::string_view str, Char32Span chars,
                              size_t offset);

    /** @brief Returns true if the upper case version of @a str is less
      *     than @a cmp.
      *
      * Only a quick comparison of code point values are performed. This
      * function should not be used to sort strings in alphabetical order as
      * what is alphabetical order varies between languages and cultures.
      * @throw YstringException if str contains an invalid UTF-8 code point.
      */
    [[nodiscard]]
    YSTRING_API bool caseInsensitiveLess(std::string_view str,
                                         std::string_view cmp);

    /** @brief Returns a copy of @a str where instances of @a from are
      *     replaced with @a to.
      *
      * @param maxReplacements The maximum number of replacements that will be
      *     performed. All instances of @a from are replaced if the
      *     value is 0. If it is negative at most abs(maxReplacements) will be
      *     made, starting at the end of the string.
      */
    [[nodiscard]]
    YSTRING_API std::string
    caseInsensitiveReplace(
        std::string_view str,
        std::string_view from,
        std::string_view to,
        ptrdiff_t maxReplacements = PTRDIFF_MAX);

    /** @brief Returns true if @a str starts with substring @a cmp.
      * @throw YstringException if @a str or @a cmp contain any invalid UTF-8
      *     code points.
      */
    [[nodiscard]]
    YSTRING_API bool
    caseInsensitiveStartsWith(std::string_view str,
                              std::string_view cmp);

    /** @brief Returns true if @a str contains code point @a chr.
      * @throw YstringException if str contains an invalid UTF-8 code point.
      */
    [[nodiscard]]
    YSTRING_API bool contains(std::string_view str, char32_t chr);

    /** @brief Returns the number of characters in @a str.
      *
      * @note A composed character can consist of multiple code points.
      * @return the number of code points.
      * @throw YstringException if str contains an invalid UTF-8 code point.
      */
    [[nodiscard]]
    YSTRING_API size_t countCodeCharacters(std::string_view str);

    /** @brief Returns the number of code points in @a str.
      *
      * @note A composed character can consist of multiple code points.
      * @return the number of code points.
      * @throw YstringException if str contains an invalid UTF-8 code point.
      */
    [[nodiscard]]
    YSTRING_API size_t countCodePoints(std::string_view str);

    /** @brief Returns true if @a str ends with @a cmp.
      * @note Composed and decomposed versions of the same characters are
      *     treated as different characters.
      */
    [[nodiscard]]
    YSTRING_API bool endsWith(std::string_view str, std::string_view cmp);

    /** @brief Returns the first substring in @a str that matches @a cmp.
      * @note Composed and decomposed versions of the same characters are
      *     treated as different characters.
      * @return A pair of iterators where first points to the start and
      *     second points to the end of the substring within @a str.
      *     If the substring can't be found both point to @a str.end().
      */
    [[nodiscard]]
    YSTRING_API Subrange findFirst(std::string_view str,
                                   std::string_view cmp,
                                   size_t offset = 0);

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
    [[nodiscard]]
    YSTRING_API Subrange findFirstNewline(std::string_view str,
                                          size_t offset = 0);

    [[nodiscard]]
    YSTRING_API std::pair<Subrange, char32_t>
    findFirstOf(std::string_view str, Char32Span chars,
                size_t offset = 0);

    template <typename Char32Predicate>
    [[nodiscard]]
    std::pair<Subrange, char32_t>
    findFirstWhere(std::string_view str, Char32Predicate pred,
                   size_t offset = 0)
    {
        auto it = str.begin() + offset, prev = it;
        char32_t ch;
        while (safeDecodeNext(it, str.end(), ch))
        {
            if (pred(ch))
                return {{str.begin(), prev, it}, ch};
            prev = it;
        }
        return {{std::string_view::npos}, INVALID_CHAR};
    }

    /** @brief Returns the last substring in @a str that matches @a cmp.
      * @note Composed and decomposed versions of the same characters are
      *     treated as different characters.
      * @return A pair of iterators where first points to the start and
      *     second points to the end of the substring within @a str.
      *     If the substring can't be found both point to @a str.begin().
      */
    [[nodiscard]]
    YSTRING_API Subrange findLast(std::string_view str,
                                  std::string_view cmp);

    /** @brief Returns the last substring in @a str that matches @a cmp.
      * @note Composed and decomposed versions of the same characters are
      *     treated as different characters.
      * @return A pair of iterators where first points to the start and
      *     second points to the end of the substring within @a str.
      *     If the substring can't be found both point to @a str.begin().
      */
    [[nodiscard]]
    YSTRING_API Subrange findLast(std::string_view str,
                                  std::string_view cmp,
                                  size_t offset);

    /**
     * @brief Returns the last substring in @a str that constitutes
     *  a newline.
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
    [[nodiscard]]
    YSTRING_API Subrange findLastNewline(std::string_view str);

    /**
     * @brief Returns the last substring in @a str that constitutes
     *  a newline.
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
    [[nodiscard]]
    YSTRING_API Subrange findLastNewline(std::string_view str,
                                         size_t offset);

    [[nodiscard]]
    YSTRING_API std::pair<Subrange, char32_t>
    findLastOf(std::string_view str, Char32Span chars);

    [[nodiscard]]
    YSTRING_API std::pair<Subrange, char32_t>
    findLastOf(std::string_view str, Char32Span chars,
               size_t offset);

    template <typename Char32Predicate>
    [[nodiscard]]
    std::pair<Subrange, char32_t>
    findLastWhere(std::string_view str, Char32Predicate pred,
                  size_t offset)
    {
        auto begin = str.begin(), it = str.begin() + offset, end = it;
        char32_t ch;
        while (safeDecodePrev(begin, it, ch))
        {
            if (pred(ch))
                return {{begin, it, end}, ch};
            end = it;
        }
        return {{std::string_view::npos}, INVALID_CHAR};
    }

    template <typename Char32Predicate>
    [[nodiscard]]
    std::pair<Subrange, char32_t>
    findLastWhere(std::string_view str, Char32Predicate pred)
    {
        return findLastWhere(str, pred, str.size());
    }

    /**
     * @brief Returns the offset to the start of character number @a pos
     *     in @a str.
     * @param pos The number of complete characters (i.e. not code
     *     points if the string has combining marks) from the
     *     start of the string. If @a pos is negative it's from the end of
     *     the string instead.
     */
    [[nodiscard]]
    YSTRING_API size_t
    getCharacterPosition(std::string_view str, ptrdiff_t pos);

    [[nodiscard]]
    YSTRING_API Subrange
    getCharacterRange(std::string_view str, ptrdiff_t pos);

    /**
     * @brief Return code point at position @a pos in @a str.
     *
     * The first code point has position 0.
     * If @a pos is negative, code points are counted from the end of @a str
     *  where the last code point in @a str is at position -1.
     */
    [[nodiscard]]
    YSTRING_API std::pair<Subrange, char32_t>
    getCodePoint(std::string_view str, ptrdiff_t pos);

    /**
     * @brief Returns the byte offset to codepoint number @a pos.
     *
     * If @a pos is negative, the function returns the offset to the start
     * of codepoint number @a -pos.
     *
     * The function returns std::string_view::npos if abs(pos) is greater
     * than the number of codepoints.
     */
    [[nodiscard]]
    YSTRING_API size_t
    getCodePointPos(std::string_view str, ptrdiff_t pos);

    [[nodiscard]]
    YSTRING_API std::string_view
    getCodePointSubstring(std::string_view str,
                          ptrdiff_t startIndex,
                          ptrdiff_t endIndex = PTRDIFF_MAX);

    /**
     * @brief Returns the offset and length of the character starting at
     *  @a offset.
      */
    [[nodiscard]]
    YSTRING_API Subrange getNextCharacterRange(std::string_view str,
                                               size_t offset);

    /**
     * @brief Returns the offset and length of the character ending at
     *  @a offset.
     */
    [[nodiscard]]
    YSTRING_API Subrange getPrevCharacterRange(std::string_view str,
                                               size_t offset);

    /**
     * @brief Returns the substring of of @a str that starts at character
     *  number @a startIndex and ends at character number @a endIndex.
     * @param startIndex The start position in complete characters (i.e. not
     *  bytes, not even code points if the string has decomposed
     *  characters) from the start of the string. If @a startIndex is
     *  negative it's from the end of the string instead.
     * @param endIndex The end position in complete characters (i.e. not
     *  bytes, not even code points if the string has decomposed
     *  characters) from the start of the string. If @a startIndex is
     *  negative it's from the end of the string instead.
     * @throw YstringException if str contains an invalid UTF-8 code point.
     */
    [[nodiscard]]
    YSTRING_API std::string_view
    getSubstring(std::string_view str,
                 ptrdiff_t startIndex,
                 ptrdiff_t endIndex = PTRDIFF_MAX);

    /**
     * @brief Inserts string @a sub into @a str at position @a pos.
     *
     * @param pos The insert position in complete characters (i.e. not bytes,
     *  not even code points if the string has decomposed characters) from
     *  the start of the string. If @a pos is negative it's from the end
     *  of the string instead.
     * @throws YstringException if @a str isn't a valid UTF-8 string.
     */
    [[nodiscard]]
    YSTRING_API std::string insertCharacters(
        std::string_view str,
        ptrdiff_t pos,
        std::string_view sub);

    /**
     * @brief Inserts character @a chr into @a str at position @a pos.
     *
     * @param pos The insert position in complete characters (i.e. not bytes,
     *  not even code points if the string has decomposed characters)
     *  from the start of the string. If @a pos is negative it's from the
     *  end of the string instead.
     * @throws YstringException if @a str isn't a valid UTF-8 string.
     */
    [[nodiscard]]
    YSTRING_API std::string insertCharacters(
        std::string_view str,
        ptrdiff_t pos,
        char32_t chr);

    /**
     * @brief Inserts character @a chr into @a str at position @a pos.
     *
     * @param pos The insert position in code points (which are not the same
     *     as characters if the string contains decomposed characters)
     *     from the start of the string. If @a pos is negative it's from the
     *     end of the string instead.
     * @throws YstringException if @a str isn't a valid UTF-8 string.
     */
    [[nodiscard]]
    YSTRING_API std::string
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
    [[nodiscard]]
    YSTRING_API std::string
    insertCodePoints(std::string_view str, ptrdiff_t pos,
                     std::string_view codePoints);

    /** @brief Returns true if all characters in @a str are valid UTF-8.
      */
    [[nodiscard]]
    YSTRING_API bool isValidUtf8(std::string_view str);

    /** @brief Returns the concatenation of the strings in @a strings
      *     delimited by @a delimiter.
      */
    template <typename It>
    [[nodiscard]]
    std::string join(It begin, It end, std::string_view delimiter = {})
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
    [[nodiscard]]
    YSTRING_API std::string replace(
            std::string_view str,
            std::string_view from,
            std::string_view to,
            ptrdiff_t maxReplacements = PTRDIFF_MAX);

    /**
     * @brief Returns a copy of @a str where the substring between code
     *  points @a start and @a end has been replaced with @a repl.
     */
    [[nodiscard]]
    YSTRING_API std::string replaceCharacters(
        std::string_view str,
        ptrdiff_t start,
        ptrdiff_t end,
        std::string_view repl);

    /**
     * @brief Returns a copy of @a str where the substring between code
     *  points @a start and @a end has been replaced with @a repl.
     */
    [[nodiscard]]
    YSTRING_API std::string replaceCodePoints(
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
    [[nodiscard]]
    YSTRING_API std::string replaceCodePoint(
            std::string_view str,
            char32_t from,
            char32_t to,
            ptrdiff_t maxReplacements = PTRDIFF_MAX);

    /** @brief Returns a copy of @a str where all invalid code points have
      *     been replaced with @a chr.
      */
    [[nodiscard]]
    YSTRING_API std::string replaceInvalidUtf8(
            std::string_view str,
            char32_t chr = REPLACEMENT_CHARACTER);

    /** @brief Replaces all invalid code points in @a str with @a chr.
      */
    [[nodiscard]]
    YSTRING_API std::string& replaceInvalidUtf8(
            std::string& str,
            char32_t chr = REPLACEMENT_CHARACTER);

    /** @brief Returns a reversed copy of @a str.
      *
      * Characters with combining marks are left intact.
      */
    [[nodiscard]]
    YSTRING_API std::string reverse(std::string_view str);

    struct SplitParams
    {
        size_t maxSplits = SIZE_MAX;
        bool ignoreEmpty = false;
        bool caseInsensitive = false;
    };

    constexpr SplitParams IGNORE_EMPTY_SPLIT = {SIZE_MAX, true, false};
    constexpr SplitParams CASE_INSENSITIVE_SPLIT = {SIZE_MAX, false, true};

    /** @brief Splits @a str where it contains whitespace characters and
      *     returns a list of the parts.
      * @param maxSplits The maximum number of times @a str will be split.
      *     If the value is 0 @a str wil be split at every newline character.
      */
    [[nodiscard]]
    YSTRING_API std::vector<std::string_view>
    split(std::string_view str, Char32Span chars, SplitParams params = {});

    /** @brief Splits @a str where it matches @a sep and returns a list of
      *     the parts.
      * @param maxSplits The maximum number of times @a str will be split.
      *     If the value is 0 @a str wil be split at every newline character.
      *     If the value is negative the splitting will start from the end of
      *     @a str, the result will have parts in reverse order (i.e. the last
      *     part is first, the second to last is second and so on).
      */
    [[nodiscard]]
    YSTRING_API std::vector<std::string_view>
    split(std::string_view str, std::string_view sep, SplitParams params = {});

    /** @brief Splits @a str at newline characters and returns a list
      *     of the parts.
      * @param maxSplits The maximum number of times @a str will be split.
      *     If the value is 0 @a str wil be split at every newline character.
      *     If the value is negative the splitting will start from the end of
      *     @a str, the result will have parts in reverse order (i.e. the last
      *     part is first, the second to last is second and so on).
      */
    [[nodiscard]]
    YSTRING_API std::vector<std::string_view>
    splitLines(std::string_view str, SplitParams params = {});

    template <typename TokenFinder>
    [[nodiscard]]
    std::vector<std::string_view>
    splitWhere(std::string_view str, TokenFinder finder,
               SplitParams params = {})
    {
        std::vector<std::string_view> result;
        TokenIterator it(str, finder);
        while (result.size() < params.maxSplits && it.next())
        {
            auto part = it.part();
            if (!params.ignoreEmpty || !part.empty())
                result.push_back(part);
        }
        if (it)
        {
            auto remainder = it.remainder();
            if (!params.ignoreEmpty || !remainder.empty())
                result.push_back(it.remainder());
        }
        return result;
    }

    /** @brief Returns true if @a str starts with substring @a cmp.
      * @throw YstringException if @a str or @a cmp contain any invalid UTF-8
      *     code points.
      */
    [[nodiscard]]
    YSTRING_API bool
    startsWith(std::string_view str, std::string_view cmp);

    /** @brief Returns a lower case copy of @a str.
      */
    [[nodiscard]]
    YSTRING_API std::string toLower(std::string_view str);

    /** @brief Returns a title-cased copy of @a str.
      */
    [[nodiscard]]
    YSTRING_API std::string toTitle(std::string_view str);

    /** @brief Returns a upper case copy of @a str.
      */
    [[nodiscard]]
    YSTRING_API std::string toUpper(std::string_view str);

    /** @brief Returns a copy of @a str where all whitespace characters at the
      *     start and end of the string have been removed.
      */
    [[nodiscard]]
    YSTRING_API std::string_view trim(std::string_view str, Char32Span chars);

    /** @brief Returns a copy of @a str where all characters satisfying
      *     @a pred at the start and end of the string have been removed.
      */
    template <typename Predicate>
    [[nodiscard]]
    std::string_view
    trimWhere(std::string_view str, Predicate pred)
    {
        return trimEndWhere(trimStartWhere(str, pred), pred);
    }

    /**
     * @brief Returns a copy of @a str where all characters in @a chars
     *  at the end of the string have been removed.
     */
    [[nodiscard]]
    YSTRING_API std::string_view
    trimEnd(std::string_view str, Char32Span chars = COMMON_WHITESPACE);

    /** @brief Returns a copy of @a str where all characters satisfying
      *     @a pred at the end of the string have been removed.
      */
    template <typename Predicate>
    [[nodiscard]]
    std::string_view trimEndWhere(std::string_view str, Predicate pred)
    {
        auto [sub, ch] = findLastWhere(str, [&](auto c){return !pred(c);});
        if (!sub)
            return {};
        return str.substr(0, sub.end());
    }

    /** @brief Returns a copy of @a str where all whitespace characters at the
      *     start of the string have been removed.
      */
    [[nodiscard]]
    YSTRING_API std::string_view
    trimStart(std::string_view str, Char32Span chars = COMMON_WHITESPACE);

    /** @brief Returns a copy of @a str where all characters that satisfy
     *      @a pred at the start of the string have been removed.
      */
    template <typename Predicate>
    [[nodiscard]]
    std::string_view trimStartWhere(std::string_view str, Predicate pred)
    {
        auto [sub, ch] = findFirstWhere(str, [&](auto c){return !pred(c);});
        if (!sub)
            return {};
        return str.substr(sub.start());
    }
}
