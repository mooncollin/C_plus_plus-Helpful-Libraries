export module cmoon.execution.set_done;

import <utility>;

import cmoon.meta;

namespace cmoon::execution
{
	namespace set_done_cpo
	{
		void set_done();

		template<class R>
		concept has_adl =
			requires(R&& r)
		{
			set_done(std::forward<R>(r));
		};

		template<class R>
		concept can_member_call =
			requires(R&& r)
		{
			std::forward<R>(r).set_done();
		};

		struct cpo
		{
			private:
				enum class state { none, member_call, non_member_call };

				template<class R>
				[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (can_member_call<R>)
					{
						return {state::member_call, noexcept((std::declval<R>()).set_done())};
					}
					else if constexpr (has_adl<R>)
					{
						return {state::non_member_call, noexcept(set_done(std::declval<R>()))};
					}
					else
					{
						return {state::none};
					}
				}

				template<class R>
				static constexpr auto choice = choose<R>();
			public:
				template<class R>
					requires(choice<R>.strategy != state::none)
				constexpr decltype(auto) operator()(R&& r) const noexcept(choice<R>.no_throw)
				{
					if constexpr (choice<R>.strategy == state::member_call)
					{
						return std::forward<R>(r).set_done();
					}
					else if constexpr (choice<R>.strategy == state::non_member_call)
					{
						return set_done(std::forward<R>(r));
					}
					else
					{
						static_assert(false, "should be unreachable");
					}
				}
		};
	}

	namespace cpos
	{
		export
		inline constexpr set_done_cpo::cpo set_done {};
	}

	using namespace cpos;
}