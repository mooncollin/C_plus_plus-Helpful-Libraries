export module cmoon.tests.algorithm.min;

import <string>;

import cmoon.test;
import cmoon.algorithm;

namespace cmoon::tests::algorithm
{
	export
	class min_single_test : public cmoon::test::test_case
	{
		public:
			min_single_test()
				: cmoon::test::test_case{"min_single_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::min(1), 1);
				cmoon::test::assert_equal(cmoon::min(4.3), 4.3);
				cmoon::test::assert_equal(cmoon::min(std::string{"Hello!"}), std::string{"Hello!"});
			}
	};

	export
	class min_test : public cmoon::test::test_case
	{
		public:
			min_test()
				: cmoon::test::test_case{"min_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::min(4, 1, 9, 3), 1);
				cmoon::test::assert_equal(cmoon::min(3.2, 5.4, 390.3, 2.2), 2.2);
				cmoon::test::assert_equal(cmoon::min(std::string{"c"}, std::string{"b"}, std::string{"a"}), std::string{"a"});
			}
	};
}