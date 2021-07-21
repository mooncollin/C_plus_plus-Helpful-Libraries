import <iostream>;
import <ratio>;

import cmoon.test;
import cmoon.tests;
import cmoon.ratio;

import cmoon.measures;
import cmoon.measures.distance;

//import cmoon.tests.measures;

int main()
{
	constexpr auto s = cmoon::measures::basic_unit_type<cmoon::measures::meters>;

	return 0;
	//auto suite = cmoon::tests::get_test_suite<cmoon::tests::library::measures>();
	//cmoon::test::text_test_runner runner{std::cout};

	//return !runner.run(suite);
}