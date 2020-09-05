#pragma once

#include "collin/finance/decimal.hpp"
#include "collin/finance/currency.hpp"

namespace collin
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

				constexpr decimal<DecimalPrecision, RoundPolicy>& amount() noexcept
				{
					return amount_;
				}

				constexpr const decimal<DecimalPrecision, RoundPolicy>& amount() const noexcept
				{
					return amount_;
				}

				constexpr basic_currency<CurrencyCodeLength>& currency() noexcept
				{
					return currency_;
				}

				constexpr const basic_currency<CurrencyCodeLength>& currency() const noexcept
				{
					return currency_;
				}
			private:
				decimal<DecimalPrecision, RoundPolicy> amount_ {};
				basic_currency<CurrencyCodeLength> currency_ {};
		};

		template<std::size_t DecimalPrecision, class RoundPolicy = default_round_policy>
		using money = basic_money<DecimalPrecision, RoundPolicy, standard_currency_code_length>;
	}
}