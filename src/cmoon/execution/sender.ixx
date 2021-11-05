export module cmoon.execution.sender;

import <concepts>;
import <type_traits>;

import cmoon.execution.sender_traits;

namespace cmoon::execution
{
	export
	template<class S>
	concept sender = std::move_constructible<std::remove_cvref_t<S>> &&
		!requires
	{
		typename sender_traits<std::remove_cvref_t<S>>::unspecialized;
	};
}