import <iostream>;
import <algorithm>;

import cmoon.test;
import cmoon.tests;

import cmoon.tests.concepts;

int main()
{
	auto suite = cmoon::tests::get_test_suite<cmoon::library::concepts>();

	cmoon::test::text_test_runner runner{std::cout};

	return std::ranges::any_of(runner.run(suite), [](const auto& r) { return !r.passed(); });
}