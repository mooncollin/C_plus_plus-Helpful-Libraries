export module cmoon.tests.net.system_executor;

import <thread>;
import <chrono>;

import cmoon.property;
import cmoon.execution;
import cmoon.net;
import cmoon.test;

namespace cmoon::tests::net
{
	export
	class system_executor_test : public cmoon::test::test_case
	{
		public:
			system_executor_test()
				: cmoon::test::test_case{"system_executor_test"} {}

			void operator()() override
			{
				constexpr int expected{5};
				int value{0};

				cmoon::net::system_executor e;

				cmoon::execution::execute(e, [&value, expected] { value = expected; });

				cmoon::query(e, cmoon::execution::context).join();
				cmoon::test::assert_equal(value, expected);
			}
	};
}