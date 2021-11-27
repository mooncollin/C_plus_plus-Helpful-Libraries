export module cmoon.tests.execution.bulk;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class bulk_test : public cmoon::test::test_case
	{
		public:
			bulk_test()
				: cmoon::test::test_case{"bulk_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				int value {0};

				auto s = cmoon::execution::just() |
						 cmoon::execution::bulk([&value](auto) { ++value; }, expected);

				cmoon::execution::start_detached(std::move(s));

				cmoon::test::assert_equal(value, expected);
			}
	};
}