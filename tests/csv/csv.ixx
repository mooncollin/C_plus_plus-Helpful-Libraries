export module cmoon.tests.csv;
export import cmoon.tests.csv.csv_reader;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::csv>()
	{
		cmoon::test::test_suite suite{"Csv library tests"};
		suite.add_test_case<csv::csv_reader_typed_test>();
		suite.add_test_case<csv::csv_reader_any_test>();

		return std::move(suite);
	}
}