export module cmoon.execution.submit;

import <utility>;
import <type_traits>;

import cmoon.meta;

import cmoon.execution.receiver;
import cmoon.execution.sender;
import cmoon.execution.sender_to;
import cmoon.execution.start;
import cmoon.execution.connect;
import cmoon.execution.set_value;
import cmoon.execution.set_error;
import cmoon.execution.set_done;

namespace cmoon::execution
{
	template<class S, class R>
	struct submit_receiver
	{
		struct wrap
		{
			submit_receiver* p_;
			
			template<class... As>
				requires(receiver_of<R, As...>)
			void set_value(As&&... as) && noexcept(is_nothrow_receiver_of_v<R, As...>)
			{
				execution::set_value(std::move(p_->r_), std::forward<As>(as)...);
				delete p_;
			}

			template<class E>
				requires(receiver<R, E>)
			void set_error(E&& e) && noexcept
			{
				execution::set_error(std::move(p_->r_), std::forward<E>(e));
				delete p_;
			}

			void set_done() && noexcept
			{
				execution::set_done(std::move(p_->r_));
				delete p_;
			}
		};

		std::remove_cvref_t<R> r_;
		connect_result_t<S, wrap> state_;

		submit_receiver(S&& s, R&& r)
			: r_{std::forward<R>(r)},
			  state_{execution::connect(std::forward<S>(s), wrap{this})} {}
	};

	namespace submit_ns
	{
		void submit();

		class submit_t
		{
			private:
				enum class state { member_fn, default_fn, start_fn };

				template<class S, class R>
				static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (sender<S> && requires(S&& s, R&& r) {
						s.submit(std::forward<R>(r));
					})
					{
						return {state::member_fn, noexcept(std::declval<S>().submit(std::declval<R>()))};
					}
					else if constexpr (sender<S> && requires(S&& s, R&& r) {
						submit(std::forward<S>(s), std::forward<R>(r));
					})
					{
						return {state::default_fn, noexcept(submit(std::declval<S>(), std::declval<R>()))};
					}
					else
					{
						return {state::start_fn};
					}
				}
			public:
				template<class S, class R>
					requires(sender_to<S, R>)
				constexpr void operator()(S&& s, R&& r) const noexcept(choose<S, R>().no_throw)
				{
					constexpr auto choice {choose<S, R>()};

					if constexpr (choice.strategy == state::member_fn)
					{
						std::forward<S>(s).submit(std::forward<R>(r));
					}
					else if constexpr (choice.strategy == state::default_fn)
					{
						submit(std::forward<S>(s), std::forward<R>(r));
					}
					else
					{
						execution::start((new submit_receiver<S, R>{std::forward<S>(s), std::forward<R>(r)})->state_);
					}
				}
		};
	}

	export
	inline constexpr submit_ns::submit_t submit{};
}