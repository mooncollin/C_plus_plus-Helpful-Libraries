import <iostream>;
import <memory>;
import <algorithm>;

import cmoon.test;
import cmoon.library;

import cmoon.tests.benchmarking;
import cmoon.tests.concepts;
import cmoon.tests.csv;
import cmoon.tests.files;
import cmoon.tests.functional;
import cmoon.tests.io;
import cmoon.tests.math;
import cmoon.tests.meta;
import cmoon.tests.ranges;
import cmoon.tests.ratio;
import cmoon.tests.thread;

int main()
{
	cmoon::test::test_suite suite;
	suite.add_test_suite(cmoon::library_traits<cmoon::library::benchmarking>::tests());
	suite.add_test_suite(cmoon::library_traits<cmoon::library::concepts>::tests());
	suite.add_test_suite(cmoon::library_traits<cmoon::library::csv>::tests());
	suite.add_test_suite(cmoon::library_traits<cmoon::library::files>::tests());
	suite.add_test_suite(cmoon::library_traits<cmoon::library::functional>::tests());
	suite.add_test_suite(cmoon::library_traits<cmoon::library::io>::tests());
	suite.add_test_suite(cmoon::library_traits<cmoon::library::math>::tests());
	suite.add_test_suite(cmoon::library_traits<cmoon::library::meta>::tests());
	suite.add_test_suite(cmoon::library_traits<cmoon::library::ranges>::tests());
	suite.add_test_suite(cmoon::library_traits<cmoon::library::ratio>::tests());
	suite.add_test_suite(cmoon::library_traits<cmoon::library::thread>::tests());

	cmoon::test::text_test_runner runner{std::cout};

	return std::ranges::any_of(runner.run(suite), [](const auto& r) { return !r.passed(); });
}