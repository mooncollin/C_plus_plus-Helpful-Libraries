export module cmoon.execution.set_done;

import <utility>;

import cmoon.meta;

namespace cmoon::execution
{
	namespace set_done_ns
	{
		void set_done();

		class set_done_t
		{
			private:
				enum class state { none, member_fn, default_fn };

				template<class R, class... Vs>
				static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (requires(R&& r) {
						std::forward<R>(r).set_done();
					})
					{
						return {state::member_fn, noexcept(std::declval<R>().set_done())};
					}
					else if constexpr (requires(R&& r) {
						set_done(std::forward<R>(r));
					})
					{
						return {state::default_fn, noexcept(set_done(std::declval<R>()))};
					}
					else
					{
						return {state::none};
					}
				}
			public:
				template<class R>
					requires(choose<R>().strategy != state::none)
				constexpr void operator()(R&& r) const noexcept(choose<R>().no_throw)
				{
					constexpr auto choice {choose<R>()};

					if constexpr (choice.strategy == state::member_fn)
					{
						std::forward<R>(r).set_done();
					}
					else if constexpr (choice.strategy == state::default_fn)
					{
						set_done(std::forward<R>(r));
					}
				}
		};
	}

	export
	inline constexpr set_done_ns::set_done_t set_done{};
}