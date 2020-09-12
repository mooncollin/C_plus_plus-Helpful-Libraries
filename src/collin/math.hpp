#pragma once

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

#include "collin/concepts.hpp"

namespace collin
{
	namespace math
	{
		template<std::intmax_t Pn>
		struct static_sign : std::integral_constant<std::intmax_t, (Pn < 0) ? -1 : 1> {};

		template<std::intmax_t Pn>
		struct static_abs : std::integral_constant<std::intmax_t, Pn* static_sign<Pn>::value> {};

		template<std::intmax_t Pn, std::intmax_t Qn>
		struct static_gcd : static_gcd<Qn, (Pn% Qn)> {};

		template<std::intmax_t Pn>
		struct static_gcd<Pn, 0> : std::integral_constant<std::intmax_t, static_abs<Pn>::value> {};

		template<std::intmax_t Qn>
		struct static_gcd<0, Qn> : std::integral_constant<std::intmax_t, static_abs<Qn>::value> {};

		template<std::uintmax_t N, std::uintmax_t P>
		struct static_pow : static_pow<N * N, P - 1> {};

		template<std::uintmax_t N>
		struct static_pow<N, 1> : std::integral_constant<std::uintmax_t, N> {};

		template<std::uintmax_t N>
		struct static_pow<N, 0> : std::integral_constant<std::uintmax_t, 1> {};

		[[nodiscard]] constexpr std::uintmax_t factorial(std::size_t n) noexcept
		{
			std::uintmax_t result {1};
			for(std::size_t i {2}; i <= n; ++i)
			{
				result *= i;
			}

			return result;
		}

		template<collin::concepts::integral T>
		[[nodiscard]] constexpr bool is_odd(const T& val) noexcept
		{
			return val & 1;
		}

		template<collin::concepts::floating_point F>
		[[nodiscard]] constexpr bool is_odd(const F& val) noexcept
		{
			return is_odd(static_cast<std::intmax_t>(val));
		}

		template<collin::concepts::arithmetic T>
		[[nodiscard]] constexpr bool is_even(const T& val) noexcept
		{
			return !is_odd(val);
		}

		template<collin::concepts::arithmetic T>
		[[nodiscard]] constexpr T abs(const T& value) noexcept
		{
			return value < T{0} ? -value : value;
		}

		template<collin::concepts::integral Rep = int, collin::concepts::floating_point Float>
		[[nodiscard]] constexpr Rep ceil(const Float& f) noexcept
		{
			const auto inum = static_cast<Rep>(f);
			
			if(f == static_cast<Float>(inum))
			{
				return inum;
			}

			return inum + (f > 0 ? 1 : 0);
		}

		template<collin::concepts::integral Rep = int, collin::concepts::floating_point Float>
		[[nodiscard]] constexpr Rep floor(const Float& f) noexcept
		{
			const auto val_int = static_cast<Rep>(f);

			if(f >= 0 || f == static_cast<Float>(val_int))
			{
				return val_int;
			}

			return val_int - 1;
		}

		namespace details
		{
			template<collin::concepts::arithmetic T, collin::concepts::integral T2, collin::concepts::arithmetic T3>
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
		}

		template<collin::concepts::arithmetic T, collin::concepts::integral T2>
		[[nodiscard]] constexpr auto pow(const T& base, const T2& exp) noexcept
		{
			if constexpr (std::numeric_limits<T>::has_quiet_NaN)
			{
				if (base == std::numeric_limits<T>::quiet_NaN())
				{
					return base;
				}
			}

			return details::pow2(1, base, exp);
		}

		template<collin::concepts::arithmetic T, collin::concepts::floating_point T2>
		[[nodiscard]] constexpr auto pow(const T& base, const T2& exp) noexcept
		{
			return std::pow(base, exp);
		}

		template<collin::concepts::arithmetic T>
		[[nodiscard]] constexpr auto cot(T t) noexcept
		{
			return 1.0L / std::tan(t);
		}
		
		template<class Rep>
		class basic_rational
		{
			public:
				using value_type = Rep;

				constexpr basic_rational(const value_type& numerator, const value_type& denominator = 1)
					: numerator_{numerator}, denominator_{denominator} {}

				template<class Rep2, class Rep3>
				constexpr basic_rational(const Rep2& numerator, const Rep3& denominator = 1)
					: numerator_{static_cast<value_type>(numerator)}, denominator_{static_cast<value_type>(denominator)} {}

				constexpr basic_rational(const basic_rational&) = default;

				template<class Rep2>
				constexpr basic_rational(const basic_rational<Rep2>& other)
					: numerator_{static_cast<value_type>(other.numerator())}, denominator_{static_cast<value_type>(other.denominator())} {}

