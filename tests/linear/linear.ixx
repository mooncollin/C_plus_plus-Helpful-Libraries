export module cmoon.tests.linear;
export import cmoon.tests.linear.matrix;
export import cmoon.tests.linear.static_matrix;
export import cmoon.tests.linear.row;
export import cmoon.tests.linear.column;
export import cmoon.tests.linear.diagonal;
export import cmoon.tests.linear.identity_matrix;
export import cmoon.tests.linear.transpose;
export import cmoon.tests.linear.sub_matrix;
export import cmoon.tests.linear.determinant;
export import cmoon.tests.linear.adjugate;
export import cmoon.tests.linear.inverse;
export import cmoon.tests.linear.elimination;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::linear>()
	{
		cmoon::test::test_suite suite{"linear library tests"};
		suite.add_test_case<linear::matrix_constructor_test>();
		suite.add_test_case<linear::matrix_add_test>();
		suite.add_test_case<linear::matrix_minus_test>();
		suite.add_test_case<linear::matrix_scale_test>();
		suite.add_test_case<linear::static_matrix_constructor_test>();
		suite.add_test_case<linear::static_matrix_add_test>();
		suite.add_test_case<linear::static_matrix_minus_test>();
		suite.add_test_case<linear::static_matrix_scale_test>();
		suite.add_test_case<linear::mixed_matrix_equal_test>();
		suite.add_test_case<linear::mixed_matrix_add_test>();
		suite.add_test_case<linear::mixed_matrix_minus_test>();
		suite.add_test_case<linear::mixed_matrix_multiply_test>();
		suite.add_test_case<linear::row_test>();
		suite.add_test_case<linear::static_row_test>();
		suite.add_test_case<linear::column_test>();
		suite.add_test_case<linear::static_column_test>();
		suite.add_test_case<linear::diagonal_test>();
		suite.add_test_case<linear::identity_matrix_test>();
		suite.add_test_case<linear::matrix_transpose_test>();
		suite.add_test_case<linear::static_matrix_transpose_test>();
		suite.add_test_case<linear::sub_matrix_test>();
		suite.add_test_case<linear::determinant_test>();
		suite.add_test_case<linear::adjugate_test>();
		suite.add_test_case<linear::inverse_test>();
		suite.add_test_case<linear::gaussian_elimination_test>();

		return std::move(suite);
	}
}