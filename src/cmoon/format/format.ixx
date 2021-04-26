module;
#include "cmoon/string/literals.hpp"
#include "cmoon/format/forward_declarations.hpp"

export module cmoon.format;

import <string>;
import <variant>;
import <string_view>;
import <stdexcept>;
import <type_traits>;
import <concepts>;
import <array>;
import <span>;
import <iterator>;
import <optional>;
import <cctype>;
import <locale>;
import <charconv>;
import <utility>;
import <algorithm>;
import <cstdlib>;
import <cstdio>;
import <bitset>;
import <sstream>;
import <ctime>;
import <chrono>;
import <iomanip>;
import <ranges>;
import <iostream>;
import <time.h>;

import cmoon.string;
import cmoon.platform;
import cmoon.utility;

export import cmoon.format.format_error;
export import cmoon.format.basic_format_context;
export import cmoon.format.basic_format_arg;
export import cmoon.format.visit_format_arg;
export import cmoon.format.basic_format_args;
export import cmoon.format.basic_format_parse_context;

import cmoon.format.is_formatter;
import cmoon.format.format_specs;
import cmoon.format.utility;
import cmoon.format.format_parser;
import cmoon.format.base_formatter;
import cmoon.format.chrono;
import cmoon.format.write_string_view;
import cmoon.format.write_integer;
import cmoon.format.write_boolean;
import cmoon.format.write_pointer;
import cmoon.format.write_floating;

