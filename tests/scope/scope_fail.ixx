export module cmoon.tests.scope.scope_fail;

import cmoon.test;
import cmoon.scope;

namespace cmoon::tests::scope
{
	export
	class scope_fail_test : public cmoon::test::test_case
	{
		public:
			scope_fail_test()
				: cmoon::test::test_case{"scope_fail_test"} {}

			void operator()() override
			{
				constexpr int goal1 = 1;
				int value1 = 0;

				try
				{
					{
						cmoon::scope_fail sf {[&value1](){ value1++; }};
						throw "";
					}
				}
				catch (...)
				{
				}

				cmoon::test::assert_equal(value1, goal1);

				constexpr int goal2 = 0;
				int value2 = 0;

				{
					cmoon::scope_fail sf {[&value2](){ value2++; }};
				}

				cmoon::test::assert_equal(value2, goal2);

				constexpr int goal3 = 0;
				int value3 = 0;

				try
				{
					{
						cmoon::scope_fail sf {[&value3](){ value3++; }};
						sf.release();
						throw "";
					}
				}
				catch (...)
				{
				}

				cmoon::test::assert_equal(value3, goal3);
			}
	};
}