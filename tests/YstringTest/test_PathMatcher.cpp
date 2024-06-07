//****************************************************************************
// Copyright © 2024 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2024-06-02.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/PathMatcher.hpp"
#include <filesystem>
#include <fstream>
#include <ranges>
#include <catch2/catch_test_macros.hpp>

class TempFiles
{
public:
    TempFiles(std::initializer_list<std::filesystem::path> sub_paths)
    {
        tmp_path = std::filesystem::temp_directory_path();
        for (auto& sub_path : sub_paths)
            make_file(tmp_path / sub_path);
    }

    ~TempFiles()
    {
        for (auto& path : paths)
            std::filesystem::remove(path);
    }

    std::filesystem::path get_path(const std::filesystem::path& path) const
    {
        return tmp_path / path;
    }

private:
    void create_dirs(std::filesystem::path path)
    {
        std::vector<std::filesystem::path> dirs_to_create;
        while (path != path.root_path() && !std::filesystem::exists(path))
        {
            dirs_to_create.push_back(path);
            path = path.parent_path();
        }
        for (auto& it : std::ranges::reverse_view(dirs_to_create))
        {
            std::filesystem::create_directory(it);
            paths.push_back(it);
        }
    }

    void make_file(const std::filesystem::path& path)
    {
        if (std::filesystem::exists(path))
            return;

        create_dirs(path.parent_path());
        std::ofstream(path) << "Hello, world!";
        paths.push_back(path);
    }

    std::filesystem::path tmp_path;
    std::vector<std::filesystem::path> paths;
};

//TempFiles temp_files({"abc/a.txt", "abc/b.txt", "abd/c.md"});

TEST_CASE("Relative glob, relative paths")
{
    ystring::PathMatcher matcher(std::filesystem::path("abc/*.txt"));
    REQUIRE(matcher.match(std::filesystem::path("abc/def.txt")));
}

TEST_CASE("Relative glob, relative path starting with ./")
{
    ystring::PathMatcher matcher(std::filesystem::path("abc/*.txt"));
    REQUIRE(matcher.match(std::filesystem::path("./abc/a.txt")));
}

TEST_CASE("Relative glob, absolute path")
{
    ystring::PathMatcher matcher(std::filesystem::path("abc/*.txt"));
    REQUIRE_FALSE(matcher.match(std::filesystem::path("/abc/a.txt")));
}

TEST_CASE("Relative glob starting with ./, relative path")
{
    ystring::PathMatcher matcher(std::filesystem::path("./abc/*.txt"));
    REQUIRE(matcher.match(std::filesystem::path("abc/a.txt")));
}

TEST_CASE("Absolute glob, relative path")
{
    ystring::PathMatcher matcher(std::filesystem::path("/abc/cde/*.txt"));
    REQUIRE_FALSE(matcher.match(std::filesystem::path("abc/cde/a.txt")));
}

TEST_CASE("Absolute glob, absolute path")
{
    ystring::PathMatcher matcher(std::filesystem::path("/abc/cde/*.txt"));
    REQUIRE(matcher.match(std::filesystem::path("/abc/cde/a.txt")));
}

TEST_CASE("Multi-glob")
{
    ystring::PathMatcher matcher(std::filesystem::path("/*/cde/*.txt"));
    REQUIRE(matcher.match(std::filesystem::path("/abc/cde/a.txt")));
}

TEST_CASE("Un-rooted multi-dir glob matches rooted path")
{
    ystring::PathMatcher matcher(std::filesystem::path("**/cde/*.txt"));
    REQUIRE(matcher.match(std::filesystem::path("/abc/cde/a.txt")));
}

TEST_CASE("Un-rooted multi-dir glob matches un-rooted path")
{
    ystring::PathMatcher matcher(std::filesystem::path("**/cde/*.txt"));
    REQUIRE(matcher.match(std::filesystem::path("abc/cde/a.txt")));
}

TEST_CASE("Rooted multi-dir glob matches rooted path")
{
    ystring::PathMatcher matcher(std::filesystem::path("/**/cde/*.txt"));
    REQUIRE(matcher.match(std::filesystem::path("/abc/cde/a.txt")));
}

TEST_CASE("Rooted multi-dir glob does not match un-rooted path")
{
    ystring::PathMatcher matcher(std::filesystem::path("/**/cde/*.txt"));
    REQUIRE_FALSE(matcher.match(std::filesystem::path("abc/cde/a.txt")));
}

TEST_CASE("Multi-dir glob 2")
{
    ystring::PathMatcher matcher(std::filesystem::path("abc/**/cde/*.txt"));
    REQUIRE(matcher.match(std::filesystem::path("abc/a/b/cde/a.txt")));
}
