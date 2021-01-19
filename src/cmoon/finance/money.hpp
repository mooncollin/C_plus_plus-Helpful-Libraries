#pragma once

#include <cstdlib>
#include <stdexcept>

#include "cmoon/finance/decimal.hpp"
#include "cmoon/finance/currency.hpp"

namespace cmoon
{
	namespace finance
	{
		template<std::size_t DecimalPrecision, class RoundPolicy, std::size_t CurrencyCodeLength>
		class basic_money
		{
			public:
				constexpr basic_money() noexcept = default;
				constexpr basic_money(const basic_currency<CurrencyCodeLength>& currency) noexcept
					: currency_{currency} {}

				constexpr basic_money(const decimal<DecimalPrecision, RoundPolicy>& amount, const basic_currency<CurrencyCodeLength>& currency) noexcept
					: amount_{amount}, currency_{currency} {}

				[[nodiscard]] constexpr const decimal<DecimalPrecision, RoundPolicy>& amount() const noexcept
				{
					return amount_;
				}

				[[nodiscard]] constexpr const basic_currency<CurrencyCodeLength>& currency() const noexcept
				{
					return currency_;
				}

				[[nodiscard]] friend constexpr basic_money operator+(const basic_money& x, const basic_money& y)
				{
					if (x.currency() != y.currency())
					{
						throw std::invalid_argument{"Cannot add money of different currencies"};
					}

					return basic_money{x.amount() + y.amount(), x.currency()};
				}

				[[nodiscard]] friend constexpr basic_money operator-(const basic_money& x, const basic_money& y)
				{
					if (x.currency() != y.currency())
					{
						throw std::invalid_argument{"Cannot subtract money of different currencies"};
					}

					return basic_money{x.amount() - y.amount(), x.currency()};
				}

				template<class T>
					requires(requires(decimal<DecimalPrecision, RoundPolicy> d, T t) { d * t; })
				[[nodiscard]] friend constexpr basic_money operator*(const basic_money& x, const T& t)
				{
					return basic_money{x.amount() * t, x.currency()};
				}

				template<class T>
					requires(requires(decimal<DecimalPrecision, RoundPolicy> d, T t) { d / t; })
				[[nodiscard]] friend constexpr basic_money operator/(const basic_money& x, const T& t)
				{
					return basic_money{x.amount() / t, x.currency()};
				}

				constexpr basic_money& operator+=(const basic_money& m)
				{
					*this = *this + m;
					return *this;
				}

				constexpr basic_money& operator-=(const basic_money& m)
				{
					*this = *this - m;
					return *this;
				}

				template<class T>
				constexpr basic_money& operator*=(const T& t)
				{
					*this = *this * t;
					return *this;
				}

				template<class T>
				constexpr basic_money& operator/=(const T& t)
				{
					*this = *this / t;
					return *this;
				}
			private:
				decimal<DecimalPrecision, RoundPolicy> amount_ {};
				basic_currency<CurrencyCodeLength> currency_ {};
		};

		template<std::size_t DecimalPrecision, class RoundPolicy = default_round_policy>
		using money = basic_money<DecimalPrecision, RoundPolicy, standard_currency_code_length>;
	}
}