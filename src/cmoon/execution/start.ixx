export module cmoon.execution.start;

import <utility>;

import cmoon.meta;

namespace cmoon::execution
{
	namespace start_ns
	{
		void start();

		class start_t
		{
			private:
				enum class state { none, member_fn, default_fn };

				template<class O>
				static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (requires(O&& o) {
						{ std::forward<O>(o).start() } noexcept;
					})
					{
						return {state::member_fn, true};
					}
					else if constexpr (requires(O&& o) {
						{ start(std::forward<O>(o)) } noexcept;
					})
					{
						return {state::default_fn, true};
					}
					else
					{
						return {state::none};
					}
				}
			public:
				template<class O>
					requires(choose<O>().strategy != state::none)
				constexpr void operator()(O&& o) const noexcept(choose<O>().no_throw)
				{
					constexpr auto choice {choose<O>()};

					if constexpr (choice.strategy == state::member_fn)
					{
						std::forward<O>(o).start();
					}
					else if constexpr (choice.strategy == state::default_fn)
					{
						start(std::forward<O>(o));
					}
				}
		};
	}

	export
	inline constexpr start_ns::start_t start{};
}