export module cmoon.tests.execution.let_value;

import cmoon.execution;
import cmoon.test;

namespace cmoon::tests::execution
{
	export
	class let_value_test : public cmoon::test::test_case
	{
		public:
			let_value_test()
				: cmoon::test::test_case{"let_value_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				int value {0};

				auto s = cmoon::execution::just() |
						 cmoon::execution::let_value([&value, expected] {
							return cmoon::execution::just() |
								   cmoon::execution::then([&value, expected] {
										value = expected;
								   });
						 });

				cmoon::execution::start_detached(std::move(s));

				cmoon::test::assert_equal(value, expected);
			}
	};
}