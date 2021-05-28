export module cmoon.tests.algorithm.repeat;

import cmoon.test;
import cmoon.algorithm;

namespace cmoon::tests::algorithm
{
	export
	class repeat_none_test : public cmoon::test::test_case
	{
		public:
			repeat_none_test()
				: cmoon::test::test_case{"repeat_none_test"} {}
			
			void operator()() override
			{
				constexpr int goal1 {0};
				int test1 {0};
				const auto func = [&test1](){ test1++; };

				cmoon::repeat(0, func);

				cmoon::test::assert_equal(test1, goal1);
			}
	};

	export
	class repeat_test : public cmoon::test::test_case
	{
		public:
			repeat_test()
				: cmoon::test::test_case{"repeat_test"} {}
			
			void operator()() override
			{
				constexpr int goal1 {5};
				int test1 {0};
				const auto func = [&test1](){ test1++; };

				cmoon::repeat(5, func);

				cmoon::test::assert_equal(test1, goal1);
			}
	};
}