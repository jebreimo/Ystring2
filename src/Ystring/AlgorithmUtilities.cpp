//
// Created by janeb on 18.08.2020.
//

#include "AlgorithmUtilities.hpp"

namespace Ystring
{
    std::vector<Subrange> findLastN(
        std::string_view str, std::string_view cmp,
        size_t maxCount)
    {
        std::vector<Subrange> result;
        auto begin = str.begin();
        auto it = str.end();
        while (true)
        {
            auto match = searchLast(begin, it, cmp.begin(), cmp.end());
            if (match.first == match.second)
                break;
            result.emplace_back(match.first - begin, cmp.size());
            if (--maxCount == 0)
                break;
            it = match.first;
        }
        return result;
    }

    size_t getCappedCharacterPos(std::string_view str, ptrdiff_t pos)
    {
        if (auto p = getCharacterPos(str, pos); p != std::string_view::npos)
            return p;
        return pos > 0 ? str.size() : 0;
    }

    size_t getCappedCodePointPos(std::string_view str, ptrdiff_t pos)
    {
        if (auto p = getCodePointPos(str, pos); p != std::string_view::npos)
            return p;
        return pos > 0 ? str.size() : 0;
    }

    Subrange getCharacterSubstringRange(std::string_view str,
                                               ptrdiff_t startIndex,
                                               ptrdiff_t endIndex)
    {
        if (endIndex < startIndex && (startIndex >= 0) == (endIndex >= 0))
            endIndex = startIndex;
        if (startIndex >= 0 && endIndex >= 0)
        {
            auto s = getCappedCharacterPos(str, startIndex);
            auto e = getCappedCharacterPos(str.substr(s), endIndex - startIndex);
            return {s, e};
        }
        else if (startIndex < 0 && endIndex < 0)
        {
            auto e = getCappedCharacterPos(str, endIndex);
            auto s = getCappedCharacterPos(str.substr(0, e), startIndex - endIndex);
            return {s, e - s};
        }
        else if (startIndex >= 0 && endIndex < 0)
        {
            auto s = getCappedCharacterPos(str, startIndex);
            auto e = getCappedCharacterPos(str.substr(s), endIndex);
            return {s, e - s};
        }
        else
        {
            auto s = getCappedCharacterPos(str, startIndex);
            auto e = getCappedCharacterPos(str, endIndex);
            return {s, e > s ? e - s : 0};
        }
    }

    Subrange getCodePointSubstringRange(std::string_view str,
                                               ptrdiff_t startIndex,
                                               ptrdiff_t endIndex)
    {
        if (endIndex < startIndex && (startIndex >= 0) == (endIndex >= 0))
            endIndex = startIndex;
        if (startIndex >= 0 && endIndex >= 0)
        {
            auto s = getCappedCodePointPos(str, startIndex);
            auto e = getCappedCodePointPos(str.substr(s), endIndex - startIndex);
            return {s, e};
        }
        else if (startIndex < 0 && endIndex < 0)
        {
            auto e = getCappedCodePointPos(str, endIndex);
            auto s = getCappedCodePointPos(str.substr(0, e), startIndex - endIndex);
            return {s, e - s};
        }
        else if (startIndex >= 0 && endIndex < 0)
        {
            auto s = getCappedCodePointPos(str, startIndex);
            auto e = getCappedCodePointPos(str.substr(s), endIndex);
            return {s, e};
        }
        else
        {
            auto s = getCappedCodePointPos(str, startIndex);
            auto e = getCappedCodePointPos(str, endIndex);
            return {s, std::max(s, e)};
        }
    }

    std::string insertAtOffset(std::string_view str, size_t offset, std::string_view codePoints)
    {
        if (codePoints.empty())
            return std::string(str);
        if (offset == std::string_view::npos)
            YSTRING_THROW("string position is out of bounds");
        std::string result(str.substr(0, offset));
        result.append(codePoints);
        result.append(str.substr(offset));
        return result;
    }

    std::string
    insertAtOffset(std::string_view str, size_t offset, char32_t codePoint)
    {
        if (offset == std::string_view::npos)
            YSTRING_THROW("string position is out of bounds");
        std::string result(str.substr(0, offset));
        append(result, codePoint);
        result.append(str.substr(offset));
        return result;
    }

    std::string
    replaceSubrange(std::string_view str, Subrange range, std::string_view repl)
    {
        std::string result;
        result.append(str.substr(0, range.start()));
        result.append(repl);
        result.append(str.substr(range.end()));
        return result;
    }
}
