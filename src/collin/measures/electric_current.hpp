#pragma once

#include "measure.hpp"

namespace collin
{
	namespace measures
	{
		template<class Rep>
		struct electric_current_values;

		template<class Rep, class System, class Ratio = std::ratio<1>>
		using electric_current = basic_unit<Rep, Ratio, electric_current_values<Rep>, System>;

		template<class T>
		struct is_electric_current : std::false_type {};

		template<class Rep, class System, class Ratio>
		struct is_electric_current<electric_current<Rep, System, Ratio>> : std::true_type {};

		template<class T>
		constexpr bool is_electric_current_v = is_electric_current<T>::value;

		template<class T>
		using basic_ampere = electric_current<T, metric_system>;

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

		template<class Rep>
		struct metric_system::suffix<basic_ampere<Rep>>
		{
			constexpr static std::string_view value {"A"};
		};
	}
}