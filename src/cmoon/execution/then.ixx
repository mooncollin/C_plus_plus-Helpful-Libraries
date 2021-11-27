export module cmoon.execution.then;

import <type_traits>;
import <functional>;
import <utility>;
import <exception>;

import cmoon.meta;
import cmoon.functional;

import cmoon.execution.receiver;
import cmoon.execution.set_value;
import cmoon.execution.set_error;
import cmoon.execution.set_done;
import cmoon.execution.sender;
import cmoon.execution.typed_sender;
import cmoon.execution.sender_to;
import cmoon.execution.sender_base;
import cmoon.execution.connect;
import cmoon.execution.operation_state;
import cmoon.execution.get_completion_scheduler;

namespace cmoon::execution
{
	template<receiver R, class F>
	struct then_receiver
	{
		public:
			constexpr then_receiver(R&& r, F&& f)
				: out_r{std::forward<R>(r)}, f_{std::forward<F>(f)} {}

			template<class... As, class Ret = std::invoke_result_t<F, As...>>
				requires(receiver_of<R, Ret>)
			constexpr friend void tag_invoke(set_value_t, then_receiver&& r, As&&... as) noexcept(std::is_nothrow_invocable_v<F, As...> && cmoon::nothrow_tag_invocable<set_value_t, R, Ret>)
			{
				try
				{
					execution::set_value(std::move(r.out_r), std::invoke(std::move(r.f_), std::forward<As>(as)...));
				}
				catch (...)
				{
					execution::set_error(std::move(r.out_r), std::current_exception());
				}
			}

			template<class... As, class Ret = std::invoke_result_t<F, As...>>
				requires(std::is_void_v<Ret> && receiver_of<R>)
			constexpr friend void tag_invoke(set_value_t, then_receiver&& r, As&&... as) noexcept(std::is_nothrow_invocable_v<F, As...> && cmoon::nothrow_tag_invocable<set_value_t, R>)
			{
				try
				{
					std::invoke(std::move(r.f_), std::forward<As>(as)...);
					execution::set_value(std::move(r.out_r));
				}
				catch (...)
				{
					execution::set_error(std::move(r.out_r), std::current_exception());
				}
			}

			template<class E>
				requires(receiver<R, E>)
			constexpr friend void tag_invoke(set_error_t, then_receiver&& r, E&& e) noexcept
			{
				execution::set_error(std::move(r.out_r), std::forward<E>(e));
			}

			constexpr friend void tag_invoke(set_done_t, then_receiver&& r) noexcept
			{
				execution::set_done(std::move(r.out_r));
			}
		private:
			R out_r;
			F f_;
	};

	template<class F>
	struct transform_apply
	{
		template<class Tuple>
		struct fn : std::type_identity<
			decltype(std::apply(std::declval<F>(), std::declval<Tuple>()))
		> {};
	};

	template<template<class...> class Tuple>
	struct transform_tuple
	{
		template<class T>
		struct fn : std::type_identity<
			Tuple<T>
		> {};
	};

	template<sender S, class F>
	struct then_sender_base : public sender_base {};


