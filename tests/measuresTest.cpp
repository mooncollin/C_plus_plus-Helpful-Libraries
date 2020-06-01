#include "collin/measures/distance.hpp"
#include "collin/measures/mass.hpp"
#include "collin/test.hpp"
#include <iostream>
#include <ratio>
#include <sstream>

class measures_suffix_test : public collin::test::test_case
{
    public:
        measures_suffix_test()
            : collin::test::test_case{"measures_suffix_test"} {}

        void operator()() override
        {
        }

        void static_asserts()
        {
            static_assert(collin::measures::metric_system::suffix_v<std::atto> == "a");
            static_assert(collin::measures::metric_system::suffix_v<std::femto> == "f");
            static_assert(collin::measures::metric_system::suffix_v<std::pico> == "p");
            static_assert(collin::measures::metric_system::suffix_v<std::nano> == "n");
            static_assert(collin::measures::metric_system::suffix_v<std::micro> == "µ");
            static_assert(collin::measures::metric_system::suffix_v<std::milli> == "m");
            static_assert(collin::measures::metric_system::suffix_v<std::centi> == "c");
            static_assert(collin::measures::metric_system::suffix_v<std::deci> == "d");
            static_assert(collin::measures::metric_system::suffix_v<std::deca> == "da");
            static_assert(collin::measures::metric_system::suffix_v<std::hecto> == "h");
            static_assert(collin::measures::metric_system::suffix_v<std::kilo> == "k");
            static_assert(collin::measures::metric_system::suffix_v<std::mega> == "M");
            static_assert(collin::measures::metric_system::suffix_v<std::giga> == "G");
            static_assert(collin::measures::metric_system::suffix_v<std::tera> == "T");
            static_assert(collin::measures::metric_system::suffix_v<std::peta> == "P");
            static_assert(collin::measures::metric_system::suffix_v<std::exa> == "E");
        }
};

class distance_suffix_test : public collin::test::test_case
{
    public:
        distance_suffix_test()
            : collin::test::test_case{"distance_suffix_test"} {}

        void operator()() override
        {
            std::stringstream ss;

            ss << collin::measures::attometers{10};
            collin::test::assert_equal(ss.str(), "10am");

            ss.str("");
            ss << collin::measures::femtometers{10};
            collin::test::assert_equal(ss.str(), "10fm");

            ss.str("");
            ss << collin::measures::picometers{10};
            collin::test::assert_equal(ss.str(), "10pm");

            ss.str("");
            ss << collin::measures::nanometers{10};
            collin::test::assert_equal(ss.str(), "10nm");

            ss.str("");
            ss << collin::measures::millimeters{10};
            collin::test::assert_equal(ss.str(), "10mm");

            ss.str("");
            ss << collin::measures::centimeters{10};
            collin::test::assert_equal(ss.str(), "10cm");

            ss.str("");
            ss << collin::measures::decimeters{10};
            collin::test::assert_equal(ss.str(), "10dm");

            ss.str("");
            ss << collin::measures::meters{10};
            collin::test::assert_equal(ss.str(), "10m");

            ss.str("");
            ss << collin::measures::decameters{10};
            collin::test::assert_equal(ss.str(), "10dam");

            ss.str("");
            ss << collin::measures::hectometers{10};
            collin::test::assert_equal(ss.str(), "10hm");

            ss.str("");
            ss << collin::measures::kilometers{10};
            collin::test::assert_equal(ss.str(), "10km");

            ss.str("");
            ss << collin::measures::megameters{10};
            collin::test::assert_equal(ss.str(), "10Mm");

            ss.str("");
            ss << collin::measures::gigameters{10};
            collin::test::assert_equal(ss.str(), "10Gm");

            ss.str("");
            ss << collin::measures::terameters{10};
            collin::test::assert_equal(ss.str(), "10Tm");

            ss.str("");
            ss << collin::measures::petameters{10};
            collin::test::assert_equal(ss.str(), "10Pm");

            ss.str("");
            ss << collin::measures::exameters{10};
            collin::test::assert_equal(ss.str(), "10Em");

            ss.str("");
            ss << collin::measures::inches{10};
            collin::test::assert_equal(ss.str(), "10in");

            ss.str("");
            ss << collin::measures::feet{10};
            collin::test::assert_equal(ss.str(), "10ft");

            ss.str("");
            ss << collin::measures::yards{10};
            collin::test::assert_equal(ss.str(), "10yd");

            ss.str("");
            ss << collin::measures::miles{10};
            collin::test::assert_equal(ss.str(), "10mi");
        }

