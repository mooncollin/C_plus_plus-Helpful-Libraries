#pragma once

#include <cstdint>
#include <cstddef>
#include <optional>
#include <limits>
#include <algorithm>
#include <numbers>

#include "collin/concepts.hpp"
#include "collin/math.hpp"

namespace collin
{
	namespace finance
	{
		using decimal_storage_t = std::intmax_t;
		using round_conversion_t = long double;

		namespace details
		{
			template<std::size_t Precision>
			struct decimal_factor
			{
				static constexpr decimal_storage_t value {10 * decimal_factor<Precision - 1>::value};
			};

			template<>
			struct decimal_factor<0>
			{
				static constexpr decimal_storage_t value {1};
			};

			template<std::size_t P>
			constexpr auto decimal_factor_v = decimal_factor<P>::value;
		}

		template<collin::concepts::arithmetic T>
		[[nodiscard]] constexpr decimal_storage_t truncate(const T& value) noexcept
		{
			return static_cast<decimal_storage_t>(value);
		}

		struct truncate_round_policy
		{
			template<collin::concepts::arithmetic T>
			[[nodiscard]] static constexpr decimal_storage_t round(const T& value) noexcept
			{
				return truncate(value);
			}

			[[nodiscard]] static constexpr std::optional<decimal_storage_t> divide_round(const decimal_storage_t a, const decimal_storage_t b) noexcept
			{
				return {a / b};
			}
		};

		struct default_round_policy
		{
			template<collin::concepts::arithmetic T>
			[[nodiscard]] static constexpr decimal_storage_t round(const T& value) noexcept
			{
				if (value < 0.0)
				{
					return truncate(value - 0.5);
				}

				return truncate(value + 0.5);
			}

			[[nodiscard]] static constexpr std::optional<decimal_storage_t> divide_round(const decimal_storage_t a, const decimal_storage_t b) noexcept
			{
				decimal_storage_t divisor_corr = collin::math::abs(b) / 2;
				if (a >= 0)
				{
					if ((std::numeric_limits<decimal_storage_t>::max() - a) >= divisor_corr)
					{
						return {(a + divisor_corr) / b};
					}
				}
				else
				{
					if (-(std::numeric_limits<decimal_storage_t>::min() - a) >= divisor_corr)
					{
						return {(a - divisor_corr) / b};
					}
				}

				return {};
			}
		};

		struct half_down_round_policy
		{
			template<collin::concepts::arithmetic T>
			[[nodiscard]] static constexpr decimal_storage_t round(const T& value) noexcept
			{
				T val1;
				T decimals;

				if (value >= 0.0)
				{
					decimals = value - collin::math::floor<decimal_storage_t>(value);
					val1 = decimals > 0.5 ? collin::math::ceil<decimal_storage_t>(value) : value;
				}
				else
				{
					decimals = collin::math::abs(value + collin::math::floor<decimal_storage_t>(collin::math::abs(value)));
					val1 = decimals < 0.5 ? collin::math::ceil<decimal_storage_t>(value) : value;
				}

				return collin::math::floor<decimal_storage_t>(val1);
			}

			[[nodiscard]] static constexpr std::optional<decimal_storage_t> divide_round(const decimal_storage_t a, const decimal_storage_t b) noexcept
			{
				const auto abs_a = collin::math::abs(a);
				const auto abs_b = collin::math::abs(b);

				const decimal_storage_t divisor_corr = abs_b / 2;
				const decimal_storage_t remainder = abs_a % abs_b;

				if (a >= 0)
				{
					if ((std::numeric_limits<decimal_storage_t>::max() - a) >= divisor_corr)
					{
						return {(remainder > divisor_corr) ? (a + divisor_corr) / b : a / b };
					}
				}
				else if (-(std::numeric_limits<decimal_storage_t>::min() - a) >= divisor_corr)
				{
					return {(a - divisor_corr) / b};
				}

				return {};
			}
		};

		struct half_up_round_policy
		{
			template<collin::concepts::arithmetic T>
			[[nodiscard]] static constexpr decimal_storage_t round(const T& value) noexcept
			{
				T val1;
				T decimals;

				if (value >= 0.0)
				{
					decimals = value - collin::math::floor<decimal_storage_t>(value);
					val1 = decimals >= 0.5 ? collin::math::ceil<decimal_storage_t>(value) : value;
				}
				else
				{
					decimals = collin::math::abs(value + collin::math::floor<decimal_storage_t>(collin::math::abs(value)));
					val1 = decimals <= 0.5 ? collin::math::ceil<decimal_storage_t>(value) : value;
				}

				return collin::math::floor<decimal_storage_t>(val1);
			}

