export module cmoon.tests.algorithm.max;

import <string>;

import cmoon.test;
import cmoon.algorithm;

namespace cmoon::tests::algorithm
{
	export
	class max_single_test : public cmoon::test::test_case
	{
		public:
			max_single_test()
				: cmoon::test::test_case{"max_single_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::max(1), 1);
				cmoon::test::assert_equal(cmoon::max(4.3), 4.3);
				cmoon::test::assert_equal(cmoon::max(std::string{"Hello!"}), std::string{"Hello!"});
			}
	};

	export
	class max_test : public cmoon::test::test_case
	{
		public:
			max_test()
				: cmoon::test::test_case{"max_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::max(4, 1, 9, 3), 9);
				cmoon::test::assert_equal(cmoon::max(3.2, 5.4, 390.3, 2.2), 390.3);
				cmoon::test::assert_equal(cmoon::max(std::string{"c"}, std::string{"b"}, std::string{"a"}), std::string{"c"});
			}
	};
}