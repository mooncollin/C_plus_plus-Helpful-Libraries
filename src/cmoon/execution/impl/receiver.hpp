#pragma once

#include <exception>
#include <type_traits>

#include "cmoon/execution/impl/set_value.hpp"
#include "cmoon/execution/impl/set_error.hpp"
#include "cmoon/execution/impl/set_done.hpp"

namespace cmoon
{
	namespace execution
	{
		template<class T, class E = std::exception_ptr>
		concept receiver = std::move_constructible<std::remove_cvref_t<T>> &&
						   (std::is_nothrow_move_constructible_v<T> || std::copy_constructible<T>) &&
			requires(T&& r, E&& e)
		{
			execution::set_error(std::forward<T>(r), std::forward<E>(e));
			execution::set_done(std::forward<T>(r));
		};

		template<class T, class... Args>
		concept receiver_of = receiver<T> &&
			requires(T&& r, Args&&... args)
		{
			execution::set_value(std::forward<T>(r), std::forward<Args>(args)...);
		};

		template<class R, class... Args>
		inline constexpr bool is_nothrow_receiver_of_v = receiver_of<R, Args...> &&
														 std::is_nothrow_invocable_v<decltype(set_value), R, Args...>;
	}
}