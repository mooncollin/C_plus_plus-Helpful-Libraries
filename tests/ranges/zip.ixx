export module cmoon.tests.ranges.zip;

import <vector>;
import <string>;
import <tuple>;
import <utility>;
import <algorithm>;
import <ranges>;
import <type_traits>;

import cmoon.test;
import cmoon.concepts;
import cmoon.ranges;

namespace cmoon::tests::ranges
{
	export
	class zip_view_constructor_test : public cmoon::test::test_case
	{
		public:
			zip_view_constructor_test()
				: cmoon::test::test_case{"zip_view_constructor_test"} {}

			void operator()() override
			{
				std::vector<std::string> vec1{"Hello", "there"};
				std::vector<int> vec2{1, 2};
				std::vector<double> vec3{5.5, 444.4};

				const auto v1 = cmoon::ranges::views::zip(vec1);
				const auto v2 = cmoon::ranges::views::zip(vec1, vec2);
				const auto v3 = cmoon::ranges::views::zip(vec1, vec2, vec3);

				cmoon::test::assert_true(std::ranges::view<std::remove_cvref_t<decltype(v1)>>);
				cmoon::test::assert_true(std::ranges::view<std::remove_cvref_t<decltype(v2)>>);
				cmoon::test::assert_true(std::ranges::view<std::remove_cvref_t<decltype(v3)>>);
			}
	};

	export
	class zip_view_empty_test : public cmoon::test::test_case
	{
		public:
			zip_view_empty_test()
				: cmoon::test::test_case{"zip_view_empty_test"} {}

			void operator()() override
			{
				using model_empty_view = std::ranges::empty_view<int>;
				struct s : public model_empty_view {};

				const auto v1 = cmoon::ranges::views::zip();
				const auto v2 = cmoon::ranges::views::zip(model_empty_view{});
				const auto v3 = cmoon::ranges::views::zip(model_empty_view{},
														  std::ranges::views::iota(10));
				const auto v4 = cmoon::ranges::views::zip(s{});
				const auto v5 = cmoon::ranges::views::zip(s{},
														  std::ranges::views::iota(10, 12));
				

				cmoon::test::assert_true(std::ranges::view<std::remove_cvref_t<decltype(v1)>>);
				cmoon::test::assert_true(std::ranges::view<std::remove_cvref_t<decltype(v2)>>);
				cmoon::test::assert_true(std::ranges::view<std::remove_cvref_t<decltype(v3)>>);
				cmoon::test::assert_true(std::ranges::view<std::remove_cvref_t<decltype(v4)>>);
				cmoon::test::assert_true(std::ranges::view<std::remove_cvref_t<decltype(v5)>>);

				cmoon::test::assert_true(std::ranges::empty(v1));
				cmoon::test::assert_true(std::ranges::empty(v2));
				cmoon::test::assert_true(std::ranges::empty(v4));
				cmoon::test::assert_true(std::ranges::empty(v5));


				cmoon::test::assert_equal(sizeof(v1), sizeof(model_empty_view));
				cmoon::test::assert_equal(sizeof(v2), sizeof(model_empty_view));
			}
	};

	export
	class zip_view_increment_test : public cmoon::test::test_case
	{
		public:
			zip_view_increment_test()
				: cmoon::test::test_case{"zip_view_increment_test"} {}

			void operator()() override
			{
				std::vector<std::string> vec1{"Hello", "there"};
				std::vector<int> vec2{1, 2};
				std::vector<double> vec3{5.5, 444.4};

				const auto v1 = cmoon::ranges::views::zip(vec1);
				const auto v2 = cmoon::ranges::views::zip(vec1, vec2);
				const auto v3 = cmoon::ranges::views::zip(vec1, vec2, vec3);

				auto it1 = std::ranges::begin(v1);
				cmoon::test::assert_equal(std::get<0>(*it1), "Hello");
				++it1;
				cmoon::test::assert_equal(std::get<0>(*it1), "there");
				++it1;
				cmoon::test::assert_equal(it1, std::ranges::end(v1));

				auto it2 = std::ranges::begin(v2);
				{
					const auto [value1, value2] = *it2;
					cmoon::test::assert_equal(value1, "Hello");
					cmoon::test::assert_equal(value2, 1);
				}
				++it2;
				{
					const auto [value1, value2] = *it2;
					cmoon::test::assert_equal(value1, "there");
					cmoon::test::assert_equal(value2, 2);
				}
				++it2;
				cmoon::test::assert_equal(it2, std::ranges::end(v2));

				auto it3 = std::ranges::begin(v3);
				{
					const auto [value1, value2, value3] = *it3;
					cmoon::test::assert_equal(value1, "Hello");
					cmoon::test::assert_equal(value2, 1);
					cmoon::test::assert_equal(value3, 5.5);
				}
				++it3;
				{
					const auto [value1, value2, value3] = *it3;
					cmoon::test::assert_equal(value1, "there");
					cmoon::test::assert_equal(value2, 2);
					cmoon::test::assert_equal(value3, 444.4);
				}
				++it3;
				cmoon::test::assert_equal(it3, std::ranges::end(v3));
			}
	};

