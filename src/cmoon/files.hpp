#pragma once

#include <fstream>
#include <string_view>
#include <algorithm>
#include <iterator>
#include <filesystem>
#include <regex>
#include <vector>

#include "cmoon/string.hpp"

namespace cmoon
{
    template<class T = std::string, class CharT = char, class Traits = std::char_traits<CharT>, class Distance = std::ptrdiff_t>
    class Lines : public std::istream_iterator<T, CharT, Traits, Distance>
    {
        using parent = std::istream_iterator<T>;

        public:
            Lines(std::istream& input)
                : input(std::addressof(input))
            {
                operator++();
            }

            Lines end() const
            {
                return Lines<T>();
            }

            Lines begin() const
            {
                return *this;
            }

            typename parent::reference operator*() const
            {
                return current;
            }

            typename parent::pointer operator->() const
            {
                return &current;
            }

            Lines& operator++()
            {
                next_input();
                return *this;
            }

            Lines operator++(int)
            {
                auto l = Lines(*this);
                next_input();

                return l;
            }

            friend bool operator==(const Lines& lhs, const Lines& rhs)
            {
                return lhs.input == rhs.input;
            }

            friend bool operator!=(const Lines& lhs, const Lines& rhs)
            {
                return !(lhs == rhs);
            }

        private:
            static constexpr auto end_of_input = nullptr;
            std::ifstream* input = end_of_input;
            T current;

            void next_input()
            {
                if(input != end_of_input)
                {
                    if(input->fail() || input->eof())
                    {
                        input = end_of_input;
                    }
                    else
                    {
                        if constexpr(std::is_same_v<T, std::string>)
                        {
                            std::getline(*input, current);
                        }
                        else
                        {
                            std::string str;
                            std::getline(*input, str);

                            current = from_string<T>(str);
                        }
                    }
                }
            }
    };

    std::string read_all(std::ifstream& input)
    {
        return std::string{(std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>()};
    }

    template<class T = std::string>
    std::vector<T> read_lines(std::ifstream& input)
    {
        return std::vector<T>(Lines<T>{input}, Lines<T>{});
    }

    std::vector<std::filesystem::directory_entry> search_for_files(const std::filesystem::path& path, const std::regex& r)
    {
        std::vector<std::filesystem::directory_entry> entries;

        for (const auto& p : std::filesystem::directory_iterator(path))
        {
            std::smatch match;
            const auto path_string = p.path().string();
            if (std::regex_match(path_string, match, r))
            {
                entries.push_back(p);
            }
        }

        return entries;
    }
}