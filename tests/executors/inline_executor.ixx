export module cmoon.tests.executors.inline_executor;

import <atomic>;

import cmoon.property;
import cmoon.execution;
import cmoon.executors;
import cmoon.test;

namespace cmoon::tests::executors
{
	export
	class inline_executor_test : public cmoon::test::test_case
	{
		public:
			inline_executor_test()
				: cmoon::test::test_case{"inline_executor_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				int v {0};
				cmoon::executors::inline_executor e;
				cmoon::execution::execute(e, [&v]{ v = expected; });

				cmoon::test::assert_equal(v, expected);
			}
	};

	template<class E, class P0>
	concept applicable = cmoon::is_applicable_property_v<std::decay_t<E>, std::decay_t<P0>> &&
							std::decay_t<P0>::is_requirable;

							template<class E, class P0>
		concept can_member_call = 
			requires(E&& e, P0&& p0)
		{
			std::forward<E>(e).require(std::forward<P0>(p0));
		};

	export
	class inline_executor_bulk_sequenced_test : public cmoon::test::test_case
	{
		public:
			inline_executor_bulk_sequenced_test()
				: cmoon::test::test_case{"inline_executor_bulk_sequenced_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				std::atomic<int> v {0};
				cmoon::executors::inline_executor e;

				cmoon::execution::bulk_execute(e, [&v](auto) { ++v; }, expected);

				cmoon::test::assert_equal(v, expected);

				auto e2 = cmoon::require(e, cmoon::execution::bulk_guarantee.sequenced);
				v = 0;

				cmoon::execution::bulk_execute(e2, [&v](auto) { ++v; }, expected);

				cmoon::test::assert_equal(v, expected);
			}
	};

	export
	class inline_executor_bulk_unsequenced_test : public cmoon::test::test_case
	{
		public:
			inline_executor_bulk_unsequenced_test()
				: cmoon::test::test_case{"inline_executor_bulk_unsequenced_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				std::atomic<int> v {0};
				auto e = cmoon::require(cmoon::executors::inline_executor{}, cmoon::execution::bulk_guarantee.unsequenced);

				cmoon::execution::bulk_execute(e, [&v](auto) { ++v; }, expected);

				cmoon::test::assert_equal(v, expected);
			}
	};
}