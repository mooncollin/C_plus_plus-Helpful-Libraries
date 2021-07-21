import <iostream>;
import <format>;
import <chrono>;
import <string_view>;

import cmoon.test;
import cmoon.tests;
import cmoon.iostream;
import cmoon.utility;

import cmoon.tests.iostream;

int main()
{
	auto suite = cmoon::tests::get_test_suite<cmoon::tests::library::iostream>();
	cmoon::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}