        void static_asserts()
        {
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::attometers> == "am");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::femtometers> == "fm");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::picometers> == "pm");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::nanometers> == "nm");
            //static_assert(collin::measures::metric_system::suffix_v<collin::measures::micrometers> == "µm");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::millimeters> == "mm");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::centimeters> == "cm");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::decimeters> == "dm");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::meters> == "m");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::decameters> == "dam");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::hectometers> == "hm");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::kilometers> == "km");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::megameters> == "Mm");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::gigameters> == "Gm");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::terameters> == "Tm");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::petameters> == "Pm");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::exameters> == "Em");

            static_assert(collin::measures::imperial_system::suffix_v<collin::measures::inches> == "in");
            static_assert(collin::measures::imperial_system::suffix_v<collin::measures::feet> == "ft");
            static_assert(collin::measures::imperial_system::suffix_v<collin::measures::yards> == "yd");
            static_assert(collin::measures::imperial_system::suffix_v<collin::measures::miles> == "mi");
        }
};

class distance_values_test : public collin::test::test_case
{
    public:
        distance_values_test()
            : collin::test::test_case{"distance_values_test"} {}

        void operator()() override
        {
        }

        void static_asserts()
        {
            static_assert(collin::measures::meters{1} == collin::measures::attometers{1000000000000000000});
            static_assert(collin::measures::meters{1} == collin::measures::femtometers{1000000000000000});
            static_assert(collin::measures::meters{1} == collin::measures::picometers{1000000000000});
            static_assert(collin::measures::meters{1} == collin::measures::nanometers{1000000000});
            static_assert(collin::measures::meters{1} == collin::measures::micrometers{1000000});
            static_assert(collin::measures::meters{1} == collin::measures::millimeters{1000});
            static_assert(collin::measures::meters{1} == collin::measures::centimeters{100});
            static_assert(collin::measures::meters{1} == collin::measures::decimeters{10});
            static_assert(collin::measures::meters{1} == collin::measures::meters{1});
            static_assert(collin::measures::meters{10} == collin::measures::decameters{1});
            static_assert(collin::measures::meters{1000} == collin::measures::kilometers{1});
            static_assert(collin::measures::meters{1000000} == collin::measures::megameters{1});
            static_assert(collin::measures::meters{1000000000} == collin::measures::gigameters{1});
            static_assert(collin::measures::meters{1000000000000} == collin::measures::terameters{1});
            static_assert(collin::measures::meters{1000000000000000} == collin::measures::petameters{1});
            static_assert(collin::measures::meters{1000000000000000000} == collin::measures::exameters{1});

            static_assert(collin::measures::system_cast<collin::measures::meters>(collin::measures::kilometers{1}) == collin::measures::meters{1000});

            static_assert(collin::measures::meters{1} * 10 == collin::measures::decameters{1});
            static_assert((collin::measures::meters{1} + collin::measures::meters{1} + collin::measures::meters{1}) == collin::measures::meters{3});
            static_assert((collin::measures::meters{3} - collin::measures::meters{2}) == collin::measures::meters{1});
            static_assert(collin::measures::meters{100} / 10 == collin::measures::meters{10});

            static_assert(collin::measures::inches{12} == collin::measures::feet{1});
            static_assert(collin::measures::inches{36} == collin::measures::yards{1});
            static_assert(collin::measures::inches{63360} == collin::measures::miles{1});
            static_assert(collin::measures::feet{3} == collin::measures::yards{1});
            static_assert(collin::measures::feet{5280} == collin::measures::miles{1});
            static_assert(collin::measures::yards{1760} == collin::measures::miles{1});
        }
};

