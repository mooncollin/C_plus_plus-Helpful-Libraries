export module cmoon.tests.multidimensional.static_multidimensional_array;

import cmoon.test;
import cmoon.multidimensional;

namespace cmoon::tests::multidimensional
{
	export
	class static_multidimensional_constructor_test : public cmoon::test::test_case
	{
		public:
			static_multidimensional_constructor_test()
				: cmoon::test::test_case{"static_multidimensional_constructor_test"} {}

			void operator()() override
			{
				constexpr cmoon::static_multidimensional_array<int, 2, 3, 2> m_arr
				{
					1, 2, 3,
					4, 5, 6,
						7, 8, 9,
						10, 11, 12
				};
				static_assert((sizeof(int) * 2 * 3 * 2) == sizeof(m_arr));
				static_assert(m_arr.dimensions() == std::array<std::size_t, 3>{2, 3, 2});
				static_assert(m_arr.size() == 12);

				static_assert(m_arr.get(0, 0, 0) == 1);
				static_assert(m_arr.get(0, 0, 1) == 2);
				static_assert(m_arr.get(0, 1, 0) == 3);

				static_assert(m_arr.get(0, 1, 1) == 4);
				static_assert(m_arr.get(0, 2, 0) == 5);
				static_assert(m_arr.get(0, 2, 1) == 6);

				static_assert(m_arr.get(1, 0, 0) == 7);
				static_assert(m_arr.get(1, 0, 1) == 8);
				static_assert(m_arr.get(1, 1, 0) == 9);

				static_assert(m_arr.get(1, 1, 1) == 10);
				static_assert(m_arr.get(1, 2, 0) == 11);
				static_assert(m_arr.get(1, 2, 1) == 12);

				constexpr cmoon::static_multidimensional_array<int, 2, 3> m_arr2
				{
					1, 2, 3,
					4, 5, 6
				};
				static_assert((sizeof(int) * 2 * 3) == sizeof(m_arr2));
				static_assert(m_arr2.dimensions() == std::array<std::size_t, 2>{2, 3});
				static_assert(m_arr2.size() == 6);

				static_assert(m_arr2.get(0, 0) == 1);
				static_assert(m_arr2.get(0, 1) == 2);
				static_assert(m_arr2.get(0, 2) == 3);

				static_assert(m_arr2.get(1, 0) == 4);
				static_assert(m_arr2.get(1, 1) == 5);
				static_assert(m_arr2.get(1, 2) == 6);

				constexpr cmoon::static_multidimensional_array<int, 3, 1> m_arr3
				{
					4,
					1,
					8
				};
				static_assert((sizeof(int) * 3 * 1) == sizeof(m_arr3));
				static_assert(m_arr3.dimensions() == std::array<std::size_t, 2>{3, 1});
				static_assert(m_arr3.size() == 3);

				static_assert(m_arr3.get(0, 0) == 4);
				static_assert(m_arr3.get(1, 0) == 1);
				static_assert(m_arr3.get(2, 0) == 8);
			}
	};
}