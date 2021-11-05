export module cmoon.execution.set_error;

import <utility>;

import cmoon.meta;

namespace cmoon::execution
{
	namespace set_error_ns
	{
		void set_error();

		class set_error_t
		{
			private:
				enum class state { none, member_fn, default_fn };

				template<class R, class E>
				static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (requires(R&& r, E&& e) {
						std::forward<R>(r).set_error(std::forward<E>(e));
					})
					{
						return {state::member_fn, noexcept(std::declval<R>().set_error(std::declval<E>()))};
					}
					else if constexpr (requires(R&& r, E&& e) {
						set_error(std::forward<R>(r), std::forward<E>(e));
					})
					{
						return {state::default_fn, noexcept(set_error(std::declval<R>(), std::declval<E>()))};
					}
					else
					{
						return {state::none};
					}
				}
			public:
				template<class R, class E>
					requires(choose<R, E>().strategy != state::none)
				constexpr void operator()(R&& r, E&& e) const noexcept(choose<R, E>().no_throw)
				{
					constexpr auto choice {choose<R, E>()};

					if constexpr (choice.strategy == state::member_fn)
					{
						std::forward<R>(r).set_error(std::forward<E>(e));
					}
					else if constexpr (choice.strategy == state::default_fn)
					{
						set_error(std::forward<R>(r), std::forward<E>(e));
					}
				}
		};
	}

	export
	inline constexpr set_error_ns::set_error_t set_error{};
}