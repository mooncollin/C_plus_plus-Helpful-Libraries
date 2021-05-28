export module cmoon.tests.ranges.enumerate;

import <vector>;
import <string>;
import <ranges>;

import cmoon.test;
import cmoon.ranges;

namespace cmoon::tests::ranges
{
	export
	class enumerate_view_constructor_test : public cmoon::test::test_case
	{
		public:
			enumerate_view_constructor_test()
				: cmoon::test::test_case{"enumerate_view_constructor_test"} {}

			void operator()() override
			{
				std::vector<std::string> vec{"Hello", "I", "Love", "C++!"};

				auto v = cmoon::ranges::views::enumerate(vec);
				auto b = std::ranges::begin(v);

				{
					auto[value, num] = *b;

					cmoon::test::assert_equal(value, "Hello");
					cmoon::test::assert_equal(num, 0);
				}

				v = cmoon::ranges::views::enumerate(vec, 10);
				b = std::ranges::begin(v);

				{
					auto[value, num] = *b;

					cmoon::test::assert_equal(value, "Hello");
					cmoon::test::assert_equal(num, 10);
				}
			}
	};

	export
	class enumerate_view_increment_test : public cmoon::test::test_case
	{
		public:
			enumerate_view_increment_test()
				: cmoon::test::test_case{"enumerate_view_increment_test"} {}

			void operator()() override
			{
				std::vector<std::string> vec{"Hello", "I", "Love", "C++!"};

				const auto v = cmoon::ranges::views::enumerate(vec);
				auto b = std::ranges::begin(v);

				{
					auto[value, num] = *b;

					cmoon::test::assert_equal(value, "Hello");
					cmoon::test::assert_equal(num, 0);
				}

				++b;

				{
					auto[value, num] = *b;

					cmoon::test::assert_equal(value, "I");
					cmoon::test::assert_equal(num, 1);
				}

				++b;

				{
					auto[value, num] = *b;

					cmoon::test::assert_equal(value, "Love");
					cmoon::test::assert_equal(num, 2);
				}

				++b;

				{
					auto[value, num] = *b;

					cmoon::test::assert_equal(value, "C++!");
					cmoon::test::assert_equal(num, 3);
				}

				++b;

				cmoon::test::assert_equal(b, std::ranges::end(v));
			}
	};

	export
	class enumerate_view_decrement_test : public cmoon::test::test_case
	{
		public:
			enumerate_view_decrement_test()
				: cmoon::test::test_case{"enumerate_view_decrement_test"} {}

			void operator()() override
			{
				std::vector<std::string> vec{"Hello", "I", "Love", "C++!"};

				const auto v = cmoon::ranges::views::enumerate(vec);
				auto b = std::ranges::begin(v);

				{
					auto[value, num] = *b;

					cmoon::test::assert_equal(value, "Hello");
					cmoon::test::assert_equal(num, 0);
				}

				++b;

				{
					auto[value, num] = *b;

					cmoon::test::assert_equal(value, "I");
					cmoon::test::assert_equal(num, 1);
				}

				++b;

				{
					auto[value, num] = *b;

					cmoon::test::assert_equal(value, "Love");
					cmoon::test::assert_equal(num, 2);
				}

				++b;

				{
					auto[value, num] = *b;

					cmoon::test::assert_equal(value, "C++!");
					cmoon::test::assert_equal(num, 3);
				}

				++b;

				cmoon::test::assert_equal(b, std::ranges::end(v));

				--b;

				{
					auto[value, num] = *b;

					cmoon::test::assert_equal(value, "C++!");
					cmoon::test::assert_equal(num, 3);
				}

				--b;

				{
					auto[value, num] = *b;

					cmoon::test::assert_equal(value, "Love");
					cmoon::test::assert_equal(num, 2);
				}

				--b;

				{
					auto[value, num] = *b;

					cmoon::test::assert_equal(value, "I");
					cmoon::test::assert_equal(num, 1);
				}

				--b;

				{
					auto[value, num] = *b;

					cmoon::test::assert_equal(value, "Hello");
					cmoon::test::assert_equal(num, 0);
				}

				cmoon::test::assert_equal(b, std::ranges::begin(v));
			}
	};

	export
	class enumerate_view_empty_test : public cmoon::test::test_case
	{
		public:
			enumerate_view_empty_test()
				: cmoon::test::test_case{"enumerate_view_empty_test"} {}

			void operator()() override
			{
				std::vector<std::string> vec1{"Hello", "I", "Love", "C++!"};
				std::vector<std::string> vec2;

				const auto v = cmoon::ranges::views::enumerate(vec1);
				cmoon::test::assert_false(v.empty());
				cmoon::test::assert_true(v);

				const auto v2 = cmoon::ranges::views::enumerate(vec2);
				cmoon::test::assert_true(v2.empty());
				cmoon::test::assert_false(v2);
			}
	};

	export
	class enumerate_view_size_test : public cmoon::test::test_case
	{
		public:
			enumerate_view_size_test()
				: cmoon::test::test_case{"enumerate_view_size_test"} {}

			void operator()() override
			{
				std::vector<std::string> vec1{"Hello", "I", "Love", "C++!"};
				std::vector<std::string> vec2;

				const auto v = cmoon::ranges::views::enumerate(vec1);
				cmoon::test::assert_equal(std::ranges::size(v), std::size(vec1));

				const auto v2 = cmoon::ranges::views::enumerate(vec2);
				cmoon::test::assert_equal(std::ranges::size(v2), std::size(vec2));
			}
	};
	
	export
	class enumerate_view_index_operator_test : public cmoon::test::test_case
	{
		public:
			enumerate_view_index_operator_test()
				: cmoon::test::test_case{"enumerate_view_index_operator_test"} {}

			void operator()() override
			{
				std::vector<std::string> vec{"Hello", "I", "Love", "C++!"};

				const auto v = cmoon::ranges::views::enumerate(vec);

				{
					auto[value, num] = v[0];
					cmoon::test::assert_equal(value, "Hello");
					cmoon::test::assert_equal(num, 0);
				}

				{
					auto[value, num] = v[1];
					cmoon::test::assert_equal(value, "I");
					cmoon::test::assert_equal(num, 1);
				}

				{
					auto[value, num] = v[2];
					cmoon::test::assert_equal(value, "Love");
					cmoon::test::assert_equal(num, 2);
				}

				{
					auto[value, num] = v[3];
					cmoon::test::assert_equal(value, "C++!");
					cmoon::test::assert_equal(num, 3);
				}
			}
	};

	export
	class enumerate_view_front_test : public cmoon::test::test_case
	{
		public:
			enumerate_view_front_test()
				: cmoon::test::test_case{"enumerate_view_front_test"} {}

			void operator()() override
			{
				std::vector<std::string> vec{"Hello", "I", "Love", "C++!"};

				const auto v = cmoon::ranges::views::enumerate(vec);

				auto[value, num] = v.front();

				cmoon::test::assert_equal(value, "Hello");
				cmoon::test::assert_equal(num, 0);
			}
	};

	export
	class enumerate_view_back_test : public cmoon::test::test_case
	{
		public:
			enumerate_view_back_test()
				: cmoon::test::test_case{"enumerate_view_back_test"} {}

			void operator()() override
			{
				std::vector<std::string> vec{"Hello", "I", "Love", "C++!"};

				const auto v = cmoon::ranges::views::enumerate(vec, 5);

				auto[value, num] = v.back();

				cmoon::test::assert_equal(value, "C++!");
				cmoon::test::assert_equal(num, 8);
			}
	};
}