#include <string>
#include <string_view>
#include <iostream>
#include <concepts>
#include <typeinfo>
#include <chrono>

#include "cmoon/format.hpp"

#include "cmoon/test/assert.hpp"
#include "cmoon/test/assert_exception.hpp"
#include "cmoon/test/test_case.hpp"
#include "cmoon/test/runner.hpp"
#include "cmoon/test/test_suite.hpp"

using namespace std::literals;

class format_char_test : public cmoon::test::test_case
{
	public:
		format_char_test()
			: cmoon::test::test_case{"format_char_test"} {}

		void operator()() override
		{
			const std::string default_expected {"h"};
			const std::string left_align_expected {"h  "};
			const std::string right_align_expected {"  h"};
			const std::string center_align_expected {" h "};
			const std::string left_fill_expected {"h**"};
			const std::string right_fill_expected {"**h"};
			const std::string center_fill_expected {"*h*"};
			const std::string multiple_variable_expected {"hello!!!"};
			const std::string multiple_variable_index_expected {"hello!!!"};

			const auto default_value = cmoon::format("{}", 'h');
			cmoon::test::assert_equal(default_value, default_expected);

			const auto left_align_value = cmoon::format("{:<3}", 'h');
			cmoon::test::assert_equal(left_align_value, left_align_expected);

			const auto right_align_value = cmoon::format("{:>3}", 'h');
			cmoon::test::assert_equal(right_align_value, right_align_expected);

			const auto center_align_value = cmoon::format("{:^3}", 'h');
			cmoon::test::assert_equal(center_align_value, center_align_expected);

			const auto left_fill_value = cmoon::format("{:*<3}", 'h');
			cmoon::test::assert_equal(left_fill_value, left_fill_expected);

			const auto right_fill_value = cmoon::format("{:*>3}", 'h');
			cmoon::test::assert_equal(right_fill_value, right_fill_expected);

			const auto center_fill_value = cmoon::format("{:*^3}", 'h');
			cmoon::test::assert_equal(center_fill_value, center_fill_expected);

			const auto left_align_value2 = cmoon::format("{:<{}}", 'h', 3);
			cmoon::test::assert_equal(left_align_value2, left_align_expected);

			const auto right_align_value2 = cmoon::format("{:>{}}", 'h', 3);
			cmoon::test::assert_equal(right_align_value2, right_align_expected);

			const auto center_align_value2 = cmoon::format("{:^{}}", 'h', 3);
			cmoon::test::assert_equal(center_align_value2, center_align_expected);

			const auto left_fill_value2 = cmoon::format("{:*<{}}", 'h', 3);
			cmoon::test::assert_equal(left_fill_value2, left_fill_expected);

			const auto right_fill_value2 = cmoon::format("{:*>{}}", 'h', 3);
			cmoon::test::assert_equal(right_fill_value2, right_fill_expected);

			const auto center_fill_value2 = cmoon::format("{:*^{}}", 'h', 3);
			cmoon::test::assert_equal(center_fill_value2, center_fill_expected);

			const auto multiple_variable_value = cmoon::format("{}{}{}{}{}!!!", 'h', 'e', 'l', 'l', 'o');
			cmoon::test::assert_equal(multiple_variable_value, multiple_variable_expected);

			const auto multiple_variable_index_value = cmoon::format("{0:}{1:}{2:}{2:}{3:}{4:}{4:}{4:}", 'h', 'e', 'l', 'o', '!');
			cmoon::test::assert_equal(multiple_variable_index_expected, multiple_variable_index_value);
		}
};

class format_wide_char_test : public cmoon::test::test_case
{
	public:
		format_wide_char_test()
			: cmoon::test::test_case{"format_wide_char_test"} {}

		void operator()() override
		{
			const std::wstring default_expected {L"h"};
			const std::wstring left_align_expected {L"h  "};
			const std::wstring right_align_expected {L"  h"};
			const std::wstring center_align_expected {L" h "};
			const std::wstring left_fill_expected {L"h**"};
			const std::wstring right_fill_expected {L"**h"};
			const std::wstring center_fill_expected {L"*h*"};
			const std::wstring multiple_variable_expected {L"hello!!!"};
			const std::wstring multiple_variable_index_expected {L"hello!!!"};

			const auto default_value = cmoon::format(L"{}", L'h');
			cmoon::test::assert_equal(default_value, default_expected);

			const auto left_align_value = cmoon::format(L"{:<3}", L'h');
			cmoon::test::assert_equal(left_align_value, left_align_expected);

			const auto right_align_value = cmoon::format(L"{:>3}", L'h');
			cmoon::test::assert_equal(right_align_value, right_align_expected);

			const auto center_align_value = cmoon::format(L"{:^3}", L'h');
			cmoon::test::assert_equal(center_align_value, center_align_expected);

			const auto left_fill_value = cmoon::format(L"{:*<3}", L'h');
			cmoon::test::assert_equal(left_fill_value, left_fill_expected);

			const auto right_fill_value = cmoon::format(L"{:*>3}", L'h');
			cmoon::test::assert_equal(right_fill_value, right_fill_expected);

			const auto center_fill_value = cmoon::format(L"{:*^3}", L'h');
			cmoon::test::assert_equal(center_fill_value, center_fill_expected);

			const auto left_align_value2 = cmoon::format(L"{:<{}}", L'h', 3);
			cmoon::test::assert_equal(left_align_value2, left_align_expected);

			const auto right_align_value2 = cmoon::format(L"{:>{}}", L'h', 3);
			cmoon::test::assert_equal(right_align_value2, right_align_expected);

			const auto center_align_value2 = cmoon::format(L"{:^{}}", L'h', 3);
			cmoon::test::assert_equal(center_align_value2, center_align_expected);

			const auto left_fill_value2 = cmoon::format(L"{:*<{}}", L'h', 3);
			cmoon::test::assert_equal(left_fill_value2, left_fill_expected);

			const auto right_fill_value2 = cmoon::format(L"{:*>{}}", L'h', 3);
			cmoon::test::assert_equal(right_fill_value2, right_fill_expected);

			const auto center_fill_value2 = cmoon::format(L"{:*^{}}", L'h', 3);
			cmoon::test::assert_equal(center_fill_value2, center_fill_expected);

			const auto multiple_variable_value = cmoon::format(L"{}{}{}{}{}!!!", L'h', L'e', L'l', L'l', L'o');
			cmoon::test::assert_equal(multiple_variable_value, multiple_variable_expected);

			const auto multiple_variable_index_value = cmoon::format(L"{0:}{1:}{2:}{2:}{3:}{4:}{4:}{4:}", L'h', L'e', L'l', L'o', L'!');
			cmoon::test::assert_equal(multiple_variable_index_expected, multiple_variable_index_value);
		}
};

template<class... Args>
	requires(cmoon::can_format<std::decay_t<Args>, cmoon::format_context> && ...)
void testing(const Args&...) {}

class format_chars_test : public cmoon::test::test_case
{
	public:
		format_chars_test()
			: cmoon::test::test_case{"format_chars_test"} {}

		void operator()() override
		{
			const std::string default_expected{"test"};
			const std::string left_align_expected{"test  "};
			const std::string right_align_expected{"  test"};
			const std::string center_align_expected{" test "};
			const std::string left_fill_expected{"test**"};
			const std::string right_fill_expected{"**test"};
			const std::string center_fill_expected{"*test*"};
			const std::string multiple_variable_expected{"hello world!!!"};
			const std::string multiple_variable_index_expected{"hello hello world!!!"};

			const auto default_value = cmoon::format("{}", "test");
			cmoon::test::assert_equal(default_value, default_expected);

			const auto left_align_value = cmoon::format("{:<6}", "test");
			cmoon::test::assert_equal(left_align_value, left_align_expected);

			const auto right_align_value = cmoon::format("{:>6}", "test");
			cmoon::test::assert_equal(right_align_value, right_align_expected);

			const auto center_align_value = cmoon::format("{:^6}", "test");
			cmoon::test::assert_equal(center_align_value, center_align_expected);

			const auto left_fill_value = cmoon::format("{:*<6}", "test");
			cmoon::test::assert_equal(left_fill_value, left_fill_expected);

			const auto right_fill_value = cmoon::format("{:*>6}", "test");
			cmoon::test::assert_equal(right_fill_value, right_fill_expected);

			const auto center_fill_value = cmoon::format("{:*^6}", "test");
			cmoon::test::assert_equal(center_fill_value, center_fill_expected);

			const auto left_align_value2 = cmoon::format("{:<{}}", "test", 6);
			cmoon::test::assert_equal(left_align_value2, left_align_expected);

			const auto right_align_value2 = cmoon::format("{:>{}}", "test", 6);
			cmoon::test::assert_equal(right_align_value2, right_align_expected);

			const auto center_align_value2 = cmoon::format("{:^{}}", "test", 6);
			cmoon::test::assert_equal(center_align_value2, center_align_expected);

			const auto left_fill_value2 = cmoon::format("{:*<{}}", "test", 6);
			cmoon::test::assert_equal(left_fill_value2, left_fill_expected);

			const auto right_fill_value2 = cmoon::format("{:*>{}}", "test", 6);
			cmoon::test::assert_equal(right_fill_value2, right_fill_expected);

			const auto center_fill_value2 = cmoon::format("{:*^{}}", "test", 6);
			cmoon::test::assert_equal(center_fill_value2, center_fill_expected);

			const auto multiple_variable_value = cmoon::format("{} {}!!!", "hello", "world");
			cmoon::test::assert_equal(multiple_variable_value, multiple_variable_expected);

			const auto multiple_variable_index_value = cmoon::format("{0:} {0:} {1:}{2:}", "hello", "world", "!!!");
			cmoon::test::assert_equal(multiple_variable_index_expected, multiple_variable_index_value);
		}
};

class format_wide_chars_test : public cmoon::test::test_case
{
	public:
		format_wide_chars_test()
			: cmoon::test::test_case{"format_chars_test"} {}

		void operator()() override
		{
			const std::wstring default_expected{L"test"};
			const std::wstring left_align_expected{L"test  "};
			const std::wstring right_align_expected{L"  test"};
			const std::wstring center_align_expected{L" test "};
			const std::wstring left_fill_expected{L"test**"};
			const std::wstring right_fill_expected{L"**test"};
			const std::wstring center_fill_expected{L"*test*"};
			const std::wstring multiple_variable_expected{L"hello world!!!"};
			const std::wstring multiple_variable_index_expected{L"hello hello world!!!"};

			const auto default_value = cmoon::format(L"{}", L"test");
			cmoon::test::assert_equal(default_value, default_expected);

			const auto left_align_value = cmoon::format(L"{:<6}", L"test");
			cmoon::test::assert_equal(left_align_value, left_align_expected);

			const auto right_align_value = cmoon::format(L"{:>6}", L"test");
			cmoon::test::assert_equal(right_align_value, right_align_expected);

			const auto center_align_value = cmoon::format(L"{:^6}", L"test");
			cmoon::test::assert_equal(center_align_value, center_align_expected);

			const auto left_fill_value = cmoon::format(L"{:*<6}", L"test");
			cmoon::test::assert_equal(left_fill_value, left_fill_expected);

			const auto right_fill_value = cmoon::format(L"{:*>6}", L"test");
			cmoon::test::assert_equal(right_fill_value, right_fill_expected);

			const auto center_fill_value = cmoon::format(L"{:*^6}", L"test");
			cmoon::test::assert_equal(center_fill_value, center_fill_expected);

			const auto left_align_value2 = cmoon::format(L"{:<{}}", L"test", 6);
			cmoon::test::assert_equal(left_align_value2, left_align_expected);

			const auto right_align_value2 = cmoon::format(L"{:>{}}", L"test", 6);
			cmoon::test::assert_equal(right_align_value2, right_align_expected);

			const auto center_align_value2 = cmoon::format(L"{:^{}}", L"test", 6);
			cmoon::test::assert_equal(center_align_value2, center_align_expected);

			const auto left_fill_value2 = cmoon::format(L"{:*<{}}", L"test", 6);
			cmoon::test::assert_equal(left_fill_value2, left_fill_expected);

			const auto right_fill_value2 = cmoon::format(L"{:*>{}}", L"test", 6);
			cmoon::test::assert_equal(right_fill_value2, right_fill_expected);

			const auto center_fill_value2 = cmoon::format(L"{:*^{}}", L"test", 6);
			cmoon::test::assert_equal(center_fill_value2, center_fill_expected);

			const auto multiple_variable_value = cmoon::format(L"{} {}!!!", L"hello", L"world");
			cmoon::test::assert_equal(multiple_variable_value, multiple_variable_expected);

			const auto multiple_variable_index_value = cmoon::format(L"{0:} {0:} {1:}{2:}", L"hello", L"world", L"!!!");
			cmoon::test::assert_equal(multiple_variable_index_expected, multiple_variable_index_value);
		}
};

