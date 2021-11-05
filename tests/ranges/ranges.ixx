export module cmoon.tests.ranges;
export import cmoon.tests.ranges.enumerate;
export import cmoon.tests.ranges.zip;
export import cmoon.tests.ranges.to;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::ranges>()
	{
		cmoon::test::test_suite suite{"ranges library tests"};
		suite.add_test_case<ranges::enumerate_view_constructor_test>();
		suite.add_test_case<ranges::enumerate_view_increment_test>();
		suite.add_test_case<ranges::enumerate_view_empty_test>();
		suite.add_test_case<ranges::enumerate_view_size_test>();
		suite.add_test_case<ranges::enumerate_view_front_test>();
		suite.add_test_case<ranges::zip_view_constructor_test>();
		suite.add_test_case<ranges::zip_view_empty_test>();
		suite.add_test_case<ranges::zip_view_increment_test>();
		suite.add_test_case<ranges::zip_view_decrement_test>();
		suite.add_test_case<ranges::zip_view_increment_different_sizes_test>();
		suite.add_test_case<ranges::zip_view_decrement_different_sizes_test>();
		suite.add_test_case<ranges::zip_view_relational_test>();
		suite.add_test_case<ranges::zip_view_iteration_test>();
		suite.add_test_case<ranges::zip_view_index_test>();
		suite.add_test_case<ranges::to_vector_test>();

		return std::move(suite);
	}
}