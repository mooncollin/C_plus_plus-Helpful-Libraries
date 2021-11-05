export module cmoon.tests.io.read;

import <string>;
import <sstream>;
import <iterator>;

import cmoon.test;
import cmoon.io;

namespace cmoon::tests::io
{
	export
	class read_all_streambuf_test : public cmoon::test::test_case
	{
		public:
			read_all_streambuf_test()
				: cmoon::test::test_case{"read_all_streambuf_test"} {}

			void operator()() override
			{
				const std::string goal{"Hello there! I love C++! Let's get streaming!"};
				std::stringstream ss{goal};

				std::string s;
				cmoon::read_all(ss.rdbuf(), s);
				cmoon::test::assert_equal(s, goal);
				cmoon::test::assert_true(ss.good());
				cmoon::test::assert_false(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());
			}
	};

	export
	class read_all_string_test : public cmoon::test::test_case
	{
		public:
			read_all_string_test()
				: cmoon::test::test_case{"read_all_string_test"} {}

			void operator()() override
			{
				const std::string goal{"Hello there! I love C++! Let's get streaming!"};
				std::stringstream ss{goal};

				std::string s;
				cmoon::read_all(ss, s);
				cmoon::test::assert_equal(s, goal);
				cmoon::test::assert_true(ss.eof());
				cmoon::test::assert_false(ss.good());
				cmoon::test::assert_false(ss.bad());
				cmoon::test::assert_false(ss.fail());
			}
	};

	export
	class read_all_iterator_streambuf_test : public cmoon::test::test_case
	{
		public:
			read_all_iterator_streambuf_test()
				: cmoon::test::test_case{"read_all_iterator_streambuf_test"} {}

			void operator()() override
			{
				const std::string goal{"Hello there! I love C++! Let's get streaming!"};
				std::stringstream ss{goal};

				std::string s;
				cmoon::read_all(ss.rdbuf(), std::back_inserter(s));
				cmoon::test::assert_equal(s, goal);
				cmoon::test::assert_true(ss.good());
				cmoon::test::assert_false(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());

				ss.clear();
				s.clear();
				s.resize(std::size(goal));
				ss.str(goal);
				cmoon::read_all(ss.rdbuf(), s.data());
				cmoon::test::assert_equal(s, goal);
				cmoon::test::assert_true(ss.good());
				cmoon::test::assert_false(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());
			}
	};

	export
	class read_all_iterator_test : public cmoon::test::test_case
	{
		public:
			read_all_iterator_test()
				: cmoon::test::test_case{"read_all_iterator_test"} {}

			void operator()() override
			{
				const std::string goal{"Hello there! I love C++! Let's get streaming!"};
				std::stringstream ss{goal};

				std::string s;
				cmoon::read_all(ss, std::back_inserter(s));
				cmoon::test::assert_equal(s, goal);
				cmoon::test::assert_true(ss.eof());
				cmoon::test::assert_false(ss.good());
				cmoon::test::assert_false(ss.bad());
				cmoon::test::assert_false(ss.fail());

				ss.clear();
				s.clear();
				s.resize(std::size(goal));
				ss.str(goal);
				cmoon::read_all(ss, s.data());
				cmoon::test::assert_equal(s, goal);
				cmoon::test::assert_true(ss.eof());
				cmoon::test::assert_false(ss.good());
				cmoon::test::assert_false(ss.bad());
				cmoon::test::assert_false(ss.fail());
			}
	};

	export
	class read_until_streambuf_test : public cmoon::test::test_case
	{
		public:
			read_until_streambuf_test()
				: cmoon::test::test_case{"read_until_streambuf_test"} {}

