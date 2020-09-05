#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <iostream>
#include <cstdint>
#include <cmath>
#include <numeric>

#include "collin/concepts.hpp"

namespace collin
{
	namespace math
	{
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
		
		template<collin::concepts::arithmetic intT>
		class basic_rational
		{
			public:
				using value_type = intT;

				constexpr basic_rational(value_type numerator, value_type denominator = 1)
					: numerator_{numerator}, denominator_{denominator} {}

				constexpr basic_rational(const basic_rational&) = default;

				template<class intT2>
					requires(collin::concepts::constructible<value_type, intT2>)
				constexpr basic_rational(const basic_rational<intT2>& other)
					: numerator_{other.numerator_}, denominator_{other.denominator} {}

				constexpr basic_rational(basic_rational&&) noexcept = default;

				template<collin::concepts::convertible_to<intT> intT2>
				constexpr basic_rational(basic_rational<intT2>&& other) noexcept
					: numerator_{std::move(other.numerator_)}, denominator_{std::move(other.denominator_)} {}
				
				constexpr basic_rational& operator=(const basic_rational&) = default;

				template<class intT2>
					requires(collin::concepts::constructible<value_type, intT2>)
				constexpr basic_rational& operator=(const basic_rational<intT2>& other)
				{
					if (this != std::addressof(other))
					{
						numerator_ = other.numerator_;
						denominator_ = other.denominator_;
					}

					return *this;
				}

				constexpr basic_rational& operator=(basic_rational&&) noexcept = default;

				template<class intT2>
					requires(collin::concepts::constructible<value_type, intT2&&>)
				constexpr basic_rational& operator=(basic_rational<intT2>&& other) noexcept
				{
					if(this != std::addressof(other))
					{
						numerator_ = std::move(other.numerator_);
						denominator_ = std::move(other.denominator_);
					}

					return *this;
				}

				[[nodiscard]] constexpr basic_rational operator+() const noexcept
				{
					return *this;
				}

				[[nodiscard]] constexpr basic_rational operator-() const noexcept
				{
					return basic_rational{-numerator_, denominator_};
				}

				template<class intT2>
					requires(requires(value_type v, intT2 v2) { v + v2; v * v2; })
				[[nodiscard]] constexpr basic_rational operator+(const basic_rational<intT2>& r) const noexcept
				{
					using common_type = std::common_type_t<value_type, intT2>;
					const auto this_numerator = common_type{numerator()};
					const auto this_denominator = common_type{denominator()};

					const auto other_numerator = common_type{r.numerator()};
					const auto other_denominator = common_type{r.denominator()};

					if (this_denominator == other_denominator)
					{
						return {this_numerator + other_numerator, this_denominator};
					}

					return { this_numerator * other_denominator + this_denominator * other_numerator,
							this_denominator * other_denominator };
				}

				template<class intT2>
					requires(requires(value_type v, intT2 v2) { v + v2; v * v2; })
				[[nodiscard]] constexpr basic_rational operator+(intT2 i) const noexcept
				{
					return *this + basic_rational(i);
				}

				template<class intT2>
					requires(requires(value_type v, intT2 v2) { v + v2; v * v2; })
				constexpr basic_rational& operator+=(const basic_rational<intT2>& r) noexcept
				{
					*this = *this + r;
					return *this;
				}

				template<class intT2>
					requires(requires(value_type v, intT2 v2) { v + v2; v * v2; })
				constexpr basic_rational& operator+=(intT2 i) noexcept
				{
					*this = *this + i;
					return *this;
				}

				template<class intT2>
					requires(requires(value_type v, intT2 v2) { v - v2; v * v2; })
				[[nodiscard]] constexpr basic_rational operator-(const basic_rational<intT2>& r) const noexcept
				{
					using common_type = std::common_type_t<value_type, intT2>;
					const auto this_numerator = common_type{numerator()};
					const auto this_denominator = common_type{denominator()};

					const auto other_numerator = common_type{r.numerator()};
					const auto other_denominator = common_type{r.denominator()};

					if (this_denominator == other_denominator)
					{
						return {this_numerator - other_numerator, this_denominator};
					}

					return { this_numerator * other_denominator - this_denominator * other_numerator,
							this_denominator * other_denominator };
				}

				template<class intT2>
					requires(requires(value_type v, intT2 v2) { v - v2; v * v2; })
				[[nodiscard]] constexpr basic_rational operator-(intT2 i) const noexcept
				{
					return *this - basic_rational(i);
				}

				template<class intT2>
					requires(requires(value_type v, intT2 v2) { v - v2; v * v2; })
				constexpr basic_rational& operator-=(const basic_rational<intT2>& r) noexcept
				{
					*this = *this - r;
					return *this;
				}

