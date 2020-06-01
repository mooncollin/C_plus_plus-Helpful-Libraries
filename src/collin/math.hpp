#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <iostream>
#include <cstdint>
#include <cmath>

namespace collin
{
	namespace math
	{
		constexpr std::uintmax_t factorial(std::size_t n)
		{
			std::uintmax_t result {1};
			for(std::size_t i {2}; i <= n; i++)
			{
				result *= i;
			}

			return result;
		}

		template<class T, typename = std::enable_if_t<
/* requires*/	std::is_integral_v<T>
		>>
		constexpr bool is_odd(const T val)
		{
			return val & 1;
		}

		template<class T, typename = std::enable_if_t<
/* requires*/	std::is_integral_v<T>
		>>
		constexpr bool is_even(const T val)
		{
			return !is_odd(val);
		}

		template<class T, typename = std::enable_if_t<
/* requires*/	std::is_integral_v<T>
		>>
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

		template<class T, class U, typename = std::enable_if_t<
/* requires */ std::is_integral_v<T> && std::is_integral_v<U>
		>>
		constexpr auto pow(T base, U exp)
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

			std::common_type_t<U, T> result = 1;
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

		template<class intT, typename = std::enable_if_t<
/* requires */	std::is_integral_v<intT>
		>>
		class basic_rational
		{
			public:
				using value_type = intT;

				constexpr basic_rational(value_type numerator, value_type denominator)
					: numerator_(numerator), denominator_(denominator) {}
				constexpr basic_rational(value_type numerator)
					: numerator_(numerator), denominator_(1) {}
				constexpr basic_rational(const basic_rational&) = default;

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
					constexpr basic_rational(const basic_rational<intT2>& other)
						: numerator_(other.numerator_), denominator_(other.denominator)
				{
				}

				constexpr basic_rational(basic_rational&&) = default;

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
					constexpr basic_rational(basic_rational<intT2>&& other)
						: numerator_(std::move(other.numerator_)), denominator_(std::move(other.denominator_))
				{
				}

				constexpr basic_rational& operator=(const basic_rational&) = default;

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
					constexpr basic_rational& operator=(const basic_rational<intT2>& other)
				{
					if (this != &other)
					{
						numerator_ = other.numerator_;
						denominator_ = other.denominator_;
					}

					return *this;
				}

				constexpr basic_rational& operator=(basic_rational&&) = default;

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
					constexpr basic_rational& operator=(basic_rational<intT2>&& other)
				{
					numerator_ = std::move(other.numerator_);
					denominator_ = std::move(other.denominator_);
					return *this;
				}

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
				constexpr basic_rational operator+(const basic_rational<intT2>& r) const
				{
					return { numerator() * r.denominator() + denominator() * r.numerator(),
							denominator() * r.denominator() };
				}

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
				constexpr basic_rational operator+(intT2 i) const
				{
					return *this + basic_rational(i);
				}

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
				basic_rational& operator+=(const basic_rational<intT2>& r)
				{
					*this = *this + r;
					return *this;
				}

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
				basic_rational& operator+=(intT2 i)
				{
					*this = *this + i;
					return *this;
				}

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
				constexpr basic_rational operator-(const basic_rational<intT2>& r) const
				{
					return { numerator() * r.denominator() - denominator() * r.numerator(),
							denominator() * r.denominator() };
				}

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
				constexpr basic_rational operator-(intT2 i) const
				{
					return *this - basic_rational(i);
				}

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
				basic_rational& operator-=(const basic_rational<intT2>& r)
				{
					*this = *this - r;
					return *this;
				}

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
				basic_rational& operator-=(intT2 i)
				{
					*this = *this - i;
					return *this;
				}

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
				constexpr basic_rational operator*(const basic_rational<intT2>& r) const
				{
					return {numerator() * r.numerator(), denominator() * r.denominator()};
				}

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
				constexpr basic_rational operator*(intT2 i) const
				{
					return *this * basic_rational(i);
				}

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
				basic_rational& operator*=(const basic_rational<intT2>& r)
				{
					*this = *this * r;
					return *this;
				}

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
				basic_rational& operator*=(intT2 i)
				{
					*this = *this * i;
					return *this;
				}

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
				constexpr basic_rational operator/(const basic_rational<intT2>& r) const
				{
					return {numerator() * r.denominator(), denominator() * r.numerator()};
				}

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
				constexpr basic_rational operator/(intT2 i) const
				{
					return *this / basic_rational(i);
				}

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
				basic_rational& operator/=(const basic_rational<intT2>& r)
				{
					*this = *this / r;
					return *this;
				}

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
				basic_rational& operator/=(intT2 i)
				{
					*this = *this / i;
					return *this;
				}

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
				void numerator(intT2 n)
				{
					numerator_ = n;
				}

				constexpr int numerator() const
				{
					return numerator_;
				}

				template<class intT2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<intT2, intT>
				>>
				void denominator(intT2 d)
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
				value_type numerator_;
				value_type denominator_;
		};

		using rational = basic_rational<int>;

		template<class intT, class intT2 = intT>
		constexpr basic_rational<intT> operator+(intT i, const basic_rational<intT2>& r)
		{
			return r + basic_rational(i);
		}

		template<class intT, class intT2 = intT>
		constexpr basic_rational<intT> operator-(intT i, const basic_rational<intT2>& r)
		{
			return r - basic_rational(i);
		}

