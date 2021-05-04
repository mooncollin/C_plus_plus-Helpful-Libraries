export module cmoon.execution.set_value;

import <utility>;

import cmoon.meta;

namespace cmoon::execution
{
	namespace set_value_cpo
	{
		void set_value();

		template<class R, class... Args>
		concept has_adl =
			requires(R&& r, Args&&... args)
		{
			set_value(std::forward<R>(r), std::forward<Args>(args)...);
		};

		template<class R, class... Args>
		concept can_member_call =
			requires(R&& r, Args&&... args)
		{
			std::forward<R>(r).set_value(std::forward<Args>(args)...);
		};

		struct cpo
		{
			private:
				enum class state { none, member_call, non_member_call };

				template<class R, class... Args>
				[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (can_member_call<R, Args...>)
					{
						return {state::member_call, noexcept((std::declval<R>()).set_value(std::declval<Args>()...))};
					}
					else if constexpr (has_adl<R, Args...>)
					{
						return {state::non_member_call, noexcept(set_value(std::declval<R>(), std::declval<Args>()...))};
					}
					else
					{
						return {state::none};
					}
				}

				template<class R, class... Args>
				static constexpr auto choice = choose<R, Args...>();
			public:
				template<class R, class... Args>
					requires(choice<R, Args...>.strategy != state::none)
				constexpr decltype(auto) operator()(R&& r, Args&&... args) const noexcept(choice<R, Args...>.no_throw)
				{
					if constexpr (choice<R, Args...>.strategy == state::member_call)
					{
						return std::forward<R>(r).set_value(std::forward<Args>(args)...);
					}
					else if constexpr (choice<R, Args...>.strategy == state::non_member_call)
					{
						return set_value(std::forward<R>(r), std::forward<Args>(args)...);
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
		inline constexpr set_value_cpo::cpo set_value {};
	}

	using namespace cpos;
}