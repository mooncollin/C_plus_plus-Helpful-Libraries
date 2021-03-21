module;
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <iostream>
#include <cstdint>
#include <cmath>
#include <numeric>
#include <array>
#include <utility>
#include <limits>
#include <algorithm>
#include <execution>
#include <cinttypes>
#include <concepts>

export module cmoon.math;
import cmoon.concepts;

namespace cmoon
{
	export
	template<std::intmax_t Pn>
	struct static_sign : std::integral_constant<std::intmax_t, (Pn < 0) ? -1 : 1> {};

	export
	template<std::intmax_t Pn>
	struct static_abs : std::integral_constant<std::intmax_t, Pn* static_sign<Pn>::value> {};

	export
	template<std::intmax_t Pn, std::intmax_t Qn>
	struct static_gcd : static_gcd<Qn, (Pn% Qn)> {};

	export
	template<std::intmax_t Pn>
	struct static_gcd<Pn, 0> : std::integral_constant<std::intmax_t, static_abs<Pn>::value> {};

	export
	template<std::intmax_t Qn>
	struct static_gcd<0, Qn> : std::integral_constant<std::intmax_t, static_abs<Qn>::value> {};

	export
	template<std::uintmax_t N, std::uintmax_t P>
	struct static_pow : static_pow<N * N, P - 1> {};

	export
	template<std::uintmax_t N>
	struct static_pow<N, 1> : std::integral_constant<std::uintmax_t, N> {};

	export
	template<std::uintmax_t N>
	struct static_pow<N, 0> : std::integral_constant<std::uintmax_t, 1> {};

	export
	template<std::integral T>
	[[nodiscard]] constexpr T power_of_10(T n) noexcept
	{
		T result {1};
		while (n)
		{
			result *= T{10};
			--n;
		}

		return result;
	}

	export
	[[nodiscard]] constexpr std::uintmax_t factorial(std::size_t n) noexcept
	{
		std::uintmax_t result {1};
		for(std::size_t i {2}; i <= n; ++i)
		{
			result *= i;
		}

		return result;
	}

	export
	template<std::integral T>
	[[nodiscard]] constexpr bool is_odd(const T& val) noexcept
	{
		return val & 1;
	}

	export
	template<std::floating_point F>
	[[nodiscard]] constexpr bool is_odd(const F& val) noexcept
	{
		return is_odd(static_cast<std::intmax_t>(val));
	}

	export
	template<cmoon::arithmetic T>
	[[nodiscard]] constexpr bool is_even(const T& val) noexcept
	{
		return !is_odd(val);
	}

	export
	template<std::unsigned_integral T>
	[[nodiscard]] constexpr bool is_power_of_2(const T& val) noexcept
	{
		return (val != 0) && ((val & (val - 1)) == 0);
	}

	export
	template<cmoon::arithmetic T>
	[[nodiscard]] constexpr T abs(const T& value) noexcept
	{
		if constexpr (std::is_unsigned_v<T>)
		{
			return value;
		}
		else
		{
			return value < T{0} ? -value : value;
		}
	}

	export
	template<std::integral Rep = int, std::floating_point Float>
	[[nodiscard]] constexpr Rep ceil(const Float& f) noexcept
	{
		const auto inum = static_cast<Rep>(f);
			
		if(f == static_cast<Float>(inum))
		{
			return inum;
		}

		return inum + (f > 0 ? 1 : 0);
	}

	export
	template<std::integral Rep = int, std::floating_point Float>
	[[nodiscard]] constexpr Rep floor(const Float& f) noexcept
	{
		const auto val_int = static_cast<Rep>(f);

		if(f >= 0 || f == static_cast<Float>(val_int))
		{
			return val_int;
		}

		return val_int - 1;
	}

