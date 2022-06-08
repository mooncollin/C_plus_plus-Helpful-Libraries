export module cmoon.tests.csv.csv_reader;

import <string>;
import <sstream>;
import <tuple>;
import <vector>;

import cmoon.test;
import cmoon.csv;

namespace cmoon::tests::csv
{
	export
	class csv_reader_typed_test : public cmoon::test::test_case
	{
		public:
			csv_reader_typed_test()
				: cmoon::test::test_case{"csv_reader_typed_test"} {}

			void operator()() override
			{
				std::stringstream testing_stream;
				testing_stream << "Hello!,394,39.2,World!\n";
				testing_stream << "\"quoted!\",93,2222.222,        spaces!       \n";

				using types = std::tuple<std::string, int, float, std::string>;

				cmoon::csv::csv_reader<std::string, int, float, std::string> c{testing_stream};
				auto current {std::ranges::begin(c)};

				cmoon::test::assert_equal(*current, types{std::string{"Hello!"}, 394, 39.2f, std::string{"World!"}});
				++current;

				cmoon::test::assert_equal(*current, types{std::string{"quoted!"}, 93, 2222.222f, std::string{"        spaces!       "}});
				++current;

				cmoon::test::assert_equal(current, std::ranges::end(c));
			}
	};

	export
	class csv_reader_any_test : public cmoon::test::test_case
	{
		public:
			csv_reader_any_test()
				: cmoon::test::test_case{"csv_reader_any_test"} {}

			void operator()() override
			{
				std::stringstream testing_stream;
				testing_stream << "We are going to change things up&<-this is such a strange delimiter\n";
				testing_stream << "Only one here, really?\n";
				testing_stream << "repeat&repeat&repeat&repeat\n";

				cmoon::csv::csv_reader<cmoon::csv::csv_any> c{testing_stream, {.delimiter = "&"}};

				auto row {c.begin()};

				cmoon::test::assert_sequence_equal(*row, std::vector<std::string>{"We are going to change things up", "<-this is such a strange delimiter"});
				++row;

				cmoon::test::assert_sequence_equal(*row, std::vector<std::string>{"Only one here, really?"});
				++row;

				cmoon::test::assert_sequence_equal(*row, std::vector<std::string>{"repeat", "repeat", "repeat", "repeat"});
				++row;

				cmoon::test::assert_equal(row, std::ranges::end(c));
			}
	};
}