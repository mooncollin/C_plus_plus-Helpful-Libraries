export module cmoon.finance.securities;

import <cstddef>;

import cmoon.finance.decimal;

namespace cmoon::finance
{
	export
	template<std::size_t Precision>
	[[nodiscard]] constexpr auto dividend_yield(const decimal<Precision>& stock_price, const decimal<Precision>& annual_dividend) noexcept
	{
		return annual_dividend / stock_price;
	}
}