export module cmoon.tests.ranges.zip_transform;

import <vector>;
import <string>;
import <tuple>;
import <utility>;
import <algorithm>;
import <ranges>;
import <type_traits>;
import <functional>;

import cmoon.test;
import cmoon.concepts;
import cmoon.ranges;

namespace cmoon::tests::ranges
{
	export
	class zip_transform_constructor_test : public cmoon::test::test_case
	{
		public:
			zip_transform_constructor_test()
				: cmoon::test::test_case{"zip_transform_constructor_test"} {}

			void operator()() override
			{
				std::vector<std::string> vec1{"Hello", "there"};
				std::vector<int> vec2{1, 2};
				std::vector<double> vec3{5.5, 444.4};

				const auto v1 = cmoon::ranges::views::zip_transform([](auto&&){ return 1; }, vec1);
				const auto v2 = cmoon::ranges::views::zip_transform([](auto&&, auto&&){ return 1; }, vec1, vec2);
				const auto v3 = cmoon::ranges::views::zip_transform([](auto&&, auto&&, auto&&){ return 1; }, vec1, vec2, vec3);

				cmoon::test::assert_true(std::ranges::view<std::remove_cvref_t<decltype(v1)>>, "This fails due to changes for std::tuple in p2321 not yet implemented");
				cmoon::test::assert_true(std::ranges::view<std::remove_cvref_t<decltype(v2)>>, "This fails due to changes for std::tuple in p2321 not yet implemented");
				cmoon::test::assert_true(std::ranges::view<std::remove_cvref_t<decltype(v3)>>, "This fails due to changes for std::tuple in p2321 not yet implemented");
			}
	};
}