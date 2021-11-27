export module cmoon.execution.impl.is_awaitable;

import <coroutine>;

namespace cmoon::execution
{
	export
	template<class T>
	concept is_awaitable = 
		requires
	{
		typename std::coroutine_traits<T>::promise_type;
	} &&
		!requires(typename std::coroutine_traits<T>::promise_type p)
	{
		p.await_transform();
	} &&
		requires(T t)
	{
		co_await t;
	};

	export
	template<is_awaitable T>
	using await_result_type = decltype(co_await std::declval<T>());
}