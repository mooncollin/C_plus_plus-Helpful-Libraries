import <iostream>;

import cmoon.test;
import cmoon.tests;

import cmoon.tests.algorithm;
import cmoon.tests.concepts;
import cmoon.tests.execution;
import cmoon.tests.executors;
import cmoon.tests.files;
import cmoon.tests.functional;
import cmoon.tests.io;
import cmoon.tests.iterator;
import cmoon.tests.math;
import cmoon.tests.meta;
import cmoon.tests.ranges;
import cmoon.tests.ratio;
import cmoon.tests.thread;

int main()
{
	cmoon::test::test_suite suite;
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::algorithm>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::concepts>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::execution>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::executors>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::files>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::functional>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::io>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::iterator>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::math>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::meta>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::ranges>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::ratio>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::thread>());

	cmoon::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}