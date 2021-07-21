export module cmoon.tests.meta.type_list;

import <tuple>;

import cmoon.test;
import cmoon.meta;

namespace cmoon::tests::meta
{
	export
	class type_list_no_type_test : public cmoon::test::test_case
	{
		public:
			type_list_no_type_test()
				: cmoon::test::test_case{"type_list_no_type_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::type_list<>;

				cmoon::test::assert_equal(l::size(), 0);
				cmoon::test::assert_false(l::contains<int>());
				cmoon::test::assert_true(l::is_all_same_type());
			}
	};

	export
	class type_list_multiple_types_test : public cmoon::test::test_case
	{
		public:
			type_list_multiple_types_test()
				: cmoon::test::test_case{"type_list_multiple_types_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::type_list<int>;
				using l2 = cmoon::meta::type_list<int, float, double>;
				using l3 = cmoon::meta::type_list<char, char, char, char>;

				cmoon::test::assert_equal(l::size(), 1);
				cmoon::test::assert_equal(l2::size(), 3);
				cmoon::test::assert_true(l::is_all_same_type());

				cmoon::test::assert_equal(l::index<int>(), 0);
				cmoon::test::assert_equal(l2::index<int>(), 0);
				cmoon::test::assert_equal(l2::index<float>(), 1);
				cmoon::test::assert_equal(l2::index<double>(), 2);

				cmoon::test::assert_true(l::contains<int>());
				cmoon::test::assert_false(l::contains<double>());

				cmoon::test::assert_true(l2::contains<int>());
				cmoon::test::assert_true(l2::contains<float>());
				cmoon::test::assert_true(l2::contains<double>());
				cmoon::test::assert_false(l2::contains<bool>());

				cmoon::test::assert_true(l3::is_all_same_type());
			}
	};

	export
	class type_list_concatenate_test : public cmoon::test::test_case
	{
		public:
			type_list_concatenate_test()
				: cmoon::test::test_case{"type_list_concatenate_test"} {}

			void operator()() override
			{
				using expected_list = cmoon::meta::type_list<bool, int, float, double>;

				using l = cmoon::meta::type_list<bool>;
				using l2 = cmoon::meta::type_list<int, float, double>;

				using l3 = cmoon::meta::concatenate_types<l, l2>;

				cmoon::test::assert_equal(l3::size(), 4);

				cmoon::test::assert_equal(l3::index<bool>(), 0);
				cmoon::test::assert_equal(l3::index<int>(), 1);
				cmoon::test::assert_equal(l3::index<float>(), 2);
				cmoon::test::assert_equal(l3::index<double>(), 3);

				cmoon::test::assert_true(l3::contains<bool>());
				cmoon::test::assert_true(l3::contains<int>());
				cmoon::test::assert_true(l3::contains<float>());
				cmoon::test::assert_true(l3::contains<double>());

				cmoon::test::assert_false(l3::contains<char>());

				cmoon::test::assert_is_type<l3, expected_list>();
			}
	};

	export
	class type_list_tuple_test : public cmoon::test::test_case
	{
		public:
			type_list_tuple_test()
				: cmoon::test::test_case{"type_list_tuple_test"} {}

			void operator()() override
			{
				using t = std::tuple<int, double, float>;

				using l = cmoon::meta::type_list_from_tuple<t>;

				cmoon::test::assert_equal(l::size(), 3);

				cmoon::test::assert_equal(l::index<int>(), 0);
				cmoon::test::assert_equal(l::index<double>(), 1);
				cmoon::test::assert_equal(l::index<float>(), 2);
			}
	};

	export
	class type_list_filter_test : public cmoon::test::test_case
	{
		public:
			type_list_filter_test()
				: cmoon::test::test_case{"type_list_filter_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::type_list<char, bool, float, int, double, unsigned long long>;
				using expected_list = cmoon::meta::type_list<char, bool, int, unsigned long long>;

				using p = std::is_integral;

				using filtered_list = l::template filter<p>;

				cmoon::test::assert_is_type<filtered_list, expected_list>();
			}
	};

	export
	class type_list_transform_test : public cmoon::test::test_case
	{
		private:

			template<class T>
			struct flip_float_int : std::type_identity<T> {};

			template<std::integral T>
			struct flip_float_int<T> : std::type_identity<float> {};

			template<std::floating_point F>
			struct flip_float_int<F> : std::type_identity<int> {};
		public:
			type_list_transform_test()
				: cmoon::test::test_case{"type_list_transform_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::type_list<int, double, int, float, char>;
				using expected_list = cmoon::meta::type_list<float, int, float, int, float>;

				using transformed_list = l::template transform<flip_float_int>;

				cmoon::test::assert_is_type<transformed_list, expected_list>();
			}
	};

	export
	class type_list_sub_list_test : public cmoon::test::test_case
	{
		public:
			type_list_sub_list_test()
				: cmoon::test::test_case{"type_list_sub_list_test"} {}

			void operator()() override
			{
				using l = cmoon::meta::type_list<int, double, int, float, char>;

				using l2 = l::template sub_list<0>;
				using l3 = l::template sub_list<1>;
				using l4 = l::template sub_list<4>;
				using l5 = l::template sub_list<0, 1>;

				cmoon::test::assert_is_type<l2, l>();
				cmoon::test::assert_is_type<l3, cmoon::meta::type_list<double, int, float, char>>();
				cmoon::test::assert_is_type<l4, cmoon::meta::type_list<char>>();
				cmoon::test::assert_is_type<l5, cmoon::meta::type_list<int>>();
			}
	};
}