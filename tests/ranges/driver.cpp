import <iostream>;

import cmoon.test;
import cmoon.tests;

import cmoon.tests.ranges;

int main()
{
	auto suite = cmoon::tests::get_test_suite<cmoon::tests::library::ranges>();

	cmoon::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}