	export
	class zip_view_decrement_test : public cmoon::test::test_case
	{
		public:
			zip_view_decrement_test()
				: cmoon::test::test_case{"zip_view_decrement_test"} {}

			void operator()() override
			{
				std::vector<std::string> vec1{"Hello", "there"};
				std::vector<int> vec2{1, 2};
				std::vector<double> vec3{5.5, 444.4};

				const auto v1 = cmoon::ranges::views::zip(vec1);
				const auto v2 = cmoon::ranges::views::zip(vec1, vec2);
				const auto v3 = cmoon::ranges::views::zip(vec1, vec2, vec3);

				auto it1 = std::ranges::begin(v1);
				cmoon::test::assert_equal(std::get<0>(*it1), "Hello");
				++it1;
				cmoon::test::assert_equal(std::get<0>(*it1), "there");
				++it1;
				cmoon::test::assert_equal(it1, std::ranges::end(v1));
				--it1;
				cmoon::test::assert_equal(std::get<0>(*it1), "there");
				--it1;
				cmoon::test::assert_equal(std::get<0>(*it1), "Hello");
				cmoon::test::assert_equal(it1, std::ranges::begin(v1));

				auto it2 = std::ranges::begin(v2);
				{
					const auto [value1, value2] = *it2;
					cmoon::test::assert_equal(value1, "Hello");
					cmoon::test::assert_equal(value2, 1);
				}
				++it2;
				{
					const auto [value1, value2] = *it2;
					cmoon::test::assert_equal(value1, "there");
					cmoon::test::assert_equal(value2, 2);
				}
				++it2;
				cmoon::test::assert_equal(it2, std::ranges::end(v2));

				--it2;
				{
					const auto [value1, value2] = *it2;
					cmoon::test::assert_equal(value1, "there");
					cmoon::test::assert_equal(value2, 2);
				}
				--it2;
				{
					const auto [value1, value2] = *it2;
					cmoon::test::assert_equal(value1, "Hello");
					cmoon::test::assert_equal(value2, 1);
				}
				cmoon::test::assert_equal(it2, std::ranges::begin(v2));

				auto it3 = std::ranges::begin(v3);
				{
					const auto [value1, value2, value3] = *it3;
					cmoon::test::assert_equal(value1, "Hello");
					cmoon::test::assert_equal(value2, 1);
					cmoon::test::assert_equal(value3, 5.5);
				}
				++it3;
				{
					const auto [value1, value2, value3] = *it3;
					cmoon::test::assert_equal(value1, "there");
					cmoon::test::assert_equal(value2, 2);
					cmoon::test::assert_equal(value3, 444.4);
				}
				++it3;
				cmoon::test::assert_equal(it3, std::ranges::end(v3));

				--it3;
				{
					const auto [value1, value2, value3] = *it3;
					cmoon::test::assert_equal(value1, "there");
					cmoon::test::assert_equal(value2, 2);
					cmoon::test::assert_equal(value3, 444.4);
				}
				--it3;
				{
					const auto [value1, value2, value3] = *it3;
					cmoon::test::assert_equal(value1, "Hello");
					cmoon::test::assert_equal(value2, 1);
					cmoon::test::assert_equal(value3, 5.5);
				}
				cmoon::test::assert_equal(it3, std::ranges::begin(v3));
			}
	};

	export
	class zip_view_increment_different_sizes_test : public cmoon::test::test_case
	{
		public:
			zip_view_increment_different_sizes_test()
				: cmoon::test::test_case{"zip_view_increment_different_sizes_test"} {}

			void operator()() override
			{
				std::vector<std::string> vec1{"Hello", "there"};
				std::vector<int> vec2{1};
				std::vector<double> vec3{5.5, 444.4};

				const auto v2 = cmoon::ranges::views::zip(vec1, vec2);
				const auto v3 = cmoon::ranges::views::zip(vec1, vec2, vec3);

				auto it2 = std::ranges::begin(v2);
				{
					const auto [value1, value2] = *it2;
					cmoon::test::assert_equal(value1, "Hello");
					cmoon::test::assert_equal(value2, 1);
				}
				++it2;
				cmoon::test::assert_equal(it2, std::ranges::end(v2));

				auto it3 = std::ranges::begin(v3);
				{
					const auto [value1, value2, value3] = *it3;
					cmoon::test::assert_equal(value1, "Hello");
					cmoon::test::assert_equal(value2, 1);
					cmoon::test::assert_equal(value3, 5.5);
				}
				++it3;
				cmoon::test::assert_equal(it3, std::ranges::end(v3));
			}
	};

	export
	class zip_view_decrement_different_sizes_test : public cmoon::test::test_case
	{
		public:
			zip_view_decrement_different_sizes_test()
				: cmoon::test::test_case{"zip_view_decrement_different_sizes_test"} {}

