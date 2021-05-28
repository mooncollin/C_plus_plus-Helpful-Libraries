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
				auto current = std::begin(c);

				cmoon::test::assert_equal(*current, types{std::string{"Hello!"}, 394, 39.2f, std::string{"World!"}});
				++current;

				cmoon::test::assert_equal(*current, types{std::string{"quoted!"}, 93, 2222.222f, std::string{"        spaces!       "}});
				++current;

				cmoon::test::assert_equal(current, decltype(c){});
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

				cmoon::csv::dialect d;
				d.delimiter = "&";

				cmoon::csv::csv_reader<cmoon::csv::csv_any> c{testing_stream, d};

				c.begin();

				cmoon::test::assert_equal(*c, std::vector<std::string_view>{"We are going to change things up", "<-this is such a strange delimiter"});
				++c;

				cmoon::test::assert_equal(*c, std::vector<std::string_view>{"Only one here, really?"});
				++c;

				cmoon::test::assert_equal(*c, std::vector<std::string_view>{"repeat", "repeat", "repeat", "repeat"});
				++c;

				cmoon::test::assert_equal(c, decltype(c){});
			}
	};
}