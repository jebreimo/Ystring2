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
#include <Ystring/PathMatcher.hpp>
#include <utility>

class PathIteratorBase
{
public:
    virtual ~PathIteratorBase() = default;

    virtual void set_base_path(std::filesystem::path base_path) = 0;

    virtual bool next() = 0;

    bool next_directory()
    {
        while (next())
        {
            if (std::filesystem::is_directory(path()))
                return true;
        }
        return false;
    }

    [[nodiscard]]
    virtual std::filesystem::path path() const = 0;
};

class SinglePathIterator : public PathIteratorBase
{
public:
    SinglePathIterator(std::filesystem::path sub_path, bool has_next)
        : sub_path_(std::move(sub_path)),
          has_next_(has_next)
    {}

    void set_base_path(std::filesystem::path base_path) override
    {
        base_path_ = std::move(base_path);
        has_next_ = true;
    }

    bool next() override
    {
        if (!has_next_)
            return false;

        has_next_ = false;
        return exists(base_path_ / sub_path_);
    }

    [[nodiscard]]
    std::filesystem::path path() const override
    {
        return base_path_ / sub_path_;
    }

public:
    std::filesystem::path base_path_;
    std::filesystem::path sub_path_;
    bool has_next_ = true;
};

class GlobIterator : public PathIteratorBase
{
public:
    explicit GlobIterator(ystring::GlobMatcher matcher)
        : matcher_(std::move(matcher))
    {}

    bool next() override
    {
        while (it_ != end_)
        {
            auto filename = it_->path().filename().u8string();
            if (matcher_.match(ystring::to_string_view(filename)))
            {
                current_path_ = it_->path();
                ++it_;
                return true;
            }
            ++it_;
        }
        return false;
    }

    void set_base_path(std::filesystem::path base_path) override
    {
        base_path_ = std::move(base_path);
        it_ = std::filesystem::directory_iterator(base_path_);
        end_ = end(it_);
    }

    [[nodiscard]]
    std::filesystem::path path() const override
    {
        return current_path_;
    }
private:
    std::filesystem::directory_iterator it_;
    std::filesystem::directory_iterator end_;
    std::filesystem::path base_path_;
    std::filesystem::path current_path_;
    ystring::GlobMatcher matcher_;
};

class DoubleStarIterator : public PathIteratorBase
{
public:
    explicit DoubleStarIterator(ystring::PathMatcher matcher)
        : matcher_(std::move(matcher))
    {}

    void set_base_path(std::filesystem::path base_path) override
    {
        base_path_ = std::move(base_path);
        it_ = std::filesystem::recursive_directory_iterator(base_path_);
        end_ = end(it_);
    }

    bool next() override
    {
        while (it_ != end_)
        {
            if (matcher_.match(it_->path()))
            {
                current_path_ = it_->path();
                ++it_;
                return true;
            }
            ++it_;
        }
        return false;
    }

    [[nodiscard]]
    std::filesystem::path path() const override
    {
        return current_path_;
    }
private:
    std::filesystem::recursive_directory_iterator it_;
    std::filesystem::recursive_directory_iterator end_;
    std::filesystem::path base_path_;
    std::filesystem::path current_path_;
    ystring::PathMatcher matcher_;
};

std::filesystem::path
make_path(const std::filesystem::path::const_iterator& begin,
          const std::filesystem::path::const_iterator& end,
          std::filesystem::path prefix = {})
{
    std::filesystem::path path = std::move(prefix);
    for (auto it = begin; it != end; ++it)
        path /= *it;
    return path;
}

std::vector<std::unique_ptr<PathIteratorBase>>
parse_glob_path(const std::filesystem::path& path)
{
    std::vector<std::unique_ptr<PathIteratorBase>> result;
    std::filesystem::path plain_path;

    for (auto it = path.begin(), end = path.end(); it != end; ++it)
    {
        auto name = it->u8string();
        if (name == u8"**")
        {
            if (!plain_path.empty())
            {
                result.emplace_back(std::make_unique<SinglePathIterator>(std::move(plain_path), result.empty()));
                plain_path = std::filesystem::path();
            }

            result.emplace_back(std::make_unique<DoubleStarIterator>(
                ystring::PathMatcher(make_path(++it, end, u8"**"))));
            break;
        }

        if (ystring::is_glob_pattern(ystring::to_string_view(name)))
        {
            if (!plain_path.empty())
            {
                result.emplace_back(std::make_unique<SinglePathIterator>(std::move(plain_path),
                                                                         result.empty()));
                plain_path = std::filesystem::path();
            }

            result.emplace_back(std::make_unique<GlobIterator>(
                ystring::GlobMatcher(ystring::to_string_view(name))));
        }
        else
        {
            plain_path /= *it;
        }
    }

    if (!plain_path.empty())
    {
        result.emplace_back(std::make_unique<SinglePathIterator>(std::move(plain_path),
                                                                 result.empty()));
    }

    return result;
}

class GlobPathIterator
{
public:
    explicit GlobPathIterator(const std::filesystem::path& glob_path)
        : iterators_(parse_glob_path(glob_path))
    {}

    bool next()
    {
        if (iterators_.empty())
            return false;
        if (iterators_.back()->next())
            return true;

        auto last = --iterators_.cend();
        auto it = last;
        while (true)
        {
            if (!find_prev_with_next(it))
                return false;

            std::filesystem::path path = (*it)->path();
            for (++it; it != last; ++it)
            {
                (*it)->set_base_path(std::move(path));
                if (!(*it)->next_directory())
                    break;
                path = (*it)->path();
            }
            if (it == last)
            {
                (*it)->set_base_path(std::move(path));
                if ((*it)->next())
                    return true;
            }
        }
    }

    [[nodiscard]]
    std::filesystem::path path() const
    {
        return iterators_.back()->path();
    }
private:
    using Container = std::vector<std::unique_ptr<PathIteratorBase>>;

    bool find_prev_with_next(Container::const_iterator& it) const
    {
        while (it != iterators_.cbegin())
        {
            --it;
            if ((*it)->next_directory())
                return true;
        }
        return false;
    }

    std::vector<std::unique_ptr<PathIteratorBase>> iterators_;
};

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path>\n";
        return 1;
    }

    std::clog << "Glob path: " << argv[1] << '\n';
    GlobPathIterator it(argv[1]);
    while (it.next())
        std::clog << it.path() << '\n';

    return 0;
}
