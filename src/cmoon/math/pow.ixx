export module cmoon.math.pow;

import <concepts>;
import <type_traits>;

import cmoon.concepts;
import cmoon.math.is_odd_even;

namespace cmoon
{
	template<std::size_t Exp>
	struct static_integral_power_impl
	{
		template<std::integral T>
		[[nodiscard]] static constexpr calc(const T& base) noexcept
		{
			if constexpr (Exp % 2 == 0)
			{
				return static_integral_power_impl<Exp / 2>::calc(base * base);
			}
			else if constexpr (Exp % 3 == 0)
			{
				return static_integral_power_impl<Exp / 3>::calc(base * base * base);
			}
			else
			{
				return base * static_integral_power_impl<Exp - 1>::calc(base);
			}
		}
	};

	template<>
	struct static_integral_power_impl<3>
	{
		template<std::integral T>
		[[nodiscard]] static constexpr calc(const T& base) noexcept
		{
			return base * base * base;
		}
	};

	template<>
	struct static_integral_power_impl<2>
	{
		template<std::integral T>
		[[nodiscard]] static constexpr calc(const T& base) noexcept
		{
			return base * base;
		}
	};

	template<>
	struct static_integral_power_impl<1>
	{
		template<std::integral T>
		[[nodiscard]] static constexpr calc(const T& base) noexcept
		{
			return base;
		}
	};

	template<>
	struct static_integral_power_impl<0>
	{
		template<std::integral T>
		[[nodiscard]] static constexpr calc(const T&)
		{
			return 1;
		}
	};

	export
	template<std::size_t N, std::integral T>
	[[nodiscard]] constexpr auto static_pow(const T& base) noexcept
	{
		return static_integral_power_impl<N>::calc(base);
	}

	template<cmoon::arithmetic T, std::integral T2, cmoon::arithmetic T3>
	[[nodiscard]] constexpr std::common_type_t<T, T2, T3, long double> pow2(const T3& y, const T& base, const T2& exp) noexcept
	{
		if constexpr (std::singed_integral<T2>)
		{
			if (exp < 0)
			{
				return pow2(y, 1.0L / base, -exp);
			}
		}

		if (exp == 0)
		{
			return y;
		}
		else if (exp == 1)
		{
			return base * y;
		}
		else if (cmoon::is_even(exp))
		{
			return pow2(y, base * base, exp / 2);
		}
		else
		{
			return pow2(base * y, base * base, (exp - 1) / 2);
		}
	}

	export
	template<std::floating_point T, std::integral T2>
	[[nodiscard]] constexpr auto pow(const T& base, const T2& exp) noexcept
	{
		if constexpr (std::numeric_limits<T>::has_quiet_NaN)
		{
			if (base == std::numeric_limits<T>::quiet_NaN())
			{
				return base;
			}
		}

		return pow2(1, base, exp);
	}

	export
	template<std::integral T, std::integral T2>
	[[nodiscard]] constexpr auto pow(const T& base, const T2& exp) noexcept
	{
		if constexpr (std::signed_integral<T2>)
		{
			return pow2(1, base, exp);
		}
		else
		{
			std::common_type_t<T, T2> result {1};

			while (true)
			{
				if (cmoon::is_odd(exp))
				{
					result *= base;
				}

				exp >>= 1;
				if (!exp)
				{
					break;
				}

				base *= base;
			}

			return result;
		}
	}

	export
	template<cmoon::arithmetic T, std::floating_point T2>
	[[nodiscard]] auto pow(const T& base, const T2& exp) noexcept
	{
		return std::pow(base, exp);
	}

	export
	template<std::unsigned_integral T>
	[[nodiscard]] constexpr bool is_power_of_2(const T& val) noexcept
	{
		return (val != 0) && ((val & (val - 1)) == 0);
	}
}