class format_string_view_test : public cmoon::test::test_case
{
	public:
		format_string_view_test()
			: cmoon::test::test_case{"format_string_view_test"} {}

		void operator()() override
		{
			const std::string default_expected{"test"};
			const std::string left_align_expected{"test  "};
			const std::string right_align_expected{"  test"};
			const std::string center_align_expected{" test "};
			const std::string left_fill_expected{"test**"};
			const std::string right_fill_expected{"**test"};
			const std::string center_fill_expected{"*test*"};
			const std::string multiple_variable_expected{"hello world!!!"};
			const std::string multiple_variable_index_expected{"hello hello world!!!"};

			const auto default_value = cmoon::format("{}", "test"sv);
			cmoon::test::assert_equal(default_value, default_expected);

			const auto left_align_value = cmoon::format("{:<6}", "test"sv);
			cmoon::test::assert_equal(left_align_value, left_align_expected);

			const auto right_align_value = cmoon::format("{:>6}", "test"sv);
			cmoon::test::assert_equal(right_align_value, right_align_expected);

			const auto center_align_value = cmoon::format("{:^6}", "test"sv);
			cmoon::test::assert_equal(center_align_value, center_align_expected);

			const auto left_fill_value = cmoon::format("{:*<6}", "test"sv);
			cmoon::test::assert_equal(left_fill_value, left_fill_expected);

			const auto right_fill_value = cmoon::format("{:*>6}", "test"sv);
			cmoon::test::assert_equal(right_fill_value, right_fill_expected);

			const auto center_fill_value = cmoon::format("{:*^6}", "test"sv);
			cmoon::test::assert_equal(center_fill_value, center_fill_expected);

			const auto left_align_value2 = cmoon::format("{:<{}}", "test"sv, 6);
			cmoon::test::assert_equal(left_align_value2, left_align_expected);

			const auto right_align_value2 = cmoon::format("{:>{}}", "test"sv, 6);
			cmoon::test::assert_equal(right_align_value2, right_align_expected);

			const auto center_align_value2 = cmoon::format("{:^{}}", "test"sv, 6);
			cmoon::test::assert_equal(center_align_value2, center_align_expected);

			const auto left_fill_value2 = cmoon::format("{:*<{}}", "test"sv, 6);
			cmoon::test::assert_equal(left_fill_value2, left_fill_expected);

			const auto right_fill_value2 = cmoon::format("{:*>{}}", "test"sv, 6);
			cmoon::test::assert_equal(right_fill_value2, right_fill_expected);

			const auto center_fill_value2 = cmoon::format("{:*^{}}", "test"sv, 6);
			cmoon::test::assert_equal(center_fill_value2, center_fill_expected);

			const auto multiple_variable_value = cmoon::format("{} {}!!!", "hello"sv, "world"sv);
			cmoon::test::assert_equal(multiple_variable_value, multiple_variable_expected);

			const auto multiple_variable_index_value = cmoon::format("{0:} {0:} {1:}{2:}", "hello"sv, "world"sv, "!!!"sv);
			cmoon::test::assert_equal(multiple_variable_index_expected, multiple_variable_index_value);
		}
};

class format_wide_string_view_test : public cmoon::test::test_case
{
	public:
		format_wide_string_view_test()
			: cmoon::test::test_case{"format_wide_string_view_test"} {}

		void operator()() override
		{
			const std::wstring default_expected{L"test"};
			const std::wstring left_align_expected{L"test  "};
			const std::wstring right_align_expected{L"  test"};
			const std::wstring center_align_expected{L" test "};
			const std::wstring left_fill_expected{L"test**"};
			const std::wstring right_fill_expected{L"**test"};
			const std::wstring center_fill_expected{L"*test*"};
			const std::wstring multiple_variable_expected{L"hello world!!!"};
			const std::wstring multiple_variable_index_expected{L"hello hello world!!!"};

			const auto default_value = cmoon::format(L"{}", L"test"sv);
			cmoon::test::assert_equal(default_value, default_expected);

			const auto left_align_value = cmoon::format(L"{:<6}", L"test"sv);
			cmoon::test::assert_equal(left_align_value, left_align_expected);

			const auto right_align_value = cmoon::format(L"{:>6}", L"test"sv);
			cmoon::test::assert_equal(right_align_value, right_align_expected);

			const auto center_align_value = cmoon::format(L"{:^6}", L"test"sv);
			cmoon::test::assert_equal(center_align_value, center_align_expected);

			const auto left_fill_value = cmoon::format(L"{:*<6}", L"test"sv);
			cmoon::test::assert_equal(left_fill_value, left_fill_expected);

			const auto right_fill_value = cmoon::format(L"{:*>6}", L"test"sv);
			cmoon::test::assert_equal(right_fill_value, right_fill_expected);

			const auto center_fill_value = cmoon::format(L"{:*^6}", L"test"sv);
			cmoon::test::assert_equal(center_fill_value, center_fill_expected);

			const auto left_align_value2 = cmoon::format(L"{:<{}}", L"test"sv, 6);
			cmoon::test::assert_equal(left_align_value2, left_align_expected);

			const auto right_align_value2 = cmoon::format(L"{:>{}}", L"test"sv, 6);
			cmoon::test::assert_equal(right_align_value2, right_align_expected);

			const auto center_align_value2 = cmoon::format(L"{:^{}}", L"test"sv, 6);
			cmoon::test::assert_equal(center_align_value2, center_align_expected);

			const auto left_fill_value2 = cmoon::format(L"{:*<{}}", L"test"sv, 6);
			cmoon::test::assert_equal(left_fill_value2, left_fill_expected);

			const auto right_fill_value2 = cmoon::format(L"{:*>{}}", L"test"sv, 6);
			cmoon::test::assert_equal(right_fill_value2, right_fill_expected);

			const auto center_fill_value2 = cmoon::format(L"{:*^{}}", L"test"sv, 6);
			cmoon::test::assert_equal(center_fill_value2, center_fill_expected);

			const auto multiple_variable_value = cmoon::format(L"{} {}!!!", L"hello"sv, L"world"sv);
			cmoon::test::assert_equal(multiple_variable_value, multiple_variable_expected);

			const auto multiple_variable_index_value = cmoon::format(L"{0:} {0:} {1:}{2:}", L"hello"sv, L"world"sv, L"!!!"sv);
			cmoon::test::assert_equal(multiple_variable_index_expected, multiple_variable_index_value);
		}
};

class format_bool_test : public cmoon::test::test_case
{
	public:
		format_bool_test()
			: cmoon::test::test_case{"format_bool_test"} {}

		void operator()() override
		{
			const std::string true_default_expected{"true"};
			const std::string true_left_align_expected{"true  "};
			const std::string true_right_align_expected{"  true"};
			const std::string true_center_align_expected{" true "};
			const std::string true_left_fill_expected{"true**"};
			const std::string true_right_fill_expected{"**true"};
			const std::string true_center_fill_expected{"*true*"};
			const std::string true_multiple_variable_expected{"true world!!!"};
			const std::string true_multiple_variable_index_expected{"true true world!!!"};

			const std::string false_default_expected{"false"};
			const std::string false_left_align_expected{"false  "};
			const std::string false_right_align_expected{"  false"};
			const std::string false_center_align_expected{" false "};
			const std::string false_left_fill_expected{"false**"};
			const std::string false_right_fill_expected{"**false"};
			const std::string false_center_fill_expected{"*false*"};
			const std::string false_multiple_variable_expected{"false world!!!"};
			const std::string false_multiple_variable_index_expected{"false false world!!!"};
			
			const auto true_default_value = cmoon::format("{}", true);
			cmoon::test::assert_equal(true_default_value, true_default_expected);

			const auto true_left_align_value = cmoon::format("{:<6}", true);
			cmoon::test::assert_equal(true_left_align_value, true_left_align_expected);

			const auto true_right_align_value = cmoon::format("{:>6}", true);
			cmoon::test::assert_equal(true_right_align_value, true_right_align_expected);

			const auto true_center_align_value = cmoon::format("{:^6}", true);
			cmoon::test::assert_equal(true_center_align_value, true_center_align_expected);

			const auto true_left_fill_value = cmoon::format("{:*<6}", true);
			cmoon::test::assert_equal(true_left_fill_value, true_left_fill_expected);

			const auto true_right_fill_value = cmoon::format("{:*>6}", true);
			cmoon::test::assert_equal(true_right_fill_value, true_right_fill_expected);

			const auto true_center_fill_value = cmoon::format("{:*^6}", true);
			cmoon::test::assert_equal(true_center_fill_value, true_center_fill_expected);

			const auto true_left_align_value2 = cmoon::format("{:<{}}", true, 6);
			cmoon::test::assert_equal(true_left_align_value2, true_left_align_expected);

			const auto true_right_align_value2 = cmoon::format("{:>{}}", true, 6);
			cmoon::test::assert_equal(true_right_align_value2, true_right_align_expected);

			const auto true_center_align_value2 = cmoon::format("{:^{}}", true, 6);
			cmoon::test::assert_equal(true_center_align_value2, true_center_align_expected);

			const auto true_left_fill_value2 = cmoon::format("{:*<{}}", true, 6);
			cmoon::test::assert_equal(true_left_fill_value2, true_left_fill_expected);

			const auto true_right_fill_value2 = cmoon::format("{:*>{}}", true, 6);
			cmoon::test::assert_equal(true_right_fill_value2, true_right_fill_expected);

			const auto true_center_fill_value2 = cmoon::format("{:*^{}}", true, 6);
			cmoon::test::assert_equal(true_center_fill_value2, true_center_fill_expected);

			const auto true_multiple_variable_value = cmoon::format("{} {}!!!", true, "world"sv);
			cmoon::test::assert_equal(true_multiple_variable_value, true_multiple_variable_expected);

			const auto true_multiple_variable_index_value = cmoon::format("{0:} {0:} {1:}{2:}", true, "world"sv, "!!!"sv);
			cmoon::test::assert_equal(true_multiple_variable_index_expected, true_multiple_variable_index_value);

			const auto false_default_value = cmoon::format("{}", false);
			cmoon::test::assert_equal(false_default_value, false_default_expected);

			const auto false_left_align_value = cmoon::format("{:<7}", false);
			cmoon::test::assert_equal(false_left_align_value, false_left_align_expected);

			const auto false_right_align_value = cmoon::format("{:>7}", false);
			cmoon::test::assert_equal(false_right_align_value, false_right_align_expected);

			const auto false_center_align_value = cmoon::format("{:^7}", false);
			cmoon::test::assert_equal(false_center_align_value, false_center_align_expected);

			const auto false_left_fill_value = cmoon::format("{:*<7}", false);
			cmoon::test::assert_equal(false_left_fill_value, false_left_fill_expected);

			const auto false_right_fill_value = cmoon::format("{:*>7}", false);
			cmoon::test::assert_equal(false_right_fill_value, false_right_fill_expected);

			const auto false_center_fill_value = cmoon::format("{:*^7}", false);
			cmoon::test::assert_equal(false_center_fill_value, false_center_fill_expected);

			const auto false_left_align_value2 = cmoon::format("{:<{}}", false, 7);
			cmoon::test::assert_equal(false_left_align_value2, false_left_align_expected);

			const auto false_right_align_value2 = cmoon::format("{:>{}}", false, 7);
			cmoon::test::assert_equal(false_right_align_value2, false_right_align_expected);

			const auto false_center_align_value2 = cmoon::format("{:^{}}", false, 7);
			cmoon::test::assert_equal(false_center_align_value2, false_center_align_expected);

			const auto false_left_fill_value2 = cmoon::format("{:*<{}}", false, 7);
			cmoon::test::assert_equal(false_left_fill_value2, false_left_fill_expected);

			const auto false_right_fill_value2 = cmoon::format("{:*>{}}", false, 7);
			cmoon::test::assert_equal(false_right_fill_value2, false_right_fill_expected);

			const auto false_center_fill_value2 = cmoon::format("{:*^{}}", false, 7);
			cmoon::test::assert_equal(false_center_fill_value2, false_center_fill_expected);

			const auto false_multiple_variable_value = cmoon::format("{} {}!!!", false, "world"sv);
			cmoon::test::assert_equal(false_multiple_variable_value, false_multiple_variable_expected);

			const auto false_multiple_variable_index_value = cmoon::format("{0:} {0:} {1:}{2:}", false, "world"sv, "!!!"sv);
			cmoon::test::assert_equal(false_multiple_variable_index_expected, false_multiple_variable_index_value);
		}
};

