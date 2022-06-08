export module cmoon.tests.csv;
export import cmoon.tests.csv.csv_reader;
export import cmoon.tests.csv.csv_writer;

import <utility>;

import cmoon.test;
import cmoon.library;

namespace cmoon
{
	export
	template<>
	struct library_traits<library::csv>
	{
		static cmoon::test::test_suite tests()
		{
			cmoon::test::test_suite suite{"csv library tests"};
			//suite.add_test_case<cmoon::tests::csv::csv_reader_typed_test>();
			suite.add_test_case<cmoon::tests::csv::csv_reader_any_test>();
			suite.add_test_case<cmoon::tests::csv::csv_writer_test>();
			//suite.add_test_case<cmoon::tests::csv::csv_write_read_test>();

			return std::move(suite);
		}
	};
}