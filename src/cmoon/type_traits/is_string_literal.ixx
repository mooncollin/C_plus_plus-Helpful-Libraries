export module cmoon.type_traits.is_string_literal;

import <cstddef>;

namespace cmoon
{
	export
	template<typename T>
	struct is_string_literal : std::false_type {};

	export
	template<std::size_t N>
	struct is_string_literal<const char[N]> : std::true_type {};

	export
	template<std::size_t N>
	struct is_string_literal<const wchar_t[N]> : std::true_type {};

	export
	template<std::size_t N>
	struct is_string_literal<const char8_t[N]> : std::true_type {};

	export
	template<std::size_t N>
	struct is_string_literal<const char16_t[N]> : std::true_type {};

	export
	template<std::size_t N>
	struct is_string_literal<const char32_t[N]> : std::true_type {};

	export
	template<std::size_t N>
	struct is_string_literal<const char(&)[N]> : std::true_type {};

	export
	template<std::size_t N>
	struct is_string_literal<const wchar_t(&)[N]> : std::true_type {};

	export
	template<std::size_t N>
	struct is_string_literal<const char8_t(&)[N]> : std::true_type {};

	export
	template<std::size_t N>
	struct is_string_literal<const char16_t(&)[N]> : std::true_type {};

	export
	template<std::size_t N>
	struct is_string_literal<const char32_t(&)[N]> : std::true_type {};

	export
	template<class T>
	constexpr auto is_string_literal_v = is_string_literal<T>::value;
}