class format_wide_bool_test : public cmoon::test::test_case
{
	public:
		format_wide_bool_test()
			: cmoon::test::test_case{"format_wide_bool_test"} {}

		void operator()() override
		{
			const std::wstring true_default_expected{L"true"};
			const std::wstring true_left_align_expected{L"true  "};
			const std::wstring true_right_align_expected{L"  true"};
			const std::wstring true_center_align_expected{L" true "};
			const std::wstring true_left_fill_expected{L"true**"};
			const std::wstring true_right_fill_expected{L"**true"};
			const std::wstring true_center_fill_expected{L"*true*"};
			const std::wstring true_multiple_variable_expected{L"true world!!!"};
			const std::wstring true_multiple_variable_index_expected{L"true true world!!!"};

			const std::wstring false_default_expected{L"false"};
			const std::wstring false_left_align_expected{L"false  "};
			const std::wstring false_right_align_expected{L"  false"};
			const std::wstring false_center_align_expected{L" false "};
			const std::wstring false_left_fill_expected{L"false**"};
			const std::wstring false_right_fill_expected{L"**false"};
			const std::wstring false_center_fill_expected{L"*false*"};
			const std::wstring false_multiple_variable_expected{L"false world!!!"};
			const std::wstring false_multiple_variable_index_expected{L"false false world!!!"};
			
			const auto true_default_value = cmoon::format(L"{}", true);
			cmoon::test::assert_equal(true_default_value, true_default_expected);

			const auto true_left_align_value = cmoon::format(L"{:<6}", true);
			cmoon::test::assert_equal(true_left_align_value, true_left_align_expected);

			const auto true_right_align_value = cmoon::format(L"{:>6}", true);
			cmoon::test::assert_equal(true_right_align_value, true_right_align_expected);

			const auto true_center_align_value = cmoon::format(L"{:^6}", true);
			cmoon::test::assert_equal(true_center_align_value, true_center_align_expected);

			const auto true_left_fill_value = cmoon::format(L"{:*<6}", true);
			cmoon::test::assert_equal(true_left_fill_value, true_left_fill_expected);

			const auto true_right_fill_value = cmoon::format(L"{:*>6}", true);
			cmoon::test::assert_equal(true_right_fill_value, true_right_fill_expected);

			const auto true_center_fill_value = cmoon::format(L"{:*^6}", true);
			cmoon::test::assert_equal(true_center_fill_value, true_center_fill_expected);

			const auto true_left_align_value2 = cmoon::format(L"{:<{}}", true, 6);
			cmoon::test::assert_equal(true_left_align_value2, true_left_align_expected);

			const auto true_right_align_value2 = cmoon::format(L"{:>{}}", true, 6);
			cmoon::test::assert_equal(true_right_align_value2, true_right_align_expected);

			const auto true_center_align_value2 = cmoon::format(L"{:^{}}", true, 6);
			cmoon::test::assert_equal(true_center_align_value2, true_center_align_expected);

			const auto true_left_fill_value2 = cmoon::format(L"{:*<{}}", true, 6);
			cmoon::test::assert_equal(true_left_fill_value2, true_left_fill_expected);

			const auto true_right_fill_value2 = cmoon::format(L"{:*>{}}", true, 6);
			cmoon::test::assert_equal(true_right_fill_value2, true_right_fill_expected);

			const auto true_center_fill_value2 = cmoon::format(L"{:*^{}}", true, 6);
			cmoon::test::assert_equal(true_center_fill_value2, true_center_fill_expected);

			const auto true_multiple_variable_value = cmoon::format(L"{} {}!!!", true, L"world"sv);
			cmoon::test::assert_equal(true_multiple_variable_value, true_multiple_variable_expected);

			const auto true_multiple_variable_index_value = cmoon::format(L"{0:} {0:} {1:}{2:}", true, L"world"sv, L"!!!"sv);
			cmoon::test::assert_equal(true_multiple_variable_index_expected, true_multiple_variable_index_value);

			const auto false_default_value = cmoon::format(L"{}", false);
			cmoon::test::assert_equal(false_default_value, false_default_expected);

			const auto false_left_align_value = cmoon::format(L"{:<7}", false);
			cmoon::test::assert_equal(false_left_align_value, false_left_align_expected);

			const auto false_right_align_value = cmoon::format(L"{:>7}", false);
			cmoon::test::assert_equal(false_right_align_value, false_right_align_expected);

			const auto false_center_align_value = cmoon::format(L"{:^7}", false);
			cmoon::test::assert_equal(false_center_align_value, false_center_align_expected);

			const auto false_left_fill_value = cmoon::format(L"{:*<7}", false);
			cmoon::test::assert_equal(false_left_fill_value, false_left_fill_expected);

			const auto false_right_fill_value = cmoon::format(L"{:*>7}", false);
			cmoon::test::assert_equal(false_right_fill_value, false_right_fill_expected);

			const auto false_center_fill_value = cmoon::format(L"{:*^7}", false);
			cmoon::test::assert_equal(false_center_fill_value, false_center_fill_expected);

			const auto false_left_align_value2 = cmoon::format(L"{:<{}}", false, 7);
			cmoon::test::assert_equal(false_left_align_value2, false_left_align_expected);

			const auto false_right_align_value2 = cmoon::format(L"{:>{}}", false, 7);
			cmoon::test::assert_equal(false_right_align_value2, false_right_align_expected);

			const auto false_center_align_value2 = cmoon::format(L"{:^{}}", false, 7);
			cmoon::test::assert_equal(false_center_align_value2, false_center_align_expected);

			const auto false_left_fill_value2 = cmoon::format(L"{:*<{}}", false, 7);
			cmoon::test::assert_equal(false_left_fill_value2, false_left_fill_expected);

			const auto false_right_fill_value2 = cmoon::format(L"{:*>{}}", false, 7);
			cmoon::test::assert_equal(false_right_fill_value2, false_right_fill_expected);

			const auto false_center_fill_value2 = cmoon::format(L"{:*^{}}", false, 7);
			cmoon::test::assert_equal(false_center_fill_value2, false_center_fill_expected);

			const auto false_multiple_variable_value = cmoon::format(L"{} {}!!!", false, L"world"sv);
			cmoon::test::assert_equal(false_multiple_variable_value, false_multiple_variable_expected);

			const auto false_multiple_variable_index_value = cmoon::format(L"{0:} {0:} {1:}{2:}", false, L"world"sv, L"!!!"sv);
			cmoon::test::assert_equal(false_multiple_variable_index_expected, false_multiple_variable_index_value);
		}
};

template<class T>
	requires(std::integral<T>)
class format_integral_test : public cmoon::test::test_case
{
	public:
		format_integral_test()
			: cmoon::test::test_case{std::string("format_int_test for type: ") + typeid(T).name()} {}
		