	template<cmoon::arithmetic T, std::integral T2, cmoon::arithmetic T3>
	[[nodiscard]] constexpr std::common_type_t<T, T2, T3, long double> pow2(const T3& y, const T& base, const T2& exp) noexcept
	{
		if (exp < 0)
		{
			return pow2(y, 1.0L / base, -exp);
		}
		else if (exp == 0)
		{
			return y;
		}
		else if (exp == 1)
		{
			return base * y;
		}
		else if (is_even(exp))
		{
			return pow2(y, base * base, exp / 2);
		}
		else
		{
			return pow2(base * y, base * base, (exp - 1) / 2);
		}
	}

	export
	template<cmoon::arithmetic T, std::integral T2>
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
	template<cmoon::arithmetic T, std::floating_point T2>
	[[nodiscard]] auto pow(const T& base, const T2& exp) noexcept
	{
		return std::pow(base, exp);
	}

	export
	template<cmoon::arithmetic T>
	[[nodiscard]] auto cot(T t) noexcept
	{
		return 1.0L / std::tan(t);
	}

	export
	template<cmoon::arithmetic T>
	[[nodiscard]] constexpr int sign(T val) noexcept
	{
		return val < T{0} ? -1 : 1;
	}

	export
	template<std::unsigned_integral T>
	[[nodiscard]] constexpr int sign(T val) noexcept
	{
		return 1;
	}
	
	export
	template<class Rep>
	class basic_rational
	{
		public:
			using value_type = Rep;

			constexpr basic_rational(const value_type& numerator, const value_type& denominator = static_cast<value_type>(1))
				: numerator_{numerator}, denominator_{denominator} {}

			template<class Rep2, class Rep3>
			constexpr basic_rational(const Rep2& numerator, const Rep3& denominator = 1)
				: numerator_{static_cast<value_type>(numerator)}, denominator_{static_cast<value_type>(denominator)} {}

			constexpr basic_rational(const basic_rational&) = default;

			template<class Rep2>
			constexpr basic_rational(const basic_rational<Rep2>& other)
				: numerator_{static_cast<value_type>(other.numerator())}, denominator_{static_cast<value_type>(other.denominator())} {}

			template<std::floating_point Float>
				requires(sizeof(value_type) >= sizeof(Float))
			explicit constexpr basic_rational(const Float& f) noexcept
			{
				auto copy = f;
				value_type n = 1;

				while (copy != cmoon::floor(copy) &&
						n < static_cast<value_type>(10000000000000000))
				{
					n *= 10;
					copy *= 10;
				}

				numerator_ = static_cast<value_type>(f * n);
				denominator_ = n;
			}

			template<std::intmax_t Num, std::intmax_t Denom>
				requires(std::is_convertible_v<std::intmax_t, value_type>)
			constexpr basic_rational(std::ratio<Num, Denom>)
				: numerator_{Num}, denominator_{Denom} {}

			constexpr basic_rational(basic_rational&&) noexcept = default;
				
			constexpr basic_rational& operator=(const basic_rational&) = default;

			template<class Rep2>
			constexpr basic_rational& operator=(const basic_rational<Rep2>& other)
			{
				if (this != std::addressof(other))
				{
					numerator_ = static_cast<value_type>(other.numerator_);
					denominator_ = static_cast<value_type>(other.denominator_);
				}

				return *this;
			}

			constexpr basic_rational& operator=(basic_rational&&) noexcept = default;

			[[nodiscard]] constexpr basic_rational operator+() const noexcept
			{
				return *this;
			}

			[[nodiscard]] constexpr basic_rational operator-() const noexcept
			{
				return basic_rational{-numerator_, denominator_};
			}

			constexpr basic_rational& operator--() noexcept
			{
				--numerator_;
				return *this;
			}

			constexpr basic_rational operator--(int) noexcept
			{
				return basic_rational{numerator_--, denominator_};
			}

			constexpr basic_rational& operator++() noexcept
			{
				++numerator_;
				return *this;
			}

			constexpr basic_rational operator++(int) noexcept
			{
				return basic_rational{numerator_++, denominator_};
			}

