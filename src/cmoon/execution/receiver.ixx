export module cmoon.execution.receiver;

import <concepts>;
import <type_traits>;
import <utility>;
import <exception>;

import cmoon.execution.set_value;
import cmoon.execution.set_error;
import cmoon.execution.set_done;

namespace cmoon::execution
{
	export
	template<class T, class E = std::exception_ptr>
	concept receiver = std::move_constructible<std::remove_cvref_t<T>> &&
					   std::constructible_from<std::remove_cvref_t<T>, T> &&
		requires(std::remove_cvref_t<T>& t, E&& e)
	{
		{ execution::set_done(std::move(t)) } noexcept;
		{ execution::set_error(std::move(t), std::forward<E>(e)) } noexcept;
	};

	export
	template<class T, class... An>
	concept receiver_of = receiver<T> &&
		requires(std::remove_cvref_t<T>&& t, An&&... an)
	{
		execution::set_value(std::move(t), std::forward<An>(an)...);
	};

	export
	template<class R, class... An>
	inline constexpr bool is_nothrow_receiver_of_v = receiver_of<R, An...> &&
													 std::is_nothrow_invocable_v<decltype(set_value), R, An...>;
}