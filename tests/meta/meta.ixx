export module cmoon.tests.meta;
export import cmoon.tests.meta.type_list;
export import cmoon.tests.meta.value_list;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::meta>()
	{
		cmoon::test::test_suite suite{"meta library tests"};
		suite.add_test_case<meta::type_list_no_type_test>();
		suite.add_test_case<meta::type_list_multiple_types_test>();
		suite.add_test_case<meta::type_list_concatenate_test>();
		suite.add_test_case<meta::type_list_tuple_test>();
		suite.add_test_case<meta::type_list_filter_test>();
		suite.add_test_case<meta::type_list_transform_test>();
		suite.add_test_case<meta::type_list_sub_list_test>();
		suite.add_test_case<meta::type_list_unique_test>();
		suite.add_test_case<meta::value_list_empty_test>();
		suite.add_test_case<meta::value_list_test>();
		suite.add_test_case<meta::value_list_all_of_test>();
		suite.add_test_case<meta::value_list_any_of_test>();
		suite.add_test_case<meta::value_list_none_of_test>();
		suite.add_test_case<meta::value_list_append_test>();
		suite.add_test_case<meta::value_list_concatenate_test>();
		suite.add_test_case<meta::value_list_filter_test>();
		suite.add_test_case<meta::value_list_transform_test>();
		suite.add_test_case<meta::value_list_sub_list_test>();
		suite.add_test_case<meta::iota_test>();
		suite.add_test_case<meta::make_integer_sequence_test>();
		suite.add_test_case<meta::make_index_sequence_test>();
		suite.add_test_case<meta::index_sequence_for_test>();

		return std::move(suite);
	}
}