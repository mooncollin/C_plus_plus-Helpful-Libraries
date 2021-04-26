export module cmoon.files;

import <fstream>;
import <string_view>;
import <algorithm>;
import <iterator>;
import <filesystem>;
import <regex>;
import <vector>;
import <string>;
import <concepts>;

import cmoon.string;

namespace cmoon
{
    export
    template<class T = std::string, class CharT = char, class Traits = std::char_traits<CharT>, class Distance = std::ptrdiff_t>
    class lines : public std::istream_iterator<T, CharT, Traits, Distance>
    {
        using parent = std::istream_iterator<T>;

        public:
            lines(std::istream& input)
                : input{std::addressof(input)}
            {
                operator++();
            }

            lines end() const
            {
                return lines<T>();
            }

            lines begin() const
            {
                return *this;
            }

            typename parent::reference operator*()
            {
                return current;
            }

            typename parent::pointer operator->() const
            {
                return &current;
            }

            lines& operator++()
            {
                next_input();
                return *this;
            }

            lines operator++(int)
            {
                auto l = lines(*this);
                next_input();

                return l;
            }

            friend bool operator==(const lines& lhs, const lines& rhs)
            {
                return lhs.input == rhs.input;
            }

            friend bool operator!=(const lines& lhs, const lines& rhs)
            {
                return !(lhs == rhs);
            }

        private:
            static constexpr auto end_of_input = nullptr;
            std::ifstream* input {end_of_input};
            T current;

            void next_input()
            {
                if (input != end_of_input)
                {
                    if (input->fail() || input->eof())
                    {
                        input = end_of_input;
                    }
                    else
                    {
                        if constexpr (std::same_as<T, std::string>)
                        {
                            std::getline(*input, current);
                        }
                        else
                        {
                            std::string str;
                            std::getline(*input, str);

                            current = cmoon::from_string<T>(str);
                        }
                    }
                }
            }
    };

    export
    std::string read_all(std::ifstream& input)
    {
        return std::string{(std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>()};
    }

    export
    template<class T = std::string>
    std::vector<T> read_lines(std::ifstream& input)
    {
        return std::vector<T>{lines<T>{input}, lines<T>{}};
    }

    export
    template<class T = std::string, std::output_iterator<T> Output>
    Output read_lines(std::ifstream& input, Output out)
    {
        return std::copy(lines<T>{input}, lines<T>{}, out);
    }

    export
    template<std::output_iterator<std::filesystem::directory_entry> Output>
    Output search_for_files(const std::filesystem::path& path, const std::regex& r, Output out)
    {
        for (const auto& p : std::filesystem::directory_iterator(path))
        {
            std::smatch match;
            const auto path_string = p.path().string();
            if (std::regex_match(path_string, match, r))
            {
                *out = p;
                ++out;
            }
        }

        return out;
    }

    export
    std::vector<std::filesystem::directory_entry> search_for_files(const std::filesystem::path& path, const std::regex& r)
    {
        std::vector<std::filesystem::directory_entry> entries;
        search_for_files(path, r, std::back_inserter(entries));
        return entries;
    }
}