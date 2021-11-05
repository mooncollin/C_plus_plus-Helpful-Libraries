export module cmoon.execution.scheduler;

import <concepts>;
import <type_traits>;

import cmoon.execution.schedule;

namespace cmoon::execution
{
	export
	template<class S>
	concept scheduler = std::copy_constructible<std::remove_cvref_t<S>> &&
						std::equality_comparable<std::remove_cvref_t<S>> &&
		requires(S&& s)
	{
		execution::schedule(std::forward<S>(s));
	};
}