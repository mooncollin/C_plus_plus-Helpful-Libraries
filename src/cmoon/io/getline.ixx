export module cmoon.io.getline;

import <string>;
import <string_view>;

namespace cmoon
{
	export
	template<class CharT, class Traits, class Allocator>
	std::basic_istream<CharT, Traits>& getline(std::basic_istream<CharT, Traits>& is, std::basic_string<CharT, Traits, Allocator>& str)
	{
		str.clear();

		typename std::basic_istream<CharT, Traits>::sentry se{is, true};
		auto sb = is.rdbuf();
		while(true)
		{
			const auto c = sb->sbumpc();
			switch (c)
			{
				case CharT('\n'):
					return is;
				case CharT('\r'):
					if (sb->sgetc() == CharT('\n'))
					{
						sb->sbumpc();
					}
					return is;
				case std::basic_istream<CharT, Traits>::traits_type::eof():
					if (str.empty())
					{
						is.setstate(std::ios::eofbit);
					}
					return is;
				default:
					str += static_cast<CharT>(c);
			}
		}
	}
}