export module cmooon.execution.submit;

import <utility>;

import cmoon.meta;

import cmoon.execution.set_error;
import cmoon.execution.set_value;
import cmoon.execution.set_done;
import cmoon.execution.start;
import cmoon.execution.receiver;
import cmoon.execution.connect;

namespace cmoon::execution
{
	namespace submit_cpo
	{
		template<class S, class R>
		struct submit_state
		{
			struct submit_receiver
			{
				submit_state* p_;

				template<class... Args>
					requires(receiver_of<R, Args...>)
				void set_value(Args&&... args) noexcept(is_nothrow_receiver_of_v<R, Args...>)
				{
					execution::set_value(std::move(p_->r_), std::forward<Args>(args)...);
					delete p_;
				}

				template<class E>
					requires(receiver<R, E>)
				void set_error(E&& e) noexcept
				{
					execution::set_error(std::move(p_->r_), std::forward<E>(e));
					delete p_;
				}

				void set_done() noexcept
				{
					execution::set_done(std::move(p_->r_));
					delete p_;
				}
			};

			std::remove_cvref_t<R> r_;
			connect_result_t<S, submit_receiver> state_;

			submit_state(S&& s, R&& r)
				: r_{std::forward<R>(R)}, state_{execution::connect(std::forward<S>(s), submit_receiver{this})} {}
		};

		void submit();

		template<class S, class R>
		concept has_adl = receiver<R> &&
			requires(S&& s, R&& r)
		{
			submit(std::forward<S>(s), std::forward<R>(r));
		};

		template<class S, class R>
		concept can_member_call = receiver<R> &&
			requires(S&& s, R&& r)
		{
			std::forward<S>(s).submit(std::forward<R>(r));
		};

		template<class S, class R>
		concept can_submit_state_call = receiver<R> &&
			requires(S&& s, R&& r)
		{
			execution::start((new submit_state<S, R>{std::forward<S>(s), std::forward<R>(r)})->state_);
		};

		struct cpo
		{
			private:
				enum class state { none, member_call, non_member_call, submit_state_call };

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
					else if constexpr (can_submit_state_call<S, R>)
					{
						return {state::submit_state_call};
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
				void operator()(S&& s, R&& r) const noexcept(choice<S, R>.no_throw)
				{
					if constexpr (choice<S, R>.strategy == state::member_call)
					{
						return std::forward<S>(s).submit(std::forward<R>(r));
					}
					else if constexpr (choice<S, R>.strategy == state::non_member_call)
					{
						return submit(std::forward<S>(s), std::forward<R>(r));
					}
					else if constexpr (choice<S, R>.strategy == state::submit_state_call)
					{
						return execution::start((new submit_state<S, R>{std::forward<S>(s), std::forward<R>(r)})->state_);
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
		inline constexpr submit_cpo::cpo submit {};
	}

	using namespace cpos;
}