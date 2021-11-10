export module cmoon.tests.execution.transform;

import <tuple>;
import <variant>;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class transform_test : public cmoon::test::test_case
	{
		public:
			transform_test()
				: cmoon::test::test_case{"transform_test"} {}

			void operator()() override
			{
				auto just_sender = cmoon::execution::just(3);

				auto transform_sender = std::move(just_sender) |
										cmoon::execution::transform([](int a) { return a + 5; });

				auto [result] = cmoon::execution::sync_wait(std::move(transform_sender));

				cmoon::test::assert_equal(result, 8);
			}
	};
}