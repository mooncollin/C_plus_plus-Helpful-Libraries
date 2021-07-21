export module cmoon.tests.scope.scope_success;

import cmoon.test;
import cmoon.scope;

namespace cmoon::tests::scope
{
	export
	class scope_success_test : public cmoon::test::test_case
	{
		public:
			scope_success_test()
				: cmoon::test::test_case{"scope_success_test"} {}

			void operator()() override
			{
				constexpr int goal1 = 1;
				int value1 = 0;

				{
					cmoon::scope_success sc {[&value1](){ value1++; }};
				}

				cmoon::test::assert_equal(value1, goal1);

				constexpr int goal2 = 0;
				int value2 = 0;

				try
				{
					{
						cmoon::scope_success sc {[&value2](){ value2++; }};
						throw "";
					}
				}
				catch (...)
				{
				}

				cmoon::test::assert_equal(value2, goal2);

				constexpr int goal3 = 0;
				int value3 = 0;

				{
					cmoon::scope_success sc {[&value1](){ value1++; }};
					sc.release();
				}

				cmoon::test::assert_equal(value3, goal3);
			}
	};
}