class mass_suffix_test : public collin::test::test_case
{
    public:
        mass_suffix_test()
            : collin::test::test_case{"mass_suffix_test"} {}

        void operator()() override
        {
            std::stringstream ss;

            ss << collin::measures::attograms{10};
            collin::test::assert_equal(ss.str(), "10ag");

            ss.str("");
            ss << collin::measures::femtograms{10};
            collin::test::assert_equal(ss.str(), "10fg");

            ss.str("");
            ss << collin::measures::picograms{10};
            collin::test::assert_equal(ss.str(), "10pg");

            ss.str("");
            ss << collin::measures::nanograms{10};
            collin::test::assert_equal(ss.str(), "10ng");

            ss.str("");
            ss << collin::measures::milligrams{10};
            collin::test::assert_equal(ss.str(), "10mg");

            ss.str("");
            ss << collin::measures::centigrams{10};
            collin::test::assert_equal(ss.str(), "10cg");

            ss.str("");
            ss << collin::measures::decigrams{10};
            collin::test::assert_equal(ss.str(), "10dg");

            ss.str("");
            ss << collin::measures::grams{10};
            collin::test::assert_equal(ss.str(), "10g");

            ss.str("");
            ss << collin::measures::decagrams{10};
            collin::test::assert_equal(ss.str(), "10dag");

            ss.str("");
            ss << collin::measures::hectograms{10};
            collin::test::assert_equal(ss.str(), "10hg");

            ss.str("");
            ss << collin::measures::kilograms{10};
            collin::test::assert_equal(ss.str(), "10kg");

            ss.str("");
            ss << collin::measures::megagrams{10};
            collin::test::assert_equal(ss.str(), "10Mg");

            ss.str("");
            ss << collin::measures::gigagrams{10};
            collin::test::assert_equal(ss.str(), "10Gg");

            ss.str("");
            ss << collin::measures::teragrams{10};
            collin::test::assert_equal(ss.str(), "10Tg");

            ss.str("");
            ss << collin::measures::petagrams{10};
            collin::test::assert_equal(ss.str(), "10Pg");

            ss.str("");
            ss << collin::measures::exagrams{10};
            collin::test::assert_equal(ss.str(), "10Eg");

            ss.str("");
            ss << collin::measures::grains{10};
            collin::test::assert_equal(ss.str(), "10gr");

            ss.str("");
            ss << collin::measures::drachm{10};
            collin::test::assert_equal(ss.str(), "10dr");

            ss.str("");
            ss << collin::measures::ounces{10};
            collin::test::assert_equal(ss.str(), "10oz");

            ss.str("");
            ss << collin::measures::pounds{10};
            collin::test::assert_equal(ss.str(), "10lb");

            ss.str("");
            ss << collin::measures::stones{10};
            collin::test::assert_equal(ss.str(), "10st");

            ss.str("");
            ss << collin::measures::quarters{10};
            collin::test::assert_equal(ss.str(), "10qr");

            ss.str("");
            ss << collin::measures::hundredweights{10};
            collin::test::assert_equal(ss.str(), "10cwt");

            ss.str("");
            ss << collin::measures::short_tons{10};
            collin::test::assert_equal(ss.str(), "10t");

            ss.str("");
            ss << collin::measures::tons{10};
            collin::test::assert_equal(ss.str(), "10t");
        }