	// MSVC has an internal compiler error with this.
	// Which means no typed then senders :(
	/*template<typed_sender S, class F>
		// This disables anything that is Variant<>
		requires(requires {
			typename value_types_of_t<S, std::tuple, std::variant>;
		})
	struct then_sender_base<S, F>
	{
		// cmoon::meta::type_list<> Does not send at all - Not allowing these

		// cmoon::meta::type_list<tuple<>> Sends void
		//		-> std::apply(f, t)
		//		-> cmoon::meta::type_list<f()>
		//		-> transform(Tuple)
		//		-> cmoon::meta::type_list<Tuple<f()>>
		//		-> complete_type(Variant)
		//		-> Variant<Tuple<f()>>

		// cmoon::meta::type_list<tuple<int>> Sends a single int
		//		-> std::apply(f, t)
		//		-> cmoon::meta::type_list<f(int)>
		//		-> transform(Tuple)
		//		-> cmoon::meta::type_list<Tuple<f(int)>>
		//		-> complete_type(Variant)
		//		-> Variant<Tuple<f(int)>>

		// cmoon::meta::type_list<tuple<int, double>> Sends an int and a double
		//		-> std::apply(f, t)
		//		-> cmoon::meta::type_list<f(int, double)>
		//		-> transform(Tuple)
		//		-> cmoon::meta::type_list<Tuple<f(int, double)>>
		//		-> complete_type(Variant)
		//		-> Variant<Tuple<f(int, double)>>

		// cmoon::meta::type_list<tuple<int>, tuple<double>> Send either an int or a double
		//		-> std::apply(f, t)
		//		-> cmoon::meta::type_list<f(int), f(double)>
		//		-> transform(Tuple)
		//		-> cmoon::meta::type_list<Tuple<f(int)>>, Tuple<f(double)>>
		//		-> complete_type(Variant)
		//		-> Variant<Tuple<f(int)>, Tuple<f(double)>>

		private:
			using original_value_types = value_types_of_t<S, std::tuple, cmoon::meta::type_list>;
			using apply_transformation = typename original_value_types::template transform<transform_apply<F>::fn>;

			template<template<class...> class Tuple>
			using tuple_transformation = typename apply_transformation::template transform<transform_tuple<Tuple>::fn>;
		public:
			template<template<class...> class Tuple, template<class...> class Variant>
			using value_types = typename tuple_transformation<Tuple>::template complete_type<Variant>;

			template<template<class...> class Variant>
			using error_types = error_types_of_t<S, Variant>;

			static constexpr bool sends_done {true};
	};*/

	template<sender S, class F>
	struct then_sender : public then_sender_base<S, F>
	{
		public:
			constexpr then_sender(S&& s, F&& f)
				: s_{std::forward<S>(s)}, f_{std::forward<F>(f)} {}

			template<receiver R>
				requires(sender_to<S, then_receiver<R, F>>)
			constexpr friend auto tag_invoke(connect_t, then_sender&& s, R&& out_r)
			{
				return execution::connect(std::move(s.s_), then_receiver<R, F>{std::forward<R>(out_r), std::move(s.f_)});
			}
		private:
			S s_;
			F f_;
	};

	template<class F>
	struct then_adapter;

	export
	struct then_t
	{
		private:
			enum class state { completion_scheduler_fn, tag_invoke_fn, other };

			template<sender S, class F>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(then_t t, S&& s, F&& f) {
					{ tag_invoke(t, get_completion_scheduler<set_value_t>(s), std::forward<S>(s), std::forward<F>(f)) } -> sender;
				})
				{
					using completion_scheduler_type = std::invoke_result_t<get_completion_scheduler<set_value_t>, S>;
					return {state::completion_scheduler_fn, cmoon::nothrow_tag_invocable<then_t, completion_scheduler_type, S, F>};
				}
				else if constexpr (requires(then_t t, S&& s, F&& f) {
					{ tag_invoke(t, std::forward<S>(s), std::forward<F>(f)) } -> sender;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<then_t, S, F>};
				}
				else
				{
					return {state::other, std::is_nothrow_constructible_v<then_sender<S, F>, S, F>};
				}
			}
		public:
			template<sender S, class F>
			constexpr decltype(auto) operator()(S&& s, F&& f) const noexcept(choose<S, F>().no_throw)
			{
				constexpr auto choice {choose<S, F>()};

				if constexpr (choice.strategy == state::completion_scheduler_fn)
				{
					return tag_invoke(*this, get_completion_scheduler<set_value_t>(s), std::forward<S>(s), std::forward<F>(f));
				}
				else if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					return tag_invoke(*this, std::forward<S>(s), std::forward<F>(f));
				}
				else if constexpr (choice.strategy == state::other)
				{
					return then_sender<S, F>{std::forward<S>(s), std::forward<F>(f)};
				}
			}

			template<class F>
			constexpr auto operator()(F&& f) const
			{
				return then_adapter<F>{std::forward<F>(f)};
			}
	};

	export
	inline constexpr then_t then{};

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
}