				template<collin::concepts::floating_point Float>
					requires(sizeof(value_type) >= sizeof(Float))
				constexpr basic_rational(const Float& f) noexcept
				{
					auto copy = f;
					value_type n = 1;

					while (copy != collin::math::floor(copy) &&
						   n < static_cast<value_type>(10000000000000000))
					{
						n *= 10;
						copy *= 10;
					}

					numerator_ = static_cast<value_type>(f * n);
					denominator_ = n;
				}

				template<std::intmax_t Num, std::intmax_t Denom>
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

				constexpr basic_rational& operator+=(const basic_rational& r) noexcept
				{
					if (denominator_ == r.denominator_)
					{
						numerator_ += r.numerator_;
					}
					else
					{
						numerator_ = numerator_ * r.denominator_ + denominator_ * r.numerator_;
						denominator_ *= r.denominator_;
					}

					return *this;
				}

				constexpr basic_rational& operator+=(const value_type& i) noexcept
				{
					numerator_ += denominator_ * i;
					return *this;
				}

				constexpr basic_rational& operator-=(const basic_rational& r) noexcept
				{
					if (denominator_ == r.denominator_)
					{
						numerator_ -= r.numerator_;
					}
					else
					{
						numerator_ = numerator_ * r.denominator_ - denominator_ * r.numerator_;
						denominator_ *= r.denominator_;
					}

					return *this;
				}

				constexpr basic_rational& operator-=(const value_type& i) noexcept
				{
					numerator_ -= denominator_ * i;
					return *this;
				}

				constexpr basic_rational& operator*=(const basic_rational& r) noexcept
				{
					numerator_ *= r.numerator_;
					denominator_ *= r.denominator_;
					return *this;
				}

				constexpr basic_rational& operator*=(const value_type& i) noexcept
				{
					numerator_ *= i;
					return *this;
				}

				constexpr basic_rational& operator/=(const basic_rational& r) noexcept
				{
					numerator_ *= r.denominator_;
					denominator_ *= r.numerator_;
					return *this;
				}

				constexpr basic_rational& operator/=(const value_type& i) noexcept
				{
					denominator_ *= i;
					return *this;
				}

				void numerator(const value_type& n) noexcept
				{
					numerator_ = n;
				}

				[[nodiscard]] constexpr value_type numerator() const noexcept
				{
					return numerator_;
				}

				void denominator(const value_type& d) noexcept
				{
					denominator_ = d;
				}

				[[nodiscard]] constexpr value_type denominator() const noexcept
				{
					return denominator_;
				}

				template<class T>
					requires(collin::concepts::convertible_to<value_type, T>)
				[[nodiscard]] constexpr explicit operator T() const noexcept
				{
					return static_cast<T>(numerator_) / static_cast<T>(denominator_);
				}
			private:
				value_type numerator_ {};
				value_type denominator_ {};
		};

		using rational = basic_rational<int>;

		template<class Ratio, class Rep = decltype(Ratio::num)>
		constexpr basic_rational<Rep> rational_ratio {Ratio::num, Ratio::den};

		template<class Rep1, class Rep2>
		[[nodiscard]] constexpr auto operator+(const Rep1& lhs, const basic_rational<Rep2>& rhs) noexcept
		{
			using common_type = basic_rational<std::common_type_t<Rep1, Rep2>>;

			common_type result{lhs};
			result += common_type{rhs};
			return result;
		}

		template<class Rep1, class Rep2>
		[[nodiscard]] constexpr auto operator+(const basic_rational<Rep1>& lhs, const Rep2& rhs) noexcept
		{
			using common_type = std::common_type_t<Rep1, Rep2>;

			basic_rational<common_type> result{lhs};
			result += static_cast<common_type>(rhs);
			return result;
		}

		template<class Rep1, class Rep2>
		[[nodiscard]] constexpr auto operator-(const Rep1& lhs, const basic_rational<Rep2>& rhs) noexcept
		{
			using common_type = basic_rational<std::common_type_t<Rep1, Rep2>>;

			common_type result{lhs};
			result -= common_type{rhs};
			return result;
		}

		template<class Rep1, class Rep2>
		[[nodiscard]] constexpr auto operator-(const basic_rational<Rep1>& lhs, const Rep2& rhs) noexcept
		{
			using common_type = std::common_type_t<Rep1, Rep2>;

			basic_rational<common_type> result{lhs};
			result -= static_cast<common_type>(rhs);
			return result;
		}

		template<class Rep1, class Rep2>
		[[nodiscard]] constexpr auto operator*(const basic_rational<Rep1>& lhs, const Rep2& rhs) noexcept
		{
			using common_type = std::common_type_t<Rep1, Rep2>;

			basic_rational<common_type> result{lhs};
			result *= static_cast<common_type>(rhs);
			return result;
		}

		template<class Rep>
		[[nodiscard]] constexpr basic_rational<Rep> operator*(const basic_rational<Rep>& lhs, const basic_rational<Rep>& rhs) noexcept
		{
			auto result {lhs};
			result *= rhs;
			return result;
		}

