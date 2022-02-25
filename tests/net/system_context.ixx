export module cmoon.tests.net.system_context;

import <atomic>;

import cmoon.execution;
import cmoon.net;
import cmoon.test;

namespace cmoon::tests::net
{
	export
	class system_context_test : public cmoon::test::test_case
	{
		public:
			system_context_test()
				: cmoon::test::test_case{"system_context_test"} {}

			void operator()() override
			{
				constexpr int expected{5};
				std::atomic<int> value{0};

				auto s = cmoon::net::system_scheduler{};
				cmoon::execution::execute(s, [&value, expected] { value = expected; value.notify_one(); });

				value.wait(0);
				cmoon::test::assert_equal(value, expected);
			}
	};
}