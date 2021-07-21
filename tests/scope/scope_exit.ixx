export module cmoon.tests.scope.scope_exit;

import cmoon.test;
import cmoon.scope;

namespace cmoon::tests::scope
{
	export
	class scope_exit_test : public cmoon::test::test_case
	{
		public:
			scope_exit_test()
				: cmoon::test::test_case{"scope_exit_test"} {}

			void operator()() override
			{
				constexpr int goal1 = 1;
				int value1 = 0;

				{
					cmoon::scope_exit se {[&value1](){ value1++; }};
				}

				cmoon::test::assert_equal(value1, goal1);

				constexpr int goal2 = 0;
				int value2 = 0;

				{
					cmoon::scope_exit se {[&value2](){ value2++; }};
					se.release();
				}

				cmoon::test::assert_equal(value2, goal2);
			}
	};
}