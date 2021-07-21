export module cmoon.tests.measures.distance;

import <sstream>;
import <format>;

import cmoon.test;
import cmoon.measures;
import cmoon.measures.distance;

namespace cmoon::tests::measures
{
    export
	class distance_string_test : public cmoon::test::test_case
    {
        public:
            distance_string_test()
                : cmoon::test::test_case{"distance_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::attometers{10};
                cmoon::test::assert_equal(ss.str(), "10am");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::attometers{10}), "10am");

                ss.str("");
                ss << cmoon::measures::femtometers{10};
                cmoon::test::assert_equal(ss.str(), "10fm");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::femtometers{10}), "10fm");

                ss.str("");
                ss << cmoon::measures::picometers{10};
                cmoon::test::assert_equal(ss.str(), "10pm");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::picometers{10}), "10pm");

                ss.str("");
                ss << cmoon::measures::nanometers{10};
                cmoon::test::assert_equal(ss.str(), "10nm");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::nanometers{10}), "10nm");

                ss.str("");
                ss << cmoon::measures::micrometers{10};
                cmoon::test::assert_equal(ss.str(), "10um");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::micrometers{10}), "10um");

                ss.str("");
                ss << cmoon::measures::millimeters{10};
                cmoon::test::assert_equal(ss.str(), "10mm");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::millimeters{10}), "10mm");

                ss.str("");
                ss << cmoon::measures::centimeters{10};
                cmoon::test::assert_equal(ss.str(), "10cm");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::centimeters{10}), "10cm");

                ss.str("");
                ss << cmoon::measures::decimeters{10};
                cmoon::test::assert_equal(ss.str(), "10dm");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::decimeters{10}), "10dm");

                ss.str("");
                ss << cmoon::measures::meters{10};
                cmoon::test::assert_equal(ss.str(), "10m");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::meters{10}), "10m");

                ss.str("");
                ss << cmoon::measures::decameters{10};
                cmoon::test::assert_equal(ss.str(), "10dam");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::decameters{10}), "10dam");

                ss.str("");
                ss << cmoon::measures::hectometers{10};
                cmoon::test::assert_equal(ss.str(), "10hm");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::hectometers{10}), "10hm");

                ss.str("");
                ss << cmoon::measures::kilometers{10};
                cmoon::test::assert_equal(ss.str(), "10km");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::kilometers{10}), "10km");

                ss.str("");
                ss << cmoon::measures::megameters{10};
                cmoon::test::assert_equal(ss.str(), "10Mm");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::megameters{10}), "10Mm");

                ss.str("");
                ss << cmoon::measures::gigameters{10};
                cmoon::test::assert_equal(ss.str(), "10Gm");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::gigameters{10}), "10Gm");

                ss.str("");
                ss << cmoon::measures::terameters{10};
                cmoon::test::assert_equal(ss.str(), "10Tm");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::terameters{10}), "10Tm");

                ss.str("");
                ss << cmoon::measures::petameters{10};
                cmoon::test::assert_equal(ss.str(), "10Pm");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::petameters{10}), "10Pm");

                ss.str("");
                ss << cmoon::measures::exameters{10};
                cmoon::test::assert_equal(ss.str(), "10Em");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::exameters{10}), "10Em");

                ss.str("");
                ss << cmoon::measures::inches{10};
                cmoon::test::assert_equal(ss.str(), "10in");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::inches{10}), "10in");

                ss.str("");
                ss << cmoon::measures::feet{10};
                cmoon::test::assert_equal(ss.str(), "10ft");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::feet{10}), "10ft");

                ss.str("");
                ss << cmoon::measures::yards{10};
                cmoon::test::assert_equal(ss.str(), "10yd");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::yards{10}), "10yd");

                ss.str("");
                ss << cmoon::measures::miles{10};
                cmoon::test::assert_equal(ss.str(), "10mi");
                //cmoon::test::assert_equal(std::format("{}", cmoon::measures::miles{10}), "10mi");
            }
    };

    export
    class distance_values_test : public cmoon::test::test_case
    {
        public:
            distance_values_test()
                : cmoon::test::test_case{"distance_values_test"} {}

            void operator()() override
            {
                static_assert(cmoon::measures::meters{1} == cmoon::measures::attometers{1000000000000000000});
                static_assert(cmoon::measures::meters{1} == cmoon::measures::femtometers{1000000000000000});
                static_assert(cmoon::measures::meters{1} == cmoon::measures::picometers{1000000000000});
                static_assert(cmoon::measures::meters{1} == cmoon::measures::nanometers{1000000000});
                static_assert(cmoon::measures::meters{1} == cmoon::measures::micrometers{1000000});
                static_assert(cmoon::measures::meters{1} == cmoon::measures::millimeters{1000});
                static_assert(cmoon::measures::meters{1} == cmoon::measures::centimeters{100});
                static_assert(cmoon::measures::meters{1} == cmoon::measures::decimeters{10});
                static_assert(cmoon::measures::meters{1} == cmoon::measures::meters{1});
                static_assert(cmoon::measures::meters{10} == cmoon::measures::decameters{1});
                static_assert(cmoon::measures::meters{1000} == cmoon::measures::kilometers{1});
                static_assert(cmoon::measures::meters{1000000} == cmoon::measures::megameters{1});
                static_assert(cmoon::measures::meters{1000000000} == cmoon::measures::gigameters{1});
                static_assert(cmoon::measures::meters{1000000000000} == cmoon::measures::terameters{1});
                static_assert(cmoon::measures::meters{1000000000000000} == cmoon::measures::petameters{1});
                static_assert(cmoon::measures::meters{1000000000000000000} == cmoon::measures::exameters{1});

                static_assert(cmoon::measures::meters{1} <= cmoon::measures::attometers{1000000000000000000});
                static_assert(cmoon::measures::meters{1} <= cmoon::measures::femtometers{1000000000000000});
                static_assert(cmoon::measures::meters{1} <= cmoon::measures::picometers{1000000000000});
                static_assert(cmoon::measures::meters{1} <= cmoon::measures::nanometers{1000000000});
                static_assert(cmoon::measures::meters{1} <= cmoon::measures::micrometers{1000000});
                static_assert(cmoon::measures::meters{1} <= cmoon::measures::millimeters{1000});
                static_assert(cmoon::measures::meters{1} <= cmoon::measures::centimeters{100});
                static_assert(cmoon::measures::meters{1} <= cmoon::measures::decimeters{10});
                static_assert(cmoon::measures::meters{1} <= cmoon::measures::meters{1});
                static_assert(cmoon::measures::meters{10} <= cmoon::measures::decameters{1});
                static_assert(cmoon::measures::meters{1000} <= cmoon::measures::kilometers{1});
                static_assert(cmoon::measures::meters{1000000} <= cmoon::measures::megameters{1});
                static_assert(cmoon::measures::meters{1000000000} <= cmoon::measures::gigameters{1});
                static_assert(cmoon::measures::meters{1000000000000} <= cmoon::measures::terameters{1});
                static_assert(cmoon::measures::meters{1000000000000000} <= cmoon::measures::petameters{1});
                static_assert(cmoon::measures::meters{1000000000000000000} <= cmoon::measures::exameters{1});

                static_assert(cmoon::measures::system_cast<cmoon::measures::meters>(cmoon::measures::kilometers{1}) == cmoon::measures::meters{1000});

                static_assert(cmoon::measures::meters{1} * 10 == cmoon::measures::decameters{1});
                static_assert((cmoon::measures::meters{1} + cmoon::measures::meters{1} + cmoon::measures::meters{1}) == cmoon::measures::meters{3});
                static_assert((cmoon::measures::meters{3} - cmoon::measures::meters{2}) == cmoon::measures::meters{1});
                static_assert(cmoon::measures::meters{100} / 10 == cmoon::measures::meters{10});

                static_assert(cmoon::measures::inches{12} == cmoon::measures::feet{1});
                static_assert(cmoon::measures::inches{36} == cmoon::measures::yards{1});
                static_assert(cmoon::measures::inches{63360} == cmoon::measures::miles{1});
                static_assert(cmoon::measures::feet{3} == cmoon::measures::yards{1});
                static_assert(cmoon::measures::feet{5280} == cmoon::measures::miles{1});
                static_assert(cmoon::measures::yards{1760} == cmoon::measures::miles{1});

                static_assert(cmoon::measures::system_cast<cmoon::measures::meters>(cmoon::measures::inches{10000}) == cmoon::measures::meters{254});
                static_assert(cmoon::measures::system_cast<cmoon::measures::centimeters>(cmoon::measures::yards{100}) == cmoon::measures::centimeters{9144});
                static_assert(cmoon::measures::system_cast<cmoon::measures::feet>(cmoon::measures::centimeters{10000}) == cmoon::measures::feet{328});
            }
    };
}