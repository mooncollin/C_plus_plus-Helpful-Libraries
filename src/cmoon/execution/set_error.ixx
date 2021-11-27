export module cmoon.execution.set_error;

import <utility>;

import cmoon.functional;

namespace cmoon::execution
{
	export
	struct set_error_t
	{
		template<class R, class E>
			requires(requires(set_error_t t, R&& r, E&& e) {
				tag_invoke(t, std::forward<R>(r), std::forward<E>(e));
			})
		constexpr void operator()(R&& r, E&& e) const noexcept(cmoon::nothrow_tag_invocable<set_error_t, R, E>)
		{
			tag_invoke(*this, std::forward<R>(r), std::forward<E>(e));
		}
	};

	export
	inline constexpr set_error_t set_error{};
}