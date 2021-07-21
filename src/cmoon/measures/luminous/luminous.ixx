module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.luminous;

import <type_traits>;
import <ratio>;

import cmoon.string;

import cmoon.measures;

namespace cmoon::measures
{
	export
	template<class Rep>
	struct luminous_intensity_values;

	export
	template<class Rep, class System, class Ratio = std::ratio<1>, dimension_type Dimension = 1>
	using luminous_intensity = basic_unit<Rep, Ratio, luminous_intensity_values<Rep>, System, Dimension>;

	template<class Rep, class System, class Ratio, dimension_type Dimension>
	std::true_type is_luminous_intensity_base_impl(const luminous_intensity<Rep, System, Ratio, Dimension>&);

	std::false_type is_luminous_intensity_base_impl(...);

	template<class U>
	constexpr auto is_based_in_luminous_intensity = decltype(is_luminous_intensity_base_impl(std::declval<U>()))::value;

	export
	template<class T>
	struct is_luminous_intensity : std::bool_constant<is_based_in_luminous_intensity<T>> {};

	export
	template<class T>
	constexpr bool is_luminous_intensity_v = is_luminous_intensity<T>::value;

	export
	template<class T>
	concept luminous_intensity_type = is_luminous_intensity_v<T>;

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_attocandela = luminous_intensity<Rep, metric_system, std::atto, Dimension>;

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_femtocandela = luminous_intensity<Rep, metric_system, std::femto, Dimension>;

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_picocandela = luminous_intensity<Rep, metric_system, std::pico, Dimension>;

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_nanocandela = luminous_intensity<Rep, metric_system, std::nano, Dimension>;

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_microcandela = luminous_intensity<Rep, metric_system, std::micro, Dimension>;

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_millicandela = luminous_intensity<Rep, metric_system, std::milli, Dimension>;

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_centicandela = luminous_intensity<Rep, metric_system, std::centi, Dimension>;

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_decicandela = luminous_intensity<Rep, metric_system, std::deci, Dimension>;

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_decacandela = luminous_intensity<Rep, metric_system, std::deca, Dimension>;

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_hectocandela = luminous_intensity<Rep, metric_system, std::hecto, Dimension>;

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_kilocandela = luminous_intensity<Rep, metric_system, std::kilo, Dimension>;

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_megacandela = luminous_intensity<Rep, metric_system, std::mega, Dimension>;

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_gigacandela = luminous_intensity<Rep, metric_system, std::giga, Dimension>;

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_teracandela = luminous_intensity<Rep, metric_system, std::tera, Dimension>;

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_exacandela = luminous_intensity<Rep, metric_system, std::exa, Dimension>;

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_candela = luminous_intensity<Rep, metric_system, std::ratio<1>, Dimension>;

	export
	using attocandela = basic_attocandela<std::intmax_t>;
	
	export
	using femtocandela = basic_femtocandela<std::intmax_t>;
	
	export
	using picocandela = basic_picocandela<std::intmax_t>;
	
	export
	using nanocandela = basic_nanocandela<std::intmax_t>;
	
	export
	using microcandela = basic_microcandela<std::intmax_t>;
	
	export
	using millicandela = basic_millicandela<std::intmax_t>;
	
	export
	using centicandela = basic_centicandela<std::intmax_t>;
	
	export
	using decicandela = basic_decicandela<std::intmax_t>;
	
	export
	using decacandela = basic_decacandela<std::intmax_t>;
	
	export
	using hectocandela = basic_hectocandela<std::intmax_t>;
	
	export
	using kilocandela = basic_kilocandela<std::intmax_t>;
	
	export
	using megacandela = basic_megacandela<std::intmax_t>;
	
	export
	using gigacandela = basic_gigacandela<std::intmax_t>;
	
	export
	using teracandela = basic_teracandela<std::intmax_t>;
	
	export
	using exacandela = basic_exacandela<std::intmax_t>;
	
	export
	using candela = basic_candela<std::intmax_t>;

	export
	template<class Rep>
	struct luminous_intensity_values
	{
		static constexpr Rep zero() noexcept
		{
			return Rep(0);
		}

		static constexpr Rep min() noexcept
		{
			return std::numeric_limits<Rep>::lowest();
		}

		static constexpr Rep max() noexcept
		{
			return std::numeric_limits<Rep>::max();
		}
	};

	export
	template<class Rep, dimension_type Dimension, class CharT>
	struct metric_system::suffix<basic_candela<Rep, Dimension>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("cd"))};
	};
}