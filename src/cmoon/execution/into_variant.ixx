export module cmoon.execution.into_variant;

import <utility>;
import <tuple>;

import cmoon.execution.typed_sender;
import cmoon.execution.connect;
import cmoon.execution.set_done;
import cmoon.execution.set_error;
import cmoon.execution.set_value;
import cmoon.execution.receiver;

namespace cmoon::execution
{
	template<typed_sender S, receiver R>
	class into_variant_receiver
	{
		public:
			constexpr into_variant_receiver(R&& r)
				: out_r{std::forward<R>(r)} {}

			template<class... Args>
			constexpr friend void tag_invoke(set_value_t, into_variant_receiver&& r, Args&&... args)
			{
				execution::set_value(std::move(r.out_r), value_types_of_t<S>{std::make_tuple(std::forward<Args>(args)...)});
			}

			template<class E>
			constexpr friend void tag_invoke(set_error_t, into_variant_receiver&& r, E&& e) noexcept
			{
				execution::set_error(std::move(r.out_r), std::forward<E>(e));
			}

			constexpr friend void tag_invoke(set_done_t, into_variant_receiver&& r) noexcept
			{
				execution::set_done(std::move(r.out_r));
			}
		private:
			R out_r;
	};

	template<typed_sender S>
	class into_variant_sender
	{
		public:
			template<template<class...> class Tuple, template<class...> class Variant>
			using value_types = Variant<Tuple<value_types_of_t<S>>>;

			template<template<class...> class Variant>
			using error_types = error_types_of_t<S, Variant>;

			static constexpr bool sends_done {true};

			constexpr into_variant_sender(S&& s)
				: s_{std::forward<S>(s)} {}

			template<receiver R>
			constexpr friend auto tag_invoke(connect_t, into_variant_sender&& s, R&& out_r)
			{
				return execution::connect(std::move(s.s_), into_variant_receiver<S, R>{std::forward<R>(out_r)});
			}
		private:
			S s_;
	};

	export
	template<typed_sender S>
	auto into_variant(S&& s)
	{
		return into_variant_sender<S>{std::forward<S>(s)};
	}
}