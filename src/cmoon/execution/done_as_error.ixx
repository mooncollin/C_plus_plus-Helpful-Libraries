export module cmoon.execution.done_as_error;

import <utility>;
import <concepts>;
import <type_traits>;

import cmoon.execution.just;
import cmoon.execution.then;
import cmoon.execution.let_done;
import cmoon.execution.sender;
import cmoon.execution.just_error;

namespace cmoon::execution
{
	template<std::move_constructible Error>
	struct done_as_error_adapter
	{
		public:
			done_as_error_adapter(Error e)
				: e{std::move(e)} {}

			template<sender S>
			constexpr friend auto operator|(S&& s, done_as_error_adapter&& a)
			{
				return done_as_error(std::forward<S>(s), std::move(a.e));
			}
		private:
			Error e;
	};

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
			return done_as_error_adapter<Error>{std::move(err)};
		}
	};

	export
	inline constexpr done_as_error_t done_as_error{};
}