        void static_asserts()
        {
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::attograms> == "ag");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::femtograms> == "fg");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::picograms> == "pg");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::nanograms> == "ng");
            //static_assert(collin::measures::metric_system::suffix_v<collin::measures::micrograms> == "µg");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::milligrams> == "mg");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::centigrams> == "cg");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::decigrams> == "dg");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::grams> == "g");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::decagrams> == "dag");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::hectograms> == "hg");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::kilograms> == "kg");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::megagrams> == "Mg");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::gigagrams> == "Gg");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::teragrams> == "Tg");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::petagrams> == "Pg");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::exagrams> == "Eg");

            static_assert(collin::measures::imperial_system::suffix_v<collin::measures::grains> == "gr");
            static_assert(collin::measures::imperial_system::suffix_v<collin::measures::drachm> == "dr");
            static_assert(collin::measures::imperial_system::suffix_v<collin::measures::ounces> == "oz");
            static_assert(collin::measures::imperial_system::suffix_v<collin::measures::pounds> == "lb");
            static_assert(collin::measures::imperial_system::suffix_v<collin::measures::stones> == "st");
            static_assert(collin::measures::imperial_system::suffix_v<collin::measures::quarters> == "qr");
            static_assert(collin::measures::imperial_system::suffix_v<collin::measures::hundredweights> == "cwt");
            static_assert(collin::measures::imperial_system::suffix_v<collin::measures::short_tons> == "t");
            static_assert(collin::measures::imperial_system::suffix_v<collin::measures::tons> == "t");
        }
};

class mass_values_test : public collin::test::test_case
{
    public:
        mass_values_test()
            : collin::test::test_case{"mass_values_test"} {}

        void operator()() override
        {
        }

        void static_asserts()
        {
            static_assert(collin::measures::grams{1} == collin::measures::attograms{1000000000000000000});
            static_assert(collin::measures::grams{1} == collin::measures::femtograms{1000000000000000});
            static_assert(collin::measures::grams{1} == collin::measures::picograms{1000000000000});
            static_assert(collin::measures::grams{1} == collin::measures::nanograms{1000000000});
            static_assert(collin::measures::grams{1} == collin::measures::micrograms{1000000});
            static_assert(collin::measures::grams{1} == collin::measures::milligrams{1000});
            static_assert(collin::measures::grams{1} == collin::measures::centigrams{100});
            static_assert(collin::measures::grams{1} == collin::measures::decigrams{10});
            static_assert(collin::measures::grams{1} == collin::measures::grams{1});
            static_assert(collin::measures::grams{10} == collin::measures::decagrams{1});
            static_assert(collin::measures::grams{1000} == collin::measures::kilograms{1});
            static_assert(collin::measures::grams{1000000} == collin::measures::megagrams{1});
            static_assert(collin::measures::grams{1000000000} == collin::measures::gigagrams{1});
            static_assert(collin::measures::grams{1000000000000} == collin::measures::teragrams{1});
            static_assert(collin::measures::grams{1000000000000000} == collin::measures::petagrams{1});
            static_assert(collin::measures::grams{1000000000000000000} == collin::measures::exagrams{1});

            //static_assert(collin::measures::mass_cast<collin::measures::ounces>(collin::measures::grams{1}) == collin::measures::ounces{0.035274});
            //static_assert(collin::measures::mass_cast<collin::measures::pounds>(collin::measures::grams{1}) == collin::measures::pounds{0.00220462});
            //static_assert(collin::measures::mass_cast<collin::measures::stones>(collin::measures::grams{1}) == collin::measures::stones{0.000157473});
        }
};

int main()
{
    collin::test::test_suite suite;
    suite.add_test_case<measures_suffix_test>();
    suite.add_test_case<distance_suffix_test>();
    suite.add_test_case<distance_values_test>();
    suite.add_test_case<mass_suffix_test>();
    suite.add_test_case<mass_values_test>();

    collin::test::text_test_runner runner(std::cout);
    
    return !runner.run(suite);
}