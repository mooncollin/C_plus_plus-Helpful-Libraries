import <iostream>;

import cmoon.test;
import cmoon.tests;

import cmoon.tests.ratio;

int main()
{
	auto suite = cmoon::tests::get_test_suite<cmoon::tests::library::ratio>();

	cmoon::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}