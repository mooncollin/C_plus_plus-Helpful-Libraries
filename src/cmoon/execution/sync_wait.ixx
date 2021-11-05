export module cmoon.execution.sync_wait;

import <utility>;
import <type_traits>;
import <exception>;
import <functional>;
import <optional>;
import <atomic>;
import <tuple>;
import <variant>;

import cmoon.meta;

import cmoon.execution.receiver;
import cmoon.execution.sender;
import cmoon.execution.sender_traits;
import cmoon.execution.typed_sender;
import cmoon.execution.connect;
import cmoon.execution.start;

namespace cmoon::execution
{
	enum class sync_wait_result { none, value, error, done };

	template<class ValueType>
	struct sync_wait_receiver
	{
		constexpr sync_wait_receiver(std::atomic<sync_wait_result>& a) noexcept
			: a_{a} {}

		constexpr void set_value(ValueType&& v)
		{
			value = std::forward<ValueType>(v);
			a_ = sync_wait_result::value;
			a_.notify_one();
		}

		template<class E>
		constexpr void set_error(E&& e) noexcept
		{
			error = [e = std::forward<E>(e)] {
				if constexpr (std::same_as<std::remove_cvref_t<E>, std::exception_ptr>)
				{
					std::rethrow_exception(e);
				}
				else
				{
					throw e;
				}
			};

			a_ = sync_wait_result::error;
			a_.notify_one();
		}

		constexpr void set_done() noexcept
		{
			a_ = sync_wait_result::done;
			a_.notify_one();
		}

		std::atomic<sync_wait_result>& a_;
		std::function<void()> error;
		std::optional<std::remove_cvref_t<ValueType>> value;
	};

	namespace sync_wait_ns
	{
		void sync_wait();

		class sync_wait_t
		{
			private:
				enum class state { member_fn, default_fn, sync_fn };

				template<sender S>
				[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (requires(S&& s) {
						std::forward<S>(s).sync_wait();
					})
					{
						return {state::member_fn, noexcept(std::declval<S>().sync_wait())};
					}
					else if constexpr (requires(S&& s) {
						sync_wait(std::forward<S>(s));
					})
					{
						return {state::default_fn, noexcept(sync_wait(std::declval<S>()))};
					}
					else
					{
						return {state::sync_fn, false};
					}
				}
			public:
				template<class ValueType, sender S>
				constexpr ValueType operator()(S&& s) const noexcept(choose<S>().no_throw)
				{
					constexpr auto choice {choose<S>()};

					if constexpr (choice.strategy == state::member_fn)
					{
						return std::forward<S>(s).sync_wait();
					}
					else if constexpr (choice.strategy == state::default_fn)
					{
						return sync_wait(std::forward<S>(s));
					}
					else if constexpr (choice.strategy == state::sync_fn)
					{
						std::atomic<sync_wait_result> sync {sync_wait_result::none};
						sync_wait_receiver<ValueType> r{sync};

						cmoon::execution::start(cmoon::execution::connect(std::forward<S>(s), r));

						sync.wait(sync_wait_result::none);
						switch (sync.load(std::memory_order::relaxed))
						{
							case sync_wait_result::error:
								std::invoke(r.error);
								break;
							case sync_wait_result::done:
								std::terminate();
								break;
						}

						return std::move(r.value.value());
					}
				}

				template<typed_sender S>
				constexpr typename sender_traits<S>::template value_types<std::tuple, std::variant> operator()(S&& s) const noexcept(choose<S>().no_throw)
				{
					return (*this).operator()<sender_traits<S>::template value_types<std::tuple, std::variant>>(std::forward<S>(s));
				}
		};
	}

	export
	inline constexpr sync_wait_ns::sync_wait_t sync_wait {};

	export
	template<class ValueType, sender S>
	constexpr auto sync_wait_r(S&& s)
	{
		return sync_wait.operator()<ValueType>(std::forward<S>(s));
	}
}