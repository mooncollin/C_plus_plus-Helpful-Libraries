export module cmoon.execution.impl.executor_impl;

import <type_traits>;
import <concepts>;

namespace cmoon::execution
{
	export
	template<class E>
	concept executor_base_concept = std::copy_constructible<E> &&
									std::is_nothrow_copy_constructible_v<E> &&
									std::equality_comparable<E>;

	export
	template<class F>
	concept executor_fn_base_concept = std::invocable<std::remove_cvref_t<F>&> &&
									   std::constructible_from<std::remove_cvref_t<F>, F> &&
									   std::move_constructible<std::remove_cvref_t<F>>;
}