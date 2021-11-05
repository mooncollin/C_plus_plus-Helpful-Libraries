export module cmoon.execution.set_value;

import <utility>;

import cmoon.meta;

namespace cmoon::execution
{
	namespace set_value_ns
	{
		void set_value();

		class set_value_t
		{
			private:
				enum class state { none, member_fn, default_fn };

				template<class R, class... Vs>
				static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (requires(R&& r, Vs&&... vs) {
						std::forward<R>(r).set_value(std::forward<Vs>(vs)...);
					})
					{
						return {state::member_fn, noexcept(std::declval<R>().set_value(std::declval<Vs>()...))};
					}
					else if constexpr (requires(R&& r, Vs&&... vs) {
						set_value(std::forward<R>(r), std::forward<Vs>(vs)...);
					})
					{
						return {state::default_fn, noexcept(set_value(std::declval<R>(), std::declval<Vs>()...))};
					}
					else
					{
						return {state::none};
					}
				}
			public:
				template<class R, class... Vs>
					requires(choose<R, Vs...>().strategy != state::none)
				constexpr void operator()(R&& r, Vs&&... vs) const noexcept(choose<R, Vs...>().no_throw)
				{
					constexpr auto choice {choose<R, Vs...>()};

					if constexpr (choice.strategy == state::member_fn)
					{
						std::forward<R>(r).set_value(std::forward<Vs>(vs)...);
					}
					else if constexpr (choice.strategy == state::default_fn)
					{
						set_value(std::forward<R>(r), std::forward<Vs>(vs)...);
					}
				}
		};
	}

	export
	inline constexpr set_value_ns::set_value_t set_value{};
}