			[[nodiscard]] friend constexpr basic_rational operator+(const basic_rational& lhs, const basic_rational& rhs) noexcept
			{
				if (lhs.denominator() == rhs.denominator())
				{
					return basic_rational{lhs.numerator() + rhs.numerator(), lhs.denominator()};
				}

				return basic_rational{lhs.numerator() * rhs.denominator() + lhs.denominator() * rhs.numerator(), lhs.denominator() * rhs.denominator()};
			}

			[[nodiscard]] friend constexpr basic_rational operator+(const basic_rational& lhs, const value_type& rhs) noexcept
			{
				return basic_rational{lhs.numerator() + lhs.denominator() * rhs, lhs.denominator()};
			}

			[[nodiscard]] friend constexpr basic_rational operator+(const value_type& lhs, const basic_rational& rhs) noexcept
			{
				return rhs + lhs;
			}

			[[nodiscard]] friend constexpr basic_rational operator-(const basic_rational& lhs, const basic_rational& rhs) noexcept
			{
				if (lhs.denominator() == rhs.denominator())
				{
					return basic_rational{lhs.numerator() - rhs.numerator(), lhs.denominator()};
				}

				return basic_rational{lhs.numerator() * rhs.denominator() - lhs.denominator() * rhs.numerator(), lhs.denominator() * rhs.denominator()};
			}

			[[nodiscard]] friend constexpr basic_rational operator-(const basic_rational& lhs, const value_type& rhs) noexcept
			{
				return basic_rational{lhs.numerator() - lhs.denominator() * rhs, lhs.denominator()};
			}

			[[nodiscard]] friend constexpr basic_rational operator-(const value_type& lhs, const basic_rational& rhs) noexcept
			{
				return basic_rational{lhs} - rhs;
			}

			[[nodiscard]] friend constexpr basic_rational operator*(const basic_rational& lhs, const basic_rational& rhs) noexcept
			{
				return basic_rational{lhs.numerator() * rhs.numerator(), lhs.denominator() * rhs.denominator()};
			}

			[[nodiscard]] friend constexpr basic_rational operator*(const basic_rational& lhs, const value_type& rhs) noexcept
			{
				return basic_rational{lhs.numerator() * rhs, lhs.denominator()};
			}

			[[nodiscard]] friend constexpr basic_rational operator*(const value_type& lhs, const basic_rational& rhs) noexcept
			{
				return basic_rational{lhs * rhs.numerator(), rhs.denominator()};
			}

			[[nodiscard]] friend constexpr basic_rational operator/(const basic_rational& lhs, const basic_rational& rhs) noexcept
			{
				return basic_rational{lhs.numerator() * rhs.denominator(), lhs.denominator() * rhs.numerator()};
			}

			[[nodiscard]] friend constexpr basic_rational operator/(const basic_rational& lhs, const value_type& rhs) noexcept
			{
				return basic_rational{lhs.numerator(), lhs.denominator() * rhs};
			}

			[[nodiscard]] friend constexpr basic_rational operator/(const value_type& lhs, const basic_rational& rhs) noexcept
			{
				return basic_rational{lhs * rhs.denominator(), rhs.numerator()};
			}

			constexpr basic_rational& operator+=(const basic_rational& r) noexcept
			{
				*this = *this + r;
				return *this;
			}

			constexpr basic_rational& operator+=(const value_type& i) noexcept
			{
				*this = *this + i;
				return *this;
			}

			constexpr basic_rational& operator-=(const basic_rational& r) noexcept
			{
				*this = *this - r;
				return *this;
			}

			constexpr basic_rational& operator-=(const value_type& i) noexcept
			{
				*this = *this - i;
				return *this;
			}

			constexpr basic_rational& operator*=(const basic_rational& r) noexcept
			{
				*this = *this * r;
				return *this;
			}

