export module cmoon.type_traits.is_char_literal;
import <type_traits>;

namespace cmoon
{
	export
	template<typename T>
	struct is_char_literal : std::false_type {};

	export
	template<>
	struct is_char_literal<char> : std::true_type {};

	export
	template<>
	struct is_char_literal<signed char> : std::true_type {};

	export
	template<>
	struct is_char_literal<unsigned char> : std::true_type {};

	export
	template<>
	struct is_char_literal<wchar_t> : std::true_type {};

	export
	template<>
	struct is_char_literal<char8_t> : std::true_type {};

	export
	template<>
	struct is_char_literal<char16_t> : std::true_type {};

	export
	template<>
	struct is_char_literal<char32_t> : std::true_type {};

	export
	template<>
	struct is_char_literal<const char> : std::true_type {};

	export
	template<>
	struct is_char_literal<const signed char> : std::true_type {};

	export
	template<>
	struct is_char_literal<const unsigned char> : std::true_type {};

	export
	template<>
	struct is_char_literal<const wchar_t> : std::true_type {};

	export
	template<>
	struct is_char_literal<const char8_t> : std::true_type {};

	export
	template<>
	struct is_char_literal<const char16_t> : std::true_type {};

	export
	template<>
	struct is_char_literal<const char32_t> : std::true_type {};

	export
	template<class T>
	constexpr auto is_char_literal_v = is_char_literal<T>::value;
}