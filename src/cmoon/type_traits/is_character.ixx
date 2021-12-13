export module cmoon.type_traits.is_character;

import <type_traits>;
import <string>;

namespace cmoon
{
	export
	template<class T>
	struct is_character : std::false_type {};

	export
	template<>
	struct is_character<char> : std::true_type {};

	export
	template<>
	struct is_character<signed char> : std::true_type {};

	export
	template<>
	struct is_character<unsigned char> : std::true_type {};

	export
	template<>
	struct is_character<wchar_t> : std::true_type {};

	export
	template<>
	struct is_character<char8_t> : std::true_type {};

	export
	template<>
	struct is_character<char16_t> : std::true_type {};

	export
	template<>
	struct is_character<char32_t> : std::true_type {};

	export
	template<>
	struct is_character<const char> : std::true_type {};

	export
	template<>
	struct is_character<const signed char> : std::true_type {};

	export
	template<>
	struct is_character<const unsigned char> : std::true_type {};

	export
	template<>
	struct is_character<const wchar_t> : std::true_type {};

	export
	template<>
	struct is_character<const char8_t> : std::true_type {};

	export
	template<>
	struct is_character<const char16_t> : std::true_type {};

	export
	template<>
	struct is_character<const char32_t> : std::true_type {};

	export
	template<>
	struct is_character<int> : std::true_type {};

	export
	template<>
	struct is_character<const int> : std::true_type {};

	export
	template<class T>
	constexpr bool is_character_v = is_character<T>::value;
}