export module cmoon.execution.then;

import <type_traits>;
import <functional>;
import <utility>;

import cmoon.execution.receiver;
import cmoon.execution.set_value;
import cmoon.execution.set_error;
import cmoon.execution.set_done;
import cmoon.execution.sender;
import cmoon.execution.sender_to;
import cmoon.execution.sender_base;
import cmoon.execution.connect;
import cmoon.execution.operation_state;

namespace cmoon::execution
{
	template<receiver R, class F>
	struct then_receiver
	{
		public:
			constexpr then_receiver(R&& r, F&& f)
				: r_{std::forward<R>(r)}, f_{std::forward<F>(f)} {}

			template<class... As, class Ret = std::invoke_result_t<F, As...>>
				requires(receiver_of<R, Ret>)
			constexpr void set_value(As&&... as) && noexcept(std::is_nothrow_invocable_v<F, As...> && is_nothrow_receiver_of_v<R, Ret>)
			{
				execution::set_value(std::move(r_), std::invoke(std::move(f_), std::forward<As>(as)...));
			}

			template<class... As, class Ret = std::invoke_result_t<F, As...>>
				requires(std::is_void_v<Ret> && receiver_of<R>)
			constexpr void set_value(As&&... as) && noexcept(std::is_nothrow_invocable_v<F, As...> && is_nothrow_receiver_of_v<R>)
			{
				std::invoke(std::move(f_), std::forward<As>(as)...);
				execution::set_value(std::move(r_));
			}

			template<class E>
				requires(receiver<R, E>)
			constexpr void set_error(E&& e) && noexcept
			{
				execution::set_error(std::move(r_), std::forward<E>(e));
			}

			constexpr void set_done() && noexcept
			{
				execution::set_done(std::move(r_));
			}
		private:
			R r_;
			F f_;
	};

	template<receiver R, class F>
	then_receiver(R&&, F&&) -> then_receiver<R, F>;

	template<sender S, class F>
	struct then_sender : public sender_base
	{
		public:
			constexpr then_sender(S&& s, F&& f)
				: s_{std::forward<S>(s)}, f_{std::forward<F>(f)} {}

			template<receiver R>
				requires(sender_to<S, then_receiver<R, F>>)
			constexpr operation_state auto connect(R&& r) &&
			{
				return execution::connect(std::move(s_), then_receiver{std::forward<R>(r), std::move(f_)});
			}
		private:
			S s_;
			F f_;
	};

	template<sender S, class F>
	then_sender(S&&, F&&) -> then_sender<S, F>;

	export
	template<sender S, class F>
	sender auto then(S&& s, F&& f)
	{
		return then_sender{std::forward<S>(s), std::forward<F>(f)};
	}

	template<class F>
	struct then_adapter
	{
		public:
			constexpr then_adapter(F&& f)
				: f_{std::forward<F>(f)} {}

			template<sender S>
			constexpr friend auto operator|(S&& s, then_adapter&& a)
			{
				return then(std::forward<S>(s), std::move(a.f_));
			}
		private:
			F f_;
	};

	template<class F>
	then_adapter(F&&) -> then_adapter<F>;

	export
	template<class F>
	auto then(F&& f)
	{
		return then_adapter{std::forward<F>(f)};
	}
}