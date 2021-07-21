export module cmoon.tests.meta.value_list;

import <tuple>;

import cmoon.test;
import cmoon.meta;

namespace cmoon::tests::meta
{
	export
	class value_list_empty_test : public cmoon::test::test_case
	{
		public:
			value_list_empty_test()
				: cmoon::test::test_case{"value_list_empty_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::value_list<>;

				cmoon::test::assert_equal(l::size(), 0);
				cmoon::test::assert_is_type<typename l::types, cmoon::meta::type_list<>>();
				cmoon::test::assert_false(l::contains(5));
				cmoon::test::assert_false(l::contains(3.4));
			}
	};

	export
	class value_list_test : public cmoon::test::test_case
	{
		public:
			value_list_test()
				: cmoon::test::test_case{"value_list_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::value_list<1>;
				using l2 = cmoon::meta::value_list<1, 3.4, 'c'>;
				
				cmoon::test::assert_equal(l::size(), 1);
				cmoon::test::assert_false(l::contains(3));
				cmoon::test::assert_true(l::contains(1));
				cmoon::test::assert_equal(l::get<0>(), 1);

				constexpr auto l_tuple = l::to_tuple();
				constexpr auto l_expected_tuple = std::tuple<int>{1};
				cmoon::test::assert_equal(l_tuple, l_expected_tuple);

				cmoon::test::assert_equal(l2::size(), 3);
				cmoon::test::assert_false(l2::contains('u'));
				cmoon::test::assert_true(l2::contains(1));
				cmoon::test::assert_true(l2::contains(3.4));
				cmoon::test::assert_true(l2::contains('c'));
				cmoon::test::assert_equal(l2::get<0>(), 1);
				cmoon::test::assert_equal(l2::get<1>(), 3.4);
				cmoon::test::assert_equal(l2::get<2>(), 'c');

				constexpr auto l2_tuple = l2::to_tuple();
				constexpr auto l2_expected_tuple = std::tuple<int, double, char>{1, 3.4, 'c'};
				cmoon::test::assert_equal(l2_tuple, l2_expected_tuple);
			}
	};

	export
	class value_list_all_of_test : public cmoon::test::test_case
	{
		public:
			value_list_all_of_test()
				: cmoon::test::test_case{"value_list_all_of_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::value_list<1, 2, 3, 4, 5, 6>;
				using l2 = cmoon::meta::value_list<2, 4, 6>;

				constexpr auto is_even = [](const auto& v) { return v % 2 == 0; };

				cmoon::test::assert_false(l::all_of(is_even));
				cmoon::test::assert_true(l2::all_of(is_even));
			}
	};
	
	export
	class value_list_any_of_test : public cmoon::test::test_case
	{
		public:
			value_list_any_of_test()
				: cmoon::test::test_case{"value_list_any_of_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::value_list<1, 2, 3, 4, 5, 6>;
				using l2 = cmoon::meta::value_list<2, 4, 6>;
				using l3 = cmoon::meta::value_list<1, 3, 5>;

				constexpr auto is_even = [](const auto& v) { return v % 2 == 0; };

				cmoon::test::assert_true(l::any_of(is_even));
				cmoon::test::assert_true(l2::any_of(is_even));
				cmoon::test::assert_false(l3::any_of(is_even));
			}
	};

	export
	class value_list_none_of_test : public cmoon::test::test_case
	{
		public:
			value_list_none_of_test()
				: cmoon::test::test_case{"value_list_none_of_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::value_list<1, 2, 3, 4, 5, 6>;
				using l2 = cmoon::meta::value_list<2, 4, 6>;
				using l3 = cmoon::meta::value_list<1, 3, 5>;

				constexpr auto is_even = [](const auto& v) { return v % 2 == 0; };

				cmoon::test::assert_false(l::none_of(is_even));
				cmoon::test::assert_false(l2::none_of(is_even));
				cmoon::test::assert_true(l3::none_of(is_even));
			}
	};
	
	export
	class value_list_append_test : public cmoon::test::test_case
	{
		public:
			value_list_append_test()
				: cmoon::test::test_case{"value_list_append_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::value_list<1, 2, 3, 4, 5>;
				using expected_list = cmoon::meta::value_list<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>;

				using appended_list = l::append<6, 7, 8, 9, 10>;

				cmoon::test::assert_is_type<appended_list, expected_list>();
			}
	};

