export module cmoon.tests.csv.csv_writer;

import <tuple>;
import <sstream>;
import <string>;

import cmoon.test;
import cmoon.csv;

namespace cmoon::tests::csv
{
	export
	class csv_writer_test : public cmoon::test::test_case
	{
		public:
			csv_writer_test()
				: cmoon::test::test_case{"csv_writer_test"} {}

			void operator()() override
			{
				std::stringstream testing_stream;

				cmoon::csv::csv_writer writer{testing_stream};

				using types = std::tuple<int, double, std::string, int>;

				types input{15, 34.4, "Hello!", 23};
				types input2{980132, 34.342, "23oiuj", 89230};
				types input3{23, 23, "jeifowjfweof,,,,,,ewfweoiffef,f,ewf,wefwe,fwef,wef,", 3};

				writer.write_row(std::get<0>(input), std::get<1>(input), std::get<2>(input), std::get<3>(input));
				cmoon::test::assert_equal(testing_stream.str(), "15,34.4,Hello!,23\n");
				testing_stream.str("");

				writer.write_row(std::get<0>(input2), std::get<1>(input2), std::get<2>(input2), std::get<3>(input2));
				cmoon::test::assert_equal(testing_stream.str(), "980132,34.342,23oiuj,89230\n");
				testing_stream.str("");

				writer.write_row(std::get<0>(input3), std::get<1>(input3), std::get<2>(input3), std::get<3>(input3));
				cmoon::test::assert_equal(testing_stream.str(), "23,23,\"jeifowjfweof,,,,,,ewfweoiffef,f,ewf,wefwe,fwef,wef,\",3\n");
			}
	};

	export
	class csv_write_read_test : public cmoon::test::test_case
	{
		public:
			csv_write_read_test()
				: cmoon::test::test_case{"csv_write_read_test"} {};

			void operator()() override
			{
				std::stringstream testing_stream;

				cmoon::csv::csv_writer writer{testing_stream};

				using types = std::tuple<int, double, std::string, int>;

				types input{15, 34.4, "Hello!", 23};
				types input2{980132, 34.342, "23oiuj", 89230};
				types input3{23, 23, "jeifowjfweof,,,,,,ewfweoiffef,f,ewf,wefwe,fwef,wef,", 3};

				writer.write_row(std::get<0>(input), std::get<1>(input), std::get<2>(input), std::get<3>(input));
				writer.write_row(std::get<0>(input2), std::get<1>(input2), std::get<2>(input2), std::get<3>(input2));
				writer.write_row(std::get<0>(input3), std::get<1>(input3), std::get<2>(input3), std::get<3>(input3));

				testing_stream.seekg(0);
				cmoon::csv::csv_reader<int, double, std::string, int> reader{testing_stream};

				auto row {std::ranges::begin(reader)};

				cmoon::test::assert_equal(*row, input);
				++row;

				cmoon::test::assert_equal(*row, input2);
				++row;

				cmoon::test::assert_equal(*row, input3);
				++row;

				cmoon::test::assert_equal(row, std::ranges::end(reader));
			}
	};
}