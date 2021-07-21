export module cmoon.tests.measures.derived_unit;

import <ratio>;

import cmoon.test;
import cmoon.measures;
import cmoon.measures.distance;
import cmoon.measures.time;

namespace cmoon::tests::measures
{
    export
	class derived_unit_construction_test : public cmoon::test::test_case
    {
        public:
            derived_unit_construction_test()
                : cmoon::test::test_case{"derived_unit_construction_test"} {}

            void operator()() override
            {
                using t = cmoon::measures::meters;
                using t2 = cmoon::measures::basic_seconds<std::intmax_t, -1>;

                using t3 = cmoon::measures::kilometers;

                using derived_t = cmoon::measures::basic_derived_unit<std::intmax_t, std::ratio<1>, t, t2>;
                using derived_t2 = cmoon::measures::basic_derived_unit<std::intmax_t, std::ratio<1>, t3, t2>;
                using derived_t3 = cmoon::measures::basic_derived_unit<std::intmax_t,
                                        std::ratio<1>,
                                        cmoon::measures::basic_meters<std::intmax_t, -1>,
                                        cmoon::measures::basic_seconds<std::intmax_t, 1>>;

                derived_t derived {10};
                derived *= 10;
                derived += derived;

                derived_t2 derived2 {5};

                derived += derived2;

                const auto derived3 = derived * t{2};
                const auto derived4 = derived * cmoon::measures::basic_seconds<std::intmax_t, 1>{1};
                const auto derived5 = derived4 * cmoon::measures::basic_meters<std::intmax_t, -1>{1};
                const auto derived6 = derived / t2{1};
                const auto derived7 = derived * derived;
                const auto derived8 = derived * derived_t3{1};
            }
    };
}