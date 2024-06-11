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

struct Expression
{
    std::filesystem::path base_path;
};

struct Stars {};

using GlobPathPart = std::variant<std::filesystem::path, Stars, ystring::GlobMatcher>;
using GlobPath = std::vector<GlobPathPart>;

std::pair<std::filesystem::path, GlobPath>
parse_glob_path(std::filesystem::path path)
{
    GlobPath result;
    while (true)
    {
        auto filename = path.filename();
        auto filename_str = filename.u8string();
        auto filename_view = ystring::to_string_view(filename_str);
        if (filename == u8"**")
            result.emplace_back(Stars());
        else if (ystring::is_glob_pattern(filename_view))
            result.emplace_back(ystring::GlobMatcher(filename_view));
        else if (!filename.empty())
            result.emplace_back(filename);
        auto parent_path = path.parent_path();
        if (parent_path == path)
        {
            if (!parent_path.empty())
                result.emplace_back(parent_path);
            break;
        }
        path = parent_path;
    }

    std::filesystem::path parent_dir;
    auto it = result.rbegin();
    while (it != result.rend())
    {
        if (!std::holds_alternative<std::filesystem::path>(*it))
            break;

        if (parent_dir.empty())
            parent_dir = std::get<std::filesystem::path>(*it);
        else
            parent_dir /= std::get<std::filesystem::path>(*it);
        ++it;
    }

    result.erase(it.base(), result.end());
    std::reverse(result.begin(), result.end());
    return {parent_dir, result};
}

std::ostream& operator<<(std::ostream& os, const GlobPathPart& part)
{
    struct Visitor
    {
        std::ostream& os;

        void operator()(const std::filesystem::path& path) const
        {
            os << path;
        }

        void operator()(const Stars&) const
        {
            os << "**";
        }

        void operator()(const ystring::GlobMatcher& matcher) const
        {
            os << matcher;
        }
    };
    std::visit(Visitor{os}, part);
    return os;
}

class GlobPathIterator
{
public:
    explicit GlobPathIterator(const std::filesystem::path& glob_path)
    {
        auto [parent_dir, parts] = parse_glob_path(glob_path);
        for (const auto& part : parts)
            std::cout << part << '\n';

    }

    bool next()
    {
        //while (m_it != std::filesystem::directory_iterator())
        //{
        //    if (m_it->is_directory())
        //    {
        //        m_stack.push_back(m_it);
        //        m_it = std::filesystem::directory_iterator(*m_it);
        //    }
        //    else
        //    {
        //        m_current = *m_it;
        //        ++m_it;
        //        return true;
        //    }
        //}
        //
        //if (!m_stack.empty())
        //{
        //    m_it = m_stack.back();
        //    m_stack.pop_back();
        //    ++m_it;
        //    return next();
        //}

        return false;
    }

    //const std::filesystem::directory_entry& operator*() const
    //{
    //    return it_.operator*();
    //}

};

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path>\n";
        return 1;
    }

    if (ystring::is_glob_pattern(argv[1]))
    {
        GlobPathIterator it(argv[1]);
        //while (it.next())
        //    std::cout << *it << '\n';
    }
    //for (const auto entry : ystring::split(argv[1], U"/"))
    //    std::cout << entry << '\n';
    GlobPathIterator matcher(argv[1]);

    return 0;
}
