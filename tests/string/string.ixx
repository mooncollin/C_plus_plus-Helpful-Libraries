export module cmoon.tests.string;
export import cmoon.tests.string.algorithm;
export import cmoon.tests.string.number_string;
export import cmoon.tests.string.to_string;

import <utility>;

import cmoon.test;
import cmoon.library;

namespace cmoon
{
	export
	template<>
	struct library_traits<library::string>
	{
		static cmoon::test::test_suite tests()
		{
			cmoon::test::test_suite suite{"string library tests"};
			suite.add_test_case<cmoon::tests::string::uppercase_test>();
			suite.add_test_case<cmoon::tests::string::lowercase_test>();
			suite.add_test_case<cmoon::tests::string::trim_front_test>();
			suite.add_test_case<cmoon::tests::string::trim_back_test>();
			suite.add_test_case<cmoon::tests::string::number_string_default_constructor_test>();
			suite.add_test_case<cmoon::tests::string::number_string_integer_test>();
			suite.add_test_case<cmoon::tests::string::number_string_float_test>();
			suite.add_test_case<cmoon::tests::string::to_string_arithmetic_test>();
			suite.add_test_case<cmoon::tests::string::to_wstring_arithmetic_test>();
			return std::move(suite);
		}
	};
}