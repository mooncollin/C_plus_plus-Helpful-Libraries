module;

#include <type_traits>
#include <concepts>

export module cmoon.format.is_formatter;
import cmoon.format.basic_format_parse_context;

namespace cmoon
{
	export
	template<class F, class CharT>
	concept is_formatter = std::is_default_constructible_v<F> &&
						   std::is_copy_constructible_v<F> &&
						   std::is_copy_assignable_v<F> &&
						   std::is_destructible_v<F> &&
						   std::swappable<F> &&
		requires(F f, basic_format_parse_context<CharT> pc)
	{
		{ f.parse(pc) } -> std::same_as<typename basic_format_parse_context<CharT>::iterator>;
	};
}