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
		template<class T>
		T get_input(std::istream& in = std::cin)
		{
			T t;
			in >> t;
			return t;
		}

		std::istream& getline(std::istream& stream, std::string& str, std::string_view delim)
		{
			str.clear();
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