//****************************************************************************
// Copyright © 2013 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2013-07-01
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <array>
#include <map>
#include <vector>
#include "CharMappingTypes.hpp"

namespace Ystring { namespace Unicode
{
    static const char16_t FAST_MAPPING_SIZE = 1024;
    // The max code point is 63 greater than the array size and not 1 less
    // because the first 64 code points are non-letters and can therefore
    // be ignored.
    static const char16_t FAST_MAPPING_MAX = FAST_MAPPING_SIZE + 64 - 1;

    class YSTRING_API CharMap
    {
    public:
        CharMap(const CompactCharMapping* compactMappings,
                size_t compactMappingsSize,
                const CharMapping* mappings,
                size_t mappingsSize);

        char32_t get(char32_t chr) const;
        bool has(char32_t chr) const;
    private:
        bool findInCompactMapping(char32_t chr, char32_t& mappedChr) const;
        bool findInMapping(char32_t chr, char32_t& mappedChr) const;
        void initializeFastMapping();

        const CompactCharMapping* m_CompactMappings;
        const CharMapping* m_Mappings;
        std::array<char16_t, FAST_MAPPING_SIZE> m_FastMapping;
        size_t m_CompactMappingsSize;
        size_t m_MappingsSize;
    };
}}
