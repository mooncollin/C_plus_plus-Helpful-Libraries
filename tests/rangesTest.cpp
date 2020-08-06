#include <iostream>
#include <array>
#include <algorithm>

#include "collin/ranges/ranges.hpp"
#include "collin/test.hpp"

class iota_test : public collin::test::test_case
{
	public:
		iota_test()
			: collin::test::test_case{"iota_test"} {}

		void operator()() override
		{
			constexpr std::array goal{1, 2, 3, 4, 5, 6, 7, 8, 9};

			constexpr auto iota_r = collin::ranges::views::iota(1, 10);
			std::array<int, 9> container;

			std::copy(std::begin(iota_r), std::end(iota_r), std::begin(container));

			collin::test::assert_equal(container, goal);
		}

	private:
		constexpr void static_information()
		{
			constexpr auto iota_r = collin::ranges::views::iota(1, 10);
			constexpr auto iota_size = sizeof(iota_r);
		}
};

class subrange_test : public collin::test::test_case
{
	public:
		subrange_test()
			: collin::test::test_case{"subrange_test"} {}

		void operator()() override
		{
			constexpr std::array goal{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

			constexpr auto subrange_r = collin::ranges::subrange{std::begin(goal), std::end(goal)};
			constexpr auto test = 5;

			collin::ranges::begin(goal);
			collin::ranges::begin(subrange_r);
			collin::ranges::end(subrange_r);

			constexpr auto t = collin::ranges::forward_range<decltype(subrange_r)>;
			constexpr auto t2 = collin::ranges::input_range<decltype(subrange_r)>;
			constexpr auto t3 = collin::ranges::range<decltype(subrange_r)>;
			constexpr auto t4 = collin::ranges::range<decltype(goal)>;

			//static_assert(!subrange_r.empty());
			//static_assert(subrange_r.size() == 10);
		}
};

//class counted_test : public collin::test::test_case
//{
//	public:
//		counted_test()
//			: collin::test::test_case{"counted_test"} {}
//
//		void operator()() override
//		{
//			constexpr std::array whole_container{1, 2, 3, 4, 5, 6, 7, 8, 9};
//			constexpr std::array goal{1, 2, 3};
//
//			constexpr auto counted_r = collin::ranges::views::counted(std::begin(whole_container), 3);
//		}
//};

int main()
{
	collin::test::test_suite suite;
	suite.add_test_case<iota_test>();
	suite.add_test_case<subrange_test>();

	collin::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}