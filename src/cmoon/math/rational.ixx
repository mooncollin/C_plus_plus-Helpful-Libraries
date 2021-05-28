export module cmoon.math.rational;

import <memory>;
import <concepts>;
import <iostream>;
import <ratio>;
import <numeric>;
import <format>;
import <string>;

import cmoon.math.pow;
import cmoon.math.floor;

namespace cmoon
{
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
				requires(std::convertible_to<std::intmax_t, value_type>)
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
			return cmoon::pow(mult_inverse(r), -p);
		}

		return { cmoon::pow(r.numerator(), p), cmoon::pow(r.denominator(), p) };
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

namespace std
{
	// TODO: MSVC is not seeing this overload in external modules. Not sure what to do...
	export
	template<class Rep>
	struct formatter<cmoon::basic_rational<Rep>> : public formatter<std::string>
	{
		template<class FormatContext>
		auto format(const cmoon::basic_rational<Rep>& r, FormatContext& ctx)
		{
			return formatter<std::string>::format(
				std::format("{}/{}", r.numerator(), r.denominator()), ctx
			);
		}
	};
}