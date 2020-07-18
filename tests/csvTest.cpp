#include <iostream>
#include <string>
#include <sstream>

#include "collin/csv.hpp"
#include "collin/test.hpp"
#include "collin/utility.hpp"

class csv_typed_test : public collin::test::test_case
{
	public:
		csv_typed_test()
			: collin::test::test_case{"csv_typed_test"} {}

		void operator()() override
		{
			std::stringstream testing_stream;
			testing_stream << "Hello!,394,39.2,World!\r\n";
			testing_stream << "\"quoted!\",93,2222.222,        spaces!       \r\n";

			using types = std::tuple<std::string, int, float, std::string>;

			collin::csv::csv_reader<std::string, int, float, std::string> c{testing_stream};
			auto current = std::begin(c);

			collin::test::assert_equal(*current, types{"Hello!", 394, 39.2, "World!"});
			++current;

			collin::test::assert_equal(*current, types{"quoted!", 93, 2222.222, "        spaces!       "});
			++current;

			collin::test::assert_equal(current, decltype(c){});
		}
};

class csv_any_test : public collin::test::test_case
{
	public:
		csv_any_test()
			: collin::test::test_case{"csv_any_test"} {}

		void operator()() override
		{
			std::stringstream testing_stream;
			testing_stream << "We are going to change things up&<-this is such a strange delimiter\r\n";
			testing_stream << "Only one here, really?\r\n";
			testing_stream << "repeat&repeat&repeat&repeat\r\n";

			collin::csv::dialect d;
			d.delimiter = "&";

			collin::csv::csv_reader<collin::csv::csv_any> c{testing_stream, d};

			c.begin();

			collin::test::assert_equal(*c, std::vector<std::string_view>{"We are going to change things up", "<-this is such a strange delimiter"});
			++c;

			collin::test::assert_equal(*c, std::vector<std::string_view>{"Only one here, really?"});
			++c;

			collin::test::assert_equal(*c, std::vector<std::string_view>{"repeat", "repeat", "repeat", "repeat"});
			++c;

			collin::test::assert_equal(c, decltype(c){});
		}
};

class csv_writer_test : public collin::test::test_case
{
	public:
		csv_writer_test()
			: collin::test::test_case{"csv_writer_test"} {}

		void operator()() override
		{
			std::stringstream testing_stream;

			collin::csv::csv_writer writer{testing_stream};

			using types = std::tuple<int, double, std::string, int>;

			types input{15, 34.4, "Hello!", 23};
			types input2{980132, 34.342, "23oiuj", 89230};
			types input3{23, 23, "jeifowjfweof,,,,,,ewfweoiffef,f,ewf,wefwe,fwef,wef,", 3};

			writer.write_row(std::get<0>(input), std::get<1>(input), std::get<2>(input), std::get<3>(input));
			collin::test::assert_equal(testing_stream.str(), "15,34.4,Hello!,23\r\n");
			testing_stream.str("");

			writer.write_row(std::get<0>(input2), std::get<1>(input2), std::get<2>(input2), std::get<3>(input2));
			collin::test::assert_equal(testing_stream.str(), "980132,34.342,23oiuj,89230\r\n");
			testing_stream.str("");

			writer.write_row(std::get<0>(input3), std::get<1>(input3), std::get<2>(input3), std::get<3>(input3));
			collin::test::assert_equal(testing_stream.str(), "23,23,\"jeifowjfweof,,,,,,ewfweoiffef,f,ewf,wefwe,fwef,wef,\",3\r\n");
		}
};

class csv_write_read_test : public collin::test::test_case
{
	public:
		csv_write_read_test()
			: collin::test::test_case{"csv_write_read_test"} {};

		void operator()() override
		{
			std::stringstream testing_stream;

			collin::csv::csv_writer writer{testing_stream};

			using types = std::tuple<int, double, std::string, int>;

			types input{15, 34.4, "Hello!", 23};
			types input2{980132, 34.342, "23oiuj", 89230};
			types input3{23, 23, "jeifowjfweof,,,,,,ewfweoiffef,f,ewf,wefwe,fwef,wef,", 3};

			writer.write_row(std::get<0>(input), std::get<1>(input), std::get<2>(input), std::get<3>(input));
			writer.write_row(std::get<0>(input2), std::get<1>(input2), std::get<2>(input2), std::get<3>(input2));
			writer.write_row(std::get<0>(input3), std::get<1>(input3), std::get<2>(input3), std::get<3>(input3));

			testing_stream.seekg(0);
			collin::csv::csv_reader<int, double, std::string, int> reader{testing_stream};

			reader.begin();

			collin::test::assert_equal(*reader, input);
			++reader;

			collin::test::assert_equal(*reader, input2);
			++reader;

			collin::test::assert_equal(*reader, input3);
			++reader;

			collin::test::assert_equal(reader, decltype(reader){});
		}
};

int main()
{
	collin::test::test_suite suite;
	suite.add_test_case<csv_typed_test>();
	suite.add_test_case<csv_any_test>();
	suite.add_test_case<csv_writer_test>();
	suite.add_test_case<csv_write_read_test>();

	collin::test::text_test_runner runner{std::cout};
	
	return !runner.run(suite);
}