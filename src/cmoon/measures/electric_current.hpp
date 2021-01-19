#pragma once

#include "cmoon/measures/measure.hpp"
#include "cmoon/ratio.hpp"

namespace cmoon
{
	namespace measures
	{
		template<class Rep>
		struct electric_current_values;

		template<class Rep, class System, cmoon::ratio_type Ratio = std::ratio<1>, dimension_type Dimension = 1>
		using electric_current = basic_unit<Rep, Ratio, electric_current_values<Rep>, System, Dimension>;

		template<class T>
		struct is_electric_current : std::false_type {};

		template<class Rep, class System, cmoon::ratio_type Ratio, dimension_type Dimension>
		struct is_electric_current<electric_current<Rep, System, Ratio, Dimension>> : std::true_type {};

		template<class T>
		constexpr bool is_electric_current_v = is_electric_current<T>::value;

		template<class T>
		concept electric_current_type = is_electric_current_v<T>;

		template<class T, dimension_type Dimension = 1>
		using basic_ampere = electric_current<T, metric_system, std::ratio<1>, Dimension>;

		using ampere = basic_ampere<std::intmax_t>;

		template<class Rep>
		struct electric_current_values
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

		template<class Rep, dimension_type Dimension>
		struct metric_system::suffix<basic_ampere<Rep, Dimension>>
		{
			constexpr static std::string_view value {"A"};
		};
	}
}