				template<class intT2>
					requires(requires(value_type v, intT2 v2) { v - v2; v * v2; })
				constexpr basic_rational& operator-=(intT2 i) noexcept
				{
					*this = *this - i;
					return *this;
				}

				template<class intT2>
					requires(requires(value_type v, intT2 v2) { v * v2; })
				[[nodiscard]] constexpr basic_rational operator*(const basic_rational<intT2>& r) const noexcept
				{
					using common_type = std::common_type_t<value_type, intT2>;
					const auto this_numerator = common_type{numerator()};
					const auto this_denominator = common_type{denominator()};

					const auto other_numerator = common_type{r.numerator()};
					const auto other_denominator = common_type{r.denominator()};

					return { this_numerator * other_numerator, this_denominator * other_denominator };
				}

				template<class intT2>
					requires(requires(value_type v, intT2 v2) { v * v2; })
				[[nodiscard]] constexpr basic_rational operator*(intT2 i) const noexcept
				{
					return *this * basic_rational{i};
				}

				template<class intT2>
					requires(requires(value_type v, intT2 v2) { v * v2; })
				constexpr basic_rational& operator*=(const basic_rational<intT2>& r) noexcept
				{
					*this = *this * r;
					return *this;
				}

				template<class intT2>
					requires(requires(value_type v, intT2 v2) { v * v2; })
				constexpr basic_rational& operator*=(intT2 i) noexcept
				{
					*this = *this * i;
					return *this;
				}

				template<class intT2>
					requires(requires(value_type v, intT2 v2) { v * v2; })
				[[nodiscard]] constexpr basic_rational operator/(const basic_rational<intT2>& r) const noexcept
				{
					using common_type = std::common_type_t<value_type, intT2>;
					const auto this_numerator = common_type{numerator()};
					const auto this_denominator = common_type{denominator()};

					const auto other_numerator = common_type{r.numerator()};
					const auto other_denominator = common_type{r.denominator()};

					return { this_numerator * other_denominator, this_denominator * other_numerator };
				}

				template<class intT2>
					requires(requires(value_type v, intT2 v2) { v * v2; })
				[[nodiscard]] constexpr basic_rational operator/(intT2 i) const noexcept
				{
					return *this / basic_rational{i};
				}

				template<class intT2>
					requires(requires(value_type v, intT2 v2) { v * v2; })
				constexpr basic_rational& operator/=(const basic_rational<intT2>& r) noexcept
				{
					*this = *this / r;
					return *this;
				}

				template<class intT2>
					requires(requires(value_type v, intT2 v2) { v * v2; })
				constexpr basic_rational& operator/=(intT2 i) noexcept
				{
					*this = *this / i;
					return *this;
				}

				template<class intT2>
					requires(collin::concepts::assignable<value_type, intT2>)
				void numerator(const intT2& n) noexcept
				{
					numerator_ = n;
				}

				[[nodiscard]] constexpr value_type numerator() const noexcept
				{
					return numerator_;
				}

				template<class intT2>
					requires(collin::concepts::assignable<value_type, intT2>)
				void denominator(const intT2& d) noexcept
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
				value_type numerator_;
				value_type denominator_;
		};

		using rational = basic_rational<int>;

		template<class Ratio, class Rep = decltype(Ratio::num)>
		constexpr basic_rational<Rep> rational_ratio {Ratio::num, Ratio::den};

		template<class intT, class intT2 = intT>
		[[nodiscard]] constexpr basic_rational<intT> operator+(intT i, const basic_rational<intT2>& r) noexcept
		{
			return r + basic_rational(i);
		}

		template<class intT, class intT2 = intT>
		[[nodiscard]] constexpr basic_rational<intT> operator-(intT i, const basic_rational<intT2>& r) noexcept
		{
			return r - basic_rational(i);
		}

		template<class intT, class intT2 = intT>
		[[nodiscard]] constexpr basic_rational<intT> operator*(intT i, const basic_rational<intT2>& r) noexcept
		{
			return r * basic_rational(i);
		}

		template<class intT, class intT2 = intT>
		[[nodiscard]] constexpr basic_rational<intT> operator/(intT i, const basic_rational<intT2>& r) noexcept
		{
			return r / basic_rational(i);
		}

		template<class intT>
		[[nodiscard]] constexpr basic_rational<intT> add_inverse(const basic_rational<intT>& r) noexcept
		{
			return { -r.numerator(), r.denominator() };
		}

		template<class intT>
		[[nodiscard]] constexpr basic_rational<intT> mult_inverse(const basic_rational<intT>& r) noexcept
		{
			return { r.denominator(), r.numerator() };
		}

		template<class intT, class intT2 = intT>
		[[nodiscard]] constexpr basic_rational<intT> pow(const basic_rational<intT>& r, intT2 p) noexcept
		{
			if (p < 0)
			{
				return pow(mult_inverse(r), -p);
			}

			return { pow(r.numerator(), p), pow(r.denominator(), p) };
		}

