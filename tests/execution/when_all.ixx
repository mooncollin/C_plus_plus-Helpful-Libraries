export module cmoon.tests.execution.when_all;

import <exception>;

import cmoon.execution;
import cmoon.test;
import cmoon.meta;

namespace cmoon::tests::execution
{
	struct receiver_s
	{
		void set_value(int v1, int v2, int v3)
		{
			value = v1 + v2 + v3;
		}

		template<class E>
		void set_error(E&&) noexcept
		{
			error_set = true;
		}

		void set_done() noexcept
		{
			done_set = true;
		}

		int value {0};
		bool error_set{false};
		bool done_set{false};
	};

	template<cmoon::execution::typed_sender S>
	struct always_send_done
	{
		template<template<class...> class Tuple, template<class...> class Variant>
		using value_types = typename cmoon::execution::sender_traits<S>::template value_types<Tuple, Variant>;

		template<template<class...> class Variant>
		using error_types = Variant<>;

		static constexpr bool sends_done {true};

		template<class R>
		struct op
		{
			constexpr void start() && noexcept
			{
				r_.set_done();
			}

			R r_;
		};

		template<class R>
		constexpr auto connect(R&& r) &&
		{
			return op<R>{std::forward<R>(r)};
		}

		S s_;
	};

	template<cmoon::execution::typed_sender S>
	struct always_send_error
	{
		template<template<class...> class Tuple, template<class...> class Variant>
		using value_types = typename cmoon::execution::sender_traits<S>::template value_types<Tuple, Variant>;

		template<template<class...> class Variant>
		using error_types = Variant<std::exception>;

		static constexpr bool sends_done {false};

		template<class R>
		struct op
		{
			constexpr void start() && noexcept
			{
				r_.set_error(std::exception{});
			}

			R r_;
		};

		template<class R>
		constexpr auto connect(R&& r) &&
		{
			return op<R>{std::forward<R>(r)};
		}

		S s_;
	};

	export
	class when_all_value_test : public cmoon::test::test_case
	{
		public:
			when_all_value_test()
				: cmoon::test::test_case{"when_all_value_test"} {}

			void operator()() override
			{
				int value {0};
				receiver_s r;

				auto work = cmoon::execution::when_all(
					cmoon::execution::just(1),
					cmoon::execution::just(2),
					cmoon::execution::just(3)
				);

				cmoon::execution::start(cmoon::execution::connect(std::move(work), r));

				cmoon::test::assert_equal(r.value, 6);
				cmoon::test::assert_false(r.error_set);
				cmoon::test::assert_false(r.done_set);
			}
	};

	export
	class when_all_error_test : public cmoon::test::test_case
	{
		public:
			when_all_error_test()
				: cmoon::test::test_case{"when_all_error_test"} {}

			void operator()() override
			{
				receiver_s r;

				auto work = cmoon::execution::when_all(
					cmoon::execution::just(1),
					cmoon::execution::just(2),
					always_send_error{cmoon::execution::just(3)}
				);

				cmoon::execution::start(cmoon::execution::connect(std::move(work), r));

				cmoon::test::assert_equal(r.value, 0);
				cmoon::test::assert_true(r.error_set);
				cmoon::test::assert_false(r.done_set);
			}
	};

	export
	class when_all_done_test : public cmoon::test::test_case
	{
		template<cmoon::execution::typed_sender S>
		struct always_send_done
		{
			template<template<class...> class Tuple, template<class...> class Variant>
			using value_types = typename cmoon::execution::sender_traits<S>::template value_types<Tuple, Variant>;

			template<template<class...> class Variant>
			using error_types = Variant<>;

			static constexpr bool sends_done {true};

			template<class R>
			struct op
			{
				constexpr void start() && noexcept
				{
					r_.set_done();
				}

				R r_;
			};

			template<class R>
			constexpr auto connect(R&& r) &&
			{
				return op<R>{std::forward<R>(r)};
			}

			S s_;
		};

		public:
			when_all_done_test()
				: cmoon::test::test_case{"when_all_done_test"} {}

			void operator()() override
			{
				receiver_s r;

				auto work = cmoon::execution::when_all(
					cmoon::execution::just(1),
					cmoon::execution::just(2),
					always_send_done{cmoon::execution::just(3)}
				);

				cmoon::execution::start(cmoon::execution::connect(std::move(work), r));

				cmoon::test::assert_equal(r.value, 0);
				cmoon::test::assert_false(r.error_set);
				cmoon::test::assert_true(r.done_set);
			}
	};

	export
	class when_all_error_and_done_test : public cmoon::test::test_case
	{
		public:
			when_all_error_and_done_test()
				: cmoon::test::test_case{"when_all_error_and_done_test"} {}

			void operator()() override
			{
				receiver_s r;

				auto work = cmoon::execution::when_all(
					cmoon::execution::just(1),
					always_send_error{cmoon::execution::just(2)},
					always_send_done{cmoon::execution::just(3)}
				);

				cmoon::execution::start(cmoon::execution::connect(std::move(work), r));

				cmoon::test::assert_equal(r.value, 0);

				// Either done or error gets set, but
				// not both
				cmoon::test::assert_true(r.done_set != r.error_set);
			}
	};
}