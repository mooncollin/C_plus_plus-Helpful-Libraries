export module cmoon.io.read;

import <string>;
import <iterator>;
import <string_view>;
import <streambuf>;
import <algorithm>;

namespace cmoon
{
	export
	template<class CharT, class Traits, class Allocator>
	std::basic_streambuf<CharT, Traits>* read_all(std::basic_streambuf<CharT, Traits>* buf, std::basic_string<CharT, Traits, Allocator>& str)
	{
		if (buf == nullptr)
		{
			return buf;
		}

		str.clear();
		std::streamsize available;
		
		while ((available = buf->in_avail()) != -1)
		{
			if (available == 0)
			{
				if (const auto ch = buf->sbumpc(); ch == Traits::eof())
				{
					break;
				}
				else
				{
					str.push_back(Traits::to_char_type(ch));
				}
			}
			else
			{
				str.resize(available + std::size(str));
				buf->sgetn(std::data(str) + (std::size(str) - available), available);
			}
		}

		return buf;
	}

	export
    template<class CharT, class Traits>
	std::basic_istream<CharT, Traits>& read_all(std::basic_istream<CharT, Traits>& input, std::basic_string<CharT, Traits>& str)
    {
		const std::basic_istream<CharT, Traits>::sentry ok{input, true};
		if (ok)
		{
			auto sb = input.rdbuf();
			read_all(sb, str);
			if (sb != nullptr && Traits::eq_int_type(sb->sgetc(), Traits::eof()))
			{
				input.setstate(std::ios::eofbit);
			}
		}

		return input;
    }

	export
	template<class CharT, class Traits, std::output_iterator<CharT> Output>
	Output read_all(std::basic_streambuf<CharT, Traits>* buf, Output out)
	{
		if constexpr (std::contiguous_iterator<Output>)
		{
			std::streamsize available;
			while ((available = buf->in_avail()) != -1)
			{
				if (available == 0)
				{
					if (const auto ch = buf->sbumpc(); ch == Traits::eof())
					{
						break;
					}
					else
					{
						*out = Traits::to_char_type(ch);
						++out; 
					}
				}
				else
				{
					out += buf->sgetn(out, available);
				}
			}

			return out;
		}
		else
		{
			return std::ranges::copy(std::istreambuf_iterator{buf}, std::default_sentinel, out).out;
		}
	}

	export
	template<class CharT, class Traits, std::output_iterator<CharT> Output>
	Output read_all(std::basic_istream<CharT, Traits>& input, Output out)
	{
		const std::basic_istream<CharT, Traits>::sentry ok{input, true};
		if (ok)
		{
			auto sb = input.rdbuf();
			out = read_all(sb, out);
			if (sb != nullptr && Traits::eq_int_type(sb->sgetc(), Traits::eof()))
			{
				input.setstate(std::ios::eofbit);
			}
		}

		return out;
	}

	export
	template<class CharT, class Traits, class Allocator>
	std::basic_streambuf<CharT, Traits>* read_until(std::basic_streambuf<CharT, Traits>* buf, std::basic_string<CharT, Traits, Allocator>& str, std::basic_string_view<CharT, Traits> delim)
	{
		if (buf == nullptr)
		{
			return buf;
		}

		str.clear();

		auto delim_matching = std::begin(delim);
		typename Traits::int_type int_ch;

		while (!Traits::eq_int_type(int_ch = buf->sbumpc(),  Traits::eof()))
		{
			const auto ch = Traits::to_char_type(int_ch);
			if (ch == *delim_matching)
			{
				if (++delim_matching == std::end(delim))
				{
					str.erase(std::size(str) - std::size(delim) + 1);
					return buf;
				}
			}
			else
			{
				delim_matching = std::begin(delim);
			}
			
			str += ch;
		}

		return buf;
	}

    export
	template<class CharT, class Traits, class Allocator>
	std::basic_istream<CharT, Traits>& read_until(std::basic_istream<CharT, Traits>& input, std::basic_string<CharT, Traits, Allocator>& str, std::basic_string_view<CharT, Traits> delim)
	{
		const std::basic_istream<CharT, Traits>::sentry ok{input, true};
		if (ok)
		{
			auto sb = input.rdbuf();
			read_until(sb, str, delim);
			if (sb != nullptr && Traits::eq_int_type(sb->sgetc(), Traits::eof()))
			{
				input.setstate(std::ios::eofbit);
			}
		}

		return input;
	}

	export
	template<class CharT, class Traits, std::output_iterator<CharT> Output>
	Output read_until(std::basic_streambuf<CharT, Traits>* buf, Output out, std::basic_string_view<CharT, Traits> delim)
	{
		if (buf == nullptr)
		{
			return out;
		}

		std::basic_string<CharT, Traits> possible_delim;
		auto delim_matching = std::begin(delim);
		typename Traits::int_type int_ch;

		while (!Traits::eq_int_type(int_ch = buf->sbumpc(),  Traits::eof()))
		{
			const auto ch = Traits::to_char_type(int_ch);
			if (ch == *delim_matching)
			{
				if (++delim_matching == std::end(delim))
				{
					return out;
				}

				possible_delim += ch;
			}
			else
			{
				if (!possible_delim.empty())
				{
					delim_matching = std::begin(delim);
					out = std::ranges::copy(possible_delim, out).out;
				}

				*out = ch;
				++out;
			}
		}

		return std::ranges::copy(possible_delim, out).out;
	}

	export
	template<class CharT, class Traits, std::output_iterator<CharT> Output>
	Output read_until(std::basic_istream<CharT, Traits>& input, Output out, std::basic_string_view<CharT, Traits> delim)
	{
		const std::basic_istream<CharT, Traits>::sentry ok{input, true};
		if (ok)
		{
			auto sb = input.rdbuf();
			out = read_until(sb, out, delim);
			if (sb != nullptr && Traits::eq_int_type(sb->sgetc(), Traits::eof()))
			{
				input.setstate(std::ios::eofbit);
			}
		}

		return out;
	}

	export
	template<class CharT, class Traits>
	std::basic_streambuf<CharT, Traits>* ignore_until(std::basic_streambuf<CharT, Traits>* buf, std::basic_string_view<CharT, Traits> delim)
	{
		if (buf == nullptr)
		{
			return buf;
		}

		auto delim_matching = std::begin(delim);
		typename Traits::int_type int_ch;

		while (!Traits::eq_int_type(int_ch = buf->sbumpc(),  Traits::eof()))
		{
			const auto ch = Traits::to_char_type(int_ch);
			if (ch == *delim_matching)
			{
				if (++delim_matching == std::end(delim))
				{
					return buf;
				}
			}
			else
			{
				delim_matching = std::begin(delim);
			}
		}

		return buf;
	}

	export
	template<class CharT, class Traits>
	std::basic_istream<CharT, Traits>& ignore_until(std::basic_istream<CharT, Traits>& input, std::basic_string_view<CharT, Traits> delim)
	{
		const std::basic_istream<CharT, Traits>::sentry ok{input, true};
		if (ok)
		{
			auto sb = input.rdbuf();
			ignore_until(sb, delim);
			if (sb != nullptr && Traits::eq_int_type(sb->sgetc(), Traits::eof()))
			{
				input.setstate(std::ios::eofbit);
			}
		}

		return input;
	}
}