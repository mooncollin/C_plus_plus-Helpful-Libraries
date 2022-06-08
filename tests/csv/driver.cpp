import <iostream>;

import cmoon.test;
import cmoon.library;

import cmoon.tests.csv;

int main()
{
	auto suite {cmoon::library_traits<cmoon::library:csv>::tests()};

	cmoon::test::text_test_runner runner{std::cout};

	return std::ranges::any_of(runner.run(suite), [](const auto& r) { return !r.passed(); });
}