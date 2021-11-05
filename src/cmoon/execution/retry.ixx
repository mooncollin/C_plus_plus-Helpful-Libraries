export module cmoon.execution.retry;

import <concepts>;
import <utility>;
import <type_traits>;
import <optional>;
import <functional>;
import <exception>;

import cmoon.execution.receiver;
import cmoon.execution.sender;
import cmoon.execution.sender_base;
import cmoon.execution.sender_to;
import cmoon.execution.connect;
import cmoon.execution.start;
import cmoon.execution.set_value;
import cmoon.execution.set_done;
import cmoon.execution.set_error;
import cmoon.execution.operation_state;

namespace cmoon::execution
{
	template<class O, class R>
	struct retry_receiver
	{
		public:
			explicit constexpr retry_receiver(O& o)
				: o_{o} {}

			template<class... As>
				requires(receiver_of<R, As...>)
			constexpr void set_value(As&&... as) && noexcept(is_nothrow_receiver_of<R, As...>)
			{
				execution::set_value(std::move(o_.get().r_), std::forward<As>(as)...);
			}

			constexpr void set_error(auto&&) && noexcept
			{
				o_.get().retry();
			}

			constexpr void set_done() && noexcept
			{
				execution::set_done(std::move(o_.get().r_));
			}
		private:
			std::reference_wrapper<O> o_;
	};

	template<sender S>
	struct retry_sender : public sender_base
	{
		private:
			S s_;

			template<class R>
			struct op
			{
				public:
					constexpr op(S&& s, R&& r)
						: s_{std::forward<S>(s)}, r_{std::forward<R>(r)}, o_{execution::connect(s_, retry_receiver<op, R>{*this})} {}

					op(op&&) = delete;

					void start() && noexcept
					{
						execution::start(std::move(*o_));
					}
					std::optional<connect_result_t<S, retry_receiver<op, R>>> o_;
				private:
					S s_;
					R r_;

					void retry() noexcept try
					{
						o_.emplace(execution::connect(s_, retry_receiver<op, R>{*this}));
						execution::start(std::move(*o_));
					}
					catch (...)
					{
						execution::set_error(std::move(r_), std::current_exception());
					}

					friend struct retry_receiver<op, R>;
			};
		public:
			constexpr explicit retry_sender(S&& s)
				: s_{std::forward<S>(s)} {}

			template<receiver R>
				// MSVC cannot compile this properly
				//requires(sender_to<S&, retry_receiver<op<R>, R>>)
			operation_state auto connect(R&& r) &&
			{
				return op<R>{std::move(s_), std::forward<R>(r)};
			}
	};

	template<class S>
	retry_sender(S&&) -> retry_sender<S>;

	export
	template<sender S>
	sender auto retry(S&& s)
	{
		return retry_sender{std::forward<S>(s)};
	}

	struct retry_adapter
	{
		template<sender S>
		constexpr friend auto operator|(S&& s, retry_adapter&&)
		{
			return retry(std::forward<S>(s));
		}
	};

	export
	auto retry()
	{
		return retry_adapter{};
	}
}