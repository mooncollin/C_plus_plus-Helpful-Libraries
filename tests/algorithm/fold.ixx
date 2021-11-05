export module cmoon.tests.algorithm.fold;

import <array>;
import <functional>;
import <string>;

import cmoon.test;
import cmoon.ranges;
import cmoon.algorithm;

namespace cmoon::tests::algorithm
{
	export
	class foldl_test : public cmoon::test::test_case
	{
		public:
			foldl_test()
				: cmoon::test::test_case{"foldl_test"} {}

			void operator()() override
			{
				constexpr std::array v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

				constexpr auto sum1 = cmoon::foldl(v, 0, std::plus<>{});
				cmoon::test::assert_equal(sum1, 55);

				constexpr auto sum2 = cmoon::foldl1(v, std::plus<>{});
				cmoon::test::assert_equal(sum2.value(), 55);

				const auto dash_fold = [](std::string a, std::string b) {
					return std::move(a) + '-' + std::move(b);
				};

				const auto str = cmoon::foldl1(v, dash_fold, [](const auto n) { return std::to_string(n); });
				cmoon::test::assert_equal(str.value(), "1-2-3-4-5-6-7-8-9-10");
			}
	};

	export
	class foldr_test : public cmoon::test::test_case
	{
		public:
			foldr_test()
				: cmoon::test::test_case{"foldr_test"} {}

			void operator()() override
			{
				constexpr std::array v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

				constexpr auto sum1 = cmoon::foldr(v, 0, std::plus<>{});
				cmoon::test::assert_equal(sum1, 55);

				constexpr auto sum2 = cmoon::foldr1(v, std::plus<>{});
				cmoon::test::assert_equal(sum2.value(), 55);

				const auto dash_fold = [](std::string a, std::string b) {
					return std::move(b) + '-' + std::move(a);
				};

				const auto str = cmoon::foldr1(v, dash_fold, [](const auto n) { return std::to_string(n); });
				cmoon::test::assert_equal(str.value(), "10-9-8-7-6-5-4-3-2-1");
			}
	};

	export
	class foldl_while_test : public cmoon::test::test_case
	{
		public:
			foldl_while_test()
				: cmoon::test::test_case{"foldl_while_test"} {}

			void operator()() override
			{
				constexpr std::array v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

				constexpr auto sum1 = cmoon::foldl_while(v, 0, [](auto& sum, const auto next) {
					if (next >= 5)
					{
						return false;
					}

					sum += next;
					return true;
				});

				cmoon::test::assert_equal(sum1.value, 10);

				constexpr auto sum2 = cmoon::foldl1_while(v, [](auto& sum, const auto next) {
					if (next >= 5)
					{
						return false;
					}

					sum += next;
					return true;
				});

				cmoon::test::assert_equal(sum2.value, 10);
			}
	};
}