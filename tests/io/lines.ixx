export module cmoon.tests.io.lines;

import <sstream>;
import <iterator>;
import <iostream>;

import cmoon.test;
import cmoon.io;

namespace cmoon::tests::io
{
	export
	class lines_string_single_test : public cmoon::test::test_case
	{
		public:
			lines_string_single_test()
				: cmoon::test::test_case{"lines_string_single_test"} {}

			void operator()() override
			{
				std::stringstream ss{"Hello!"};

				cmoon::lines<std::string> l{ss};

				cmoon::test::assert_equal(*l, "Hello!");
				++l;
				cmoon::test::assert_equal(l, std::default_sentinel);
			}
	};

	export
	class lines_string_test : public cmoon::test::test_case
	{
		public:
			lines_string_test()
				: cmoon::test::test_case{"lines_string_test"} {}

			void operator()() override
			{
				std::stringstream ss{"I\nLove\nC++!"};

				cmoon::lines<std::string> l{ss};

				cmoon::test::assert_equal(*l, "I");
				++l;
				cmoon::test::assert_equal(*l, "Love");
				++l;
				cmoon::test::assert_equal(*l, "C++!");
				++l;
				cmoon::test::assert_equal(l, std::default_sentinel);
			}
	};

	export
	class lines_integer_single_test : public cmoon::test::test_case
	{
		public:
			lines_integer_single_test()
				: cmoon::test::test_case{"lines_integer_single_test"} {}

			void operator()() override
			{
				std::stringstream ss{"50"};

				cmoon::lines<int> l{ss};

				cmoon::test::assert_equal(*l, 50);
				++l;

				cmoon::test::assert_equal(l, std::default_sentinel);
			}
	};
}