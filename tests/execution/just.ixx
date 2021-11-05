export module cmoon.tests.execution.just;

import <exception>;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class just_set_value_test : public cmoon::test::test_case
	{
		struct just_r
		{
			template<class... Ts>
			void set_value(Ts&&...)
			{
				value_set = true;
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

			bool value_set {false};
			bool error_set {false};
			bool done_set {false};
		};

		static_assert(cmoon::execution::receiver<just_r>, "just_r must be a receiver for this test to work");

		public:
			just_set_value_test()
				: cmoon::test::test_case{"just_set_value_test"} {}

			void operator()() override
			{
				just_r r;
				auto j = cmoon::execution::just(1, 2, 3);

				cmoon::execution::start(cmoon::execution::connect(std::move(j), r));

				cmoon::test::assert_true(r.value_set);
				cmoon::test::assert_false(r.error_set);
				cmoon::test::assert_false(r.done_set);
			}
	};

	export
	class just_set_error_test : public cmoon::test::test_case
	{
		struct just_r
		{
			template<class... Ts>
			void set_value(Ts&&...)
			{
				throw std::exception{};
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

			bool value_set {false};
			bool error_set {false};
			bool done_set {false};
		};

		public:
			just_set_error_test()
				: cmoon::test::test_case{"just_set_error_test"} {}

			void operator()() override
			{
				just_r r;
				auto j = cmoon::execution::just(1, 2, 3);

				cmoon::execution::start(cmoon::execution::connect(std::move(j), r));

				cmoon::test::assert_false(r.value_set);
				cmoon::test::assert_true(r.error_set);
				cmoon::test::assert_false(r.done_set);
			}
	};
}