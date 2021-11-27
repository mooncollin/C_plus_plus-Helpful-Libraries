export module cmoon.execution.set_value;

import <utility>;

import cmoon.functional;

namespace cmoon::execution
{
	export
	struct set_value_t
	{
		template<class R, class... Vs>
			requires(requires(set_value_t t, R&& r, Vs&&... vs) {
				tag_invoke(t, std::forward<R>(r), std::forward<Vs>(vs)...);
			})
		constexpr void operator()(R&& r, Vs&&... vs) const noexcept(cmoon::nothrow_tag_invocable<set_value_t, R, Vs...>)
		{
			tag_invoke(*this, std::forward<R>(r), std::forward<Vs>(vs)...);
		}
	};

	export
	inline constexpr set_value_t set_value{};
}