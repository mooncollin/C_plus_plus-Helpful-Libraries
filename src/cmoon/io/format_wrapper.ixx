export module cmoon.io.format_wrapper;

import <format>;
import <functional>;
import <iostream>;

import cmoon.concepts;

namespace cmoon
{
	export
	template<class T>
		requires(cmoon::stream_writable<T, std::stringstream> ||
				 cmoon::formattable<T>)
	class format_wrapper
	{
		public:
			using type = T;

			format_wrapper(const T& v) noexcept
				: value{v} {}

			const type& get() const noexcept
			{
				return value;
			}
		private:
			std::reference_wrapper<const type> value;
	};

	template<class T, class CharT, class Traits>
	std::basic_ostream<CharT, Traits> operator<<(std::basic_ostream<CharT, Traits>& os, const format_wrapper<T>& fw)
	{
		if constexpr (cmoon::stream_writable<T, std::basic_ostream<CharT, Traits>>)
		{
			return os << fw.get();
		}
		else
		{
			return os << std::format("{}", fw.get());
		}
	}
}

export
template<class T>
	requires(cmoon::formattable<T>)
struct std::formatter<cmoon::format_wrapper<T>> : public std::formatter<T>
{
	private:
		using base = std::formatter<T>;
	public:
		template<class FormatContext>
		auto format(const cmoon::format_wrapper<T>& fw, FormatContext& ctx)
		{
			return base::format(fw.get(), ctx);
		}
};

export
template<class T>
	requires(!cmoon::formattable<T>)
struct std::formatter<cmoon::format_wrapper<T>> : public std::formatter<std::string>
{
	private:
		using base = std::formatter<std::string>;
	public:
		template<class FormatContext>
		auto format(const cmoon::format_wrapper<T>& fw, FormatContext& ctx)
		{
			std::stringstream ss;
			ss << fw.get();
			return base::format(ss.str(), ctx);
		}
};