			constexpr basic_rational& operator*=(const value_type& i) noexcept
			{
				*this = *this * i;
				return *this;
			}

			constexpr basic_rational& operator/=(const basic_rational& r) noexcept
			{
				*this = *this / r;
				return *this;
			}

			constexpr basic_rational& operator/=(const value_type& i) noexcept
			{
				*this = *this / i;
				return *this;
			}

			void numerator(const value_type& n) noexcept
			{
				numerator_ = n;
			}

			[[nodiscard]] constexpr const value_type& numerator() const noexcept
			{
				return numerator_;
			}

			void denominator(const value_type& d) noexcept
			{
				denominator_ = d;
			}

			[[nodiscard]] constexpr const value_type& denominator() const noexcept
			{
				return denominator_;
			}

			template<class T>
				requires(std::convertible_to<value_type, T>)
			[[nodiscard]] constexpr explicit operator T() const noexcept
			{
				return static_cast<T>(numerator_) / static_cast<T>(denominator_);
			}

			[[nodiscard]] friend constexpr bool operator==(const basic_rational& lhs, const basic_rational& rhs) noexcept
			{
				return lhs.numerator() * rhs.denominator() == lhs.denominator() * rhs.numerator();
			}

			template<class Rep2>
				requires(requires(basic_rational r){ static_cast<Rep2>(r); })
			[[nodiscard]] friend constexpr bool operator==(const basic_rational& lhs, const Rep2& rhs) noexcept
			{
				return static_cast<Rep2>(lhs) == rhs;
			}

			[[nodiscard]] friend constexpr bool operator!=(const basic_rational&, const basic_rational&) noexcept = default;

			template<class Rep2>
				requires(requires(basic_rational r){ static_cast<Rep2>(r); })
			[[nodiscard]] friend constexpr bool operator!=(const basic_rational& lhs, const Rep2& rhs) noexcept
			{
				return !(lhs == rhs);
			}

			[[nodiscard]] friend constexpr auto operator<=>(const basic_rational& lhs, const basic_rational& rhs) noexcept
			{
				return lhs.numerator() * rhs.denominator() <=> lhs.denominator() * rhs.numerator();
			}

			template<class Rep2>
				requires(requires(basic_rational r){ static_cast<Rep2>(r); })
			[[nodiscard]] friend constexpr auto operator<=>(const basic_rational& lhs, const Rep2& rhs) noexcept
			{
				return static_cast<Rep2>(lhs) <=> rhs;
			}
		private:
			value_type numerator_ {};
			value_type denominator_ {};
	};

	export
	using rational = basic_rational<int>;

	export
	template<class Ratio, class Rep = decltype(Ratio::num)>
	constexpr basic_rational<Rep> rational_ratio {Ratio::num, Ratio::den};

	export
	template<class Rep>
	[[nodiscard]] constexpr basic_rational<Rep> add_inverse(const basic_rational<Rep>& r) noexcept
	{
		return { -r.numerator(), r.denominator() };
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr basic_rational<Rep> mult_inverse(const basic_rational<Rep>& r) noexcept
	{
		return { r.denominator(), r.numerator() };
	}

	export
	template<class Rep, class Rep2>
	[[nodiscard]] constexpr basic_rational<Rep> pow(const basic_rational<Rep>& r, const Rep2& p) noexcept
	{
		if (p < 0)
		{
			return pow(mult_inverse(r), -p);
		}

		return { pow(r.numerator(), p), pow(r.denominator(), p) };
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr basic_rational<Rep> canonical(const basic_rational<Rep>& r) noexcept
	{
		const auto gcd_ = std::gcd(r.numerator(), r.denominator());
		return {r.numerator() / gcd_, r.denominator() / gcd_};
	}

	export
	template<class Rep>
	inline std::ostream& operator<<(std::ostream& os, const basic_rational<Rep>& r)
	{
		return os << r.numerator() << "/" << r.denominator();
	}
}