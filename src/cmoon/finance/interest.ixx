export module cmoon.finance.interest;

import <chrono>;
import <cstddef>;

import cmoon.math;

import cmoon.finance.decimal;
import cmoon.finance.time;

namespace cmoon::finance
{
	export
	template<std::size_t Precision, class RoundPolicy, class Duration, class YearType = basic_years365<typename Duration::rep>>
	[[nodiscard]] constexpr auto simple_interest(const decimal<Precision, RoundPolicy>& principal, const decimal<Precision, RoundPolicy>& annual_interest_rate, const Duration& time, const YearType& = {}) noexcept
	{
		const auto years = std::chrono::duration_cast<YearType>(time);
		return principal * (annual_interest_rate * decimal<Precision>{years.count()});
	}

	export
	template<std::size_t Precision, class RoundPolicy = default_round_policy>
	struct compounding_factor
	{
		decimal<Precision, RoundPolicy> value;
	};
	
	export
	template<std::size_t Precision, class RoundPolicy = default_round_policy>
	constexpr auto compound_yearly = compounding_factor<Precision, RoundPolicy>{1};
	
	export
	template<std::size_t Precision, class RoundPolicy = default_round_policy>
	constexpr auto compound_quartly = compounding_factor<Precision, RoundPolicy>{4};
	
	export
	template<std::size_t Precision, class RoundPolicy = default_round_policy>
	constexpr auto compound_monthly = compounding_factor<Precision, RoundPolicy>{12};

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

	export
	template<class T, std::size_t Precision, class RoundPolicy = default_round_policy>
	constexpr compounding_factor compound_daily{compound_daily_impl<T>::template value<Precision, RoundPolicy>};

	export
	template<std::size_t Precision, class RoundPolicy, class Duration>
	[[nodiscard]] constexpr auto compound_interest(const decimal<Precision, RoundPolicy>& principal, const decimal<Precision, RoundPolicy>& interest_rate, const compounding_factor<Precision, RoundPolicy>& num_compounds, const Duration& time) noexcept
	{
		const auto years = std::chrono::duration_cast<std::chrono::duration<std::common_type_t<typename Duration::rep, round_conversion_t>, typename std::chrono::years::period>>(time);
		const auto calc1 = 1 + (interest_rate / num_compounds.value);
		const auto power = pow(calc1, num_compounds.value * decimal<Precision, RoundPolicy>{years.count()});
		return principal * (power - 1);
	}

	// Used when days are being used as the time period. This requires the user to specify
	// how long a year should be in days.
	export
	template<std::size_t Precision, class RoundPolicy, class Rep, class YearType = basic_years365<std::common_type_t<Rep, round_conversion_t>>>
	[[nodiscard]] constexpr auto compound_interest(const decimal<Precision, RoundPolicy>& principal, const decimal<Precision, RoundPolicy>& interest_rate, const compounding_factor<Precision, RoundPolicy>& num_compounds, const std::chrono::duration<Rep, typename std::chrono::days::period>& time, const YearType& = {}) noexcept
	{
		const auto years = std::chrono::duration_cast<YearType>(time);
		const auto calc1 = 1 + (interest_rate / num_compounds.value);
		const auto power = pow(calc1, num_compounds.value * decimal<Precision, RoundPolicy>{years.count()});
		return principal * (power - 1);
	}

	export
	template<std::size_t Precision, class RoundPolicy, class Rep, class YearType = basic_years365<std::common_type_t<Rep, round_conversion_t>>>
	[[nodiscard]] constexpr auto compound_interest_continously(const decimal<Precision, RoundPolicy>& principal, const decimal<Precision, RoundPolicy>& interest_rate, const std::chrono::duration<Rep, typename std::chrono::days::period>& time, const YearType & = {}) noexcept
	{
		const auto years = std::chrono::duration_cast<YearType>(time);
		const auto power = pow(decimal_e<Precision, RoundPolicy>, interest_rate * decimal<Precision, RoundPolicy>{years.count()});
		return principal * (power - 1);
	}

	export
	template<std::size_t Precision, class RoundPolicy, class Duration>
	[[nodiscard]] constexpr auto compound_interest_continously(const decimal<Precision, RoundPolicy>& principal, const decimal<Precision, RoundPolicy>& interest_rate, const Duration& time) noexcept
	{
		const auto years = std::chrono::duration_cast<std::chrono::duration<std::common_type_t<typename Duration::rep, round_conversion_t>, typename std::chrono::years::period>>(time);
		const auto power = pow(decimal_e<Precision, RoundPolicy>, interest_rate * decimal<Precision, RoundPolicy>{years.count()});
		return principal * (power - 1);
	}
}