export module cmoon.tests.multidimensional.fixed_multidimensional_array;

import cmoon.test;
import cmoon.multidimensional;

namespace cmoon::tests::multidimensional
{
	export
	class fixed_multidimensional_constructor_test : public cmoon::test::test_case
	{
		public:
			fixed_multidimensional_constructor_test()
				: cmoon::test::test_case{"fixed_multidimensional_constructor_test"} {}

			void operator()() override
			{
				const cmoon::fixed_multidimensional_array<int, 2> m_arr{2, 3};
				cmoon::test::assert_equal(m_arr.dimensions(), std::array<std::size_t, 2>{2, 3});
				cmoon::test::assert_equal(m_arr.size(), 6);

				cmoon::test::assert_equal(m_arr.get(0, 0), 0);
				cmoon::test::assert_equal(m_arr.get(0, 1), 0);
				cmoon::test::assert_equal(m_arr.get(0, 2), 0);

				cmoon::test::assert_equal(m_arr.get(1, 0), 0);
				cmoon::test::assert_equal(m_arr.get(1, 1), 0);
				cmoon::test::assert_equal(m_arr.get(1, 2), 0);

				constexpr cmoon::static_multidimensional_array<int, 2, 2> constant = {1, 2, 3, 4};

				cmoon::fixed_multidimensional_array<int, 2> goal{2, 2};
				goal = {
					1, 2,
					3, 4
				};

				cmoon::fixed_multidimensional_array<int, 2> m_arr2{constant};

				cmoon::test::assert_equal(m_arr2.get(0, 0), goal.get(0, 0));
				cmoon::test::assert_equal(m_arr2.get(0, 1), goal.get(0, 1));
				cmoon::test::assert_equal(m_arr2.get(1, 0), goal.get(1, 0));
				cmoon::test::assert_equal(m_arr2.get(1, 1), goal.get(1, 1));

				m_arr2 = constant;

				cmoon::test::assert_equal(m_arr2.get(0, 0), goal.get(0, 0));
				cmoon::test::assert_equal(m_arr2.get(0, 1), goal.get(0, 1));
				cmoon::test::assert_equal(m_arr2.get(1, 0), goal.get(1, 0));
				cmoon::test::assert_equal(m_arr2.get(1, 1), goal.get(1, 1));
			}
	};

	export
	class fixed_multidimensional_value_test : public cmoon::test::test_case
	{
		public:
			fixed_multidimensional_value_test()
				: cmoon::test::test_case{"fixed_multidimensional_value_test"} {}

			void operator()() override
			{
				cmoon::fixed_multidimensional_array<int, 2> m_arr{3, 2};

				m_arr.get(0, 0) = 1;
				m_arr.get(0, 1) = 2;
				m_arr.get(1, 0) = 3;

				m_arr.get(1, 1) = 4;
				m_arr.get(2, 0) = 5;
				m_arr.get(2, 1) = 6;

				cmoon::test::assert_equal(m_arr.get(0, 0), 1);
				cmoon::test::assert_equal(m_arr.get(0, 1), 2);
				cmoon::test::assert_equal(m_arr.get(1, 0), 3);

				cmoon::test::assert_equal(m_arr.get(1, 1), 4);
				cmoon::test::assert_equal(m_arr.get(2, 0), 5);
				cmoon::test::assert_equal(m_arr.get(2, 1), 6);

				cmoon::fixed_multidimensional_array<int, 3> m_arr2{2, 3, 2};

				m_arr2.get(0, 0, 0) = 1;
				m_arr2.get(0, 0, 1) = 2;
				m_arr2.get(0, 1, 0) = 3;

				m_arr2.get(0, 1, 1) = 4;
				m_arr2.get(0, 2, 0) = 5;
				m_arr2.get(0, 2, 1) = 6;

				m_arr2.get(1, 0, 0) = 7;
				m_arr2.get(1, 0, 1) = 8;
				m_arr2.get(1, 1, 0) = 9;

				m_arr2.get(1, 1, 1) = 10;
				m_arr2.get(1, 2, 0) = 11;
				m_arr2.get(1, 2, 1) = 12;

				cmoon::test::assert_equal(m_arr2.get(0, 0, 0), 1);
				cmoon::test::assert_equal(m_arr2.get(0, 0, 1), 2);
				cmoon::test::assert_equal(m_arr2.get(0, 1, 0), 3);

				cmoon::test::assert_equal(m_arr2.get(0, 1, 1), 4);
				cmoon::test::assert_equal(m_arr2.get(0, 2, 0), 5);
				cmoon::test::assert_equal(m_arr2.get(0, 2, 1), 6);

				cmoon::test::assert_equal(m_arr2.get(1, 0, 0), 7);
				cmoon::test::assert_equal(m_arr2.get(1, 0, 1), 8);
				cmoon::test::assert_equal(m_arr2.get(1, 1, 0), 9);

				cmoon::test::assert_equal(m_arr2.get(1, 1, 1), 10);
				cmoon::test::assert_equal(m_arr2.get(1, 2, 0), 11);
				cmoon::test::assert_equal(m_arr2.get(1, 2, 1), 12);
			}
	};
}