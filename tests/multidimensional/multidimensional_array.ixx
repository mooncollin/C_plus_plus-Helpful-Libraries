export module cmoon.tests.multidimensional.multidimensional_array;

import cmoon.test;
import cmoon.multidimensional;

namespace cmoon::tests::multidimensional
{
	export
	class multidimensional_array_constructor_test : public cmoon::test::test_case
	{
		public:
			multidimensional_array_constructor_test()
				: cmoon::test::test_case{"multidimensional_array_constructor_test"} {}

			void operator()() override
			{
				const cmoon::multidimensional_array<int> m_arr{2, 3};
				cmoon::test::assert_equal(m_arr.dimensions(), std::vector<std::size_t>{2, 3});
				cmoon::test::assert_equal(m_arr.size(), 6);

				cmoon::test::assert_equal(m_arr.get(0, 0), 0);
				cmoon::test::assert_equal(m_arr.get(0, 1), 0);
				cmoon::test::assert_equal(m_arr.get(0, 2), 0);

				cmoon::test::assert_equal(m_arr.get(1, 0), 0);
				cmoon::test::assert_equal(m_arr.get(1, 1), 0);
				cmoon::test::assert_equal(m_arr.get(1, 2), 0);

				const cmoon::multidimensional_array<int> m_arr2{2, 3, 2};
				cmoon::test::assert_equal(m_arr2.dimensions(), std::vector<std::size_t>{2, 3, 2});
				cmoon::test::assert_equal(m_arr2.size(), 12);

				cmoon::test::assert_equal(m_arr2.get(0, 0, 0), 0);
				cmoon::test::assert_equal(m_arr2.get(0, 0, 1), 0);
				cmoon::test::assert_equal(m_arr2.get(0, 1, 0), 0);

				cmoon::test::assert_equal(m_arr2.get(0, 1, 1), 0);
				cmoon::test::assert_equal(m_arr2.get(0, 2, 0), 0);
				cmoon::test::assert_equal(m_arr2.get(0, 2, 1), 0);

				cmoon::test::assert_equal(m_arr2.get(1, 0, 0), 0);
				cmoon::test::assert_equal(m_arr2.get(1, 0, 1), 0);
				cmoon::test::assert_equal(m_arr2.get(1, 1, 0), 0);

				cmoon::test::assert_equal(m_arr2.get(1, 1, 1), 0);
				cmoon::test::assert_equal(m_arr2.get(1, 2, 0), 0);
				cmoon::test::assert_equal(m_arr2.get(1, 2, 1), 0);

				cmoon::fixed_multidimensional_array<int, 2> fixed {2, 2};
				fixed = {
					1, 2,
					3, 4
				};

				constexpr cmoon::static_multidimensional_array<int, 2, 2> constant = {1, 2, 3, 4};

				cmoon::multidimensional_array<int> goal{2, 2};
				goal = {
					1, 2,
					3, 4
				};

				cmoon::multidimensional_array<int> m_arr3{fixed};
				cmoon::multidimensional_array<int> m_arr4{constant};

				cmoon::test::assert_equal(m_arr3.get(0, 0), goal.get(0, 0));
				cmoon::test::assert_equal(m_arr3.get(0, 1), goal.get(0, 1));
				cmoon::test::assert_equal(m_arr3.get(1, 0), goal.get(1, 0));
				cmoon::test::assert_equal(m_arr3.get(1, 1), goal.get(1, 1));

				cmoon::test::assert_equal(m_arr4.get(0, 0), goal.get(0, 0));
				cmoon::test::assert_equal(m_arr4.get(0, 1), goal.get(0, 1));
				cmoon::test::assert_equal(m_arr4.get(1, 0), goal.get(1, 0));
				cmoon::test::assert_equal(m_arr4.get(1, 1), goal.get(1, 1));

				m_arr3 = constant;
				m_arr4 = fixed;

				cmoon::test::assert_equal(m_arr3.get(0, 0), goal.get(0, 0));
				cmoon::test::assert_equal(m_arr3.get(0, 1), goal.get(0, 1));
				cmoon::test::assert_equal(m_arr3.get(1, 0), goal.get(1, 0));
				cmoon::test::assert_equal(m_arr3.get(1, 1), goal.get(1, 1));

				cmoon::test::assert_equal(m_arr4.get(0, 0), goal.get(0, 0));
				cmoon::test::assert_equal(m_arr4.get(0, 1), goal.get(0, 1));
				cmoon::test::assert_equal(m_arr4.get(1, 0), goal.get(1, 0));
				cmoon::test::assert_equal(m_arr4.get(1, 1), goal.get(1, 1));
			}
	};

	export
	class multidimensional_array_assignment_test : public cmoon::test::test_case
	{
		public:
			multidimensional_array_assignment_test()
				: cmoon::test::test_case{"multidimensional_array_assignment_test"} {}

			void operator()() override
			{
				cmoon::multidimensional_array<int> m_arr{3, 2};

				m_arr = {1, 2, 3, 4, 5, 6};

				cmoon::test::assert_equal(m_arr.get(0, 0), 1);
				cmoon::test::assert_equal(m_arr.get(0, 1), 2);
				cmoon::test::assert_equal(m_arr.get(1, 0), 3);

				cmoon::test::assert_equal(m_arr.get(1, 1), 4);
				cmoon::test::assert_equal(m_arr.get(2, 0), 5);
				cmoon::test::assert_equal(m_arr.get(2, 1), 6);

				cmoon::multidimensional_array<int> m_arr2{2, 3, 2};

				m_arr2 = {
					1, 2, 3,
					4, 5, 6,
						7, 8, 9,
						10, 11, 12
				};

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

	export
	class multidimensional_array_value_test : public cmoon::test::test_case
	{
		public:
			multidimensional_array_value_test()
				: cmoon::test::test_case{"multidimensional_array_value_test"} {}

			void operator()() override
			{
				cmoon::multidimensional_array<int> m_arr{3, 2};

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

				cmoon::multidimensional_array<int> m_arr2{2, 3, 2};

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