			[[nodiscard]] static constexpr std::optional<decimal_storage_t> divide_round(const decimal_storage_t a, const decimal_storage_t b) noexcept
			{
				const auto abs_a = collin::math::abs(a);
				const auto abs_b = collin::math::abs(b);

				const decimal_storage_t divisor_corr = abs_b / 2;
				const decimal_storage_t remainder = abs_a % abs_b;

				if (a >= 0)
				{
					if ((std::numeric_limits<decimal_storage_t>::max() - a) >= divisor_corr)
					{
						return { (remainder > divisor_corr) ? (a + divisor_corr) / b : a / b };
					}
				}
				else if (-(std::numeric_limits<decimal_storage_t>::min() - a) >= divisor_corr)
				{
					if (remainder < divisor_corr)
					{
						return {(a - remainder) / b};
					}
					else if (remainder == divisor_corr)
					{
						return {(a + divisor_corr) / b};
					}

					return {(a + remainder - abs_b) / b};
				}

				return {};
			}
		};

		struct ceiling_round_policy
		{
			template<collin::concepts::arithmetic T>
			[[nodiscard]] static constexpr decimal_storage_t round(const T& value) noexcept
			{
				return collin::math::ceil<decimal_storage_t>(value);
			}

			[[nodiscard]] static constexpr std::optional<decimal_storage_t> divide_round(const decimal_storage_t a, const decimal_storage_t b) noexcept
			{
				const auto abs_a = collin::math::abs(a);
				const auto abs_b = collin::math::abs(b);

				const decimal_storage_t remainder = abs_a & abs_b;

				if (remainder == 0)
				{
					return {a / b};
				}
				else if (a >= 0)
				{
					return {(a + abs_b) / b};
				}

				return {a / b};
			}
		};

		struct floor_round_policy
		{
			template<collin::concepts::arithmetic T>
			[[nodiscard]] static constexpr decimal_storage_t round(const T& value) noexcept
			{
				return collin::math::floor<decimal_storage_t>(value);
			}

			[[nodiscard]] static constexpr decimal_storage_t divide_round(const decimal_storage_t a, const decimal_storage_t b) noexcept
			{
				const auto abs_a = collin::math::abs(a);
				const auto abs_b = collin::math::abs(b);

				const decimal_storage_t remainder = abs_a & abs_b;

				if (remainder == 0)
				{
					return {a / b};
				}
				else if (a >= 0)
				{
					return {(a - remainder) / b};
				}

				return {(a + remainder - abs_b) / b};
			}
		};

		struct round_down_round_policy : public truncate_round_policy {};

		struct round_up_round_policy
		{
			template<collin::concepts::arithmetic T>
			[[nodiscard]] static constexpr decimal_storage_t round(const T& value) noexcept
			{
				return value > 0.0
					? collin::math::ceil<decimal_storage_t>(value)
					: collin::math::floor<decimal_storage_t>(value);
			}

			[[nodiscard]] static constexpr std::optional<decimal_storage_t> divide_round(const decimal_storage_t a, const decimal_storage_t b) noexcept
			{
				const auto abs_a = collin::math::abs(a);
				const auto abs_b = collin::math::abs(b);

				const decimal_storage_t remainder = abs_a % abs_b;

				if (remainder == 0)
				{
					return {a / b};
				}
				else if (a >= 0)
				{
					return {(a + abs_b) / b};
				}

				return {(a - abs_b) / b};
			}
		};

		template<std::size_t Precision, class RoundPolicy = default_round_policy>
		class decimal
		{
			public:
				static constexpr auto precision = Precision;
				using round_policy = RoundPolicy;

				constexpr decimal() noexcept = default;

				template<collin::concepts::integral T>
					requires(collin::concepts::convertible_to<T, decimal_storage_t>)
				constexpr decimal(const T& value) noexcept
					: amount_{value * precision_factor()} {}

				template<collin::concepts::floating_point F>
				constexpr explicit decimal(const F& value) noexcept
					: amount_{fp_to_storage(value)} {}

				template<collin::concepts::integral T>
				decimal& operator=(const T& value) noexcept
				{
					amount_ = value * precision_factor();
					return *this;
				}

				template<collin::concepts::floating_point F>
				decimal& operator=(const F& value) noexcept
				{
					amount_ = fp_to_storage(value);
					return *this;
				}

				[[nodiscard]] static constexpr auto precision_factor() noexcept
				{
					return details::decimal_factor_v<precision>;
				}

