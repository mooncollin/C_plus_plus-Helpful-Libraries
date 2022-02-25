export module cmoon.tests.ranges;
export import cmoon.tests.ranges.enumerate;
export import cmoon.tests.ranges.zip;
export import cmoon.tests.ranges.to;

import <utility>;
import <memory>;

import cmoon.test;
import cmoon.library;

namespace cmoon
{
	export
	template<>
	struct library_traits<library::ranges>
	{
		template<class Allocator = std::allocator<cmoon::test::test_case>>
		static cmoon::test::test_suite<Allocator> tests(const Allocator& alloc = {})
		{
			cmoon::test::test_suite<Allocator> suite{"ranges library tests", alloc};
			suite.add_test_case<cmoon::tests::ranges::enumerate_view_constructor_test>();
			suite.add_test_case<cmoon::tests::ranges::enumerate_view_increment_test>();
			suite.add_test_case<cmoon::tests::ranges::enumerate_view_empty_test>();
			suite.add_test_case<cmoon::tests::ranges::enumerate_view_size_test>();
			suite.add_test_case<cmoon::tests::ranges::enumerate_view_front_test>();
			suite.add_test_case<cmoon::tests::ranges::zip_view_constructor_test>();
			suite.add_test_case<cmoon::tests::ranges::zip_view_empty_test>();
			suite.add_test_case<cmoon::tests::ranges::zip_view_increment_test>();
			suite.add_test_case<cmoon::tests::ranges::zip_view_decrement_test>();
			suite.add_test_case<cmoon::tests::ranges::zip_view_increment_different_sizes_test>();
			suite.add_test_case<cmoon::tests::ranges::zip_view_decrement_different_sizes_test>();
			suite.add_test_case<cmoon::tests::ranges::zip_view_relational_test>();
			suite.add_test_case<cmoon::tests::ranges::zip_view_iteration_test>();
			suite.add_test_case<cmoon::tests::ranges::zip_view_index_test>();
			suite.add_test_case<cmoon::tests::ranges::to_vector_test>();

			return std::move(suite);
		}
	};
}