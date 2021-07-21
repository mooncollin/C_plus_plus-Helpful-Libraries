export module cmoon.tests.string;
export import cmoon.tests.string.algorithm;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::string>()
	{
		cmoon::test::test_suite suite{"string library tests"};
		suite.add_test_case<string::remove_punctuation_test>();
		suite.add_test_case<string::remove_spaces_test>();
		suite.add_test_case<string::remove_alphanumerics_test>();
		suite.add_test_case<string::remove_blanks_test>();
		suite.add_test_case<string::remove_hexdigits_test>();
		suite.add_test_case<string::remove_upper_test>();
		suite.add_test_case<string::remove_lower_test>();
		suite.add_test_case<string::remove_character_test>();
		suite.add_test_case<string::uppercase_test>();
		suite.add_test_case<string::lowercase_test>();
		suite.add_test_case<string::split_test>();
		suite.add_test_case<string::join_test>();


		return std::move(suite);
	}
}