				friend constexpr bool operator==(const decimal& lhs, const decimal& rhs) noexcept
				{
					return lhs.amount_ == rhs.amount_;
				}

				friend constexpr bool operator!=(const decimal& lhs, const decimal& rhs) noexcept
				{
					return !(lhs == rhs);
				}

				friend constexpr bool operator<(const decimal& lhs, const decimal& rhs) noexcept
				{
					return lhs.amount_ < rhs.amount_;
				}

				friend constexpr bool operator>(const decimal& lhs, const decimal& rhs) noexcept
				{
					return rhs < lhs;
				}

				friend constexpr bool operator<=(const decimal& lhs, const decimal& rhs) noexcept
				{
					return !(rhs < lhs);
				}

				friend constexpr bool operator>=(const decimal& lhs, const decimal& rhs) noexcept
				{
					return !(lhs < rhs);
				}

				template<std::size_t OtherPrecision>
					requires(OtherPrecision <= Precision)
				constexpr decimal& operator+=(const decimal<OtherPrecision, RoundPolicy>& rhs) noexcept
				{
					amount_ += rhs.unbiased() * details::decimal_factor_v<Precision - OtherPrecision>;
					return *this;
				}

				template<collin::concepts::integral N>
				constexpr decimal& operator+=(const N& n) noexcept
				{
					amount_ += n * precision_factor();
					return *this;
				}

				template<std::size_t Precision2>
				friend constexpr auto operator+(const decimal<Precision, RoundPolicy>& lhs, const decimal<Precision2, RoundPolicy>& rhs) noexcept
				{
					if constexpr (Precision >= Precision2)
					{
						auto result = lhs;
						result += rhs;
						return result;
					}
					else
					{
						auto result = rhs;
						result += lhs;
						return result;
					}
				}

				template<collin::concepts::integral N>
				friend constexpr auto operator+(const decimal<Precision, RoundPolicy>& lhs, const N& rhs) noexcept
				{
					auto result = lhs;
					result += rhs;
					return result;
				}


				template<collin::concepts::integral N>
				friend constexpr auto operator+(const N& lhs, const decimal<Precision, RoundPolicy>& rhs) noexcept
				{
					auto result = rhs;
					result += lhs;
					return result;
				}

				template<std::size_t OtherPrecision>
					requires(OtherPrecision <= Precision)
				constexpr decimal& operator-=(const decimal<OtherPrecision, RoundPolicy>& rhs) noexcept
				{
					amount_ -= rhs.unbiased() * details::decimal_factor_v<Precision - OtherPrecision>;
					return *this;
				}

				template<collin::concepts::integral N>
				constexpr decimal& operator-=(const N& n) noexcept
				{
					amount_ -= n * precision_factor();
					return *this;
				}

				template<std::size_t Precision2>
				friend constexpr auto operator-(const decimal<Precision, RoundPolicy>& lhs, const decimal<Precision2, RoundPolicy>& rhs) noexcept
				{
					if constexpr (Precision >= Precision2)
					{
						decimal<Precision, RoundPolicy> result{lhs};
						result -= rhs;
						return result;
					}
					else
					{
						decimal<Precision2, RoundPolicy> result{rhs};
						result -= lhs;
						return result;
					}
				}

				template<collin::concepts::integral N>
				friend constexpr auto operator-(const decimal<Precision, RoundPolicy>& lhs, const N& rhs) noexcept
				{
					auto result = lhs;
					result -= rhs;
					return result;
				}


				template<collin::concepts::integral N>
				friend constexpr auto operator-(const N& lhs, const decimal<Precision, RoundPolicy>& rhs) noexcept
				{
					auto result = decimal{lhs};
					result -= rhs;
					return result;
				}

				constexpr decimal operator+() const noexcept
				{
					return *this;
				}

				constexpr decimal operator-() const noexcept
				{
					auto result = *this;
					result.amount_ = -result.amount_;
					return result;
				}

				template<collin::concepts::integral T>
					requires(collin::concepts::convertible_to<T, decimal_storage_t>)
				constexpr decimal& operator*=(const T& rhs) noexcept
				{
					amount_ *= rhs;
					return *this;
				}
				
				template<std::size_t OtherPrecision>
					requires(OtherPrecision <= Precision)
				constexpr decimal& operator*=(const decimal<OtherPrecision, RoundPolicy>& rhs) noexcept
				{
					amount_ = mult_div(amount_, rhs.unbiased() * details::decimal_factor_v<Precision - OtherPrecision>, precision_factor());
					return *this;
				}

