#pragma once

#include <cstdlib>

#include "cmoon/concepts.hpp"
#include "cmoon/measures/time.hpp"
#include "cmoon/finance/time.hpp"
#include "cmoon/finance/decimal.hpp"
#include "cmoon/math.hpp"

namespace cmoon
{
	namespace finance
	{
		template<std::size_t Precision>
		[[nodiscard]] constexpr auto dividend_yield(const decimal<Precision>& stock_price, const decimal<Precision>& annual_dividend) noexcept
		{
			return annual_dividend / stock_price;
		}

		template<std::size_t Precision, class RoundPolicy, cmoon::measures::time_type T, cmoon::measures::time_type YearType = basic_years365<typename T::rep>>
		[[nodiscard]] constexpr auto simple_interest(const decimal<Precision, RoundPolicy>& principal, const decimal<Precision, RoundPolicy>& interest_rate, const T& time, const YearType& = {}) noexcept
		{
			const auto years = cmoon::measures::unit_cast<YearType>(time);
			return principal * interest_rate * decimal<Precision>{years.count()};
		}

		template<std::size_t Precision, class RoundPolicy = default_round_policy>
		constexpr auto compound_yearly = decimal<Precision, RoundPolicy>{1};

		template<std::size_t Precision, class RoundPolicy = default_round_policy>
		constexpr auto compound_quartly = decimal<Precision, RoundPolicy>{4};

		template<std::size_t Precision, class RoundPolicy = default_round_policy>
		constexpr auto compound_monthly = decimal<Precision, RoundPolicy>{12};

		namespace details
		{
			template<class T>
			struct compound_daily_impl;

			template<>
			struct compound_daily_impl<years365>
			{
				template<std::size_t Precision, class RoundPolicy = default_round_policy>
				static constexpr auto value = decimal<Precision, RoundPolicy>{365};
			};

			template<>
			struct compound_daily_impl<years360>
			{
				template<std::size_t Precision, class RoundPolicy = default_round_policy>
				static constexpr auto value = decimal<Precision, RoundPolicy>{360};
			};
		}

		template<class T, std::size_t Precision, class RoundPolicy = default_round_policy>
		constexpr auto compound_daily = details::compound_daily_impl<T>::value<Precision, RoundPolicy>;


		template<std::size_t Precision, class RoundPolicy, cmoon::measures::time_type T>
		[[nodiscard]] constexpr auto compound_interest(const decimal<Precision, RoundPolicy>& principal, const decimal<Precision, RoundPolicy>& interest_rate, const decimal<Precision, RoundPolicy>& num_compounds, const T& time) noexcept
		{
			const auto years = cmoon::measures::unit_cast<cmoon::measures::basic_years<std::common_type_t<typename T::rep, round_conversion_t>>>(time);
			const auto calc1 = 1 + (interest_rate / num_compounds);
			const auto power = pow(calc1, num_compounds * decimal<Precision, RoundPolicy>{years.count()});
			return principal * (power - 1);
		}

		// Used when days are being used as the time period. This requires the user to specify
		// how long a year should be in days.
		template<std::size_t Precision, class RoundPolicy, class Rep, cmoon::measures::time_type YearType = basic_years365<std::common_type_t<Rep, round_conversion_t>>>
		[[nodiscard]] constexpr auto compound_interest(const decimal<Precision, RoundPolicy>& principal, const decimal<Precision, RoundPolicy>& interest_rate, const decimal<Precision, RoundPolicy>& num_compounds, const cmoon::measures::basic_days<Rep>& time, const YearType& = {}) noexcept
		{
			const auto years = cmoon::measures::unit_cast<YearType>(time);
			const auto calc1 = 1 + (interest_rate / num_compounds);
			const auto power = pow(calc1, num_compounds * decimal<Precision, RoundPolicy>{years.count()});
			return principal * (power - 1);
		}

		template<std::size_t Precision, class RoundPolicy, class Rep, cmoon::measures::time_type YearType = basic_years365<std::common_type_t<Rep, round_conversion_t>>>
		[[nodiscard]] constexpr auto compound_interest_continously(const decimal<Precision, RoundPolicy>& principal, const decimal<Precision, RoundPolicy>& interest_rate, const cmoon::measures::basic_days<Rep>& time, const YearType& = {}) noexcept
		{
			const auto years = cmoon::measures::unit_cast<YearType>(time);
			const auto power = pow(decimal_e<Precision, RoundPolicy>, interest_rate * decimal<Precision, RoundPolicy>{years.count()});
			return principal * (power - 1);
		}

		template<std::size_t Precision, class RoundPolicy, cmoon::measures::time_type T>
		[[nodiscard]] constexpr auto compound_interest_continously(const decimal<Precision, RoundPolicy>& principal, const decimal<Precision, RoundPolicy>& interest_rate, const T& time) noexcept
		{
			const auto years = cmoon::measures::unit_cast<cmoon::measures::basic_years<std::common_type_t<typename T::rep, round_conversion_t>>>(time);
			const auto power = pow(decimal_e<Precision, RoundPolicy>, interest_rate * decimal<Precision, RoundPolicy>{years.count()});
			return principal * (power - 1);
		}
	}
}