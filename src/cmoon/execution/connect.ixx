export module cmoon.execution.connect;

import <utility>;
import <type_traits>;

import cmoon.meta;

import cmoon.execution.set_error;
import cmoon.execution.set_value;
import cmoon.execution.set_done;
import cmoon.execution.receiver;
import cmoon.execution.as_invocable;

namespace cmoon::execution
{
	namespace execute_cpo
	{
		struct cpo;
	}

	namespace connect_cpo
	{
		template<class S, class R>
		struct as_operation
		{
			std::remove_cvref_t<S> e_;
			std::remove_cvref_t<R> r_;

			void start() noexcept try
			{
				execute_cpo::cpo{}(std::move(e_), as_invocable<std::remove_cvref_t<R>, S>{r_});
			}
			catch (...)
			{
				execution::set_error(std::move(r_), std::current_exception());
			}
		};

		void connect();

		template<class S, class R>
		concept has_adl = receiver<R> &&
			requires(S&& s, R&& r)
		{
			connect(std::forward<S>(s), std::forward<R>(r));
		};

		template<class S, class R>
		concept can_member_call = receiver<R> &&
			requires(S&& s, R&& r)
		{
			std::forward<S>(s).connect(std::forward<R>(r));
		};

		template<class S, class R>
		concept can_as_operation = receiver<R> &&
			requires(S&& s, R&& r)
		{
			as_operation{std::forward<S>(s), std::forward<R>(r)};
		};

		struct cpo
		{
			private:
				enum class state { none, member_call, non_member_call, as_operation_call };

				template<class S, class R>
				[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (can_member_call<S, R>)
					{
						return {state::member_call, noexcept((std::declval<S>()).connect(std::declval<R>()))};
					}
					else if constexpr (has_adl<S, R>)
					{
						return {state::non_member_call, noexcept(connect(std::declval<S>(), std::declval<R>()))};
					}
					else if constexpr (can_as_operation<S, R>)
					{
						return {state::as_operation_call, noexcept(as_operation{std::declval<S>(), std::declval<R>()})};
					}
					else
					{
						return {state::none};
					}
				}

				template<class S, class R>
				static constexpr auto choice = choose<S, R>();
			public:
				template<class S, class R>
					requires(choice<S, R>.strategy != state::none)
				constexpr decltype(auto) operator()(S&& s, R&& r) const noexcept(choice<S, R>.no_throw)
				{
					if constexpr (choice<S, R>.strategy == state::member_call)
					{
						return std::forward<S>(s).connect(std::forward<R>(r));
					}
					else if constexpr (choice<S, R>.strategy == state::non_member_call)
					{
						return connect(std::forward<S>(s), std::forward<R>(r));
					}
					else if constexpr (choice<S, R>.strategy == state::as_operation_call)
					{
						return as_operation{std::forward<S>(s), std::forward<R>(r)};
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
		inline constexpr connect_cpo::cpo connect{};
	}

	using namespace cpos;

	export
	template<class S, class R>
	using connect_result_t = std::invoke_result_t<decltype(connect), S, R>;
}