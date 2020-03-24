#pragma once

#include <fstream>
#include <string_view>
#include <algorithm>
#include <iterator>
#include <filesystem>
#include <regex>
#include <vector>
#include "collin_string.hpp"

namespace collin
{
    namespace files
    {
        template<typename T = std::string>
        class Lines : public std::istream_iterator<T>
        {
            using parent = std::istream_iterator<T>;

            public:
                Lines() = default;

                Lines(std::ifstream& input) : input(&input)
                {
                    operator++();
                }

                Lines<T> end() const
                {
                    return Lines<T>();
                }

                Lines<T> begin() const
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

                std::istream_iterator<T>& operator++()
                {
                    next_input();
                    return *this;
                }

                std::istream_iterator<T> operator++(int)
                {
                    auto l = Lines(*this);
                    next_input();

                    return l;
                }

                friend bool operator==(const Lines<T>& lhs, const Lines<T>& rhs)
                {
                    return lhs.input == rhs.input;
                }

                friend bool operator!=(const Lines<T>& lhs, const Lines<T>& rhs)
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
                                std::getline(*input, current, '\n');
                            }
                            else
                            {
                                std::string str;
                                std::getline(*input, str, '\n');

                                current = from_string<T>(str);
                            }
                        }
                    }
                }
        };

        std::string read_all(std::ifstream& input)
        {
            return std::string((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
        }

        template<class T = std::string>
        std::vector<T> read_lines(std::ifstream& input)
        {
            return std::vector<T>(Lines<T>(input), Lines<T>());
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
}