		template<class intT, class intT2 = intT>
		constexpr basic_rational<intT> operator*(intT i, const basic_rational<intT2>& r)
		{
			return r * basic_rational(i);
		}

		template<class intT, class intT2 = intT>
		constexpr basic_rational<intT> operator/(intT i, const basic_rational<intT2>& r)
		{
			return r / basic_rational(i);
		}

		template<class intT>
		constexpr basic_rational<intT> add_inverse(const basic_rational<intT>& r)
		{
			return { -r.numerator(), r.denominator() };
		}

		template<class intT>
		constexpr basic_rational<intT> mult_inverse(const basic_rational<intT>& r)
		{
			return { r.denominator(), r.numerator() };
		}

		template<class intT, class intT2 = intT>
		constexpr basic_rational<intT> pow(const basic_rational<intT>& r, intT2 p)
		{
			if (p < 0)
			{
				return pow(mult_inverse(r), -p);
			}

			return { pow(r.numerator(), p), pow(r.denominator(), p) };
		}

		template<class intT>
		constexpr basic_rational<intT> canonical(const basic_rational<intT>& r)
		{
			auto gcd_ = gcd(r.numerator(), r.denominator());
			return {r.numerator() / gcd_, r.denominator() / gcd_};
		}

		template<class intT, class intT2 = intT>
		constexpr bool operator==(const basic_rational<intT>& first, const basic_rational<intT2>& second)
		{
			return first.numerator() * second.denominator() == first.denominator() * second.numerator();
		}

		template<class intT>
		constexpr bool operator==(const basic_rational<intT>& first, const double second)
		{
			return static_cast<double>(first) == second;
		}

		template<class intT, class intT2 = intT>
		constexpr bool operator==(const basic_rational<intT>& first, const intT2 second)
		{
			return canonical(first).numerator() == second;
		}
		
		template<class intT, class intT2 = intT>
		constexpr bool operator!=(const basic_rational<intT>& first, const basic_rational<intT2>& second)
		{
			return !(first == second);
		}

		template<class intT>
		constexpr bool operator!=(const basic_rational<intT>& first, const double second)
		{
			return !(first == second);
		}

		template<class intT, class intT2 = intT>
		constexpr bool operator!=(const basic_rational<intT>& first, const intT2 second)
		{
			return !(first == second);
		}

		template<class intT, class intT2 = intT>
		constexpr bool operator<(const basic_rational<intT>& first, const basic_rational<intT2>& second)
		{
			return first.numerator() * second.denominator() < first.denominator() * second.numerator();
		}

		template<class intT>
		constexpr bool operator<(const basic_rational<intT>& first, const double second)
		{
			return static_cast<double>(first) < second;
		}

		template<class intT, class intT2 = intT>
		constexpr bool operator<(const basic_rational<intT>& first, const intT2 second)
		{
			return canonical(first).numerator() < second;
		}

		template<class intT, class intT2 = intT>
		constexpr bool operator<=(const basic_rational<intT>& first, const basic_rational<intT2>& second)
		{
			return first.numerator() * second.denominator() <= first.denominator() * second.numerator();
		}

		template<class intT>
		constexpr bool operator<=(const basic_rational<intT>& first, const double second)
		{
			return static_cast<double>(first) <= second;
		}

		template<class intT, class intT2 = intT>
		constexpr bool operator<=(const basic_rational<intT>& first, const intT2 second)
		{
			return canonical(first).numerator() <= second;
		}

		template<class intT, class intT2 = intT>
		constexpr bool operator>(const basic_rational<intT>& first, const basic_rational<intT2>& second)
		{
			return !(first <= second);
		}

		template<class intT>
		constexpr bool operator>(const basic_rational<intT>& first, const double second)
		{
			return !(first <= second);
		}

		template<class intT, class intT2 = intT>
		constexpr bool operator>(const basic_rational<intT>& first, const intT2 second)
		{
			return !(first <= second);
		}

		template<class intT, class intT2 = intT>
		constexpr bool operator>=(const basic_rational<intT>& first, const basic_rational<intT2>& second)
		{
			return !(first < second);
		}

		template<class intT>
		constexpr bool operator>=(const basic_rational<intT>& first, const double second)
		{
			return !(first < second);
		}

		template<class intT, class intT2>
		constexpr bool operator>=(const basic_rational<intT>& first, const intT2 second)
		{
			return !(first < second);
		}

		template<class intT>
		inline std::ostream& operator<<(std::ostream& os, const basic_rational<intT>& r)
		{
			return os << r.numerator() << "/" << r.denominator();
		}

		template<class IntT = int, class Float, typename = std::enable_if_t<
/* requires */	std::is_integral_v<IntT> && std::is_floating_point_v<Float>
		>>
		constexpr IntT ceil(Float f)
		{
			const auto inum = static_cast<IntT>(f);
			
			if(f == static_cast<Float>(inum))
			{
				return inum;
			}

			return inum + (f > 0 ? 1 : 0);
		}

		template<class IntT = int, class Float, typename = std::enable_if_t<
/* requires */	std::is_integral_v<IntT> && std::is_floating_point_v<Float>
		>>
		constexpr IntT floor(Float f)
		{
			const auto val_int = static_cast<IntT>(f);

			if(f >= 0 || f == static_cast<Float>(val_int))
			{
				return val_int;
			}

			return val_int - 1;
		}
	}
}