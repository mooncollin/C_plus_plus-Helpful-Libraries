module;

#include <variant>
#include <string_view>
#include <concepts>
#include <locale>

#include "cmoon/format/forward_declarations.hpp"

export module cmoon.format.basic_format_arg;

namespace cmoon
{
	export
	template<class Context>
	class basic_format_arg
	{
		private:
			using char_type = typename Context::char_type;

		public:
			class handle
			{
				public:
					void format(basic_format_parse_context<char_type>& parse_ctx, Context& ctx) const
					{
						format_(parse_ctx, ctx, ptr_);
					}
				private:
					const void* ptr_;

					void (*format_)(basic_format_parse_context<char_type>&, Context&, const void*);

					template<class T>
					explicit handle(const T& val) noexcept
						: ptr_{std::addressof(val)}, format_{[](basic_format_parse_context<char_type>& parse_ctx, Context& format_ctx, const void* ptr) {
																	typename Context::template formatter_type<T> f;
																	parse_ctx.advance_to(f.parse(parse_ctx));
																	format_ctx.advance_to(f.format(*static_cast<const T*>(ptr), format_ctx));
																}} {}

					friend class basic_format_arg<Context>;
			};

			basic_format_arg() noexcept = default;

			explicit operator bool() const noexcept
			{
				return !std::holds_alternative<std::monostate>(value);
			}
		private:
			std::variant<std::monostate, bool, char_type, int, unsigned int,
				long long int, unsigned long long int, double, long double,
				const char_type*, std::basic_string_view<char_type>,
				const void*, handle> value {};

			template<context_has_formatter_type<Context> T>
			[[nodiscard]] static constexpr auto choose_constructor(const T& v) noexcept
			{
				if constexpr (std::same_as<T, bool> || std::same_as<T, char_type>)
				{
					return v;
				}
				else if constexpr (std::same_as<T, char> && std::same_as<char_type, wchar_t>)
				{
					return static_cast<wchar_t>(v);
				}
				else if constexpr (std::signed_integral<T> && sizeof(T) <= sizeof(int))
				{
					return static_cast<int>(v);
				}
				else if constexpr (std::unsigned_integral<T> && sizeof(T) <= sizeof(unsigned int))
				{
					return static_cast<unsigned int>(v);
				}
				else if constexpr (std::signed_integral<T> && sizeof(T) <= sizeof(long long int))
				{
					return static_cast<long long int>(v);
				}
				else if constexpr (std::unsigned_integral<T> && sizeof(T) <= sizeof(unsigned long long int))
				{
					return static_cast<unsigned long long int>(v);
				}
				else
				{
					return handle{v};
				}
			}

			template<context_has_formatter_type<Context> T>
			explicit basic_format_arg(const T& v) noexcept
				: value{choose_constructor(v)} {}

			explicit basic_format_arg(const float n) noexcept
				: value{static_cast<double>(n)} {}

			explicit basic_format_arg(const double n) noexcept
				: value{n} {}

			explicit basic_format_arg(const long double n) noexcept
				: value{n} {}

			explicit basic_format_arg(const char_type* s) noexcept
				: value{s} {}

			template<class Traits>
			explicit basic_format_arg(std::basic_string_view<char_type, Traits> s) noexcept
				: value{s} {}

			template<class Traits, class Allocator>
			explicit basic_format_arg(const std::basic_string<char_type, Traits, Allocator>& s) noexcept
				: value{std::basic_string_view<char_type>{s.data(), s.size()}} {}

			explicit basic_format_arg(std::nullptr_t) noexcept
				: value{static_cast<const void*>(nullptr)} {}

			template<class T>
				requires(std::is_void_v<T>)
			explicit basic_format_arg(const T* p) noexcept
				: value{p} {}

			template<class Visitor, class Ctx>
			friend decltype(auto) visit_format_arg(Visitor&& vis, basic_format_arg<Ctx> arg);

			template<class Ctx, class... Args>
			friend struct format_arg_store;

			template<typename CharT, class Out>
			friend Out details::vformat_to(Out, const std::locale&, std::basic_string_view<CharT>, format_args_t<Out, CharT>&&);

			template<typename T, typename CharT>
			friend class format_parser;
	};
}