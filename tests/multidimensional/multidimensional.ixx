export module cmoon.tests.multidimensional;
export import cmoon.tests.multidimensional.multidimensional_array;
export import cmoon.tests.multidimensional.fixed_multidimensional_array;
export import cmoon.tests.multidimensional.static_mutlidimensional_array;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::multidimensional>()
	{
		cmoon::test::test_suite suite{"multidimensional library tests"};
		suite.add_test_case<multidimensional::multidimensional_array_constructor_test>();
		suite.add_test_case<multidimensional::multidimensional_array_assignment_test>();
		suite.add_test_case<multidimensional::multidimensional_array_value_test>();
		suite.add_test_case<multidimensional::fixed_multidimensional_constructor_test>();
		suite.add_test_case<multidimensional::fixed_multidimensional_value_test>();
		suite.add_test_case<multidimensional::static_multidimensional_constructor_test>();

		return std::move(suite);
	}
}