namespace cmoon
{
	export
	template<typename CharT, class Traits>
	struct formatter<std::basic_string_view<CharT, Traits>, CharT> : public base_formatter<std::basic_string_view<CharT, Traits>, CharT>
	{
		template<class OutputIt>
		auto format(std::basic_string_view<CharT, Traits> str, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_string_view(str, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<CharT, CharT> : public base_formatter<CharT, CharT>
	{
		template<class OutputIt>
		auto format(CharT c, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_string_view(std::basic_string_view<CharT>{std::addressof(c), 1}, ctx, this->parser);
		}
	};

	export
	template<>
	struct formatter<char, wchar_t> : public base_formatter<char, wchar_t>
	{
		template<class OutputIt>
		auto format(char c, basic_format_context<OutputIt, wchar_t>& ctx)
		{
			wchar_t wide;
			std::mbtowc(std::addressof(wide), std::addressof(c), 1);
			return write_string_view(std::wstring_view{std::addressof(wide), 1}, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<const CharT*, CharT> : public formatter<std::basic_string_view<CharT>, CharT>
	{
		template<class OutputIt>
		auto format(const CharT* c, basic_format_context<OutputIt, CharT>& ctx)
		{
			return formatter<std::basic_string_view<CharT>, CharT>::format(c, ctx);
		}
	};

	export
	template<typename CharT>
	struct formatter<CharT*, CharT> : public formatter<std::basic_string_view<CharT>, CharT>
	{
		template<class OutputIt>
		auto format(CharT* c, basic_format_context<OutputIt, CharT>& ctx)
		{
			return formatter<std::basic_string_view<CharT>, CharT>(c, ctx);
		}
	};

	export
	template<typename CharT, std::size_t N>
	struct formatter<const CharT[N], CharT> : public formatter<std::basic_string_view<CharT>, CharT>
	{
		template<class OutputIt>
		auto format(const CharT c[N], basic_format_context<OutputIt, CharT>& ctx)
		{
			return formatter<std::basic_string_view<CharT>, CharT>::format({c, N}, ctx);
		}
	};

	export
	template<typename CharT, class Traits, class Alloc>
	struct formatter<std::basic_string<CharT, Traits, Alloc>, CharT> : public formatter<std::basic_string_view<CharT, Traits>, CharT>
	{
		template<class OutputIt>
		auto format(const std::basic_string<CharT, Traits, Alloc>& str, basic_format_context<OutputIt, CharT>& ctx)
		{
			return formatter<std::basic_string_view<CharT>, CharT>::format(str, ctx);
		}
	};

	export
	template<typename CharT>
	struct formatter<int, CharT> : public base_formatter<int, CharT>
	{
		template<class OutputIt>
		auto format(int value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_integer(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<short, CharT> : public base_formatter<short, CharT>
	{
		template<class OutputIt>
		auto format(short value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_integer(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<unsigned short, CharT> : public base_formatter<unsigned short, CharT>
	{
		template<class OutputIt>
		auto format(unsigned short value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_integer(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<unsigned int, CharT> : public base_formatter<unsigned int, CharT>
	{
		template<class OutputIt>
		auto format(unsigned int value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_integer(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<long, CharT> : public base_formatter<long, CharT>
	{
		template<class OutputIt>
		auto format(long value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_integer(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<unsigned long, CharT> : public base_formatter<unsigned long, CharT>
	{
		template<class OutputIt>
		auto format(unsigned long value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_integer(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<long long, CharT> : public base_formatter<long long, CharT>
	{
		template<class OutputIt>
		auto format(long long value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_integer(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<unsigned long long, CharT> : public base_formatter<unsigned long long, CharT>
	{
		template<class OutputIt>
		auto format(unsigned long long value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_integer(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<bool, CharT> : public base_formatter<bool, CharT>
	{
		template<class OutputIt>
		auto format(bool value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_boolean(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<const void*, CharT> : public base_formatter<const void*, CharT>
	{
		template<class OutputIt>
		auto format(const void* value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_pointer(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<std::nullptr_t, CharT> : public formatter<const void*, CharT>
	{
		template<class OutputIt>
		auto format(std::nullptr_t value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return formatter<const void*, CharT>::format(value, ctx);
		}
	};

	export
	template<typename CharT>
	struct formatter<void*, CharT> : public formatter<const void*, CharT>
	{
		template<class OutputIt>
		auto format(void* value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return formatter<const void*, CharT>::format(value, ctx);
		}
	};

	export
	template<typename CharT>
	struct formatter<double, CharT> : public base_formatter<double, CharT>
	{
		template<class OutputIt>
		auto format(double value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_floating(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<long double, CharT> : public base_formatter<long double, CharT>
	{
		template<class OutputIt>
		auto format(long double value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_floating(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<float, CharT> : public formatter<double, CharT>
	{
		template<class OutputIt>
		auto format(float value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return formatter<double, CharT>::format(value, ctx);
		}
	};

	export
	using format_args = basic_format_args<format_context>;

	export
	using wformat_args = basic_format_args<wformat_context>;

	export
	template<class Ctx, class... Args>
	format_arg_store<Ctx, Args...> make_format_args(const Args&... args)
	{
		return {args...};
	}

	export
	template<class... Args>
	format_arg_store<wformat_context, Args...> make_wformat_args(const Args&... args)
	{
		return {args...};
	}

	namespace details
	{
		template<typename CharT, class Out>
		Out vformat_to(Out out, const std::locale& loc, std::basic_string_view<CharT> fmt, format_args_t<Out, CharT>&& args)
		{
			basic_format_parse_context<CharT> parse_context {fmt, args.data_.size()};
			basic_format_context<Out, CharT> format_context {std::move(args), out, loc};

			constexpr auto open_curly = CharT('{');
			constexpr auto close_curly = CharT('}');
			constexpr auto colon = CharT(':');

			bool in_open_curly {false};
			bool in_close_curly {false};
			while (!at_end(parse_context))
			{
				const auto ch = get_char(parse_context);
				if (in_open_curly)
				{
					if (ch == open_curly)
					{
						set_char(format_context, open_curly);
						advance_one(format_context);
						advance_one(parse_context);
					}
					else
					{
						const auto id = parse_arg_id(parse_context);
						if (get_char(parse_context) == colon)
						{
							advance_one(parse_context);
						}

						// TODO: Replace with 'visit_format_arg' at some point.
						// MSVC won't let me and is giving me error C1126.
						std::visit(cmoon::overloaded (
							[](std::monostate) {
								throw format_error{"Invalid format argument"};
							},
							[&parse_context, &format_context](const auto& v) {
								// Cannot have as another overload, because MSVC will try to generate
								// a formatter with type of basic_format_arg::handle, which does not exist.
								// So I need to constexpr that out.
								if constexpr (std::same_as<std::decay_t<decltype(v)>, typename basic_format_arg<decltype(format_context)>::handle>)
								{
									v.format(parse_context, format_context);
								}
								else
								{
									formatter<std::decay_t<decltype(v)>, CharT> form;
									parse_context.advance_to(form.parse(parse_context));
									format_context.advance_to(form.format(v, format_context));
								}
							}
						), format_context.arg(id).value);

						if (at_end(parse_context))
						{
							throw format_error{"invalid format string"};
						}
						else
						{
							advance_one(parse_context);
						}
					}

					in_open_curly = false;
				}
				else if (in_close_curly)
				{
					if (ch == close_curly)
					{
						set_char(format_context, close_curly);
						advance_one(format_context);
						advance_one(parse_context);
					}
					else
					{
						throw format_error{"invalid format string"};
					}

					in_close_curly = false;
				}
				else if (ch == open_curly)
				{
					in_open_curly = true;
					advance_one(parse_context);
				}
				else if (ch == close_curly)
				{
					in_close_curly = true;
					advance_one(parse_context);
				}
				else
				{
					set_char(format_context, ch);
					advance_one(format_context);
					advance_one(parse_context);
				}
			}

			return format_context.out();
		}
	}

	export
	template<class T, class Context = format_context>
	concept can_format = 
		requires()
	{
		typename Context::template formatter_type<std::decay_t<T>>;
		sizeof(typename Context::template formatter_type<std::decay_t<T>>);
	};

	export
	template<std::output_iterator<char> Out>
	Out vformat_to(Out out, const std::locale& loc, std::string_view fmt, format_args_t<std::type_identity_t<Out>, char> args)
	{
		return details::vformat_to(out, loc, fmt, std::move(args));
	}

	export
	template<std::output_iterator<wchar_t> Out>
	Out vformat_to(Out out, const std::locale& loc, std::wstring_view fmt, format_args_t<std::type_identity_t<Out>, char> args)
	{
		return details::vformat_to(out, loc, fmt, std::move(args));
	}

	export
	template<std::output_iterator<char> Out>
	Out vformat_to(Out out, std::string_view fmt, format_args_t<Out, char> args)
	{
		return details::vformat_to(out, std::locale{}, fmt, std::move(args));
	}

	export
	template<std::output_iterator<wchar_t> Out>
	Out vformat_to(Out out, std::wstring_view fmt, format_args_t<Out, wchar_t> args)
	{
		return details::vformat_to(out, std::locale{}, fmt, std::move(args));
	}

	export
	std::string vformat(const std::locale& loc, std::string_view fmt, format_args args)
	{
		std::string result;
		details::vformat_to(std::back_inserter(result), loc, fmt, std::move(args));
		return result;
	}

	export
	std::wstring vformat(const std::locale& loc, std::wstring_view fmt, wformat_args args)
	{
		std::wstring result;
		details::vformat_to(std::back_inserter(result), loc, fmt, std::move(args));
		return result;
	}

	export
	std::string vformat(std::string_view fmt, format_args args)
	{
		std::string result;
		details::vformat_to(std::back_inserter(result), std::locale{}, fmt, std::move(args));
		return result;
	}

	export
	std::wstring vformat(std::wstring_view fmt, wformat_args args)
	{
		std::wstring result;
		details::vformat_to(std::back_inserter(result), std::locale{}, fmt, std::move(args));
		return result;
	}

	export
	template<class... Args>
		requires(can_format<Args, format_context> && ...)
	std::string format(const std::locale& l, std::string_view fmt, const Args&... args)
	{
		return vformat(l, fmt, make_format_args<format_context>(args...));
	}

	export
	template<class... Args>
		requires(can_format<Args, wformat_context> && ...)
	std::wstring format(const std::locale& l, std::wstring_view fmt, const Args&... args)
	{
		return vformat(l, fmt, make_wformat_args(args...));
	}

	export
	template<class... Args>
		requires(can_format<Args, format_context> && ...)
	std::string format(std::string_view fmt, const Args&... args)
	{
		return vformat(fmt, make_format_args<format_context>(args...));
	}

	export
	template<class... Args>
		requires(can_format<Args, wformat_context> && ...)
	std::wstring format(std::wstring_view fmt, const Args&... args)
	{
		return vformat(fmt, make_wformat_args(args...));
	}

	export
	template<std::output_iterator<char> Out, class... Args>
	Out format_to(Out out, const std::locale& l, std::string_view fmt, const Args&... args)
	{
		using context = basic_format_context<Out, char>;
		return vformat_to(out, l, fmt, {make_format_args<context>(args...)});
	}

	export
	template<std::output_iterator<wchar_t> Out, class... Args>
	Out format_to(Out out, const std::locale& l, std::wstring_view fmt, const Args&... args)
	{
		using context = basic_format_context<Out, wchar_t>;
		return vformat_to(out, l, fmt, {make_wformat_args<context>(args...)});
	}

	export
	template<std::output_iterator<char> Out, class... Args>
	Out format_to(Out out, std::string_view fmt, const Args&... args)
	{
		using context = basic_format_context<Out, char>;
		return vformat_to(out, fmt, {make_format_args<context>(args...)});
	}

	export
	template<std::output_iterator<wchar_t> Out, class... Args>
	Out format_to(Out out, std::wstring_view fmt, const Args&... args)
	{
		using context = basic_format_context<Out, wchar_t>;
		return vformat_to(out, fmt, {make_format_args<context>(args...)});
	}

	export
	template<class Out>
	struct format_to_n_result
	{
		Out out;
		std::iter_difference_t<Out> size;
	};

	template<class CharT>
	class format_size_counter
	{
		public:
			using iterator_category = std::output_iterator_tag;
			using value_type = CharT;
			using difference_type = std::ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;

			constexpr format_size_counter() noexcept = default;

			constexpr reference operator*() noexcept
			{
				++count_;
				return dummy_;
			}

			constexpr format_size_counter& operator++() noexcept
			{
				return *this;
			}

			constexpr format_size_counter operator++(int) noexcept
			{
				auto copy = *this;
				++*this;
				return copy;
			}

			[[nodiscard]] constexpr std::size_t count() const noexcept
			{
				return count_;
			}
		private:
			std::size_t count_ {};
			value_type dummy_;
	};

	template<class Out>
	class format_to_n_tracker
	{
		public:
			using iterator_category = std::output_iterator_tag;
			using value_type = std::iter_value_t<Out>;
			using difference_type = std::iter_difference_t<Out>;
			using pointer = value_type*;
			using reference = std::iter_reference_t<Out>;

			constexpr format_to_n_tracker() noexcept(std::is_nothrow_default_constructible_v<Out>) = default;

			constexpr format_to_n_tracker(Out out, difference_type limit)
				: out_{out}, limit_{limit} {}

			[[nodiscard]] constexpr Out out() noexcept
			{
				return out_;
			}

			constexpr reference operator*() noexcept(noexcept(*std::declval<Out>()))
			{
				if (limit_ == 0)
				{
					return dummy_;
				}

				--limit_;
				return *out_;
			}

			constexpr format_to_n_tracker& operator++() noexcept(noexcept(++std::declval<Out>()))
			{
				++out_;
				return *this;
			}

			constexpr format_to_n_tracker operator++(int)  noexcept(noexcept(std::declval<Out>()++))
			{
				auto copy = *this;
				++*this;
				return copy;
			}

			[[nodiscard]] constexpr difference_type limit() const noexcept
			{
				return limit_;
			}
		private:
			Out out_ {};
			difference_type limit_ {};
			value_type dummy_ {};
	};

	export
	template<std::output_iterator<char> Out, class... Args>
	format_to_n_result<Out> format_to_n(Out out, std::iter_difference_t<Out> n, const std::locale& l, std::string_view fmt, const Args&... args)
	{
		format_to_n_tracker tracker{out, n};
		tracker = format_to(tracker, l, fmt, args...);
		return {tracker.out(), n - tracker.limit()};
	}

	export
	template<std::output_iterator<wchar_t> Out, class... Args>
	format_to_n_result<Out> format_to_n(Out out, std::iter_difference_t<Out> n, const std::locale& l, std::wstring_view fmt, const Args&... args)
	{
		format_to_n_tracker tracker{out, n};
		tracker = format_to(tracker, l, fmt, args...);
		return {tracker.out(), n - tracker.limit()};
	}

	export
	template<std::output_iterator<char> Out, class... Args>
	format_to_n_result<Out> format_to_n(Out out, std::iter_difference_t<Out> n, std::string_view fmt, const Args&... args)
	{
		format_to_n_tracker tracker{out, n};
		tracker = format_to(tracker, fmt, args...);
		return {tracker.out(), n - tracker.limit()};
	}

	export
	template<std::output_iterator<wchar_t> Out, class... Args>
	format_to_n_result<Out> format_to_n(Out out, std::iter_difference_t<Out> n, std::wstring_view fmt, const Args&... args)
	{
		format_to_n_tracker tracker{out, n};
		tracker = format_to(tracker, fmt, args...);
		return {tracker.out(), n - tracker.limit()};
	}

	export
	template<class... Args>
	std::size_t formatted_size(const std::locale& l, std::string_view fmt, const Args&... args)
	{
		format_size_counter<char> c;
		c = format_to(c, l, fmt, args...);
		return c.count();
	}

	export
	template<class... Args>
	std::size_t formatted_size(const std::locale& l, std::wstring_view fmt, const Args&... args)
	{
		format_size_counter<wchar_t> c;
		c = format_to(c, l, fmt, args...);
		return c.count();
	}

	export
	template<class... Args>
	std::size_t formatted_size(std::string_view fmt, const Args&... args)
	{
		format_size_counter<char> c;
		c = format_to(c, fmt, args...);
		return c.count();
	}

	export
	template<class... Args>
	std::size_t formatted_size(std::wstring_view fmt, const Args&... args)
	{
		format_size_counter<wchar_t> c;
		c = format_to(c, fmt, args...);
		return c.count();
	}
}