			void operator()() override
			{
				const std::string goal1{"Hello there! I love C++! Let's get streaming!"};
				const std::string goal2{"Hello there!"};
				std::stringstream ss{goal1};

				std::string s;
				cmoon::read_until(ss.rdbuf(), s, std::string_view{"\n"});
				cmoon::test::assert_equal(s, goal1);
				cmoon::test::assert_true(ss.good());
				cmoon::test::assert_false(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());

				ss.str("Hello there!\n I love C++! Let's get streaming!");
				cmoon::read_until(ss.rdbuf(), s, std::string_view{"\n"});
				cmoon::test::assert_equal(s, goal2);
				cmoon::test::assert_true(ss.good());
				cmoon::test::assert_false(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());

				ss.str("Hello there! I love C++! Let's get streaming!\n");
				cmoon::read_until(ss.rdbuf(), s, std::string_view{"\n"});
				cmoon::test::assert_equal(s, goal1);
				cmoon::test::assert_true(ss.good());
				cmoon::test::assert_false(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());
			}
	};

	export
	class read_until_string_test : public cmoon::test::test_case
	{
		public:
			read_until_string_test()
				: cmoon::test::test_case{"read_until_string_test"} {}

			void operator()() override
			{
				const std::string goal1{"Hello there! I love C++! Let's get streaming!"};
				const std::string goal2{"Hello there!"};
				std::stringstream ss{goal1};

				std::string s;
				cmoon::read_until(ss, s, std::string_view{"\n"});
				cmoon::test::assert_equal(s, goal1);
				cmoon::test::assert_false(ss.good());
				cmoon::test::assert_true(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());

				ss.clear();
				ss.str("Hello there!\n I love C++! Let's get streaming!");
				cmoon::read_until(ss, s, std::string_view{"\n"});
				cmoon::test::assert_equal(s, goal2);
				cmoon::test::assert_true(ss.good());
				cmoon::test::assert_false(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());

				ss.clear();
				ss.str("Hello there! I love C++! Let's get streaming!\n");
				cmoon::read_until(ss, s, std::string_view{"\n"});
				cmoon::test::assert_equal(s, goal1);
				cmoon::test::assert_false(ss.good());
				cmoon::test::assert_true(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());
			}
	};

	export
	class read_until_iterator_streambuf_test : public cmoon::test::test_case
	{
		public:
			read_until_iterator_streambuf_test()
				: cmoon::test::test_case{"read_until_iterator_streambuf_test"} {}

			void operator()() override
			{
				const std::string goal1{"Hello there! I love C++! Let's get streaming!"};
				const std::string goal2{"Hello there!"};
				std::stringstream ss{goal1};

				std::string s;
				cmoon::read_until(ss.rdbuf(), std::back_inserter(s), std::string_view{"\n"});
				cmoon::test::assert_equal(s, goal1);
				cmoon::test::assert_true(ss.good());
				cmoon::test::assert_false(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());

				s.clear();
				ss.str("Hello there!\n I love C++! Let's get streaming!");
				cmoon::read_until(ss.rdbuf(), std::back_inserter(s), std::string_view{"\n"});
				cmoon::test::assert_equal(s, goal2);
				cmoon::test::assert_true(ss.good());
				cmoon::test::assert_false(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());

				s.clear();
				ss.str("Hello there! I love C++! Let's get streaming!\n");
				cmoon::read_until(ss.rdbuf(), std::back_inserter(s), std::string_view{"\n"});
				cmoon::test::assert_equal(s, goal1);
				cmoon::test::assert_true(ss.good());
				cmoon::test::assert_false(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());

				s.clear();
				s.resize(std::size(goal1));
				ss.str(goal1);
				cmoon::read_all(ss.rdbuf(), s.data());
				cmoon::test::assert_equal(s, goal1);
				cmoon::test::assert_true(ss.good());
				cmoon::test::assert_false(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());
			}
	};

	export
	class read_until_iterator_test : public cmoon::test::test_case
	{
		public:
			read_until_iterator_test()
				: cmoon::test::test_case{"read_until_iterator_test"} {}

