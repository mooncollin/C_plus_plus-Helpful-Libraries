export module cmoon.execution.done_as_error;

import <utility>;
import <concepts>;
import <type_traits>;

import cmoon.execution.just;
import cmoon.execution.then;
import cmoon.execution.let_done;
import cmoon.execution.sender;
import cmoon.execution.just_error;
import cmoon.execution.sender_adapter;

namespace cmoon::execution
{
	export
	struct done_as_error_t
	{
		template<std::move_constructible Error, sender S>
		constexpr decltype(auto) operator()(S&& s, Error err = Error{}) const
		{
			return std::forward<S>(s) |
				   execution::let_done([err2 = std::move(err)] () mutable {
						return execution::just_error(std::move(err2));
				   });
		}

		template<std::move_constructible Error>
		constexpr auto operator()(Error err = Error{}) const noexcept
		{
			return sender_adapter<done_as_error_t, Error>{std::move(err)};
		}
	};

	export
	inline constexpr done_as_error_t done_as_error{};
}