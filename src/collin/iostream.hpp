#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include <iterator>
#include <locale>
#include <array>

namespace collin
{
	namespace iostream
	{
		template<class InputIterator>
		std::ostream& print(InputIterator begin, InputIterator end, const char* sep="", std::ostream& stream=std::cout)
		{
			std::copy(begin, end, std::ostream_iterator<decltype(*begin)>(stream, sep));

			return stream;
		}

		template<class Container>
		std::ostream& print(const Container& container, const char* sep="", std::ostream& stream=std::cout)
		{
			return print(std::cbegin(container), std::cend(container), sep, stream);
		}

		template<>
		std::ostream& print<std::string>(const std::string& str, const char* sep, std::ostream& stream)
		{
			stream << str << sep;
			return stream;
		}

		std::ostream& print(std::string_view str, const char* sep="", std::ostream& stream=std::cout)
		{
			stream << str << sep;
			return stream;
		}

		std::ostream& println(const std::string& str, std::ostream& stream=std::cout)
		{
			return print(str, "\n", stream);
		}

		std::ostream& println(std::string_view str="", std::ostream& stream=std::cout)
		{
			return print(str, "\n", stream);
		}

		std::istream& getline(std::istream& stream, std::string& str, std::string_view delim)
		{
			std::string possible_delim;
			auto delim_matching = std::begin(delim);
			char ch;
			while (stream.get(ch))
			{
				if (ch == *delim_matching)
				{
					if (++delim_matching == std::end(delim))
					{
						return stream;
					}
					
					possible_delim += ch;
				}
				else
				{
					if (!possible_delim.empty())
					{
						str += possible_delim;
						possible_delim.clear();
						delim_matching = std::begin(delim);
					}

					str += ch;
				}
			}

			str += possible_delim;
			return stream;
		}
	}
}