			void operator()() override
			{
				const std::string goal1{"Hello there! I love C++! Let's get streaming!"};
				const std::string goal2{"Hello there!"};
				std::stringstream ss{goal1};

				std::string s;
				cmoon::read_until(ss, s, std::string_view{"\n"});
				cmoon::test::assert_equal(s, goal1);
				cmoon::test::assert_false(ss.good());
				cmoon::test::assert_true(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());

				ss.clear();
				ss.str("Hello there!\n I love C++! Let's get streaming!");
				cmoon::read_until(ss, s, std::string_view{"\n"});
				cmoon::test::assert_equal(s, goal2);
				cmoon::test::assert_true(ss.good());
				cmoon::test::assert_false(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());

				ss.clear();
				ss.str("Hello there! I love C++! Let's get streaming!\n");
				cmoon::read_until(ss, s, std::string_view{"\n"});
				cmoon::test::assert_equal(s, goal1);
				cmoon::test::assert_false(ss.good());
				cmoon::test::assert_true(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());

				ss.clear();
				s.clear();
				s.resize(std::size(goal1));
				ss.str(goal1);
				cmoon::read_all(ss, s.data());
				cmoon::test::assert_equal(s, goal1);
				cmoon::test::assert_false(ss.good());
				cmoon::test::assert_true(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());
			}
	};

	export
	class ignore_until_streambuf_test : public cmoon::test::test_case
	{
		public:
			ignore_until_streambuf_test()
				: cmoon::test::test_case{"ignore_until_streambuf_test"} {}

			void operator()() override
			{
				const std::string text1{"Hello there! I love C++! Let's get streaming!"};
				const std::string text2{"Hello there!"};
				std::stringstream ss{text1};
				std::string s;

				cmoon::ignore_until(ss.rdbuf(), std::string_view{"\n"});
				cmoon::test::assert_true(ss.good());
				cmoon::test::assert_false(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());
				cmoon::read_all(ss.rdbuf(), s);
				cmoon::test::assert_equal(s, "");

				ss.str("Hello there!\n I love C++! Let's get streaming!");
				cmoon::ignore_until(ss.rdbuf(), std::string_view{ "\n" });
				cmoon::test::assert_true(ss.good());
				cmoon::test::assert_false(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());
				cmoon::read_all(ss.rdbuf(), s);
				cmoon::test::assert_equal(s, " I love C++! Let's get streaming!");

				ss.str("Hello there! I love C++! Let's get streaming!\n");
				cmoon::ignore_until(ss.rdbuf(), std::string_view{"\n"});
				cmoon::test::assert_true(ss.good());
				cmoon::test::assert_false(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());
				cmoon::read_all(ss.rdbuf(), s);
				cmoon::test::assert_equal(s, "");
			}
	};

	export
	class ignore_until_test : public cmoon::test::test_case
	{
		public:
			ignore_until_test()
				: cmoon::test::test_case{"ignore_until_test"} {}

			void operator()() override
			{
				const std::string text1{"Hello there! I love C++! Let's get streaming!"};
				const std::string text2{"Hello there!"};
				std::stringstream ss{text1};
				std::string s;

				cmoon::ignore_until(ss, std::string_view{"\n"});
				cmoon::test::assert_false(ss.good());
				cmoon::test::assert_true(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());
				cmoon::read_all(ss, s);
				cmoon::test::assert_equal(s, "");

				s.clear();
				ss.clear();
				ss.str("Hello there!\n I love C++! Let's get streaming!");
				cmoon::ignore_until(ss, std::string_view{"\n"});
				cmoon::test::assert_true(ss.good());
				cmoon::test::assert_false(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());
				cmoon::read_all(ss, s);
				cmoon::test::assert_equal(s, " I love C++! Let's get streaming!");

				s.clear();
				ss.clear();
				ss.str("Hello there! I love C++! Let's get streaming!\n");
				cmoon::ignore_until(ss, std::string_view{"\n"});
				cmoon::test::assert_false(ss.good());
				cmoon::test::assert_true(ss.eof());
				cmoon::test::assert_false(ss.fail());
				cmoon::test::assert_false(ss.bad());
				cmoon::read_all(ss, s);
				cmoon::test::assert_equal(s, "");
			}
	};
}