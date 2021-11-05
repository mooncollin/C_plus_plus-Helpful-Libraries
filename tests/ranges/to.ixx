export module cmoon.tests.ranges.to;

import <vector>;
import <ranges>;

import cmoon.test;
import cmoon.ranges;

namespace cmoon::tests::ranges
{
	export
	class to_vector_test : public cmoon::test::test_case
	{
		public:
			to_vector_test()
				: cmoon::test::test_case{"to_vector_test"} {}

			void operator()() override
			{
				auto view = std::ranges::views::iota(1, 11);

				const auto container = cmoon::ranges::to<std::vector<int>>(view);

				cmoon::test::assert_equal(container.size(), 10);

				cmoon::test::assert_equal(container[0], 1);
				cmoon::test::assert_equal(container[1], 2);
				cmoon::test::assert_equal(container[2], 3);
				cmoon::test::assert_equal(container[3], 4);
				cmoon::test::assert_equal(container[4], 5);
				cmoon::test::assert_equal(container[5], 6);
				cmoon::test::assert_equal(container[6], 7);
				cmoon::test::assert_equal(container[7], 8);
				cmoon::test::assert_equal(container[8], 9);
				cmoon::test::assert_equal(container[9], 10);
			}
	};
}