export module cmoon.tests.parallel.simd;

import <format>;

import cmoon.test;
import cmoon.parallel;

import cmoon.simd.impl.operations;

namespace cmoon::tests::parallel
{
	export
	template<class T>
	class simd_default_constructor_test : public cmoon::test::test_case
	{
		public:
			simd_default_constructor_test()
				: cmoon::test::test_case{std::format("simd_default_constructor_test ({})", typeid(T).name())} {}

			void operator()() final
			{
				cmoon::native_simd<T> {};
			}
	};

	export
	template<class T>
	class simd_fill_constructor_test : public cmoon::test::test_case
	{
		public:
			simd_fill_constructor_test()
				: cmoon::test::test_case{std::format("simd_fill_constructor_test ({})", typeid(T).name())} {}

			void operator()() final
			{
				constexpr T value = static_cast<T>(5);
				cmoon::native_simd<T> s{value};

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					cmoon::test::assert_equal(s[i], value);
				}
			}
	};

	export
	template<class T>
	class simd_load_constructor_test : public cmoon::test::test_case
	{
		public:
			simd_load_constructor_test()
				: cmoon::test::test_case{std::format("simd_load_constructor_test ({})", typeid(T).name())} {}

			void operator()() final
			{
				constexpr std::size_t size {cmoon::simd_abi::simd_size_v<T, cmoon::simd_abi::native<T>>};
				T data[size];
				int data2[size];
				for (std::size_t i {0}; i < size; ++i)
				{
					data[i] = static_cast<T>(i);
					data2[i] = static_cast<int>(i);
				}

				cmoon::native_simd<T> s{data, cmoon::element_aligned};

				for (std::size_t i {0}; i < size; ++i)
				{
					cmoon::test::assert_equal(s[i], static_cast<T>(i));
				}

				cmoon::native_simd<T> s2{data2, cmoon::element_aligned};

				for (std::size_t i {0}; i < size; ++i)
				{
					cmoon::test::assert_equal(s2[i], static_cast<int>(i));
				}
			}
	};

	export
	template<class T>
	class simd_set_test : public cmoon::test::test_case
	{
		public:
			simd_set_test()
				: cmoon::test::test_case{std::format("simd_set_test ({})", typeid(T).name())} {}

			void operator()() final
			{
				cmoon::native_simd<T> s;

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					s[i] = static_cast<T>(i);
				}

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					cmoon::test::assert_equal(s[i], static_cast<T>(i));
				}
			}
	};

	export
	template<class T>
	class simd_swap_test : public cmoon::test::test_case
	{
		public:
			simd_swap_test()
				: cmoon::test::test_case{std::format("simd_swap_test ({})", typeid(T).name())} {}

			void operator()() final
			{
				cmoon::native_simd<T> s;
				cmoon::native_simd<T> s2;

				s[0] = static_cast<T>(5);
				s2[0] = static_cast<T>(10);

				swap(s[0], s2[0]);

				cmoon::test::assert_equal(s[0], 10);
				cmoon::test::assert_equal(s2[0], 5);

				T v = static_cast<T>(15);
				swap(s[0], v);

				cmoon::test::assert_equal(s[0], 15);
				cmoon::test::assert_equal(v, 10);

				swap(v, s[0]);

				cmoon::test::assert_equal(s[0], 10);
				cmoon::test::assert_equal(v, 15);
			}
	};

	export
	template<class T>
	class simd_add_test : public cmoon::test::test_case
	{
		public:
			simd_add_test()
				: cmoon::test::test_case{std::format("simd_add_test ({})", typeid(T).name())} {}

			void operator()() final
			{
				constexpr T start = static_cast<T>(5);
				constexpr T addition = static_cast<T>(3);
				cmoon::native_simd<T> s{start};

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					s[i] += addition;
				}

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					cmoon::test::assert_equal(s[i], start + addition);
				}

				cmoon::native_simd<T> m1 {start};
				cmoon::native_simd<T> m2 {addition};
				cmoon::native_simd<T> m3 = m1 + m2;

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					cmoon::test::assert_equal(m3[i], start + addition);
				}
			}
	};

	export
	template<class T>
	class simd_sub_test : public cmoon::test::test_case
	{
		public:
			simd_sub_test()
				: cmoon::test::test_case{std::format("simd_sub_test ({})", typeid(T).name())} {}

			void operator()() final
			{
				constexpr T start = static_cast<T>(5);
				constexpr T subtraction = static_cast<T>(3);
				cmoon::native_simd<T> s{start};

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					s[i] -= subtraction;
				}

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					cmoon::test::assert_equal(s[i], start - subtraction);
				}

				cmoon::native_simd<T> m1 {start};
				cmoon::native_simd<T> m2 {subtraction};
				cmoon::native_simd<T> m3 = m1 - m2;

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					cmoon::test::assert_equal(m3[i], start - subtraction);
				}
			}
	};

	export
	template<class T>
	class simd_multiply_test : public cmoon::test::test_case
	{
		public:
			simd_multiply_test()
				: cmoon::test::test_case{std::format("simd_multiply_test ({})", typeid(T).name())} {}

			void operator()() final
			{
				constexpr T start = static_cast<T>(5);
				constexpr T multi = static_cast<T>(3);
				cmoon::native_simd<T> s{start};

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					s[i] *= multi;
				}

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					cmoon::test::assert_equal(s[i], start * multi);
				}

				cmoon::native_simd<T> m1 {start};
				cmoon::native_simd<T> m2 {multi};
				cmoon::native_simd<T> m3 = m1 * m2;

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					cmoon::test::assert_equal(m3[i], start * multi);
				}
			}
	};

	export
	template<class T>
	class simd_divide_test : public cmoon::test::test_case
	{
		public:
			simd_divide_test()
				: cmoon::test::test_case{std::format("simd_divide_test ({})", typeid(T).name())} {}

			void operator()() final
			{
				constexpr T start = static_cast<T>(6);
				constexpr T div = static_cast<T>(3);
				cmoon::native_simd<T> s{start};

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					s[i] /= div;
				}

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					cmoon::test::assert_equal(s[i], start / div);
				}

				cmoon::native_simd<T> m1 {start};
				cmoon::native_simd<T> m2 {div};
				cmoon::native_simd<T> m3 = m1 / m2;

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					cmoon::test::assert_equal(m3[i], start / div);
				}
			}
	};

	export
	template<class T>
	class simd_modulo_test : public cmoon::test::test_case
	{
		public:
			simd_modulo_test()
				: cmoon::test::test_case{std::format("simd_modulo_test ({})", typeid(T).name())} {}

			void operator()() final
			{
				constexpr T start = static_cast<T>(15);
				constexpr T mod = static_cast<T>(3);
				cmoon::native_simd<T> s{start};

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					s[i] %= mod;
				}

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					cmoon::test::assert_equal(s[i], start % mod);
				}

				cmoon::native_simd<T> m1 {start};
				cmoon::native_simd<T> m2 {mod};
				cmoon::native_simd<T> m3 = m1 % m2;

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					cmoon::test::assert_equal(m3[i], start % mod);
				}
			}
	};

	export
	template<class T>
	class simd_negate_test : public cmoon::test::test_case
	{
		public:
			simd_negate_test()
				: cmoon::test::test_case{std::format("simd_negate_test ({})", typeid(T).name())} {}

			void operator()() final
			{
				constexpr T start = static_cast<T>(15);
				cmoon::native_simd<T> s{start};
				const auto s2 = -s;

				for (std::size_t i {0}; i < s.size(); ++i)
				{
					cmoon::test::assert_equal(s2[i], -start);
				}
			}
	};

	export
	template<class T>
	class simd_equal_test : public cmoon::test::test_case
	{
		public:
			simd_equal_test()
				: cmoon::test::test_case{std::format("simd_equal_test ({})", typeid(T).name())} {}

			void operator()() final
			{
				constexpr T value = static_cast<T>(15);
				constexpr T value2 = static_cast<T>(10);
				cmoon::native_simd<T> s{value};
				cmoon::native_simd<T> s2{value};
				cmoon::native_simd<T> s3{value2};

				const auto r {s == s2};

				for (std::size_t i {0}; i < r.size(); ++i)
				{
					cmoon::test::assert_true(r[i]);
				}

				const auto r2 {s == s3};

				for (std::size_t i {0}; i < r2.size(); ++i)
				{
					cmoon::test::assert_false(r2[i]);
				}
			}
	};

	export
	template<class T>
	class simd_not_equal_test : public cmoon::test::test_case
	{
		public:
			simd_not_equal_test()
				: cmoon::test::test_case{std::format("simd_not_equal_test ({})", typeid(T).name())} {}

			void operator()() final
			{
				constexpr T value = static_cast<T>(15);
				constexpr T value2 = static_cast<T>(10);
				cmoon::native_simd<T> s{value};
				cmoon::native_simd<T> s2{value2};

				const auto r {s != s2};

				for (std::size_t i {0}; i < r.size(); ++i)
				{
					cmoon::test::assert_true(r[i]);
				}

				const auto r2 {s != s};

				for (std::size_t i {0}; i < r2.size(); ++i)
				{
					cmoon::test::assert_false(r2[i]);
				}
			}
	};

	export
	template<class T>
	class simd_less_than_test : public cmoon::test::test_case
	{
		public:
			simd_less_than_test()
				: cmoon::test::test_case{std::format("simd_less_than_test ({})", typeid(T).name())} {}

			void operator()() final
			{
				constexpr T value = static_cast<T>(15);
				constexpr T value2 = static_cast<T>(10);
				cmoon::native_simd<T> s{value};
				cmoon::native_simd<T> s2{value2};

				const auto r {s2 < s};

				for (std::size_t i {0}; i < r.size(); ++i)
				{
					cmoon::test::assert_true(r[i]);
				}

				const auto r2 {s < s};

				for (std::size_t i {0}; i < r2.size(); ++i)
				{
					cmoon::test::assert_false(r2[i]);
				}
			}
	};

	export
	template<class T>
	class simd_less_than_equal_test : public cmoon::test::test_case
	{
		public:
			simd_less_than_equal_test()
				: cmoon::test::test_case{std::format("simd_less_than_equal_test ({})", typeid(T).name())} {}

			void operator()() final
			{
				constexpr T value = static_cast<T>(15);
				constexpr T value2 = static_cast<T>(10);
				cmoon::native_simd<T> s{value};
				cmoon::native_simd<T> s2{value2};

				const auto r {s2 <= s};

				for (std::size_t i {0}; i < r.size(); ++i)
				{
					cmoon::test::assert_true(r[i]);
				}

				const auto r2 {s <= s};

				for (std::size_t i {0}; i < r2.size(); ++i)
				{
					cmoon::test::assert_true(r2[i]);
				}
			}
	};

	export
	template<class T>
	class simd_greater_than_test : public cmoon::test::test_case
	{
		public:
			simd_greater_than_test()
				: cmoon::test::test_case{std::format("simd_greater_than_test ({})", typeid(T).name())} {}

			void operator()() final
			{
				constexpr T value = static_cast<T>(15);
				constexpr T value2 = static_cast<T>(10);
				cmoon::native_simd<T> s{value};
				cmoon::native_simd<T> s2{value2};

				const auto r {s > s2};

				for (std::size_t i {0}; i < r.size(); ++i)
				{
					cmoon::test::assert_true(r[i]);
				}

				const auto r2 {s > s};

				for (std::size_t i {0}; i < r2.size(); ++i)
				{
					cmoon::test::assert_false(r2[i]);
				}
			}
	};

	export
	template<class T>
	class simd_greater_than_equal_test : public cmoon::test::test_case
	{
		public:
			simd_greater_than_equal_test()
				: cmoon::test::test_case{std::format("simd_greater_than_equal_test ({})", typeid(T).name())} {}

			void operator()() final
			{
				constexpr T value = static_cast<T>(15);
				constexpr T value2 = static_cast<T>(10);
				cmoon::native_simd<T> s{value};
				cmoon::native_simd<T> s2{value2};

				const auto r {s >= s2};

				for (std::size_t i {0}; i < r.size(); ++i)
				{
					cmoon::test::assert_true(r[i]);
				}

				const auto r2 {s >= s};

				for (std::size_t i {0}; i < r2.size(); ++i)
				{
					cmoon::test::assert_true(r2[i]);
				}
			}
	};
}