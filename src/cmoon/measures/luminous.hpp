#pragma once

#include <type_traits>

#include "cmoon/measures/measure.hpp"
#include "cmoon/ratio.hpp"
#include "cmoon/string.hpp"

namespace cmoon
{
	namespace measures
	{
		template<class Rep>
		struct luminous_intensity_values;

		template<class Rep, class System, cmoon::ratio_type Ratio = std::ratio<1>, dimension_type Dimension = 1>
		using luminous_intensity = basic_unit<Rep, Ratio, luminous_intensity_values<Rep>, System, Dimension>;

		namespace details
		{
			template<class Rep, class System, cmoon::ratio_type Ratio, dimension_type Dimension>
			std::true_type is_luminous_intensity_base_impl(const luminous_intensity<Rep, System, Ratio, Dimension>&);
			std::false_type is_luminous_intensity_base_impl(...);

			template<class U>
			constexpr auto is_based_in_luminous_intensity = decltype(is_luminous_intensity_base_impl(std::declval<U>()))::value;
		}

		template<class T>
		struct is_luminous_intensity : std::bool_constant<details::is_based_in_luminous_intensity<T>> {};

		template<class T>
		constexpr bool is_luminous_intensity_v = is_luminous_intensity<T>::value;

		template<class T>
		concept luminous_intensity_type = is_luminous_intensity_v<T>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_attocandela = luminous_intensity<Rep, metric_system, std::atto, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_femtocandela = luminous_intensity<Rep, metric_system, std::femto, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_picocandela = luminous_intensity<Rep, metric_system, std::pico, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_nanocandela = luminous_intensity<Rep, metric_system, std::nano, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_microcandela = luminous_intensity<Rep, metric_system, std::micro, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_millicandela = luminous_intensity<Rep, metric_system, std::milli, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_centicandela = luminous_intensity<Rep, metric_system, std::centi, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_decicandela = luminous_intensity<Rep, metric_system, std::deci, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_decacandela = luminous_intensity<Rep, metric_system, std::deca, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_hectocandela = luminous_intensity<Rep, metric_system, std::hecto, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_kilocandela = luminous_intensity<Rep, metric_system, std::kilo, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_megacandela = luminous_intensity<Rep, metric_system, std::mega, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_gigacandela = luminous_intensity<Rep, metric_system, std::giga, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_teracandela = luminous_intensity<Rep, metric_system, std::tera, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_exacandela = luminous_intensity<Rep, metric_system, std::exa, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_candela = luminous_intensity<Rep, metric_system, std::ratio<1>, Dimension>;

		using attocandela = basic_attocandela<std::intmax_t>;
		using femtocandela = basic_femtocandela<std::intmax_t>;
		using picocandela = basic_picocandela<std::intmax_t>;
		using nanocandela = basic_nanocandela<std::intmax_t>;
		using microcandela = basic_microcandela<std::intmax_t>;
		using millicandela = basic_millicandela<std::intmax_t>;
		using centicandela = basic_centicandela<std::intmax_t>;
		using decicandela = basic_decicandela<std::intmax_t>;
		using decacandela = basic_decacandela<std::intmax_t>;
		using hectocandela = basic_hectocandela<std::intmax_t>;
		using kilocandela = basic_kilocandela<std::intmax_t>;
		using megacandela = basic_megacandela<std::intmax_t>;
		using gigacandela = basic_gigacandela<std::intmax_t>;
		using teracandela = basic_teracandela<std::intmax_t>;
		using exacandela = basic_exacandela<std::intmax_t>;
		using candela = basic_candela<std::intmax_t>;

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

		template<class Rep, dimension_type Dimension, class CharT>
		struct metric_system::suffix<basic_candela<Rep, Dimension>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("cd"))};
		};
	}
}