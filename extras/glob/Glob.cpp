//****************************************************************************
// Copyright © 2024 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2024-04-26.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <filesystem>
#include <iostream>
#include <Ystring/Algorithms.hpp>
#include <Ystring/GlobMatcher.hpp>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path>\n";
        return 1;
    }

    for (const auto entry : ystring::split(argv[1], U"/"))
        std::cout << entry << '\n';
    ystring::GlobMatcher matcher(argv[1]);

    return 0;
}
