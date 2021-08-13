import <iostream>;

import cmoon.test;
import cmoon.tests;

import cmoon.tests.algorithm;

int main()
{
	cmoon::test::test_suite suite;
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::algorithm>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::concepts>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::files>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::iostream>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::iterator>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::math>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::meta>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::ranges>());
	suite.add_test_suite(cmoon::tests::get_test_suite<cmoon::tests::library::ratio>());

	cmoon::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}