				template<std::size_t Precision2>
				friend constexpr auto operator*(const decimal<Precision, RoundPolicy>& lhs, const decimal<Precision2, RoundPolicy>& rhs) noexcept
				{
					if constexpr (Precision >= Precision2)
					{
						decimal<Precision, RoundPolicy> result{lhs};
						result *= rhs;
						return result;
					}
					else
					{
						decimal<Precision2, RoundPolicy> result{rhs};
						result *= lhs;
						return result;
					}
				}

				template<collin::concepts::integral T>
					requires(collin::concepts::convertible_to<T, decimal_storage_t>)
				friend constexpr decimal operator*(const decimal& lhs, const T& rhs) noexcept
				{
					auto result = lhs;
					result *= rhs;
					return result;
				}

				template<collin::concepts::integral T>
					requires(collin::concepts::convertible_to<T, decimal_storage_t>)
				friend constexpr decimal operator*(const T& lhs, const decimal& rhs) noexcept
				{
					auto result = rhs;
					result *= lhs;
					return result;
				}

				template<collin::concepts::integral T>
					requires(collin::concepts::convertible_to<T, decimal_storage_t>)
				constexpr decimal& operator/=(const T& rhs) noexcept
				{
					const auto new_amount = round_policy::divide_round(amount_, rhs);
					if (new_amount)
					{
						amount_ = new_amount.value();
					}
					else
					{
						amount_ = mult_div(amount_, 1, rhs);
					}
					return *this;
				}

				template<std::size_t OtherPrecision>
					requires(OtherPrecision <= Precision)
				constexpr decimal& operator/=(const decimal<OtherPrecision, RoundPolicy>& rhs) noexcept
				{
					amount_ = mult_div(amount_, precision_factor(), rhs.unbiased() * details::decimal_factor_v<Precision - OtherPrecision>);
					return *this;
				}

				template<std::size_t Precision2>
				friend constexpr decimal operator/(const decimal<Precision, RoundPolicy>& lhs, const decimal<Precision2, RoundPolicy>& rhs) noexcept
				{
					if constexpr (Precision >= Precision2)
					{
						decimal<Precision, RoundPolicy> result{lhs};
						result /= rhs;
						return result;
					}
					else
					{
						decimal<Precision2, RoundPolicy> result{rh};
						result /= lhs;
						return result;
					}
				}

				template<collin::concepts::integral T>
					requires(collin::concepts::convertible_to<T, decimal_storage_t>)
				friend constexpr decimal operator/(const decimal& lhs, const T& rhs) noexcept
				{
					auto result = lhs;
					result /= rhs;
					return result;
				}

				template<collin::concepts::integral T>
					requires(collin::concepts::convertible_to<T, decimal_storage_t>)
				friend constexpr decimal operator/(const T& lhs, const decimal& rhs) noexcept
				{
					auto result = decimal{lhs};
					result /= rhs;
					return result;
				}

				template<collin::concepts::floating_point F>
				[[nodiscard]] constexpr operator F() const noexcept
				{
					return static_cast<F>(amount_) / static_cast<F>(precision_factor());
				}

				template<std::size_t OtherPrecision, class OtherRoundPolicy>
					requires(OtherPrecision < Precision)
				[[nodiscard]] constexpr operator explicit decimal<OtherPrecision, OtherRoundPolicy>() const noexcept
				{
					decimal<OtherPrecision, OtherRoundPolicy> result;
					const auto new_amount = round_policy::divide_round(amount_, details::decimal_factor_v<Precision - OtherPrecision>);
					if (new_amount)
					{
						result.unbiased(new_amount.value());
					}
					else
					{
						result.unbiased(mult_div(amount_, 1, details::decimal_factor_v<Precision - OtherPrecision>));
					}
					return result;
				}

				template<std::size_t OtherPrecision, class OtherRoundPolicy>
					requires(OtherPrecision > Precision)
				[[nodiscard]] constexpr operator decimal<OtherPrecision, OtherRoundPolicy>() const noexcept
				{
					decimal<OtherPrecision, OtherRoundPolicy> result;
					result.unbiased(amount_ * details::decimal_factor_v<OtherPrecision - Precision>);
					return result;
				}

				[[nodiscard]] constexpr decimal_storage_t unbiased() const noexcept
				{
					return amount_;
				}

				constexpr void unbiased(decimal_storage_t amount) noexcept
				{
					amount_ = amount;
				}
			private:
				decimal_storage_t amount_ {0};

				template<collin::concepts::floating_point F>
				[[nodiscard]] static constexpr auto fp_to_storage(const F& value) noexcept
				{
					const auto int_part = truncate(value);
					const F fractional_part = value - int_part;
					return round_policy::round(static_cast<F>(precision_factor()) * fractional_part) + (precision_factor() * int_part);
				}