			void operator()() override
			{
				std::vector<std::string> vec1{"Hello", "there"};
				std::vector<int> vec2{1};
				std::vector<double> vec3{5.5, 444.4};

				const auto v2 = cmoon::ranges::views::zip(vec1, vec2);
				const auto v3 = cmoon::ranges::views::zip(vec1, vec2, vec3);

				auto it2 = std::ranges::begin(v2);
				{
					const auto [value1, value2] = *it2;
					cmoon::test::assert_equal(value1, "Hello");
					cmoon::test::assert_equal(value2, 1);
				}
				++it2;
				cmoon::test::assert_equal(it2, std::ranges::end(v2));

				--it2;
				{
					const auto [value1, value2] = *it2;
					cmoon::test::assert_equal(value1, "Hello");
					cmoon::test::assert_equal(value2, 1);
				}
				cmoon::test::assert_equal(it2, std::ranges::begin(v2));

				auto it3 = std::ranges::begin(v3);
				{
					const auto [value1, value2, value3] = *it3;
					cmoon::test::assert_equal(value1, "Hello");
					cmoon::test::assert_equal(value2, 1);
					cmoon::test::assert_equal(value3, 5.5);
				}
				++it3;
				cmoon::test::assert_equal(it3, std::ranges::end(v3));

				--it3;
				{
					const auto [value1, value2, value3] = *it3;
					cmoon::test::assert_equal(value1, "Hello");
					cmoon::test::assert_equal(value2, 1);
					cmoon::test::assert_equal(value3, 5.5);
				}
				cmoon::test::assert_equal(it3, std::ranges::begin(v3));
			}
	};

	export
	class zip_view_relational_test : public cmoon::test::test_case
	{
		public:
			zip_view_relational_test()
				: cmoon::test::test_case{"zip_view_relational_test"} {}

			void operator()() override
			{
				std::vector<std::string> vec1{"Hello", "there"};
				std::vector<int> vec2{1, 2};
				std::vector<double> vec3{5.5, 444.4};

				auto z = cmoon::ranges::views::zip(vec1, vec2, vec3);
				auto it1 = std::ranges::begin(z);
				auto it2 = std::ranges::begin(z);

				cmoon::test::assert_equal(it1, it2);
				cmoon::test::assert_greater_equal(it1, it2);
				cmoon::test::assert_less_equal(it1, it2);
				cmoon::test::assert_greater_equal(it2, it1);
				cmoon::test::assert_less_equal(it2, it1);

				++it2;

				cmoon::test::assert_not_equal(it1, it2);
				cmoon::test::assert_greater(it2, it1);
				cmoon::test::assert_greater_equal(it2, it1);
				cmoon::test::assert_less(it1, it2);
				cmoon::test::assert_less_equal(it1, it2);

				++it2;

				cmoon::test::assert_equal(it2, std::ranges::end(z));
			}
	};

	export
	class zip_view_iteration_test : public cmoon::test::test_case
	{
		public:
			zip_view_iteration_test()
				: cmoon::test::test_case{"zip_view_iteration_test"} {}

			void operator()() override
			{
				std::vector<int> vec1{1, 2, 3, 4, 5};
				std::vector<int> vec2{10, 20, 30, 40, 50};
				std::vector<int> vec3{100, 200, 300, 400};

				int count {0};
				int count2 {0};

				for (const auto [i1, i2, i3] : cmoon::ranges::views::zip(vec1, vec2, vec3))
				{
					count += i1;
					count += i2;
					count += i3;
				}

				cmoon::test::assert_equal(count, 1110);

				for (const auto [val] : cmoon::ranges::views::zip(std::ranges::views::iota(1, 11)))
				{
					count2 += val;
				}
			}
	};

	export
	class zip_view_index_test : public cmoon::test::test_case
	{
		public:
			zip_view_index_test()
				: cmoon::test::test_case{"zip_view_index_test"} {}

			void operator()() override
			{
				std::vector<int> vec1{1, 2, 3, 4, 5};
				std::vector<int> vec2{10, 20, 30, 40, 50};
				std::vector<int> vec3{100, 200, 300, 400};

				auto z = cmoon::ranges::views::zip(vec1, vec2, vec3);

				{
					auto [v1, v2, v3] = z[0];
					cmoon::test::assert_equal(v1, 1);
					cmoon::test::assert_equal(v2, 10);
					cmoon::test::assert_equal(v3, 100);
				}

				{
					auto [v1, v2, v3] = z[1];
					cmoon::test::assert_equal(v1, 2);
					cmoon::test::assert_equal(v2, 20);
					cmoon::test::assert_equal(v3, 200);
				}

				{
					auto [v1, v2, v3] = z[2];
					cmoon::test::assert_equal(v1, 3);
					cmoon::test::assert_equal(v2, 30);
					cmoon::test::assert_equal(v3, 300);
				}

				{
					auto [v1, v2, v3] = z[3];
					cmoon::test::assert_equal(v1, 4);
					cmoon::test::assert_equal(v2, 40);
					cmoon::test::assert_equal(v3, 400);
				}
			}
	};
}
