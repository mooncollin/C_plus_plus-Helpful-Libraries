export module cmoon.tests.execution.sync_wait;

import <optional>;
import <type_traits>;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class sync_wait_value_test : public cmoon::test::test_case
	{
		public:
			sync_wait_value_test()
				: cmoon::test::test_case{"sync_wait_value_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				const auto [value] = cmoon::execution::sync_wait(cmoon::execution::just(expected)).value();
				cmoon::test::assert_equal(value, expected);
			}
	};

	export
	class sync_wait_error_test : public cmoon::test::test_case
	{
		struct sender_s
		{
			template<template<class...> class Tuple, template<class...> class Variant>
			using value_types = Variant<Tuple<int>>;

			template<template<class...> class Variant>
			using error_types = Variant<std::exception_ptr, std::exception>;

			static constexpr bool sends_done {false};

			template<class R>
			struct op
			{
				R r_;
				friend void tag_invoke(cmoon::execution::start_t, op& o) noexcept
				{
					cmoon::execution::set_error(std::move(o.r_), std::exception{});
				}
			};

			template<class R>
			friend auto tag_invoke(cmoon::execution::connect_t, sync_wait_error_test::sender_s&& s, R&& r)
			{
				return op<R>{std::forward<R>(r)};
			}
		};

		public:
			sync_wait_error_test()
				: cmoon::test::test_case{"sync_wait_error_test"} {}

			void operator()() override
			{
				auto fn = [] {
					cmoon::execution::sync_wait(sender_s{});
				};

				cmoon::test::assert_throws(std::exception{}, fn);
			}
	};

	export
	class sync_wait_done_test : public cmoon::test::test_case
	{
		struct sender_s
		{
			template<template<class...> class Tuple, template<class...> class Variant>
			using value_types = Variant<Tuple<int>>;

			template<template<class...> class Variant>
			using error_types = Variant<std::exception_ptr>;

			static constexpr bool sends_done {false};

			template<class R>
			struct op
			{
				R r_;
				friend void tag_invoke(cmoon::execution::start_t, op& o) noexcept
				{
					cmoon::execution::set_done(std::move(o.r_));
				}
			};

			template<class R>
			friend auto tag_invoke(cmoon::execution::connect_t, sync_wait_done_test::sender_s&& s, R&& r)
			{
				return op<R>{std::forward<R>(r)};
			}
		};

		public:
			sync_wait_done_test()
				: cmoon::test::test_case{"sync_wait_done_test"} {}

			void operator()() override
			{
				auto value = cmoon::execution::sync_wait(sender_s{});

				cmoon::test::assert_false(value.has_value());
			}
	};
}