	export
	class value_list_concatenate_test : public cmoon::test::test_case
	{
		public:
			value_list_concatenate_test()
				: cmoon::test::test_case{"value_list_concatenate_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::value_list<1, 2, 3>;
				using l2 = cmoon::meta::value_list<4, 5, 6>;
				using l3 = cmoon::meta::value_list<7, 8, 9>;
				using expected_list = cmoon::meta::value_list<1, 2, 3, 4, 5, 6, 7, 8, 9>;

				using concatenated_list = cmoon::meta::concatenate_values<l, l2, l3>;
				cmoon::test::assert_is_type<concatenated_list, expected_list>();
			}
	};

	export
	class value_list_filter_test : public cmoon::test::test_case
	{
		template<auto V>
		struct is_even
		{
			static constexpr auto value {V % 2 == 0};
		};

		public:
			value_list_filter_test()
				: cmoon::test::test_case{"value_list_filter_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::value_list<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>;
				using expected_list = cmoon::meta::value_list<2, 4, 6, 8, 10>;

				using filtered_list = l::filter<is_even>;

				cmoon::test::assert_is_type<filtered_list, expected_list>();
			}
	};

	export
	class value_list_transform_test : public cmoon::test::test_case
	{
		template<auto V>
		struct double_value
		{
			static constexpr auto value {V * 2};
		};

		public:
			value_list_transform_test()
				: cmoon::test::test_case {"value_list_transform_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::value_list<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>;
				using expected_list = cmoon::meta::value_list<2, 4, 6, 8, 10, 12, 14, 16, 18, 20>;

				using transformed_list = l::transform<double_value>;

				cmoon::test::assert_is_type<transformed_list, expected_list>();
			}
	};

	export
	class value_list_sub_list_test : public cmoon::test::test_case
	{
		public:
			value_list_sub_list_test()
				: cmoon::test::test_case{"value_list_sub_list_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::value_list<1, 4.3, 5, 2.3f, 'p'>;

				using l2 = l::template sub_list<0>;
				using l3 = l::template sub_list<1>;
				using l4 = l::template sub_list<4>;
				using l5 = l::template sub_list<0, 1>;

				cmoon::test::assert_is_type<l2, l>();
				cmoon::test::assert_is_type<l3, cmoon::meta::value_list<4.3, 5, 2.3f, 'p'>>();
				cmoon::test::assert_is_type<l4, cmoon::meta::value_list<'p'>>();
				cmoon::test::assert_is_type<l5, cmoon::meta::value_list<1>>();
			}
	};

	export
	class iota_test : public cmoon::test::test_case
	{
		public:
			iota_test()
				: cmoon::test::test_case{"iota_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::iota<0, 0>;
				using l2 = cmoon::meta::iota<0, 20>;
				using l3 = cmoon::meta::iota<-5, 10>;
				using l4 = cmoon::meta::iota<5, 1>;
				using l5 = cmoon::meta::iota<5, -6>;

				cmoon::test::assert_is_type<l, cmoon::meta::value_list<>>();

				cmoon::test::assert_is_type<l2, cmoon::meta::value_list<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19>>();

				cmoon::test::assert_is_type<l3, cmoon::meta::value_list<-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9>>();

				cmoon::test::assert_is_type<l4, cmoon::meta::value_list<5, 4, 3, 2>>();

				cmoon::test::assert_is_type<l5, cmoon::meta::value_list<5, 4, 3, 2, 1, 0, -1, -2, -3, -4, -5>>();
			}
	};

	export
	class make_integer_sequence_test : public cmoon::test::test_case
	{
		public:
			make_integer_sequence_test()
				: cmoon::test::test_case{"make_integer_sequence_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::make_integer_sequence<int, 11>;
				using expected = cmoon::meta::value_list<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10>;

				cmoon::test::assert_is_type<l, expected>();
			}
	};

	export
	class make_index_sequence_test : public cmoon::test::test_case
	{
		public:
			make_index_sequence_test()
				: cmoon::test::test_case{"make_index_sequence_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::make_index_sequence<11>;
				using expected = cmoon::meta::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10>;

				cmoon::test::assert_is_type<l, expected>();
			}
	};

	export
	class index_sequence_for_test : public cmoon::test::test_case
	{
		public:
			index_sequence_for_test()
				: cmoon::test::test_case{"index_sequence_for_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::index_sequence_for<int, float, double>;
				using expected = cmoon::meta::index_sequence<0, 1, 2>;

				cmoon::test::assert_is_type<l, expected>();
			}
	};
}