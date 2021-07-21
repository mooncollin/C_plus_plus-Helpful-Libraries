export module cmoon.tests.geometry.polygon;

import cmoon.test;
import cmoon.geometry;

namespace cmoon::tests::geometry
{
	export
	class regular_polygon_construction_test : public cmoon::test::test_case
	{
		public:
			regular_polygon_construction_test()
				: cmoon::test::test_case{"regular_polygon_construction_test"} {}

			void operator()() override
			{
				constexpr cmoon::geometry::regular_polygon<int, 3> tri{10};
				constexpr cmoon::geometry::regular_polygon<int, 3> tri2{20};

				static_assert(tri.side() == 10);
				cmoon::test::assert_equal(tri.side(), 10);

				static_assert(cmoon::geometry::get_angle<1>(tri) == cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});
				static_assert(cmoon::geometry::get_angle<2>(tri) == cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});
				static_assert(cmoon::geometry::get_angle<3>(tri) == cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});

				cmoon::test::assert_equal(cmoon::geometry::get_angle<1>(tri), cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});
				cmoon::test::assert_equal(cmoon::geometry::get_angle<2>(tri), cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});
				cmoon::test::assert_equal(cmoon::geometry::get_angle<3>(tri), cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});
			
				constexpr auto angles = cmoon::geometry::get_angles(tri);

				static_assert(std::get<0>(angles) == cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});
				static_assert(std::get<1>(angles) == cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});
				static_assert(std::get<1>(angles) == cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});

				cmoon::test::assert_equal(std::get<0>(angles), cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});
				cmoon::test::assert_equal(std::get<1>(angles), cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});
				cmoon::test::assert_equal(std::get<1>(angles), cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});

				static_assert(cmoon::geometry::get_side<1>(tri) == 10);
				static_assert(cmoon::geometry::get_side<2>(tri) == 10);
				static_assert(cmoon::geometry::get_side<3>(tri) == 10);

				cmoon::test::assert_equal(cmoon::geometry::get_side<1>(tri), 10);
				cmoon::test::assert_equal(cmoon::geometry::get_side<2>(tri), 10);
				cmoon::test::assert_equal(cmoon::geometry::get_side<3>(tri), 10);

				constexpr auto sides = cmoon::geometry::get_sides(tri);

				static_assert(std::get<0>(sides) == 10);
				static_assert(std::get<1>(sides) == 10);
				static_assert(std::get<2>(sides) == 10);

				cmoon::test::assert_equal(std::get<0>(sides), 10);
				cmoon::test::assert_equal(std::get<1>(sides), 10);
				cmoon::test::assert_equal(std::get<2>(sides), 10);

				static_assert(cmoon::geometry::similar(tri, tri2));
				static_assert(!cmoon::geometry::congruent(tri, tri2));

				cmoon::test::assert_true(cmoon::geometry::similar(tri, tri2));
				cmoon::test::assert_false(cmoon::geometry::congruent(tri, tri2));

				static_assert(cmoon::geometry::perimeter(tri) == 30);
				cmoon::test::assert_equal(cmoon::geometry::perimeter(tri), 30);

				static_assert(cmoon::geometry::semi_perimeter(tri) == 15);
				cmoon::test::assert_equal(cmoon::geometry::semi_perimeter(tri), 15);

				static_assert(cmoon::geometry::regular(tri));
				cmoon::test::assert_true(cmoon::geometry::regular(tri));
			}
	};
}