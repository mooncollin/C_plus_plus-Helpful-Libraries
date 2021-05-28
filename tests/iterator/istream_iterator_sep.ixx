export module cmoon.tests.iterator.istream_iterator_sep;

import <sstream>;
import <iterator>;

import cmoon.test;
import cmoon.iterator;

namespace cmoon::tests::iterator
{
	export
	class istream_iterator_sep_string_single_test : public cmoon::test::test_case
	{
		public:
			istream_iterator_sep_string_single_test()
				: cmoon::test::test_case{"istream_iterator_sep_string_single_test"} {}

			void operator()() override
			{
				std::stringstream ss{"Hello!"};

				cmoon::istream_iterator_sep<std::string> l{ss};

				cmoon::test::assert_equal(*l, "Hello!");
				++l;
				cmoon::test::assert_equal(l, std::default_sentinel);
			}
	};

	export
	class istream_iterator_sep_string_test : public cmoon::test::test_case
	{
		public:
			istream_iterator_sep_string_test()
				: cmoon::test::test_case{"istream_iterator_sep_string_test"} {}

			void operator()() override
			{
				std::stringstream ss{"I Love C++!"};

				cmoon::istream_iterator_sep<std::string> l{ss};

				cmoon::test::assert_equal(*l, "I");
				++l;
				cmoon::test::assert_equal(*l, "Love");
				++l;
				cmoon::test::assert_equal(*l, "C++!");
				++l;
				cmoon::test::assert_equal(l, std::default_sentinel);

				ss.clear();
				ss.str("Hello!SEPThere!SEPISEPLoveSEPC++!");

				l = {ss, "SEP"};
				cmoon::test::assert_equal(*l, "Hello!");
				++l;
				cmoon::test::assert_equal(*l, "There!");
				++l;
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
	class istream_iterator_sep_integer_single_test : public cmoon::test::test_case
	{
		public:
			istream_iterator_sep_integer_single_test()
				: cmoon::test::test_case{"istream_iterator_sep_integer_single_test"} {}

			void operator()() override
			{
				std::stringstream ss{"50"};

				cmoon::istream_iterator_sep<int> l{ss};

				cmoon::test::assert_equal(*l, 50);
				++l;

				cmoon::test::assert_equal(l, std::default_sentinel);
			}
	};

	export
	class istream_iterator_sep_integer_test : public cmoon::test::test_case
	{
		public:
			istream_iterator_sep_integer_test()
				: cmoon::test::test_case{"istream_iterator_sep_integer_test"} {}

			void operator()() override
			{
				std::stringstream ss{"50 40 30 20 10 0"};

				cmoon::istream_iterator_sep<int> l{ss};

				cmoon::test::assert_equal(*l, 50);
				++l;
				cmoon::test::assert_equal(*l, 40);
				++l;
				cmoon::test::assert_equal(*l, 30);
				++l;
				cmoon::test::assert_equal(*l, 20);
				++l;
				cmoon::test::assert_equal(*l, 10);
				++l;
				cmoon::test::assert_equal(*l, 0);
				++l;

				cmoon::test::assert_equal(l, std::default_sentinel);
			}
	};
}