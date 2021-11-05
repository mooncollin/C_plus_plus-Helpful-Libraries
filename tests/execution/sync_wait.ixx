export module cmoon.tests.execution.sync_wait;

import <optional>;
import <type_traits>;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class sync_wait_member_fn_test : public cmoon::test::test_case
	{
		template<class Sender>
		struct sender_s
		{
			using value_type = int;

			template<template<class...> class, template<class...> class>
			using value_types = value_type;

			template<template<class...> class Variant>
			using error_types = Variant<>;

			static constexpr bool sends_done {false};

			sender_s(Sender&& j)
				: j_{std::forward<Sender>(j)} {}

			value_type sync_wait() &&
			{
				std::optional<value_type> v;
				auto r = cmoon::execution::as_receiver([&v](auto&& value) { v = std::forward<decltype(value)>(value); });
				cmoon::execution::start(cmoon::execution::connect(std::move(j_), std::move(r)));
				return v.value();
			}

			Sender j_;
		};

		public:
			sync_wait_member_fn_test()
				: cmoon::test::test_case {"sync_wait_member_fn_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				sender_s s{cmoon::execution::just(expected)};

				static_assert(cmoon::execution::typed_sender<decltype(s)>);

				const auto value = cmoon::execution::sync_wait(std::move(s));
				cmoon::test::assert_equal(value, expected);
			}
	};

	template<class Sender>
	struct default_sender_s : public cmoon::execution::sender_base
	{
		default_sender_s(Sender&& j)
			: j_{std::forward<Sender>(j)} {}

		Sender j_;
	};

	template<class Sender>
	int sync_wait(default_sender_s<Sender>&& s)
	{
		std::optional<int> v;
		auto r = cmoon::execution::as_receiver([&v](auto&& value) { v = std::forward<decltype(value)>(value); });
		cmoon::execution::start(cmoon::execution::connect(std::move(s.j_), std::move(r)));
		return v.value();
	}

	export
	class sync_wait_default_fn_test : public cmoon::test::test_case
	{
		public:
			sync_wait_default_fn_test()
				: cmoon::test::test_case{"sync_wait_default_fn_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				default_sender_s s{cmoon::execution::just(expected)};

				static_assert(cmoon::execution::sender<decltype(s)>);

				const auto value = cmoon::execution::sync_wait_r<long>(std::move(s));
				cmoon::test::assert_equal(value, expected);
			}
	};

	export
	class sync_wait_test : public cmoon::test::test_case
	{
		public:
			sync_wait_test()
				: cmoon::test::test_case{"sync_wait_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				const auto [value] = cmoon::execution::sync_wait(cmoon::execution::just(expected));
				cmoon::test::assert_equal(value, expected);
			}
	};
}