				[[nodiscard]] static constexpr decimal_storage_t mult_div(const decimal_storage_t a, const decimal_storage_t b, decimal_storage_t divisor) noexcept
				{
					const auto a_int = a / divisor;
					auto a_dec = a % divisor;
					const auto b_int = b / divisor;
					auto b_dec = b % divisor;

					const auto result = a * b_int + a_int * b_dec;
					if (a_dec == 0 || b_dec == 0)
					{
						return result;
					}

					if (!multiply_overflow(a_dec, b_dec))
					{
						return result + round_policy::divide_round(a_dec * b_dec, divisor).value_or(0);
					}

					{
						auto c = collin::math::gcd(a_dec, divisor);
						if (c != 1)
						{
							a_dec /= c;
							divisor /= c;
						}

						c = collin::math::gcd(b_dec, divisor);
						if (c != 1)
						{
							b_dec /= c;
							divisor /= c;
						}
					}

					if (!multiply_overflow(a_dec, b_dec))
					{
						const auto op = round_policy::divide_round(a_dec * b_dec, divisor);
						if (op)
						{
							return result + op.value();
						}
					}

					return result + round_policy::round(static_cast<round_conversion_t>(a_dec) * static_cast<round_conversion_t>(b_dec) / static_cast<round_conversion_t>(divisor));
				}

				[[nodiscard]] static constexpr bool multiply_overflow(const decimal_storage_t a, const decimal_storage_t b) noexcept
				{
					if (a == 0 || b == 0)
					{
						return false;
					}

					if ((a < 0) != (b < 0))
					{
						if (a == std::numeric_limits<decimal_storage_t>::min())
						{
							return b > 1;
						}
						else if (b == std::numeric_limits<decimal_storage_t>::min())
						{
							return a > 1;
						}

						if (a < 0)
						{
							return multiply_overflow(-a, b);
						}

						if (b < 0)
						{
							return multiply_overflow(a, -b);
						}
					}
					else if (a < 0 && b < 0)
					{
						if (a == std::numeric_limits<decimal_storage_t>::min())
						{
							return b < -1;
						}
						else if (b == std::numeric_limits<decimal_storage_t>::min())
						{
							return a < -1;
						}

						return multiply_overflow(-a, -b);
					}

					return a > (std::numeric_limits<decimal_storage_t>::max() / b);
				}
		};

		template<std::size_t Precision, class RoundPolicy>
		[[nodiscard]] constexpr decimal<Precision, RoundPolicy> abs(const decimal<Precision, RoundPolicy>& d) noexcept
		{
			if (d.unbiased() >= 0)
			{
				return d;
			}

			return -d;
		}

		namespace details
		{
			template<std::size_t Precision, class RoundPolicy, collin::concepts::integral N>
			[[nodiscard]] constexpr decimal<Precision, RoundPolicy> pow2(const decimal<Precision, RoundPolicy>& y, const decimal<Precision, RoundPolicy>& base, const N& exp) noexcept
			{
				if (exp < 0)
				{
					return pow2(y, 1 / base, -exp);
				}
				else if (exp == 0)
				{
					return y;
				}
				else if (exp == 1)
				{
					return base * y;
				}
				else if (collin::math::is_even(exp))
				{
					return pow2(y, base * base, exp / 2);
				}
				else
				{
					return pow2(base * y, base * base, (exp - 1) / 2);
				}
			}
		}

		template<std::size_t Precision, class RoundPolicy = default_round_policy>
		constexpr auto decimal_e = decimal<Precision, RoundPolicy>{std::numbers::e_v<round_conversion_t>};

		template<std::size_t Precision, class RoundPolicy = default_round_policy>
		constexpr auto decimal_pi = decimal<Precision, RoundPolicy>{std::numbers::pi_v<round_conversion_t>};

		template<std::size_t Precision, class RoundPolicy, collin::concepts::integral N>
		[[nodiscard]] constexpr decimal<Precision, RoundPolicy> pow(const decimal<Precision, RoundPolicy>& base, const N& exp) noexcept
		{
			return details::pow2(decimal<Precision, RoundPolicy>{1}, base, exp);
		}

		template<std::size_t Precision, class RoundPolicy>
		[[nodiscard]] constexpr decimal<Precision, RoundPolicy> pow(const decimal<Precision, RoundPolicy>& base, const decimal<Precision, RoundPolicy>& exp)
		{
			return decimal<Precision, RoundPolicy>{std::pow(static_cast<double>(base), static_cast<double>(exp))};
		}
	}
}