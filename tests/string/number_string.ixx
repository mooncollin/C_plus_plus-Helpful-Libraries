export module cmoon.tests.string.number_string;

import cmoon.test;
import cmoon.string;

namespace cmoon::tests::string
{
	export
    class number_string_default_constructor_test : public cmoon::test::test_case
	{
		public:
			number_string_default_constructor_test()
				: cmoon::test::test_case{"number_string_default_constructor_test"} {}

			void operator()() final
			{
				constexpr cmoon::number_string<int> s;
				constexpr cmoon::number_string<unsigned int> s2;
				constexpr cmoon::number_string<float> s3;
				constexpr cmoon::number_string<double> s4;

				cmoon::test::assert_equal(s.size(), 1);
				cmoon::test::assert_equal(s.view(), "0");

				cmoon::test::assert_equal(s2.size(), 1);
				cmoon::test::assert_equal(s2.view(), "0");

				cmoon::test::assert_equal(s3.size(), 1);
				cmoon::test::assert_equal(s3.view(), "0");

				cmoon::test::assert_equal(s4.size(), 1);
				cmoon::test::assert_equal(s4.view(), "0");
			}
	};

	export
	class number_string_integer_test : public cmoon::test::test_case
	{
		public:
			number_string_integer_test()
				: cmoon::test::test_case{"number_string_integer_test"} {}

			void operator()() final
			{
				const cmoon::number_string s1{15};
				const cmoon::number_string s2{-23894823};
				const cmoon::number_string s3{0};

				cmoon::test::assert_equal(s1.size(), 2);
				cmoon::test::assert_equal(s1.view(), "15");

				cmoon::test::assert_equal(s2.size(), 9);
				cmoon::test::assert_equal(s2.view(), "-23894823");

				cmoon::test::assert_equal(s3.size(), 1);
				cmoon::test::assert_equal(s3.view(), "0");
			}
	};

	export
	class number_string_float_test : public cmoon::test::test_case
	{
		public:
			number_string_float_test()
				: cmoon::test::test_case{"number_string_float_test"} {}

			void operator()() final
			{
				const cmoon::number_string s1{45.22f};
				const cmoon::number_string s2{-893.38290};
				const cmoon::number_string<double> s3{0};

				cmoon::test::assert_equal(s1.size(), 5);
				cmoon::test::assert_equal(s1.view(), "45.22");

				cmoon::test::assert_equal(s2.size(), 9);
				cmoon::test::assert_equal(s2.view(), "-893.3829");

				cmoon::test::assert_equal(s3.size(), 1);
				cmoon::test::assert_equal(s3.view(), "0");
			}
	};
}