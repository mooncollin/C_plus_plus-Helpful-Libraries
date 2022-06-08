export module cmoon.tests.parallel;
export import cmoon.tests.parallel.simd;

import <utility>;

import cmoon.test;
import cmoon.concepts;
import cmoon.library;

namespace cmoon
{
	template<class T>
	void add_simd_tests(cmoon::test::test_suite& suite)
	{
		suite.add_test_case<cmoon::tests::parallel::simd_default_constructor_test<T>>();
		suite.add_test_case<cmoon::tests::parallel::simd_fill_constructor_test<T>>();
		suite.add_test_case<cmoon::tests::parallel::simd_set_test<T>>();
		suite.add_test_case<cmoon::tests::parallel::simd_swap_test<T>>();
		suite.add_test_case<cmoon::tests::parallel::simd_add_test<T>>();
		suite.add_test_case<cmoon::tests::parallel::simd_load_constructor_test<T>>();
		suite.add_test_case<cmoon::tests::parallel::simd_sub_test<T>>();
		suite.add_test_case<cmoon::tests::parallel::simd_multiply_test<T>>();
		suite.add_test_case<cmoon::tests::parallel::simd_divide_test<T>>();

		if constexpr (cmoon::modulus_assignable<T, T>)
		{
			suite.add_test_case<cmoon::tests::parallel::simd_modulo_test<T>>();
		}

		suite.add_test_case<cmoon::tests::parallel::simd_negate_test<T>>();
		suite.add_test_case<cmoon::tests::parallel::simd_equal_test<T>>();
		suite.add_test_case<cmoon::tests::parallel::simd_not_equal_test<T>>();
		suite.add_test_case<cmoon::tests::parallel::simd_less_than_test<T>>();
		suite.add_test_case<cmoon::tests::parallel::simd_less_than_equal_test<T>>();
		suite.add_test_case<cmoon::tests::parallel::simd_greater_than_test<T>>();
		suite.add_test_case<cmoon::tests::parallel::simd_greater_than_equal_test<T>>();
	}

	export
	template<>
	struct library_traits<library::parallel>
	{
		static cmoon::test::test_suite tests()
		{
			cmoon::test::test_suite suite{"parallel library tests"};
			add_simd_tests<float>(suite);
			add_simd_tests<double>(suite);
			add_simd_tests<std::int8_t>(suite);
			add_simd_tests<std::int16_t>(suite);
			add_simd_tests<std::int32_t>(suite);
			add_simd_tests<std::int64_t>(suite);
			return std::move(suite);
		}
	};
}