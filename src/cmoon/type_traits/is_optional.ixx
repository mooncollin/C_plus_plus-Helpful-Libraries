export module cmoon.type_traits.is_optional;
import <optional>;

namespace cmoon
{
	export
	template<typename T>
	struct is_optional : std::false_type {};

	export
	template<typename T>
	struct is_optional<std::optional<T>> : std::true_type {};

	export
	template<typename T>
	struct is_optional<std::optional<const T>> : std::true_type {};
}