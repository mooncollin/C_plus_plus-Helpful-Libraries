#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <iostream>

namespace collin
{
	namespace math
	{
		constexpr auto factorial(std::size_t n)
		{
			std::uintmax_t result = 1;
			for(std::size_t i = 2; i <= n; i++)
			{
				result *= i;
			}

			return result;
		}

		template<class T, class = std::enable_if_t<std::is_integral_v<T>>>
		constexpr bool is_odd(const T val)
		{
			return val & 1;
		}

		template<class T, class = std::enable_if_t<std::is_integral_v<T>>>
		constexpr bool is_even(const T val)
		{
			return !is_odd(val);
		}

		template<class T, class = std::enable_if_t<std::is_integral_v<T>>>
		constexpr T gcd(T first, T second)
		{
			while(second != 0)
			{
				const auto t = second;
				second = first % second;
				first = t;
			}

			return first;
		}

		template<class T, class = std::enable_if_t<std::is_integral_v<T>>>
		constexpr T pow(T base, int exp)
		{
			if(exp < 0)
			{
				if(base == 1 || base  == -1)
				{
					return base;
				}
				else
				{
					return 0;
				}
			}

			T result = 1;
			while(true)
			{
				if(is_odd(exp))
				{
					result *= base;
				}
				exp >>= 1;
				if(exp == 0)
				{
					break;
				}
				base *= base;
			}

			return result;
		}

		class Rational
		{
			public:
				constexpr Rational(int numerator, int denominator)
					: numerator_(numerator), denominator_(denominator) {}
				constexpr Rational(int numerator)
					: numerator_(numerator), denominator_(1) {}
				constexpr Rational(const Rational&) = default;
				constexpr Rational(Rational&&) = default;
				constexpr Rational& operator=(const Rational&) = default;
				constexpr Rational& operator=(Rational&&) = default;

				constexpr Rational operator+(const Rational& r) const
				{
					return { numerator() * r.denominator() + denominator() * r.numerator(),
							denominator() * r.denominator() };
				}

				constexpr Rational operator+(int i) const
				{
					return *this + Rational(i);
				}

				Rational& operator+=(const Rational& r)
				{
					*this = *this + r;
					return *this;
				}

				Rational& operator+=(int i)
				{
					*this = *this + i;
					return *this;
				}

				constexpr Rational operator-(const Rational& r) const
				{
					return { numerator() * r.denominator() - denominator() * r.numerator(),
							denominator() * r.denominator() };
				}

				constexpr Rational operator-(int i) const
				{
					return *this - Rational(i);
				}

				Rational& operator-=(const Rational& r)
				{
					*this = *this - r;
					return *this;
				}

				Rational& operator-=(int i)
				{
					*this = *this - i;
					return *this;
				}

				constexpr Rational operator*(const Rational& r) const
				{
					return {numerator() * r.numerator(), denominator() * r.denominator()};
				}

				constexpr Rational operator*(int i) const
				{
					return *this * Rational(i);
				}

				Rational& operator*=(const Rational& r)
				{
					*this = *this * r;
					return *this;
				}

				Rational& operator*=(int i)
				{
					*this = *this * i;
					return *this;
				}

				constexpr Rational operator/(const Rational& r) const
				{
					return {numerator() * r.denominator(), denominator() * r.numerator()};
				}

				constexpr Rational operator/(int i) const
				{
					return *this / Rational(i);
				}

				Rational& operator/=(const Rational& r)
				{
					*this = *this / r;
					return *this;
				}

				Rational& operator/=(int i)
				{
					*this = *this / i;
					return *this;
				}

				void numerator(int n)
				{
					numerator_ = n;
				}

				constexpr int numerator() const
				{
					return numerator_;
				}

				void denominator(int d)
				{
					denominator_ = d;
				}

				constexpr int denominator() const
				{
					return denominator_;
				}

				constexpr operator double() const
				{
					return static_cast<double>(numerator_) / static_cast<double>(denominator_);
				}
			private:
				int numerator_;
				int denominator_;
		};

		constexpr Rational operator+(int i, const Rational& r)
		{
			return r + Rational(i);
		}

		constexpr Rational operator-(int i, const Rational& r)
		{
			return r - Rational(i);
		}

		constexpr Rational operator*(int i, const Rational& r)
		{
			return r * Rational(i);
		}

		constexpr Rational operator/(int i, const Rational& r)
		{
			return r / Rational(i);
		}

		constexpr Rational add_inverse(const Rational& r)
		{
			return { -r.numerator(), r.denominator() };
		}

		constexpr Rational mult_inverse(const Rational& r)
		{
			return { r.denominator(), r.numerator() };
		}

		constexpr Rational pow(const Rational& r, int p)
		{
			if (p < 0)
			{
				return pow(mult_inverse(r), -p);
			}

			return { pow(r.numerator(), p), pow(r.denominator(), p) };
		}

		constexpr Rational canonical(const Rational& r)
		{
			auto gcd_ = gcd(r.numerator(), r.denominator());
			return {r.numerator() / gcd_, r.denominator() / gcd_};
		}

		constexpr bool operator==(const Rational& first, const Rational& second)
		{
			return first.numerator() * second.denominator() == first.denominator() * second.numerator();
		}

		constexpr bool operator==(const Rational& first, const double second)
		{
			return static_cast<double>(first) == second;
		}

		constexpr bool operator==(const Rational& first, const int second)
		{
			return canonical(first).numerator() == second;
		}
		
		constexpr bool operator!=(const Rational& first, const Rational& second)
		{
			return !(first == second);
		}

		constexpr bool operator!=(const Rational& first, const double second)
		{
			return !(first == second);
		}

		constexpr bool operator!=(const Rational& first, const int second)
		{
			return !(first == second);
		}

		constexpr bool operator<(const Rational& first, const Rational& second)
		{
			return first.numerator() * second.denominator() < first.denominator() * second.numerator();
		}

		constexpr bool operator<(const Rational& first, const double second)
		{
			return static_cast<double>(first) < second;
		}

		constexpr bool operator<(const Rational& first, const int second)
		{
			return canonical(first).numerator() < second;
		}

		constexpr bool operator<=(const Rational& first, const Rational& second)
		{
			return first.numerator() * second.denominator() <= first.denominator() * second.numerator();
		}

		constexpr bool operator<=(const Rational& first, const double second)
		{
			return static_cast<double>(first) <= second;
		}

		constexpr bool operator<=(const Rational& first, const int second)
		{
			return canonical(first).numerator() <= second;
		}

		constexpr bool operator>(const Rational& first, const Rational& second)
		{
			return !(first <= second);
		}

		constexpr bool operator>(const Rational& first, const double second)
		{
			return !(first <= second);
		}

		constexpr bool operator>(const Rational& first, const int second)
		{
			return !(first <= second);
		}

		constexpr bool operator>=(const Rational& first, const Rational& second)
		{
			return !(first < second);
		}

		constexpr bool operator>=(const Rational& first, const double second)
		{
			return !(first < second);
		}

		constexpr bool operator>=(const Rational& first, const int second)
		{
			return !(first < second);
		}

		inline std::ostream& operator<<(std::ostream& os, const Rational& r)
		{
			return os << r.numerator() << "/" << r.denominator();
		}
	}
}