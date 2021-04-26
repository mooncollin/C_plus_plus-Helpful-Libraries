export module cmoon.iostream;
import <iostream>;
import <string>;
import <string_view>;
import <algorithm>;
import <iterator>;
import <locale>;
import <array>;
import <iostream>;

namespace cmoon
{
	export
	template<class T>
	T get_input(std::istream& in = std::cin)
	{
		T t;
		in >> t;
		return t;
	}

	export
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

	export
	std::istream& getline(std::istream& stream, std::string& str, std::string_view delim)
	{
		str.clear();
		std::string possible_delim;
		auto delim_matching = std::begin(delim);
		std::istream::sentry se(stream, true);
		auto sb = stream.rdbuf();

		while (true)
		{
			const auto ch = sb->sbumpc();
			switch (ch)
			{
				case std::streambuf::traits_type::eof():
					str += possible_delim;
					if (str.empty())
					{
						stream.setstate(std::ios::eofbit);
					}
					return stream;
				default:
					if (ch == *delim_matching)
					{
						if (++delim_matching == std::end(delim))
						{
							return stream;
						}

						possible_delim += static_cast<char>(ch);
					}
					else
					{
						if (!possible_delim.empty())
						{
							str += possible_delim;
							possible_delim.clear();
							delim_matching = std::begin(delim);
						}

						str += static_cast<char>(ch);
					}
			}
				
		}

		str += possible_delim;
		return stream;
	}
}