		template<class Rep>
		[[nodiscard]] constexpr basic_rational<Rep> operator*(const Rep& lhs, const basic_rational<Rep>& rhs) noexcept
		{
			basic_rational<Rep> result{lhs};
			result *= rhs;
			return result;
		}

		template<class Rep>
		[[nodiscard]] constexpr basic_rational<Rep> operator/(const Rep& lhs, const basic_rational<Rep>& rhs) noexcept
		{
			basic_rational<Rep> result {lhs};
			result /= rhs;
			return result;
		}

		template<class Rep>
		[[nodiscard]] constexpr basic_rational<Rep> operator/(const basic_rational<Rep>& lhs, const Rep& rhs) noexcept
		{
			auto result {lhs};
			result /= rhs;
			return result; 
		}

		template<class Rep>
		[[nodiscard]] constexpr basic_rational<Rep> add_inverse(const basic_rational<Rep>& r) noexcept
		{
			return { -r.numerator(), r.denominator() };
		}

		template<class Rep>
		[[nodiscard]] constexpr basic_rational<Rep> mult_inverse(const basic_rational<Rep>& r) noexcept
		{
			return { r.denominator(), r.numerator() };
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr basic_rational<Rep> pow(const basic_rational<Rep>& r, const Rep2& p) noexcept
		{
			if (p < 0)
			{
				return pow(mult_inverse(r), -p);
			}

			return { pow(r.numerator(), p), pow(r.denominator(), p) };
		}

		template<class Rep>
		[[nodiscard]] constexpr basic_rational<Rep> canonical(const basic_rational<Rep>& r) noexcept
		{
			const auto gcd_ = std::gcd(r.numerator(), r.denominator());
			return {r.numerator() / gcd_, r.denominator() / gcd_};
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator==(const basic_rational<Rep>& first, const basic_rational<Rep2>& second) noexcept
		{
			return first.numerator() * second.denominator() == first.denominator() * second.numerator();
		}

		template<class Rep>
		[[nodiscard]] constexpr bool operator==(const basic_rational<Rep>& first, const double second) noexcept
		{
			return static_cast<double>(first) == second;
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator==(const basic_rational<Rep>& first, const Rep2 second) noexcept
		{
			return canonical(first).numerator() == second;
		}
		
		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator!=(const basic_rational<Rep>& first, const basic_rational<Rep2>& second) noexcept
		{
			return !(first == second);
		}

		template<class Rep>
		[[nodiscard]] constexpr bool operator!=(const basic_rational<Rep>& first, const double second) noexcept
		{
			return !(first == second);
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator!=(const basic_rational<Rep>& first, const Rep2 second) noexcept
		{
			return !(first == second);
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator<(const basic_rational<Rep>& first, const basic_rational<Rep2>& second) noexcept
		{
			return first.numerator() * second.denominator() < first.denominator() * second.numerator();
		}

		template<class Rep>
		[[nodiscard]] constexpr bool operator<(const basic_rational<Rep>& first, const double second) noexcept
		{
			return static_cast<double>(first) < second;
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator<(const basic_rational<Rep>& first, const Rep2 second) noexcept
		{
			return canonical(first).numerator() < second;
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator<=(const basic_rational<Rep>& first, const basic_rational<Rep2>& second) noexcept
		{
			return first.numerator() * second.denominator() <= first.denominator() * second.numerator();
		}

		template<class Rep>
		[[nodiscard]] constexpr bool operator<=(const basic_rational<Rep>& first, const double second) noexcept
		{
			return static_cast<double>(first) <= second;
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator<=(const basic_rational<Rep>& first, const Rep2 second) noexcept
		{
			return canonical(first).numerator() <= second;
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator>(const basic_rational<Rep>& first, const basic_rational<Rep2>& second) noexcept
		{
			return !(first <= second);
		}

		template<class Rep>
		[[nodiscard]] constexpr bool operator>(const basic_rational<Rep>& first, const double second) noexcept
		{
			return !(first <= second);
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator>(const basic_rational<Rep>& first, const Rep2 second) noexcept
		{
			return !(first <= second);
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator>=(const basic_rational<Rep>& first, const basic_rational<Rep2>& second) noexcept
		{
			return !(first < second);
		}

		template<class Rep>
		[[nodiscard]] constexpr bool operator>=(const basic_rational<Rep>& first, const double second) noexcept
		{
			return !(first < second);
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator>=(const basic_rational<Rep>& first, const Rep2 second) noexcept
		{
			return !(first < second);
		}

		template<class Rep>
		inline std::ostream& operator<<(std::ostream& os, const basic_rational<Rep>& r)
		{
			return os << r.numerator() << "/" << r.denominator();
		}
	}
}