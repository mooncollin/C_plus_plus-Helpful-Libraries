export module cmoon.tests.ratio.is_ratio;

import <ratio>;

import cmoon.test;
import cmoon.ratio;

namespace cmoon::tests::ratio
{
	export
	class is_ratio_test : public cmoon::test::test_case
	{
		public:
			is_ratio_test()
				: cmoon::test::test_case{"is_ratio test"} {}

			void operator()() override
			{
				constexpr cmoon::is_ratio<int> v;
				constexpr cmoon::is_ratio<double> v2;
				constexpr cmoon::is_ratio<std::ratio<1, 1>> v3;

				cmoon::test::assert_false(v());
				cmoon::test::assert_false(v2());
				cmoon::test::assert_true(v3());

				static_assert(!v());
				static_assert(!v2());
				static_assert(v3());
			}
	};

	export
	class is_ratio_v_test : public cmoon::test::test_case
	{
		public:
			is_ratio_v_test()
				: cmoon::test::test_case{"is_ratio_v test"} {}

			void operator()() override
			{
				constexpr auto v = cmoon::is_ratio_v<int>;
				constexpr auto v2 = cmoon::is_ratio_v<double>;
				constexpr auto v3 = cmoon::is_ratio_v<std::ratio<1, 1>>;

				cmoon::test::assert_false(v);
				cmoon::test::assert_false(v2);
				cmoon::test::assert_true(v3);

				static_assert(!v);
				static_assert(!v2);
				static_assert(v3);
			}
	};

	export
	class ratio_type_test : public cmoon::test::test_case
	{
		public:
			ratio_type_test()
				: cmoon::test::test_case{"ratio_type test"} {}

			void operator()() override
			{
				constexpr auto v = cmoon::ratio_type<int>;
				constexpr auto v2 = cmoon::ratio_type<double>;
				constexpr auto v3 = cmoon::ratio_type<std::ratio<1, 1>>;

				cmoon::test::assert_false(v);
				cmoon::test::assert_false(v2);
				cmoon::test::assert_true(v3);

				static_assert(!v);
				static_assert(!v2);
				static_assert(v3);
			}
	};
}