export module cmoon.tests.string.to_string;

import cmoon.test;
import cmoon.string;

namespace cmoon::tests::string
{
	export
	class to_string_arithmetic_test : public cmoon::test::test_case
	{
		public:
			to_string_arithmetic_test()
				: cmoon::test::test_case{"to_string_arithmetic_test"} {}

			void operator()() final
			{
				cmoon::test::assert_equal(
					cmoon::to_string(45),
					"45"
				);

				cmoon::test::assert_equal(
					cmoon::to_string(12.44),
					"12.44"
				);

				cmoon::test::assert_equal(
					cmoon::to_string(20u),
					"20"
				);

				cmoon::test::assert_equal(
					cmoon::to_string(123.4f),
					"123.4"
				);
			}
	};

	export
	class to_wstring_arithmetic_test : public cmoon::test::test_case
	{
		public:
			to_wstring_arithmetic_test()
				: cmoon::test::test_case{"to_wstring_arithmetic_test"} {}

			void operator()() final
			{
				cmoon::test::assert_equal(
					cmoon::to_wstring(45),
					L"45"
				);

				cmoon::test::assert_equal(
					cmoon::to_wstring(12.44),
					L"12.44"
				);

				cmoon::test::assert_equal(
					cmoon::to_wstring(20u),
					L"20"
				);

				cmoon::test::assert_equal(
					cmoon::to_wstring(123.4f),
					L"123.4"
				);
			}
	};
}