		void operator()() override
		{
			constexpr auto used_integral {static_cast<T>(123)};
			const std::string default_expected{"123"};
			const std::string left_align_expected{"123  "};
			const std::string right_align_expected{"  123"};
			const std::string center_align_expected{" 123 "};
			const std::string left_fill_expected{"123**"};
			const std::string right_fill_expected{"**123"};
			const std::string center_fill_expected{"*123*"};
			const std::string multiple_variable_expected{"123 world!!!"};
			const std::string multiple_variable_index_expected{"123 123 world!!!"};

			const std::string binary_expected{"1111011"};
			const std::string binary_small_expected{"0b1111011"};
			const std::string binary_big_expected{"0B1111011"};
			const std::string octal_expected{"173"};
			const std::string octal_hash_expected{"0173"};
			const std::string hex_small_expected{"7b"};
			const std::string hex_big_expected{"7B"};
			const std::string hex_small_hash_expected{"0x7b"};
			const std::string hex_big_hash_expected{"0X7B"};

			const std::string plus_sign_expected {"+123"};
			const std::string minus_sign_expected {"123"};
			const std::string space_sign_expected {" 123"};

			const auto default_value = cmoon::format("{}", used_integral);
			cmoon::test::assert_equal(default_value, default_expected);

			const auto left_align_value = cmoon::format("{:<5}", used_integral);
			cmoon::test::assert_equal(left_align_value, left_align_expected);

			const auto right_align_value = cmoon::format("{:>5}", used_integral);
			cmoon::test::assert_equal(right_align_value, right_align_expected);

			const auto center_align_value = cmoon::format("{:^5}", used_integral);
			cmoon::test::assert_equal(center_align_value, center_align_expected);

			const auto left_fill_value = cmoon::format("{:*<5}", used_integral);
			cmoon::test::assert_equal(left_fill_value, left_fill_expected);

			const auto right_fill_value = cmoon::format("{:*>5}", used_integral);
			cmoon::test::assert_equal(right_fill_value, right_fill_expected);

			const auto center_fill_value = cmoon::format("{:*^5}", used_integral);
			cmoon::test::assert_equal(center_fill_value, center_fill_expected);

			const auto left_align_value2 = cmoon::format("{:<{}}", used_integral, 5);
			cmoon::test::assert_equal(left_align_value2, left_align_expected);

			const auto right_align_value2 = cmoon::format("{:>{}}", used_integral, 5);
			cmoon::test::assert_equal(right_align_value2, right_align_expected);

			const auto center_align_value2 = cmoon::format("{:^{}}", used_integral, 5);
			cmoon::test::assert_equal(center_align_value2, center_align_expected);

			const auto left_fill_value2 = cmoon::format("{:*<{}}", used_integral, 5);
			cmoon::test::assert_equal(left_fill_value2, left_fill_expected);

			const auto right_fill_value2 = cmoon::format("{:*>{}}", used_integral, 5);
			cmoon::test::assert_equal(right_fill_value2, right_fill_expected);

			const auto center_fill_value2 = cmoon::format("{:*^{}}", used_integral, 5);
			cmoon::test::assert_equal(center_fill_value2, center_fill_expected);

			const auto multiple_variable_value = cmoon::format("{} {}!!!", used_integral, "world"sv);
			cmoon::test::assert_equal(multiple_variable_value, multiple_variable_expected);

			const auto multiple_variable_index_value = cmoon::format("{0:} {0:} {1:}{2:}", used_integral, "world"sv, "!!!"sv);
			cmoon::test::assert_equal(multiple_variable_index_expected, multiple_variable_index_value);

			const auto binary_value = cmoon::format("{:b}", used_integral);
			cmoon::test::assert_equal(binary_value, binary_expected);

			const auto binary_value2 = cmoon::format("{:B}", used_integral);
			cmoon::test::assert_equal(binary_value2, binary_expected);

			const auto binary_small_value = cmoon::format("{:#b}", used_integral);
			cmoon::test::assert_equal(binary_small_value, binary_small_expected);

			const auto binary_big_value = cmoon::format("{:#B}", used_integral);
			cmoon::test::assert_equal(binary_big_value, binary_big_expected);

			const auto octal_value = cmoon::format("{:o}", used_integral);
			cmoon::test::assert_equal(octal_value, octal_expected);

			const auto octal_hash_value = cmoon::format("{:#o}", used_integral);
			cmoon::test::assert_equal(octal_hash_value, octal_hash_expected);

			const auto hex_small_value = cmoon::format("{:x}", used_integral);
			cmoon::test::assert_equal(hex_small_value, hex_small_expected);

			const auto hex_big_value = cmoon::format("{:X}", used_integral);
			cmoon::test::assert_equal(hex_big_value, hex_big_expected);

			const auto hex_small_hash_value = cmoon::format("{:#x}", used_integral);
			cmoon::test::assert_equal(hex_small_hash_value, hex_small_hash_expected);

			const auto hex_big_hash_value = cmoon::format("{:#X}", used_integral);
			cmoon::test::assert_equal(hex_big_hash_value, hex_big_hash_expected);

			const auto decimal_value = cmoon::format("{:d}", used_integral);
			cmoon::test::assert_equal(decimal_value, default_expected);

			const auto decimal_value2 = cmoon::format("{:#d}", used_integral);
			cmoon::test::assert_equal(decimal_value2, default_expected);

			const auto plus_sign_value = cmoon::format("{:+}", used_integral);
			cmoon::test::assert_equal(plus_sign_value, plus_sign_expected);

			const auto minus_sign_value = cmoon::format("{:-}", used_integral);
			cmoon::test::assert_equal(minus_sign_value, minus_sign_expected);

			const auto space_sign_value = cmoon::format("{: }", used_integral);
			cmoon::test::assert_equal(space_sign_value, space_sign_expected);

			if constexpr (std::signed_integral<T>)
			{
				constexpr auto negative_used_integral{static_cast<T>(-44)};

				const std::string negative_default_expected{"-44"};
				const std::string negative_left_align_expected{"-44  "};
				const std::string negative_right_align_expected{"  -44"};
				const std::string negative_center_align_expected{" -44 "};
				const std::string negative_left_fill_expected{"-44**"};
				const std::string negative_right_fill_expected{"**-44"};
				const std::string negative_center_fill_expected{"*-44*"};
				const std::string negative_multiple_variable_expected{"-44 world!!!"};
				const std::string negative_multiple_variable_index_expected{"-44 -44 world!!!"};

				const std::string negative_binary_expected{"-101100"};
				const std::string negative_binary_small_expected{"0b-101100"};
				const std::string negative_binary_big_expected{"0B-101100"};
				const std::string negative_octal_expected{"-54"};
				const std::string negative_octal_hash_expected{"0-54"};
				const std::string negative_hex_small_expected{"-2c"};
				const std::string negative_hex_big_expected{"-2C"};
				const std::string negative_hex_small_hash_expected{"0x-2c"};
				const std::string negative_hex_big_hash_expected{"0X-2C"};

				const std::string negative_plus_sign_expected{"-44"};
				const std::string negative_minus_sign_expected{"-44"};
				const std::string negative_space_sign_expected{"-44"};

				const auto negative_default_value = cmoon::format("{}", negative_used_integral);
				cmoon::test::assert_equal(negative_default_value, negative_default_expected);

				const auto negative_left_align_value = cmoon::format("{:<5}", negative_used_integral);
				cmoon::test::assert_equal(negative_left_align_value, negative_left_align_expected);

				const auto negative_right_align_value = cmoon::format("{:>5}", negative_used_integral);
				cmoon::test::assert_equal(negative_right_align_value, negative_right_align_expected);

				const auto negative_center_align_value = cmoon::format("{:^5}", negative_used_integral);
				cmoon::test::assert_equal(negative_center_align_value, negative_center_align_expected);

				const auto negative_left_fill_value = cmoon::format("{:*<5}", negative_used_integral);
				cmoon::test::assert_equal(negative_left_fill_value, negative_left_fill_expected);

				const auto negative_right_fill_value = cmoon::format("{:*>5}", negative_used_integral);
				cmoon::test::assert_equal(negative_right_fill_value, negative_right_fill_expected);

				const auto negative_center_fill_value = cmoon::format("{:*^5}", negative_used_integral);
				cmoon::test::assert_equal(negative_center_fill_value, negative_center_fill_expected);

				const auto negative_left_align_value2 = cmoon::format("{:<{}}", negative_used_integral, 5);
				cmoon::test::assert_equal(negative_left_align_value2, negative_left_align_expected);

				const auto negative_right_align_value2 = cmoon::format("{:>{}}", negative_used_integral, 5);
				cmoon::test::assert_equal(negative_right_align_value2, negative_right_align_expected);

				const auto negative_center_align_value2 = cmoon::format("{:^{}}", negative_used_integral, 5);
				cmoon::test::assert_equal(negative_center_align_value2, negative_center_align_expected);

				const auto negative_left_fill_value2 = cmoon::format("{:*<{}}", negative_used_integral, 5);
				cmoon::test::assert_equal(negative_left_fill_value2, negative_left_fill_expected);

				const auto negative_right_fill_value2 = cmoon::format("{:*>{}}", negative_used_integral, 5);
				cmoon::test::assert_equal(negative_right_fill_value2, negative_right_fill_expected);

				const auto negative_center_fill_value2 = cmoon::format("{:*^{}}", negative_used_integral, 5);
				cmoon::test::assert_equal(negative_center_fill_value2, negative_center_fill_expected);

				const auto negative_multiple_variable_value = cmoon::format("{} {}!!!", negative_used_integral, "world"sv);
				cmoon::test::assert_equal(negative_multiple_variable_value, negative_multiple_variable_expected);

				const auto negative_multiple_variable_index_value = cmoon::format("{0:} {0:} {1:}{2:}", negative_used_integral, "world"sv, "!!!"sv);
				cmoon::test::assert_equal(negative_multiple_variable_index_expected, negative_multiple_variable_index_value);

				const auto negative_binary_value = cmoon::format("{:b}", negative_used_integral);
				cmoon::test::assert_equal(negative_binary_value, negative_binary_expected);

				const auto negative_binary_value2 = cmoon::format("{:B}", negative_used_integral);
				cmoon::test::assert_equal(negative_binary_value2, negative_binary_expected);

				const auto negative_binary_small_value = cmoon::format("{:#b}", negative_used_integral);
				cmoon::test::assert_equal(negative_binary_small_value, negative_binary_small_expected);

				const auto negative_binary_big_value = cmoon::format("{:#B}", negative_used_integral);
				cmoon::test::assert_equal(negative_binary_big_value, negative_binary_big_expected);

				const auto negative_octal_value = cmoon::format("{:o}", negative_used_integral);
				cmoon::test::assert_equal(negative_octal_value, negative_octal_expected);

				const auto negative_octal_hash_value = cmoon::format("{:#o}", negative_used_integral);
				cmoon::test::assert_equal(negative_octal_hash_value, negative_octal_hash_expected);

				const auto negative_hex_small_value = cmoon::format("{:x}", negative_used_integral);
				cmoon::test::assert_equal(negative_hex_small_value, negative_hex_small_expected);

				const auto negative_hex_big_value = cmoon::format("{:X}", negative_used_integral);
				cmoon::test::assert_equal(negative_hex_big_value, negative_hex_big_expected);

				const auto negative_hex_small_hash_value = cmoon::format("{:#x}", negative_used_integral);
				cmoon::test::assert_equal(negative_hex_small_hash_value, negative_hex_small_hash_expected);

				const auto negative_hex_big_hash_value = cmoon::format("{:#X}", negative_used_integral);
				cmoon::test::assert_equal(negative_hex_big_hash_value, negative_hex_big_hash_expected);

				const auto negative_decimal_value = cmoon::format("{:d}", negative_used_integral);
				cmoon::test::assert_equal(negative_decimal_value, negative_default_expected);

				const auto negative_decimal_value2 = cmoon::format("{:#d}", negative_used_integral);
				cmoon::test::assert_equal(negative_decimal_value2, negative_default_expected);

				const auto negative_plus_sign_value = cmoon::format("{:+}", negative_used_integral);
				cmoon::test::assert_equal(negative_plus_sign_value, negative_plus_sign_expected);

				const auto negative_minus_sign_value = cmoon::format("{:-}", negative_used_integral);
				cmoon::test::assert_equal(negative_minus_sign_value, negative_minus_sign_expected);

				const auto negative_space_sign_value = cmoon::format("{: }", negative_used_integral);
				cmoon::test::assert_equal(negative_space_sign_value, negative_space_sign_expected);
			}
		}
};

template<class T>
	requires(std::integral<T>)
class format_wide_integral_test : public cmoon::test::test_case
{
	public:
		format_wide_integral_test()
			: cmoon::test::test_case{std::string("format_wide_integral_test for type: ") + typeid(T).name()} {}
		