		template<class intT>
		[[nodiscard]] constexpr basic_rational<intT> canonical(const basic_rational<intT>& r) noexcept
		{
			const auto gcd_ = std::gcd(r.numerator(), r.denominator());
			return {r.numerator() / gcd_, r.denominator() / gcd_};
		}

		template<class intT, class intT2 = intT>
		[[nodiscard]] constexpr bool operator==(const basic_rational<intT>& first, const basic_rational<intT2>& second) noexcept
		{
			return first.numerator() * second.denominator() == first.denominator() * second.numerator();
		}

		template<class intT>
		[[nodiscard]] constexpr bool operator==(const basic_rational<intT>& first, const double second) noexcept
		{
			return static_cast<double>(first) == second;
		}

		template<class intT, class intT2 = intT>
		[[nodiscard]] constexpr bool operator==(const basic_rational<intT>& first, const intT2 second) noexcept
		{
			return canonical(first).numerator() == second;
		}
		
		template<class intT, class intT2 = intT>
		[[nodiscard]] constexpr bool operator!=(const basic_rational<intT>& first, const basic_rational<intT2>& second) noexcept
		{
			return !(first == second);
		}

		template<class intT>
		[[nodiscard]] constexpr bool operator!=(const basic_rational<intT>& first, const double second) noexcept
		{
			return !(first == second);
		}

		template<class intT, class intT2 = intT>
		[[nodiscard]] constexpr bool operator!=(const basic_rational<intT>& first, const intT2 second) noexcept
		{
			return !(first == second);
		}

		template<class intT, class intT2 = intT>
		[[nodiscard]] constexpr bool operator<(const basic_rational<intT>& first, const basic_rational<intT2>& second) noexcept
		{
			return first.numerator() * second.denominator() < first.denominator() * second.numerator();
		}

		template<class intT>
		[[nodiscard]] constexpr bool operator<(const basic_rational<intT>& first, const double second) noexcept
		{
			return static_cast<double>(first) < second;
		}

		template<class intT, class intT2 = intT>
		[[nodiscard]] constexpr bool operator<(const basic_rational<intT>& first, const intT2 second) noexcept
		{
			return canonical(first).numerator() < second;
		}

		template<class intT, class intT2 = intT>
		[[nodiscard]] constexpr bool operator<=(const basic_rational<intT>& first, const basic_rational<intT2>& second) noexcept
		{
			return first.numerator() * second.denominator() <= first.denominator() * second.numerator();
		}

		template<class intT>
		[[nodiscard]] constexpr bool operator<=(const basic_rational<intT>& first, const double second) noexcept
		{
			return static_cast<double>(first) <= second;
		}

		template<class intT, class intT2 = intT>
		[[nodiscard]] constexpr bool operator<=(const basic_rational<intT>& first, const intT2 second) noexcept
		{
			return canonical(first).numerator() <= second;
		}

		template<class intT, class intT2 = intT>
		[[nodiscard]] constexpr bool operator>(const basic_rational<intT>& first, const basic_rational<intT2>& second) noexcept
		{
			return !(first <= second);
		}

		template<class intT>
		[[nodiscard]] constexpr bool operator>(const basic_rational<intT>& first, const double second) noexcept
		{
			return !(first <= second);
		}

		template<class intT, class intT2 = intT>
		[[nodiscard]] constexpr bool operator>(const basic_rational<intT>& first, const intT2 second) noexcept
		{
			return !(first <= second);
		}

		template<class intT, class intT2 = intT>
		[[nodiscard]] constexpr bool operator>=(const basic_rational<intT>& first, const basic_rational<intT2>& second) noexcept
		{
			return !(first < second);
		}

		template<class intT>
		[[nodiscard]] constexpr bool operator>=(const basic_rational<intT>& first, const double second) noexcept
		{
			return !(first < second);
		}

		template<class intT, class intT2>
		[[nodiscard]] constexpr bool operator>=(const basic_rational<intT>& first, const intT2 second) noexcept
		{
			return !(first < second);
		}

		template<class intT>
		inline std::ostream& operator<<(std::ostream& os, const basic_rational<intT>& r)
		{
			return os << r.numerator() << "/" << r.denominator();
		}

		template<collin::concepts::integral IntT = int, collin::concepts::floating_point Float>
		[[nodiscard]] constexpr IntT ceil(const Float& f) noexcept
		{
			const auto inum = static_cast<IntT>(f);
			
			if(f == static_cast<Float>(inum))
			{
				return inum;
			}

			return inum + (f > 0 ? 1 : 0);
		}

		template<collin::concepts::integral IntT = int, collin::concepts::floating_point Float>
		[[nodiscard]] constexpr IntT floor(const Float& f) noexcept
		{
			const auto val_int = static_cast<IntT>(f);

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
	}
}