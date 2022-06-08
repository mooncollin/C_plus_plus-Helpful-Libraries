export module cmoon.math.rational;

import <memory>;
import <concepts>;
import <iostream>;
import <ratio>;
import <numeric>;
import <format>;
import <string>;
import <cstdint>;

namespace cmoon
{
	export
	template<std::integral Rep>
	class basic_rational
	{
		public:
			using value_type = Rep;

			constexpr basic_rational() noexcept = default;

			template<std::convertible_to<value_type> Rep2, std::convertible_to<value_type> Rep3 = value_type>
				requires(std::integral<Rep2> && std::integral<Rep3>)
			constexpr basic_rational(const Rep2& numerator, const Rep3& denominator = 1) noexcept
				: numerator_{static_cast<value_type>(numerator)}, denominator_{static_cast<value_type>(denominator)} {}

			constexpr basic_rational(const basic_rational&) noexcept = default;

			template<std::convertible_to<value_type> Rep2>
			constexpr basic_rational(const basic_rational<Rep2>& other) noexcept
				: numerator_{static_cast<value_type>(other.numerator())}, denominator_{static_cast<value_type>(other.denominator())} {}

			template<std::floating_point Float>
			explicit basic_rational(const Float f) noexcept
			{
				auto copy {f};
				value_type n {1};

				while (copy != std::floor(copy) &&
						n < static_cast<value_type>(10000000000000000))
				{
					n *= 10;
					copy *= 10;
				}

				numerator_ = static_cast<value_type>(f * n);
				denominator_ = n;
			}

			template<std::intmax_t Num, std::intmax_t Denom>
				requires(std::convertible_to<std::intmax_t, value_type>)
			constexpr explicit basic_rational(std::ratio<Num, Denom>) noexcept
				: numerator_{static_cast<value_type>(Num)}, denominator_{static_cast<value_type>(Denom)} {}

			constexpr basic_rational(basic_rational&&) noexcept = default;

			template<std::convertible_to<value_type> Rep2>
			constexpr basic_rational(basic_rational<Rep2>&& other)
				: numerator_{static_cast<value_type>(other.numerator_)}, denominator_{static_cast<value_type>(other.denominator_)} {}
				
			constexpr basic_rational& operator=(const basic_rational&) = default;

			template<std::convertible_to<value_type> Rep2>
			constexpr basic_rational& operator=(const basic_rational<Rep2>& other)
			{
				numerator_ = static_cast<value_type>(other.numerator_);
				denominator_ = static_cast<value_type>(other.denominator_);

				return *this;
			}

			constexpr basic_rational& operator=(basic_rational&&) noexcept = default;

			template<std::convertible_to<value_type> Rep2>
			constexpr basic_rational& operator=(basic_rational<Rep2>&& other)
			{
				numerator_ = static_cast<value_type>(other.numerator_);
				denominator_ = static_cast<value_type>(other.denominator_);

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
				if (denominator_ == r.denominator_)
				{
					numerator_ += r.numerator_;
				}
				else
				{
					numerator_ = numerator_ * r.denominator_ + denominator_ * r.denominator_;
					denominator_ *= r.denominator;
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
					numerator_ = numerator_ * r.denominator_ - denominator_ * r.denominator_;
					denominator_ *= r.denominator;
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

			[[nodiscard]] constexpr value_type& numerator() noexcept
			{
				return numerator_;
			}

			[[nodiscard]] constexpr value_type numerator() const noexcept
			{
				return numerator_;
			}

			[[nodiscard]] constexpr value_type& denominator() noexcept
			{
				return denominator_;
			}

			[[nodiscard]] constexpr value_type denominator() const noexcept
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

			[[nodiscard]] friend constexpr bool operator!=(const basic_rational&, const basic_rational&) noexcept = default;

			[[nodiscard]] friend constexpr bool operator<(const basic_rational& lhs, const basic_rational& rhs) noexcept
			{
				return lhs.numerator_ * rhs.denominator_ < lhs.denominator_ * rhs.numerator_;
			}

			[[nodiscard]] friend constexpr bool operator>(const basic_rational& lhs, const basic_rational& rhs) noexcept
			{
				return rhs < lhs;
			}

			[[nodiscard]] friend constexpr bool operator<=(const basic_rational& lhs, const basic_rational& rhs) noexcept
			{
				return !(rhs < lhs);
			}

			[[nodiscard]] friend constexpr bool operator>=(const basic_rational& lhs, const basic_rational& rhs) noexcept
			{
				return !(lhs < rhs);
			}
		private:
			value_type numerator_ {};
			value_type denominator_ {1};
	};

	export
	template<class T>
	basic_rational(T, T) -> basic_rational<T>;

	export
	template<std::floating_point F>
	basic_rational(F) -> basic_rational<std::intmax_t>;

	export
	using rational = basic_rational<int>;

	export
	template<class Ratio, class Rep = decltype(Ratio::num)>
	constexpr basic_rational<Rep> rational_ratio {Ratio::num, Ratio::den};

	export
	template<class Rep>
	[[nodiscard]] constexpr basic_rational<Rep> add_inverse(const basic_rational<Rep>& r) noexcept
	{
		return {-r.numerator(), r.denominator()};
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr basic_rational<Rep> mult_inverse(const basic_rational<Rep>& r) noexcept
	{
		return {r.denominator(), r.numerator()};
	}

	export
	template<class Rep, std::floating_point Rep2>
	[[nodiscard]] basic_rational<Rep> pow(const basic_rational<Rep>& r, const Rep2 p) noexcept
	{
		if (p < 0)
		{
			return pow(mult_inverse(r), -p);
		}

		return basic_rational<Rep>{std::pow(r.numerator(), p) / std::pow(r.denominator(), p)};
	}

	export
	template<class Rep, std::integral Rep2>
	[[nodiscard]] basic_rational<std::common_type_t<Rep, Rep2>> pow(const basic_rational<Rep>& r, const Rep2 p) noexcept
	{
		using common_type = std::common_type_t<Rep, Rep2>;
		if (p < 0)
		{
			return pow(mult_inverse(r), -p);
		}

		return basic_rational<common_type> {
			static_cast<common_type>(std::pow(r.numerator(), p)),
			static_cast<common_type>(std::pow(r.denominator(), p))
		};
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr basic_rational<Rep> canonical(const basic_rational<Rep>& r) noexcept
	{
		const auto gcd_ {std::gcd(r.numerator(), r.denominator())};
		return {r.numerator() / gcd_, r.denominator() / gcd_};
	}

	export
	template<class Rep, class CharT, class Traits>
	inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const basic_rational<Rep>& r)
	{
		return os << r.numerator() << "/" << r.denominator();
	}
}