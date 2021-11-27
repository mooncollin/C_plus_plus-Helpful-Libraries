export module cmoon.execution.sync_wait;

import <utility>;
import <type_traits>;
import <exception>;
import <functional>;
import <optional>;
import <atomic>;
import <tuple>;

import cmoon.meta;
import cmoon.functional;

import cmoon.execution.receiver;
import cmoon.execution.sender;
import cmoon.execution.sender_traits;
import cmoon.execution.typed_sender;
import cmoon.execution.connect;
import cmoon.execution.start;
import cmoon.execution.into_variant;
import cmoon.execution.set_value;
import cmoon.execution.set_error;
import cmoon.execution.set_done;
import cmoon.execution.get_completion_scheduler;

namespace cmoon::execution
{
	template<class... Ts>
		requires(sizeof...(Ts) == 1)
	using one = void;

	template<typed_sender S>
	using sync_wait_type = std::optional<value_types_of_t<S, std::tuple, std::type_identity_t>>;

	template<typed_sender S>
	using sync_wait_error_type = std::optional<error_types_of_t<S>>;

	template<typed_sender S>
	using sync_wait_with_variant_type = std::optional<value_types_of_t<S>>;

	template<typed_sender S>
	struct sync_wait_receiver
	{
		constexpr sync_wait_receiver(std::atomic_flag& a,
									 sync_wait_type<S>& v,
									 sync_wait_error_type<S>& e) noexcept
			: a_{a}, value{v}, error{e} {}

		template<class... Args>
		friend void tag_invoke(set_value_t, sync_wait_receiver&& r, Args&&... args)
		{
			r.value.get() = std::make_tuple(std::forward<Args>(args)...);
			r.a_.get().test_and_set();
			r.a_.get().notify_one();
		}

		template<class E>
		friend void tag_invoke(set_error_t, sync_wait_receiver&& r, E&& e) noexcept
		{
			r.error.get() = std::forward<E>(e);
			r.a_.get().test_and_set();
			r.a_.get().notify_one();
		}

		friend void tag_invoke(set_done_t, sync_wait_receiver&& r) noexcept
		{
			r.a_.get().test_and_set();
			r.a_.get().notify_one();
		}

		std::reference_wrapper<std::atomic_flag> a_;
		std::reference_wrapper<sync_wait_type<S>> value;
		std::reference_wrapper<sync_wait_error_type<S>> error;
	};

	struct error_throw_fn
	{
		[[noreturn]] void operator()(std::exception_ptr e) const
		{
			std::rethrow_exception(std::move(e));
		}

		template<class E>
		[[noreturn]] void operator()(E&& e) const
		{
			throw std::forward<E>(e);
		}
	};

	export
	struct sync_wait_t
	{
		private:
			enum class state { completion_scheduler_fn, tag_invoke_fn, other };

			template<class S>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(sync_wait_t t, S&& s) {
					{ tag_invoke(t, get_completion_scheduler<set_value_t>(s), std::forward<S>(s)) } -> std::same_as<sync_wait_type<S>>;
				})
				{
					using cs = std::invoke_result_t<get_completion_scheduler_t<set_value_t>, S>;
					return {state::completion_scheduler_fn, cmoon::nothrow_tag_invocable<sync_wait_t, cs, S>};
				}
				else if constexpr (requires(sync_wait_t t, S&& s) {
					{ tag_invoke(t, std::forward<S>(s)) } -> std::same_as<sync_wait_type<S>>;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<sync_wait_t, S>};
				}
				else
				{
					return {state::other};
				}
			}
		public:
			template<typed_sender S>
				requires(requires {
					typename value_types_of_t<S, std::tuple, one>;
				})
			constexpr sync_wait_type<S> operator()(S&& s) const noexcept(choose<S>().no_throw)
			{
				constexpr auto choice {choose<S>()};

				if constexpr (choice.strategy == state::completion_scheduler_fn)
				{
					return tag_invoke(*this, get_completion_scheduler<set_value_t>(s), std::forward<S>(s));
				}
				else if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					return tag_invoke(*this, std::forward<S>(s));
				}
				else if constexpr (choice.strategy == state::other)
				{
					std::atomic_flag sync;
					sync_wait_type<S> results;
					sync_wait_error_type<S> err;

					cmoon::execution::start(
						cmoon::execution::connect(
							std::forward<S>(s),
							sync_wait_receiver<S>{sync, results, err}
						)
					);

					sync.wait(false);
					if (results)
					{
						return std::move(results.value());
					}

					if (err)
					{
						std::visit(error_throw_fn{}, std::move(err.value()));
					}

					return {};
				}
			}
	};

	export
	inline constexpr sync_wait_t sync_wait{};

	export
	struct sync_wait_with_variant_t
	{
		private:
			enum class state { completion_scheduler_fn, tag_invoke_fn, other };

			template<class S>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(sync_wait_with_variant_t t, S&& s) {
					{ tag_invoke(t, get_completion_scheduler<set_value_t>(s), std::forward<S>(s)) } -> std::same_as<sync_wait_with_variant_type<S>>;
				})
				{
					using cs = std::invoke_result_t<get_completion_scheduler_t<set_value_t>, S>;
					return {state::completion_scheduler_fn, cmoon::nothrow_tag_invocable<sync_wait_with_variant_t, cs, S>};
				}
				else if constexpr (requires(sync_wait_with_variant_t t, S&& s) {
					{ tag_invoke(t, std::forward<S>(s)) } -> std::same_as<sync_wait_with_variant_type<S>>;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<sync_wait_with_variant_t, S>};
				}
				else
				{
					return {state::other};
				}
			}
		public:
			template<typed_sender S>
			constexpr sync_wait_with_variant_type<S> operator()(S&& s) const noexcept(choose<S>().no_throw)
			{
				constexpr auto choice {choose<S>()};

				if constexpr (choice.strategy == state::completion_scheduler_fn)
				{
					return tag_invoke(*this, get_completion_scheduler<set_value_t>(s), std::forward<S>(s));
				}
				else if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					return tag_invoke(*this, std::forward<S>(s));
				}
				else if constexpr (choice.strategy == state::other)
				{
					return sync_wait(into_variant(std::forward<S>(s)));
				}
			}
	};

	export
	inline constexpr sync_wait_with_variant_t sync_wait_with_variant{};
}