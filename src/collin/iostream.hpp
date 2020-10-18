#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include <iterator>
#include <locale>
#include <array>
#include <iostream>

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

		std::istream& getline(std::istream& is, std::string& str)
		{
			str.clear();

			std::istream::sentry se(is, true);
			auto sb = is.rdbuf();
			while(true)
			{
				auto c = sb->sbumpc();
				switch (c)
				{
					case '\n':
						return is;
					case '\r':
						if (sb->sgetc() == '\n')
						{
							sb->sbumpc();
						}
						return is;
					case std::streambuf::traits_type::eof():
						if (str.empty())
						{
							is.setstate(std::ios::eofbit);
						}
						return is;
					default:
						str += static_cast<char>(c);
				}
			}
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