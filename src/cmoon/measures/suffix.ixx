module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.suffix;

import <type_traits>;
import <string_view>;

import cmoon.string;

namespace cmoon::measures
{
	export
	template<class T>
	struct suffix_dimension : std::false_type {};

	export
	template<class T>
		requires(requires() { T::dimension_opt_out; })
	struct suffix_dimension<T> : std::bool_constant<T::dimension_opt_out> {};

	template<class T, class CharT>
	struct suffix
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS(""))};
		static constexpr bool dimension_opt_out {false};
	};

	template<class T, class CharT>
	constexpr auto suffix_v = suffix<T, CharT>::value;

	template<class T, class CharT>
	constexpr auto suffix_dimension_v = suffix_dimension<suffix<T, CharT>>::value;
}