		void operator()() override
		{
			constexpr auto used_integral {static_cast<T>(123)};
			const std::wstring default_expected{L"123"};
			const std::wstring left_align_expected{L"123  "};
			const std::wstring right_align_expected{L"  123"};
			const std::wstring center_align_expected{L" 123 "};
			const std::wstring left_fill_expected{L"123**"};
			const std::wstring right_fill_expected{L"**123"};
			const std::wstring center_fill_expected{L"*123*"};
			const std::wstring multiple_variable_expected{L"123 world!!!"};
			const std::wstring multiple_variable_index_expected{L"123 123 world!!!"};

			const std::wstring binary_expected{L"1111011"};
			const std::wstring binary_small_expected{L"0b1111011"};
			const std::wstring binary_big_expected{L"0B1111011"};
			const std::wstring octal_expected{L"173"};
			const std::wstring octal_hash_expected{L"0173"};
			const std::wstring hex_small_expected{L"7b"};
			const std::wstring hex_big_expected{L"7B"};
			const std::wstring hex_small_hash_expected{L"0x7b"};
			const std::wstring hex_big_hash_expected{L"0X7B"};

			const std::wstring plus_sign_expected{L"+123"};
			const std::wstring minus_sign_expected{L"123"};
			const std::wstring space_sign_expected{L" 123"};

			const auto default_value = cmoon::format(L"{}", used_integral);
			cmoon::test::assert_equal(default_value, default_expected);

			const auto left_align_value = cmoon::format(L"{:<5}", used_integral);
			cmoon::test::assert_equal(left_align_value, left_align_expected);

			const auto right_align_value = cmoon::format(L"{:>5}", used_integral);
			cmoon::test::assert_equal(right_align_value, right_align_expected);

			const auto center_align_value = cmoon::format(L"{:^5}", used_integral);
			cmoon::test::assert_equal(center_align_value, center_align_expected);

			const auto left_fill_value = cmoon::format(L"{:*<5}", used_integral);
			cmoon::test::assert_equal(left_fill_value, left_fill_expected);

			const auto right_fill_value = cmoon::format(L"{:*>5}", used_integral);
			cmoon::test::assert_equal(right_fill_value, right_fill_expected);

			const auto center_fill_value = cmoon::format(L"{:*^5}", used_integral);
			cmoon::test::assert_equal(center_fill_value, center_fill_expected);

			const auto left_align_value2 = cmoon::format(L"{:<{}}", used_integral, 5);
			cmoon::test::assert_equal(left_align_value2, left_align_expected);

			const auto right_align_value2 = cmoon::format(L"{:>{}}", used_integral, 5);
			cmoon::test::assert_equal(right_align_value2, right_align_expected);

			const auto center_align_value2 = cmoon::format(L"{:^{}}", used_integral, 5);
			cmoon::test::assert_equal(center_align_value2, center_align_expected);

			const auto left_fill_value2 = cmoon::format(L"{:*<{}}", used_integral, 5);
			cmoon::test::assert_equal(left_fill_value2, left_fill_expected);

			const auto right_fill_value2 = cmoon::format(L"{:*>{}}", used_integral, 5);
			cmoon::test::assert_equal(right_fill_value2, right_fill_expected);

			const auto center_fill_value2 = cmoon::format(L"{:*^{}}", used_integral, 5);
			cmoon::test::assert_equal(center_fill_value2, center_fill_expected);

			const auto multiple_variable_value = cmoon::format(L"{} {}!!!", used_integral, L"world"sv);
			cmoon::test::assert_equal(multiple_variable_value, multiple_variable_expected);

			const auto multiple_variable_index_value = cmoon::format(L"{0:} {0:} {1:}{2:}", used_integral, L"world"sv, L"!!!"sv);
			cmoon::test::assert_equal(multiple_variable_index_expected, multiple_variable_index_value);

			const auto binary_value = cmoon::format(L"{:b}", used_integral);
			cmoon::test::assert_equal(binary_value, binary_expected);

			const auto binary_value2 = cmoon::format(L"{:B}", used_integral);
			cmoon::test::assert_equal(binary_value2, binary_expected);

			const auto binary_small_value = cmoon::format(L"{:#b}", used_integral);
			cmoon::test::assert_equal(binary_small_value, binary_small_expected);

			const auto binary_big_value = cmoon::format(L"{:#B}", used_integral);
			cmoon::test::assert_equal(binary_big_value, binary_big_expected);

			const auto octal_value = cmoon::format(L"{:o}", used_integral);
			cmoon::test::assert_equal(octal_value, octal_expected);

			const auto octal_hash_value = cmoon::format(L"{:#o}", used_integral);
			cmoon::test::assert_equal(octal_hash_value, octal_hash_expected);

			const auto hex_small_value = cmoon::format(L"{:x}", used_integral);
			cmoon::test::assert_equal(hex_small_value, hex_small_expected);

			const auto hex_big_value = cmoon::format(L"{:X}", used_integral);
			cmoon::test::assert_equal(hex_big_value, hex_big_expected);

			const auto hex_small_hash_value = cmoon::format(L"{:#x}", used_integral);
			cmoon::test::assert_equal(hex_small_hash_value, hex_small_hash_expected);

			const auto hex_big_hash_value = cmoon::format(L"{:#X}", used_integral);
			cmoon::test::assert_equal(hex_big_hash_value, hex_big_hash_expected);

			const auto decimal_value = cmoon::format(L"{:d}", used_integral);
			cmoon::test::assert_equal(decimal_value, default_expected);

			const auto decimal_value2 = cmoon::format(L"{:#d}", used_integral);
			cmoon::test::assert_equal(decimal_value2, default_expected);

			const auto plus_sign_value = cmoon::format(L"{:+}", used_integral);
			cmoon::test::assert_equal(plus_sign_value, plus_sign_expected);

			const auto minus_sign_value = cmoon::format(L"{:-}", used_integral);
			cmoon::test::assert_equal(minus_sign_value, minus_sign_expected);

			const auto space_sign_value = cmoon::format(L"{: }", used_integral);
			cmoon::test::assert_equal(space_sign_value, space_sign_expected);

			if constexpr (std::signed_integral<T>)
			{
				constexpr auto negative_used_integral{static_cast<T>(-44)};

				const std::wstring negative_default_expected{L"-44"};
				const std::wstring negative_left_align_expected{L"-44  "};
				const std::wstring negative_right_align_expected{L"  -44"};
				const std::wstring negative_center_align_expected{L" -44 "};
				const std::wstring negative_left_fill_expected{L"-44**"};
				const std::wstring negative_right_fill_expected{L"**-44"};
				const std::wstring negative_center_fill_expected{L"*-44*"};
				const std::wstring negative_multiple_variable_expected{L"-44 world!!!"};
				const std::wstring negative_multiple_variable_index_expected{L"-44 -44 world!!!"};

				const std::wstring negative_binary_expected{L"-101100"};
				const std::wstring negative_binary_small_expected{L"0b-101100"};
				const std::wstring negative_binary_big_expected{L"0B-101100"};
				const std::wstring negative_octal_expected{L"-54"};
				const std::wstring negative_octal_hash_expected{L"0-54"};
				const std::wstring negative_hex_small_expected{L"-2c"};
				const std::wstring negative_hex_big_expected{L"-2C"};
				const std::wstring negative_hex_small_hash_expected{L"0x-2c"};
				const std::wstring negative_hex_big_hash_expected{L"0X-2C"};

				const std::wstring negative_plus_sign_expected{L"-44"};
				const std::wstring negative_minus_sign_expected{L"-44"};
				const std::wstring negative_space_sign_expected{L"-44"};

				const auto negative_default_value = cmoon::format(L"{}", negative_used_integral);
				cmoon::test::assert_equal(negative_default_value, negative_default_expected);

				const auto negative_left_align_value = cmoon::format(L"{:<5}", negative_used_integral);
				cmoon::test::assert_equal(negative_left_align_value, negative_left_align_expected);

				const auto negative_right_align_value = cmoon::format(L"{:>5}", negative_used_integral);
				cmoon::test::assert_equal(negative_right_align_value, negative_right_align_expected);

				const auto negative_center_align_value = cmoon::format(L"{:^5}", negative_used_integral);
				cmoon::test::assert_equal(negative_center_align_value, negative_center_align_expected);

				const auto negative_left_fill_value = cmoon::format(L"{:*<5}", negative_used_integral);
				cmoon::test::assert_equal(negative_left_fill_value, negative_left_fill_expected);

				const auto negative_right_fill_value = cmoon::format(L"{:*>5}", negative_used_integral);
				cmoon::test::assert_equal(negative_right_fill_value, negative_right_fill_expected);

				const auto negative_center_fill_value = cmoon::format(L"{:*^5}", negative_used_integral);
				cmoon::test::assert_equal(negative_center_fill_value, negative_center_fill_expected);

				const auto negative_left_align_value2 = cmoon::format(L"{:<{}}", negative_used_integral, 5);
				cmoon::test::assert_equal(negative_left_align_value2, negative_left_align_expected);

				const auto negative_right_align_value2 = cmoon::format(L"{:>{}}", negative_used_integral, 5);
				cmoon::test::assert_equal(negative_right_align_value2, negative_right_align_expected);

				const auto negative_center_align_value2 = cmoon::format(L"{:^{}}", negative_used_integral, 5);
				cmoon::test::assert_equal(negative_center_align_value2, negative_center_align_expected);

				const auto negative_left_fill_value2 = cmoon::format(L"{:*<{}}", negative_used_integral, 5);
				cmoon::test::assert_equal(negative_left_fill_value2, negative_left_fill_expected);

				const auto negative_right_fill_value2 = cmoon::format(L"{:*>{}}", negative_used_integral, 5);
				cmoon::test::assert_equal(negative_right_fill_value2, negative_right_fill_expected);

				const auto negative_center_fill_value2 = cmoon::format(L"{:*^{}}", negative_used_integral, 5);
				cmoon::test::assert_equal(negative_center_fill_value2, negative_center_fill_expected);

				const auto negative_multiple_variable_value = cmoon::format(L"{} {}!!!", negative_used_integral, L"world"sv);
				cmoon::test::assert_equal(negative_multiple_variable_value, negative_multiple_variable_expected);

				const auto negative_multiple_variable_index_value = cmoon::format(L"{0:} {0:} {1:}{2:}", negative_used_integral, L"world"sv, L"!!!"sv);
				cmoon::test::assert_equal(negative_multiple_variable_index_expected, negative_multiple_variable_index_value);

				const auto negative_binary_value = cmoon::format(L"{:b}", negative_used_integral);
				cmoon::test::assert_equal(negative_binary_value, negative_binary_expected);

				const auto negative_binary_value2 = cmoon::format(L"{:B}", negative_used_integral);
				cmoon::test::assert_equal(negative_binary_value2, negative_binary_expected);

				const auto negative_binary_small_value = cmoon::format(L"{:#b}", negative_used_integral);
				cmoon::test::assert_equal(negative_binary_small_value, negative_binary_small_expected);

				const auto negative_binary_big_value = cmoon::format(L"{:#B}", negative_used_integral);
				cmoon::test::assert_equal(negative_binary_big_value, negative_binary_big_expected);

				const auto negative_octal_value = cmoon::format(L"{:o}", negative_used_integral);
				cmoon::test::assert_equal(negative_octal_value, negative_octal_expected);

				const auto negative_octal_hash_value = cmoon::format(L"{:#o}", negative_used_integral);
				cmoon::test::assert_equal(negative_octal_hash_value, negative_octal_hash_expected);

				const auto negative_hex_small_value = cmoon::format(L"{:x}", negative_used_integral);
				cmoon::test::assert_equal(negative_hex_small_value, negative_hex_small_expected);

				const auto negative_hex_big_value = cmoon::format(L"{:X}", negative_used_integral);
				cmoon::test::assert_equal(negative_hex_big_value, negative_hex_big_expected);

				const auto negative_hex_small_hash_value = cmoon::format(L"{:#x}", negative_used_integral);
				cmoon::test::assert_equal(negative_hex_small_hash_value, negative_hex_small_hash_expected);

				const auto negative_hex_big_hash_value = cmoon::format(L"{:#X}", negative_used_integral);
				cmoon::test::assert_equal(negative_hex_big_hash_value, negative_hex_big_hash_expected);

				const auto negative_decimal_value = cmoon::format(L"{:d}", negative_used_integral);
				cmoon::test::assert_equal(negative_decimal_value, negative_default_expected);

				const auto negative_decimal_value2 = cmoon::format(L"{:#d}", negative_used_integral);
				cmoon::test::assert_equal(negative_decimal_value2, negative_default_expected);

				const auto negative_plus_sign_value = cmoon::format(L"{:+}", negative_used_integral);
				cmoon::test::assert_equal(negative_plus_sign_value, negative_plus_sign_expected);

				const auto negative_minus_sign_value = cmoon::format(L"{:-}", negative_used_integral);
				cmoon::test::assert_equal(negative_minus_sign_value, negative_minus_sign_expected);

				const auto negative_space_sign_value = cmoon::format(L"{: }", negative_used_integral);
				cmoon::test::assert_equal(negative_space_sign_value, negative_space_sign_expected);
			}
		}
};

