export module cmoon.tests.algorithm.max;

import <string>;
import <functional>;

import cmoon.test;
import cmoon.algorithm;

namespace cmoon::tests::algorithm
{
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

	export
	class max_custom_test : public cmoon::test::test_case
	{
		public:
			max_custom_test()
				: cmoon::test::test_case{"max_custom_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::max(std::greater{}, 4, 1, 9, 3), 9);
				cmoon::test::assert_equal(cmoon::max(std::greater{}, 3.2, 5.4, 390.3, 2.2), 390.3);
				cmoon::test::assert_equal(cmoon::max(std::greater{}, std::string{"c"}, std::string{"b"}, std::string{"a"}), std::string{"c"});
			}
	};
}