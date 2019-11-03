#ifndef COLLIN_FILES
#define COLLIN_FILES

#include <fstream>
#include <string_view>
#include <algorithm>
#include <iterator>
#include "collin_string.hpp"

namespace collin
{
	template<typename T = std::string>
    class Lines : public std::istream_iterator<T>
    {
        using parent = std::istream_iterator<T>;

        public:
            Lines() = default;

            Lines(std::ifstream& input) : input(&input)
            {
                this->operator++();
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
            std::ifstream* input = nullptr;
            T current {};

            void next_input()
            {
                if(input != nullptr)
                {
                    if(input->fail() || input->eof())
                    {
                        input = nullptr;
                    }
                    else
                    {
                        std::string str;
                        std::getline(*input, str, '\n');

                        current = from_string<T>(std::string_view(str.c_str(), str.size()));
                    }
                }
            }
    };

    std::string read_all(std::string_view name)
    {
        std::ifstream file(name.data());

        return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }

    template<class T = std::string>
    std::vector<T> readlines(std::ifstream& input)
    {
        std::vector<T> lines;
        std::copy(Lines<T>(input), Lines<T>(), std::back_inserter(lines));

        return lines;
    }
}

#endif