template<class T>
	requires(std::floating_point<T>)
class format_floating_test : public cmoon::test::test_case
{
	public:
		format_floating_test()
			: cmoon::test::test_case{std::string{"format_floating_test for type: "} + typeid(T).name()} {}

		void operator()() override
		{
			constexpr auto used_floating {static_cast<T>(123.48773)};
			const std::string default_expected{"123.48773"};
			const std::string left_align_expected{"123.48773  "};
			const std::string right_align_expected{"  123.48773"};
			const std::string center_align_expected{" 123.48773 "};
			const std::string left_fill_expected{"123.48773**"};
			const std::string right_fill_expected{"**123.48773"};
			const std::string center_fill_expected{"*123.48773*"};
			const std::string multiple_variable_expected{"123.48773 world!!!"};
			const std::string multiple_variable_index_expected{"123.48773 123.48773 world!!!"};

			const std::string fixed_expected {"123.487730"};
			const std::string fixed_precision_expected {"123.488"};

			const auto default_value = cmoon::format("{}", used_floating);
			cmoon::test::assert_equal(default_value, default_expected);

			const auto left_align_value = cmoon::format("{:<11}", used_floating);
			cmoon::test::assert_equal(left_align_value, left_align_expected);

			const auto right_align_value = cmoon::format("{:>11}", used_floating);
			cmoon::test::assert_equal(right_align_value, right_align_expected);

			const auto center_align_value = cmoon::format("{:^11}", used_floating);
			cmoon::test::assert_equal(center_align_value, center_align_expected);

			const auto left_fill_value = cmoon::format("{:*<11}", used_floating);
			cmoon::test::assert_equal(left_fill_value, left_fill_expected);

			const auto right_fill_value = cmoon::format("{:*>11}", used_floating);
			cmoon::test::assert_equal(right_fill_value, right_fill_expected);

			const auto center_fill_value = cmoon::format("{:*^11}", used_floating);
			cmoon::test::assert_equal(center_fill_value, center_fill_expected);

			const auto left_align_value2 = cmoon::format("{:<{}}", used_floating, 11);
			cmoon::test::assert_equal(left_align_value2, left_align_expected);

			const auto right_align_value2 = cmoon::format("{:>{}}", used_floating, 11);
			cmoon::test::assert_equal(right_align_value2, right_align_expected);

			const auto center_align_value2 = cmoon::format("{:^{}}", used_floating, 11);
			cmoon::test::assert_equal(center_align_value2, center_align_expected);

			const auto left_fill_value2 = cmoon::format("{:*<{}}", used_floating, 11);
			cmoon::test::assert_equal(left_fill_value2, left_fill_expected);

			const auto right_fill_value2 = cmoon::format("{:*>{}}", used_floating, 11);
			cmoon::test::assert_equal(right_fill_value2, right_fill_expected);

			const auto center_fill_value2 = cmoon::format("{:*^{}}", used_floating, 11);
			cmoon::test::assert_equal(center_fill_value2, center_fill_expected);

			const auto multiple_variable_value = cmoon::format("{} {}!!!", used_floating, "world"sv);
			cmoon::test::assert_equal(multiple_variable_value, multiple_variable_expected);

			const auto multiple_variable_index_value = cmoon::format("{0:} {0:} {1:}{2:}", used_floating, "world"sv, "!!!"sv);
			cmoon::test::assert_equal(multiple_variable_index_expected, multiple_variable_index_value);

			// Hex floats confuse me, so I don't know what value it actually should be.
			const auto hex_small_value = cmoon::format("{:a}", used_floating);
			const auto hex_big_value = cmoon::format("{:A}", used_floating);
			const auto hex_small_precision_value = cmoon::format("{:.3a}", used_floating);
			const auto hex_big_precision_value = cmoon::format("{:.3A}", used_floating);

			const auto scientific_small_value = cmoon::format("{:e}", used_floating);
			const auto scientific_big_value = cmoon::format("{:E}", used_floating);

			const auto fixed_value = cmoon::format("{:f}", used_floating);
			cmoon::test::assert_equal(fixed_value, fixed_expected);

			const auto fixed_value2 = cmoon::format("{:F}", used_floating);
			cmoon::test::assert_equal(fixed_value2, fixed_expected);
			cmoon::test::assert_equal(fixed_value, fixed_value2);

			const auto fixed_precision_value = cmoon::format("{:.3f}", used_floating);
			cmoon::test::assert_equal(fixed_precision_value, fixed_precision_expected);

			const auto fixed_precision_value2 = cmoon::format("{:.3F}", used_floating);
			cmoon::test::assert_equal(fixed_precision_value2, fixed_precision_expected);
			cmoon::test::assert_equal(fixed_precision_value, fixed_precision_value2);

			const auto general_value = cmoon::format("{:g}", used_floating);
			const auto general_value2 = cmoon::format("{:G}", used_floating);

			if constexpr (std::numeric_limits<T>::has_quiet_NaN)
			{
				const std::string nan_expected {"nan"};
				const std::string positive_nan_expected {"+nan"};
				const std::string space_nan_expected {" nan"};

				const auto nan_value = cmoon::format("{}", std::numeric_limits<T>::quiet_NaN());
				cmoon::test::assert_equal(nan_value, nan_expected);

				const auto nan_value2 = cmoon::format("{:-}", std::numeric_limits<T>::quiet_NaN());
				cmoon::test::assert_equal(nan_value2, nan_expected);

				const auto positive_nan_value = cmoon::format("{:+}", std::numeric_limits<T>::quiet_NaN());
				cmoon::test::assert_equal(positive_nan_value, positive_nan_expected);

				const auto space_nan_value = cmoon::format("{: }", std::numeric_limits<T>::quiet_NaN());
				cmoon::test::assert_equal(space_nan_value, space_nan_expected);
			}

			if constexpr (std::numeric_limits<T>::has_infinity)
			{
				const std::string inf_expected {"inf"};
				const std::string negative_inf_expected {"-inf"};
				const std::string positive_inf_expected {"+inf"};
				const std::string space_inf_expected {" inf"};

				const auto inf_value = cmoon::format("{}", std::numeric_limits<T>::infinity());
				cmoon::test::assert_equal(inf_value, inf_expected);

				const auto negative_inf_value = cmoon::format("{}", -std::numeric_limits<T>::infinity());
				cmoon::test::assert_equal(negative_inf_value, negative_inf_expected);

				const auto negative_inf_value2 = cmoon::format("{:+}", -std::numeric_limits<T>::infinity());
				cmoon::test::assert_equal(negative_inf_value2, negative_inf_expected);

				const auto negative_inf_value3 = cmoon::format("{: }", -std::numeric_limits<T>::infinity());
				cmoon::test::assert_equal(negative_inf_value3, negative_inf_expected);

				const auto negative_inf_value4 = cmoon::format("{:-}", -std::numeric_limits<T>::infinity());
				cmoon::test::assert_equal(negative_inf_value4, negative_inf_expected);

				const auto positive_inf_value = cmoon::format("{:+}", std::numeric_limits<T>::infinity());
				cmoon::test::assert_equal(positive_inf_value, positive_inf_expected);

				const auto space_inf_value = cmoon::format("{: }", std::numeric_limits<T>::infinity());
				cmoon::test::assert_equal(space_inf_value, space_inf_expected);
			}
		}
};

template<class T>
	requires(std::floating_point<T>)
class format_wide_floating_test : public cmoon::test::test_case
{
	public:
		format_wide_floating_test()
			: cmoon::test::test_case{std::string{"format_wide_floating_test for type: "} + typeid(T).name()} {}

		void operator()() override
		{
			constexpr auto used_floating {static_cast<T>(123.48773)};
			const std::wstring default_expected{L"123.488"};
			const std::wstring left_align_expected{L"123.488  "};
			const std::wstring right_align_expected{L"  123.488"};
			const std::wstring center_align_expected{L" 123.488 "};
			const std::wstring left_fill_expected{L"123.488**"};
			const std::wstring right_fill_expected{L"**123.488"};
			const std::wstring center_fill_expected{L"*123.488*"};
			const std::wstring multiple_variable_expected{L"123.488 world!!!"};
			const std::wstring multiple_variable_index_expected{L"123.488 123.488 world!!!"};

			const std::wstring fixed_expected {L"123.487730"};
			const std::wstring fixed_precision_expected {L"123.488"};

			const auto default_value = cmoon::format(L"{}", used_floating);
			cmoon::test::assert_equal(default_value, default_expected);

			const auto left_align_value = cmoon::format(L"{:<9}", used_floating);
			cmoon::test::assert_equal(left_align_value, left_align_expected);

			const auto right_align_value = cmoon::format(L"{:>9}", used_floating);
			cmoon::test::assert_equal(right_align_value, right_align_expected);

			const auto center_align_value = cmoon::format(L"{:^9}", used_floating);
			cmoon::test::assert_equal(center_align_value, center_align_expected);

			const auto left_fill_value = cmoon::format(L"{:*<9}", used_floating);
			cmoon::test::assert_equal(left_fill_value, left_fill_expected);

			const auto right_fill_value = cmoon::format(L"{:*>9}", used_floating);
			cmoon::test::assert_equal(right_fill_value, right_fill_expected);

			const auto center_fill_value = cmoon::format(L"{:*^9}", used_floating);
			cmoon::test::assert_equal(center_fill_value, center_fill_expected);

			const auto left_align_value2 = cmoon::format(L"{:<{}}", used_floating, 9);
			cmoon::test::assert_equal(left_align_value2, left_align_expected);

			const auto right_align_value2 = cmoon::format(L"{:>{}}", used_floating, 9);
			cmoon::test::assert_equal(right_align_value2, right_align_expected);

			const auto center_align_value2 = cmoon::format(L"{:^{}}", used_floating, 9);
			cmoon::test::assert_equal(center_align_value2, center_align_expected);

			const auto left_fill_value2 = cmoon::format(L"{:*<{}}", used_floating, 9);
			cmoon::test::assert_equal(left_fill_value2, left_fill_expected);

			const auto right_fill_value2 = cmoon::format(L"{:*>{}}", used_floating, 9);
			cmoon::test::assert_equal(right_fill_value2, right_fill_expected);

			const auto center_fill_value2 = cmoon::format(L"{:*^{}}", used_floating, 9);
			cmoon::test::assert_equal(center_fill_value2, center_fill_expected);

			const auto multiple_variable_value = cmoon::format(L"{} {}!!!", used_floating, L"world"sv);
			cmoon::test::assert_equal(multiple_variable_value, multiple_variable_expected);

			const auto multiple_variable_index_value = cmoon::format(L"{0:} {0:} {1:}{2:}", used_floating, L"world"sv, L"!!!"sv);
			cmoon::test::assert_equal(multiple_variable_index_expected, multiple_variable_index_value);

			// Hex floats confuse me, so I don't know what value it actually should be.
			const auto hex_small_value = cmoon::format(L"{:a}", used_floating);
			const auto hex_big_value = cmoon::format(L"{:A}", used_floating);
			const auto hex_small_precision_value = cmoon::format(L"{:.3a}", used_floating);
			const auto hex_big_precision_value = cmoon::format(L"{:.3A}", used_floating);

			const auto scientific_small_value = cmoon::format(L"{:e}", used_floating);
			const auto scientific_big_value = cmoon::format(L"{:E}", used_floating);

			const auto fixed_value = cmoon::format(L"{:f}", used_floating);
			cmoon::test::assert_equal(fixed_value, fixed_expected);

			const auto fixed_value2 = cmoon::format(L"{:F}", used_floating);
			cmoon::test::assert_equal(fixed_value2, fixed_expected);
			cmoon::test::assert_equal(fixed_value, fixed_value2);

			const auto fixed_precision_value = cmoon::format(L"{:.3f}", used_floating);
			cmoon::test::assert_equal(fixed_precision_value, fixed_precision_expected);

			const auto fixed_precision_value2 = cmoon::format(L"{:.3F}", used_floating);
			cmoon::test::assert_equal(fixed_precision_value2, fixed_precision_expected);
			cmoon::test::assert_equal(fixed_precision_value, fixed_precision_value2);

			const auto general_value = cmoon::format(L"{:g}", used_floating);
			const auto general_value2 = cmoon::format(L"{:G}", used_floating);

			if constexpr (std::numeric_limits<T>::has_quiet_NaN)
			{
				const std::wstring nan_expected {L"nan"};
				const std::wstring positive_nan_expected {L"+nan"};
				const std::wstring space_nan_expected {L" nan"};

				const auto nan_value = cmoon::format(L"{}", std::numeric_limits<T>::quiet_NaN());
				cmoon::test::assert_equal(nan_value, nan_expected);

				const auto nan_value2 = cmoon::format(L"{:-}", std::numeric_limits<T>::quiet_NaN());
				cmoon::test::assert_equal(nan_value2, nan_expected);

				const auto positive_nan_value = cmoon::format(L"{:+}", std::numeric_limits<T>::quiet_NaN());
				cmoon::test::assert_equal(positive_nan_value, positive_nan_expected);

				const auto space_nan_value = cmoon::format(L"{: }", std::numeric_limits<T>::quiet_NaN());
				cmoon::test::assert_equal(space_nan_value, space_nan_expected);
			}

			if constexpr (std::numeric_limits<T>::has_infinity)
			{
				const std::wstring inf_expected {L"inf"};
				const std::wstring negative_inf_expected {L"-inf"};
				const std::wstring positive_inf_expected {L"+inf"};
				const std::wstring space_inf_expected {L" inf"};

				const auto inf_value = cmoon::format(L"{}", std::numeric_limits<T>::infinity());
				cmoon::test::assert_equal(inf_value, inf_expected);

				const auto negative_inf_value = cmoon::format(L"{}", -std::numeric_limits<T>::infinity());
				cmoon::test::assert_equal(negative_inf_value, negative_inf_expected);

				const auto negative_inf_value2 = cmoon::format(L"{:+}", -std::numeric_limits<T>::infinity());
				cmoon::test::assert_equal(negative_inf_value2, negative_inf_expected);

				const auto negative_inf_value3 = cmoon::format(L"{: }", -std::numeric_limits<T>::infinity());
				cmoon::test::assert_equal(negative_inf_value3, negative_inf_expected);

				const auto negative_inf_value4 = cmoon::format(L"{:-}", -std::numeric_limits<T>::infinity());
				cmoon::test::assert_equal(negative_inf_value4, negative_inf_expected);

				const auto positive_inf_value = cmoon::format(L"{:+}", std::numeric_limits<T>::infinity());
				cmoon::test::assert_equal(positive_inf_value, positive_inf_expected);

				const auto space_inf_value = cmoon::format(L"{: }", std::numeric_limits<T>::infinity());
				cmoon::test::assert_equal(space_inf_value, space_inf_expected);
			}
		}
};

