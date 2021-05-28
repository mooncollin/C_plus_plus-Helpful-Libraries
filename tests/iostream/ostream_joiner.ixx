export module cmoon.tests.iostream.ostream_joiner;

import <sstream>;
import <vector>;
import <algorithm>;

import cmoon.test;
import cmoon.iostream;

namespace cmoon::tests::iostream
{
	export
	class ostream_joiner_test : public cmoon::test::test_case
	{
		public:
			ostream_joiner_test()
				: cmoon::test::test_case{"ostream_joiner_test"} {}

			void operator()() override
			{
				std::vector<int> nums{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
				std::stringstream ss;

				auto joiner = cmoon::make_ostream_joiner(ss, ", ");
				std::ranges::copy(nums, joiner);
				cmoon::test::assert_equal(ss.str(), "1, 2, 3, 4, 5, 6, 7, 8, 9, 10");

				ss.str("");
				std::ranges::copy_if(nums, joiner, [](const auto& v) { return v % 2 == 0; });
				cmoon::test::assert_equal(ss.str(), "2, 4, 6, 8, 10");
			}
	};
}