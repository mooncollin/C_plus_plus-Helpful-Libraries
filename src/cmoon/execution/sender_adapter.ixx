export module cmoon.execution.sender_adapter;

import <tuple>;
import <type_traits>;

import cmoon.concepts;
import cmoon.utility;

import cmoon.execution.sender;

namespace cmoon::execution
{
	export
	template<cmoon::class_type D, cmoon::movable_value... Args>
	struct sender_adapter
	{
		public:
			constexpr sender_adapter(Args&&... args)
				: args_{std::forward<Args>(args)...} {}

			sender_adapter(const sender_adapter&) = delete;
			sender_adapter& operator=(const sender_adapter&) = delete;

			template<sender S>
				requires(std::invocable<D, S, Args...>)
			constexpr friend decltype(auto) operator|(S&& s, sender_adapter&& a) noexcept(std::is_nothrow_invocable<D, S, Args...>)
			{
				return std::apply(
					[&s](auto&&... args)
					{
						return std::invoke(D{}, std::forward<S>(s), std::forward<decltype(args)>(args)...);
					}
				, std::move(a.args_));
			}
		private:
			std::tuple<Args...> args_;
	};
}