class format_pointer_test : public cmoon::test::test_case
{
	public:
		format_pointer_test()
			: cmoon::test::test_case{"format_pointer_test"} {}

		void operator()()
		{
			const std::string default_expected {"0x0"};

			const auto default_value = cmoon::format("{}", nullptr);
			cmoon::test::assert_equal(default_value, default_expected);
		}
};

class format_wide_pointer_test : public cmoon::test::test_case
{
	public:
		format_wide_pointer_test()
			: cmoon::test::test_case{"format_wide_pointer_test"} {}

		void operator()()
		{
			const std::wstring default_expected {L"0x0"};

			const auto default_value = cmoon::format(L"{}", nullptr);
			cmoon::test::assert_equal(default_value, default_expected);
		}
};

class format_time_point_test : public cmoon::test::test_case
{
	public:
		format_time_point_test()
			: cmoon::test::test_case{"format_time_point_test"} {}

		void operator()() override
		{
			const auto time = std::chrono::time_point<std::chrono::system_clock>{std::chrono::seconds{1'610'712'189}};
			
			const std::string literal_expected {"%"};
			const std::string newline_expected {"\n"};
			const std::string tab_expected {"\t"};
			const std::string century_expected {"20"};
			const std::string year_abbr_expected {"21"};
			const std::string year_expected {"2021"};
			const std::string month_expected {"01"};
			const std::string day_expected {"15"};
			const std::string weekday0_expected {"5"};
			const std::string weekday1_expected {"5"};
			const std::string day_of_year_expected {"015"};
			const std::string us_date_expected {"01/15/21"};
			const std::string iso_date_expected {"2021-01-15"};
			const std::string hour24_expected {"06"};
			const std::string hour12_expected {"06"};
			const std::string minute_expected {"03"};
			const std::string second_expected {"09"};
			const std::string hour_minute_expected {"06:03"};
			const std::string hour_minute_second_expected {"06:03:09"};
			const std::string timezone_expected{"UTC"};
			const std::string utc_offset_expected {"+0000"};
			const std::string utc_offset2_expected {"+00:00"};

			const auto literal_value = cmoon::format("{:%%}", time);
			cmoon::test::assert_equal(literal_value, literal_expected);

			const auto newline_value = cmoon::format("{:%n}", time);
			cmoon::test::assert_equal(newline_value, newline_expected);

			const auto tab_value = cmoon::format("{:%t}", time);
			cmoon::test::assert_equal(tab_value, tab_expected);

			const auto century_value = cmoon::format("{:%C}", time);
			cmoon::test::assert_equal(century_value, century_expected);

			const auto year_abbr_value = cmoon::format("{:%y}", time);
			cmoon::test::assert_equal(year_abbr_value, year_abbr_expected);

			const auto year_value = cmoon::format("{:%Y}", time);
			cmoon::test::assert_equal(year_value, year_expected);

			const auto month_value = cmoon::format("{:%m}", time);
			cmoon::test::assert_equal(month_value, month_expected);

			const auto day_value = cmoon::format("{:%d}", time);
			cmoon::test::assert_equal(day_value, day_expected);

			const auto weekday0_value = cmoon::format("{:%w}", time);
			cmoon::test::assert_equal(weekday0_value, weekday0_expected);

			const auto weekday1_value = cmoon::format("{:%u}", time);
			cmoon::test::assert_equal(weekday1_value, weekday1_expected);

			const auto day_of_year_value = cmoon::format("{:%j}", time);
			cmoon::test::assert_equal(day_of_year_value, day_of_year_expected);

			const auto us_date_value = cmoon::format("{:%D}", time);
			cmoon::test::assert_equal(us_date_value, us_date_expected);

			const auto iso_date_value = cmoon::format("{:%F}", time);
			cmoon::test::assert_equal(iso_date_value, iso_date_expected);

			const auto hour24_value = cmoon::format("{:%H}", time);
			cmoon::test::assert_equal(hour24_value, hour24_expected);

			const auto hour12_value = cmoon::format("{:%I}", time);
			cmoon::test::assert_equal(hour12_value, hour12_expected);

			const auto minute_value = cmoon::format("{:%M}", time);
			cmoon::test::assert_equal(minute_value, minute_expected);

			const auto second_value = cmoon::format("{:%S}", time);
			cmoon::test::assert_equal(second_value, second_expected);

			const auto hour_minute_value = cmoon::format("{:%R}", time);
			cmoon::test::assert_equal(hour_minute_value, hour_minute_expected);

			const auto hour_minute_second_value = cmoon::format("{:%T}", time);
			cmoon::test::assert_equal(hour_minute_second_value, hour_minute_second_expected);

			const auto timezone_value = cmoon::format("{:%Z}", time);
			cmoon::test::assert_equal(timezone_value, timezone_expected);

			const auto utc_offset_value = cmoon::format("{:%z}", time);
			cmoon::test::assert_equal(utc_offset_value, utc_offset_expected);

			const auto utc_offset2_value = cmoon::format("{:%Ez}", time);
			cmoon::test::assert_equal(utc_offset2_value, utc_offset2_expected);

			const auto utc_offset3_value = cmoon::format("{:%Oz}", time);
			cmoon::test::assert_equal(utc_offset3_value, utc_offset2_expected);

			// Running the locale versions to make sure they don't throw.
			// Will not check them due to unknown values from locale.
			cmoon::format("{:%EC}", time);
			cmoon::format("{:%Oy}", time);
			cmoon::format("{:%Ey}", time);
			cmoon::format("{:%EY}", time);
			cmoon::format("{:%b}", time);
			cmoon::format("{:%h}", time);
			cmoon::format("{:%B}", time);
			cmoon::format("{:%Om}", time);
			cmoon::format("{:%Od}", time);
			cmoon::format("{:%Oe}", time);
			cmoon::format("{:%a}", time);
			cmoon::format("{:%A}", time);
			cmoon::format("{:%Ou}", time);
			cmoon::format("{:%Ow}", time);
			cmoon::format("{:%g}", time);
			cmoon::format("{:%G}", time);
			cmoon::format("{:%V}", time);
			cmoon::format("{:%OV}", time);
			cmoon::format("{:%U}", time);
			cmoon::format("{:%OU}", time);
			cmoon::format("{:%W}", time);
			cmoon::format("{:%OW}", time);
			cmoon::format("{:%x}", time);
			cmoon::format("{:%Ex}", time);
			cmoon::format("{:%OH}", time);
			cmoon::format("{:%OI}", time);
			cmoon::format("{:%OM}", time);
			cmoon::format("{:%OS}", time);
			cmoon::format("{:%p}", time);
			cmoon::format("{:%r}", time);
			cmoon::format("{:%x}", time);
			cmoon::format("{:%EX}", time);
			cmoon::format("{:%c}", time);
			cmoon::format("{:%Ec}", time);
		}
};

class format_wide_time_point_test : public cmoon::test::test_case
{
	public:
		format_wide_time_point_test()
			: cmoon::test::test_case{"format_wide_time_point_test"} {}

		void operator()() override
		{
			const auto time = std::chrono::time_point<std::chrono::system_clock>{std::chrono::seconds{1'610'712'189}};
			
			const std::wstring literal_expected {L"%"};
			const std::wstring newline_expected {L"\n"};
			const std::wstring tab_expected {L"\t"};
			const std::wstring century_expected {L"20"};
			const std::wstring year_abbr_expected {L"21"};
			const std::wstring year_expected {L"2021"};
			const std::wstring month_expected {L"01"};
			const std::wstring day_expected {L"15"};
			const std::wstring weekday0_expected {L"5"};
			const std::wstring weekday1_expected {L"5"};
			const std::wstring day_of_year_expected {L"015"};
			const std::wstring us_date_expected {L"01/15/21"};
			const std::wstring iso_date_expected {L"2021-01-15"};
			const std::wstring hour24_expected {L"06"};
			const std::wstring hour12_expected {L"06"};
			const std::wstring minute_expected {L"03"};
			const std::wstring second_expected {L"09"};
			const std::wstring hour_minute_expected {L"06:03"};
			const std::wstring hour_minute_second_expected {L"06:03:09"};
			const std::wstring timezone_expected {L"UTC"};
			const std::wstring utc_offset_expected{L"+0000"};
			const std::wstring utc_offset2_expected{L"+00:00"};

			const auto literal_value = cmoon::format(L"{:%%}", time);
			cmoon::test::assert_equal(literal_value, literal_expected);

			const auto newline_value = cmoon::format(L"{:%n}", time);
			cmoon::test::assert_equal(newline_value, newline_expected);

			const auto tab_value = cmoon::format(L"{:%t}", time);
			cmoon::test::assert_equal(tab_value, tab_expected);

			const auto century_value = cmoon::format(L"{:%C}", time);
			cmoon::test::assert_equal(century_value, century_expected);

			const auto year_abbr_value = cmoon::format(L"{:%y}", time);
			cmoon::test::assert_equal(year_abbr_value, year_abbr_expected);

			const auto year_value = cmoon::format(L"{:%Y}", time);
			cmoon::test::assert_equal(year_value, year_expected);

			const auto month_value = cmoon::format(L"{:%m}", time);
			cmoon::test::assert_equal(month_value, month_expected);

			const auto day_value = cmoon::format(L"{:%d}", time);
			cmoon::test::assert_equal(day_value, day_expected);

			const auto weekday0_value = cmoon::format(L"{:%w}", time);
			cmoon::test::assert_equal(weekday0_value, weekday0_expected);

			const auto weekday1_value = cmoon::format(L"{:%u}", time);
			cmoon::test::assert_equal(weekday1_value, weekday1_expected);

			const auto day_of_year_value = cmoon::format(L"{:%j}", time);
			cmoon::test::assert_equal(day_of_year_value, day_of_year_expected);

			const auto us_date_value = cmoon::format(L"{:%D}", time);
			cmoon::test::assert_equal(us_date_value, us_date_expected);

			const auto iso_date_value = cmoon::format(L"{:%F}", time);
			cmoon::test::assert_equal(iso_date_value, iso_date_expected);

			const auto hour24_value = cmoon::format(L"{:%H}", time);
			cmoon::test::assert_equal(hour24_value, hour24_expected);

			const auto hour12_value = cmoon::format(L"{:%I}", time);
			cmoon::test::assert_equal(hour12_value, hour12_expected);

			const auto minute_value = cmoon::format(L"{:%M}", time);
			cmoon::test::assert_equal(minute_value, minute_expected);

			const auto second_value = cmoon::format(L"{:%S}", time);
			cmoon::test::assert_equal(second_value, second_expected);

			const auto hour_minute_value = cmoon::format(L"{:%R}", time);
			cmoon::test::assert_equal(hour_minute_value, hour_minute_expected);

			const auto hour_minute_second_value = cmoon::format(L"{:%T}", time);
			cmoon::test::assert_equal(hour_minute_second_value, hour_minute_second_expected);

			const auto timezone_value = cmoon::format(L"{:%Z}", time);
			cmoon::test::assert_equal(timezone_value, timezone_expected);

			const auto utc_offset_value = cmoon::format(L"{:%z}", time);
			cmoon::test::assert_equal(utc_offset_value, utc_offset_expected);

			const auto utc_offset2_value = cmoon::format(L"{:%Ez}", time);
			cmoon::test::assert_equal(utc_offset2_value, utc_offset2_expected);

			const auto utc_offset3_value = cmoon::format(L"{:%Oz}", time);
			cmoon::test::assert_equal(utc_offset3_value, utc_offset2_expected);

			// Running the locale versions to make sure they don't throw.
			// Will not check them due to unknown values from locale.
			cmoon::format(L"{:%EC}", time);
			cmoon::format(L"{:%Oy}", time);
			cmoon::format(L"{:%Ey}", time);
			cmoon::format(L"{:%EY}", time);
			cmoon::format(L"{:%b}", time);
			cmoon::format(L"{:%h}", time);
			cmoon::format(L"{:%B}", time);
			cmoon::format(L"{:%Om}", time);
			cmoon::format(L"{:%Od}", time);
			cmoon::format(L"{:%Oe}", time);
			cmoon::format(L"{:%a}", time);
			cmoon::format(L"{:%A}", time);
			cmoon::format(L"{:%Ou}", time);
			cmoon::format(L"{:%Ow}", time);
			cmoon::format(L"{:%g}", time);
			cmoon::format(L"{:%G}", time);
			cmoon::format(L"{:%V}", time);
			cmoon::format(L"{:%OV}", time);
			cmoon::format(L"{:%U}", time);
			cmoon::format(L"{:%OU}", time);
			cmoon::format(L"{:%W}", time);
			cmoon::format(L"{:%OW}", time);
			cmoon::format(L"{:%x}", time);
			cmoon::format(L"{:%Ex}", time);
			cmoon::format(L"{:%OH}", time);
			cmoon::format(L"{:%OI}", time);
			cmoon::format(L"{:%OM}", time);
			cmoon::format(L"{:%OS}", time);
			cmoon::format(L"{:%p}", time);
			cmoon::format(L"{:%r}", time);
			cmoon::format(L"{:%x}", time);
			cmoon::format(L"{:%EX}", time);
			cmoon::format(L"{:%c}", time);
			cmoon::format(L"{:%Ec}", time);
		}
};

class format_duration_test : public cmoon::test::test_case
{
	public:
		format_duration_test()
			: cmoon::test::test_case{"format_duration_test"} {}

		void operator()() override
		{
			constexpr auto overall_seconds = std::chrono::seconds{4856};

			const std::wstring seconds_expected {L"56"};
			const std::wstring minutes_expected {L"20"};
			const std::wstring hours_expected {L"01"};
			const std::wstring hour_minute_expected {L"01:20"};
			const std::wstring hour_minute_second_expected {L"01:20:56"};

			const auto hours_value = cmoon::format(L"{:%H}", overall_seconds);
			cmoon::test::assert_equal(hours_value, hours_expected);

			const auto hours_value2 = cmoon::format(L"{:%I}", overall_seconds);
			cmoon::test::assert_equal(hours_value2, hours_expected);

			const auto minutes_value = cmoon::format(L"{:%M}", overall_seconds);
			cmoon::test::assert_equal(minutes_value, minutes_expected);

			const auto seconds_value = cmoon::format(L"{:%S}", overall_seconds);
			cmoon::test::assert_equal(seconds_value, seconds_expected);

			const auto hour_minute_value = cmoon::format(L"{:%R}", overall_seconds);
			cmoon::test::assert_equal(hour_minute_value, hour_minute_expected);

			const auto hour_minute_second_value = cmoon::format(L"{:%T}", overall_seconds);
			cmoon::test::assert_equal(hour_minute_second_value, hour_minute_second_expected);

			// Running the locale versions to make sure they don't throw.
			// Will not check them due to unknown values from locale.
			cmoon::format(L"{:%OH}", overall_seconds);
			cmoon::format(L"{:%OI}", overall_seconds);
			cmoon::format(L"{:%OM}", overall_seconds);
			cmoon::format(L"{:%OS}", overall_seconds);
			cmoon::format(L"{:%p}", overall_seconds);
			cmoon::format(L"{:%r}", overall_seconds);
			cmoon::format(L"{:%X}", overall_seconds);
			cmoon::format(L"{:%EX}", overall_seconds);

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%C}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%EC}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%y}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%Oy}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%Ey}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%Y}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%EY}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%b}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%h}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%B}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%m}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%Om}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%d}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%Od}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%e}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%Oe}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%a}", overall_seconds);
			});
			
			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%A}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%u}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%Ou}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%w}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%Ow}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%g}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%G}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%V}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%OV}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%j}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%U}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%OU}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%W}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%OW}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%D}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%F}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%x}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%Ex}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%c}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%Ec}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%z}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%Ez}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%Oz}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format(L"{:%Z}", overall_seconds);
			});
		}
};

