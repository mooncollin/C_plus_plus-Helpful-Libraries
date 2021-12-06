export module cmoon.execution.done_as_optional;

import <utility>;
import <optional>;
import <type_traits>;

import cmoon.execution.just;
import cmoon.execution.then;
import cmoon.execution.let_done;
import cmoon.execution.typed_sender;
import cmoon.execution.single_typed_sender;
import cmoon.execution.sender_adapter;

namespace cmoon::execution
{
	export
	struct done_as_optional_t
	{
		// MSVC will not compile using single_typed_sender as
		// the constraint. Errors about other unrelated
		// concepts already being defined. Most
		// definitely a compiler bug.
		template</*single_typed_sender*/ typed_sender S>
		constexpr decltype(auto) operator()(S&& s) const
		{
			using op_t = std::optional<std::decay_t<single_sender_value_type<S>>>;

			return std::forward<S>(s) |
				   execution::then([](auto&& t) {
					return op_t{
						t
					};
				   }) |
				   execution::let_done([]() noexcept {
					return execution::just(op_t{});
				   });
		}

		constexpr auto operator()() const noexcept
		{
			return sender_adapter<done_as_optional_t>{};
		}
	};

	export
	inline constexpr done_as_optional_t done_as_optional{};
}