class format_wide_duration_test : public cmoon::test::test_case
{
	public:
		format_wide_duration_test()
			: cmoon::test::test_case{"format_wide_duration_test"} {}

		void operator()() override
		{
			constexpr auto overall_seconds = std::chrono::seconds{4856};

			const std::string seconds_expected {"56"};
			const std::string minutes_expected {"20"};
			const std::string hours_expected {"01"};
			const std::string hour_minute_expected {"01:20"};
			const std::string hour_minute_second_expected {"01:20:56"};

			const auto hours_value = cmoon::format("{:%H}", overall_seconds);
			cmoon::test::assert_equal(hours_value, hours_expected);

			const auto hours_value2 = cmoon::format("{:%I}", overall_seconds);
			cmoon::test::assert_equal(hours_value2, hours_expected);

			const auto minutes_value = cmoon::format("{:%M}", overall_seconds);
			cmoon::test::assert_equal(minutes_value, minutes_expected);

			const auto seconds_value = cmoon::format("{:%S}", overall_seconds);
			cmoon::test::assert_equal(seconds_value, seconds_expected);

			const auto hour_minute_value = cmoon::format("{:%R}", overall_seconds);
			cmoon::test::assert_equal(hour_minute_value, hour_minute_expected);

			const auto hour_minute_second_value = cmoon::format("{:%T}", overall_seconds);
			cmoon::test::assert_equal(hour_minute_second_value, hour_minute_second_expected);

			// Running the locale versions to make sure they don't throw.
			// Will not check them due to unknown values from locale.
			cmoon::format("{:%OH}", overall_seconds);
			cmoon::format("{:%OI}", overall_seconds);
			cmoon::format("{:%OM}", overall_seconds);
			cmoon::format("{:%OS}", overall_seconds);
			cmoon::format("{:%p}", overall_seconds);
			cmoon::format("{:%r}", overall_seconds);
			cmoon::format("{:%X}", overall_seconds);
			cmoon::format("{:%EX}", overall_seconds);

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%C}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%EC}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%y}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%Oy}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%Ey}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%Y}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%EY}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%b}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%h}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%B}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%m}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%Om}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%d}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%Od}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%e}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%Oe}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%a}", overall_seconds);
			});
			
			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%A}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%u}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%Ou}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%w}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%Ow}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%g}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%G}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%V}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%OV}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%j}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%U}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%OU}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%W}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%OW}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%D}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%F}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%x}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%Ex}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%c}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%Ec}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%z}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%Ez}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%Oz}", overall_seconds);
			});

			cmoon::test::assert_throws(cmoon::format_error{""}, [overall_seconds] {
				cmoon::format("{:%Z}", overall_seconds);
			});
		}
};

int main()
{
	cmoon::test::test_suite suite;

	suite.add_test_case<format_char_test>();
	suite.add_test_case<format_wide_char_test>();
	suite.add_test_case<format_chars_test>();
	suite.add_test_case<format_wide_chars_test>();
	suite.add_test_case<format_bool_test>();
	suite.add_test_case<format_wide_bool_test>();
	suite.add_test_case<format_integral_test<int>>();
	suite.add_test_case<format_wide_integral_test<int>>();
	suite.add_test_case<format_integral_test<unsigned int>>();
	suite.add_test_case<format_wide_integral_test<unsigned int>>();
	suite.add_test_case<format_integral_test<long long int>>();
	suite.add_test_case<format_wide_integral_test<long long int>>();
	suite.add_test_case<format_integral_test<unsigned long long int>>();
	suite.add_test_case<format_wide_integral_test<unsigned long long int>>();
	suite.add_test_case<format_floating_test<double>>();
	suite.add_test_case<format_floating_test<long double>>();
	suite.add_test_case<format_wide_floating_test<double>>();
	suite.add_test_case<format_wide_floating_test<long double>>();
	suite.add_test_case<format_pointer_test>();
	suite.add_test_case<format_wide_pointer_test>();
	suite.add_test_case<format_time_point_test>();
	suite.add_test_case<format_wide_time_point_test>();
	suite.add_test_case<format_duration_test>();
	suite.add_test_case<format_wide_duration_test>();

	cmoon::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}