#include "collin/measures/distance.hpp"
#include "collin/measures/mass.hpp"
#include "collin/measures/time.hpp"
#include "collin/measures/kinematic.hpp"
#include "collin//measures/mechanical.hpp"
#include "collin/measures/temperature.hpp"
#include "collin/measures/constants.hpp"
#include "collin/test.hpp"
#include "collin/utility.hpp"
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
            static_assert(collin::measures::metric_system::suffix_v<std::atto> == "a");
            static_assert(collin::measures::metric_system::suffix_v<std::femto> == "f");
            static_assert(collin::measures::metric_system::suffix_v<std::pico> == "p");
            static_assert(collin::measures::metric_system::suffix_v<std::nano> == "n");
            static_assert(collin::measures::metric_system::suffix_v<std::micro> == "u");
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

            collin::test::assert_equal(collin::measures::metric_system::suffix_v<std::atto>, "a");
            collin::test::assert_equal(collin::measures::metric_system::suffix_v<std::femto>, "f");
            collin::test::assert_equal(collin::measures::metric_system::suffix_v<std::pico>, "p");
            collin::test::assert_equal(collin::measures::metric_system::suffix_v<std::nano>, "n");
            collin::test::assert_equal(collin::measures::metric_system::suffix_v<std::micro>, "u");
            collin::test::assert_equal(collin::measures::metric_system::suffix_v<std::milli>, "m");
            collin::test::assert_equal(collin::measures::metric_system::suffix_v<std::centi>, "c");
            collin::test::assert_equal(collin::measures::metric_system::suffix_v<std::deci>, "d");
            collin::test::assert_equal(collin::measures::metric_system::suffix_v<std::deca>, "da");
            collin::test::assert_equal(collin::measures::metric_system::suffix_v<std::hecto>, "h");
            collin::test::assert_equal(collin::measures::metric_system::suffix_v<std::kilo>, "k");
            collin::test::assert_equal(collin::measures::metric_system::suffix_v<std::mega>, "M");
            collin::test::assert_equal(collin::measures::metric_system::suffix_v<std::giga>, "G");
            collin::test::assert_equal(collin::measures::metric_system::suffix_v<std::tera>, "T");
            collin::test::assert_equal(collin::measures::metric_system::suffix_v<std::peta>, "P");
            collin::test::assert_equal(collin::measures::metric_system::suffix_v<std::exa>, "E");
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
            ss << collin::measures::micrometers{10};
            collin::test::assert_equal(ss.str(), "10um");

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
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::micrometers> == "um");
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

            static_assert(collin::measures::system_cast<collin::measures::meters>(collin::measures::inches{10000}) == collin::measures::meters{254});
            static_assert(collin::measures::system_cast<collin::measures::centimeters>(collin::measures::yards{100}) == collin::measures::centimeters{9144});
            static_assert(collin::measures::system_cast<collin::measures::feet>(collin::measures::centimeters{10000}) == collin::measures::feet{328});
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
            ss << collin::measures::micrograms{10};
            collin::test::assert_equal(ss.str(), "10ug");

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
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::micrograms> == "ug");
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

            static_assert(collin::measures::grains{7000} == collin::measures::pounds{1});
            static_assert(collin::measures::drachm{256} == collin::measures::pounds{1});
            static_assert(collin::measures::ounces{16} == collin::measures::pounds{1});
            static_assert(collin::measures::pounds{1} == collin::measures::pounds{1});
            static_assert(collin::measures::stones{1} == collin::measures::pounds{14});
            static_assert(collin::measures::quarters{1} == collin::measures::pounds{28});
            static_assert(collin::measures::hundredweights{1} == collin::measures::pounds{112});
            static_assert(collin::measures::short_tons{1} == collin::measures::pounds{2000});
            static_assert(collin::measures::tons{1} == collin::measures::pounds{2240});

            static_assert(collin::measures::system_cast<collin::measures::pounds>(collin::measures::short_tons{10}) == collin::measures::pounds{20000});
            static_assert(collin::measures::system_cast<collin::measures::grams>(collin::measures::pounds{1}) == collin::measures::grams{453});
            static_assert(collin::measures::system_cast<collin::measures::pounds>(collin::measures::kilograms{10}) == collin::measures::pounds{22});
        }
};

class time_suffix_test : public collin::test::test_case
{
    public:
        time_suffix_test()
            : collin::test::test_case{"time_suffix_test"} {}

        void operator()() override
        {
            std::stringstream ss;

            ss << collin::measures::attoseconds{10};
		    collin::test::assert_equal(ss.str(), "10as");

		    ss.str("");
            ss << collin::measures::femtoseconds{10};
		    collin::test::assert_equal(ss.str(), "10fs");

		    ss.str("");
            ss << collin::measures::picoseconds{10};
		    collin::test::assert_equal(ss.str(), "10ps");

		    ss.str("");
            ss << collin::measures::nanoseconds{10};
		    collin::test::assert_equal(ss.str(), "10ns");

		    ss.str("");
            ss << collin::measures::microseconds{10};
		    collin::test::assert_equal(ss.str(), "10us");

		    ss.str("");
            ss << collin::measures::milliseconds{10};
		    collin::test::assert_equal(ss.str(), "10ms");

		    ss.str("");
            ss << collin::measures::centiseconds{10};
		    collin::test::assert_equal(ss.str(), "10cs");

		    ss.str("");
            ss << collin::measures::deciseconds{10};
		    collin::test::assert_equal(ss.str(), "10ds");

		    ss.str("");
            ss << collin::measures::seconds{10};
		    collin::test::assert_equal(ss.str(), "10s");

		    ss.str("");
            ss << collin::measures::decaseconds{10};
		    collin::test::assert_equal(ss.str(), "10das");

		    ss.str("");
            ss << collin::measures::hectoseconds{10};
		    collin::test::assert_equal(ss.str(), "10hs");

		    ss.str("");
            ss << collin::measures::kiloseconds{10};
		    collin::test::assert_equal(ss.str(), "10ks");

		    ss.str("");
            ss << collin::measures::megaseconds{10};
		    collin::test::assert_equal(ss.str(), "10Ms");

		    ss.str("");
            ss << collin::measures::gigaseconds{10};
		    collin::test::assert_equal(ss.str(), "10Gs");

		    ss.str("");
            ss << collin::measures::teraseconds{10};
		    collin::test::assert_equal(ss.str(), "10Ts");

		    ss.str("");
            ss << collin::measures::petaseconds{10};
		    collin::test::assert_equal(ss.str(), "10Ps");

		    ss.str("");
            ss << collin::measures::exaseconds{10};
		    collin::test::assert_equal(ss.str(), "10Es");

		    ss.str("");
            ss << collin::measures::minutes{10};
		    collin::test::assert_equal(ss.str(), "10min");

		    ss.str("");
            ss << collin::measures::hours{10};
		    collin::test::assert_equal(ss.str(), "10h");

		    ss.str("");
            ss << collin::measures::days{10};
		    collin::test::assert_equal(ss.str(), "10d");

		    ss.str("");
            ss << collin::measures::weeks{10};
		    collin::test::assert_equal(ss.str(), "10w");

		    ss.str("");
            ss << collin::measures::months{10};
		    collin::test::assert_equal(ss.str(), "10mo");

		    ss.str("");
            ss << collin::measures::years{10};
		    collin::test::assert_equal(ss.str(), "10y");
        }

        void static_asserts()
        {
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::attoseconds> == "as");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::femtoseconds> == "fs");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::picoseconds> == "ps");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::nanoseconds> == "ns");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::microseconds> == "us");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::milliseconds> == "ms");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::centiseconds> == "cs");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::deciseconds> == "ds");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::seconds> == "s");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::decaseconds> == "das");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::hectoseconds> == "hs");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::kiloseconds> == "ks");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::megaseconds> == "Ms");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::gigaseconds> == "Gs");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::teraseconds> == "Ts");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::petaseconds> == "Ps");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::exaseconds> == "Es");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::minutes> == "min");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::hours> == "h");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::days> == "d");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::weeks> == "w");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::months> == "mo");
            static_assert(collin::measures::metric_system::suffix_v<collin::measures::years> == "y");
        }
};

class time_values_test : public collin::test::test_case
{
    public:
        time_values_test()
            : collin::test::test_case{"time_values_test"} {}

        void operator()() override
        {

        }

        void static_asserts()
        {
            static_assert(collin::measures::seconds{1} == collin::measures::attoseconds{1000000000000000000});
            static_assert(collin::measures::seconds{1} == collin::measures::femtoseconds{1000000000000000});
            static_assert(collin::measures::seconds{1} == collin::measures::picoseconds{1000000000000});
            static_assert(collin::measures::seconds{1} == collin::measures::nanoseconds{1000000000});
            static_assert(collin::measures::seconds{1} == collin::measures::microseconds{1000000});
            static_assert(collin::measures::seconds{1} == collin::measures::milliseconds{1000});
            static_assert(collin::measures::seconds{1} == collin::measures::centiseconds{100});
            static_assert(collin::measures::seconds{1} == collin::measures::deciseconds{10});
            static_assert(collin::measures::seconds{1} == collin::measures::seconds{1});
            static_assert(collin::measures::seconds{10} == collin::measures::decaseconds{1});
            static_assert(collin::measures::seconds{1000} == collin::measures::kiloseconds{1});
            static_assert(collin::measures::seconds{1000000} == collin::measures::megaseconds{1});
            static_assert(collin::measures::seconds{1000000000} == collin::measures::gigaseconds{1});
            static_assert(collin::measures::seconds{1000000000000} == collin::measures::teraseconds{1});
            static_assert(collin::measures::seconds{1000000000000000} == collin::measures::petaseconds{1});
            static_assert(collin::measures::seconds{1000000000000000000} == collin::measures::exaseconds{1});

            static_assert(collin::measures::system_cast<collin::measures::seconds>(collin::measures::kiloseconds{1}) == collin::measures::seconds{1000});

            static_assert(collin::measures::seconds{1} * 10 == collin::measures::decaseconds{1});
            static_assert((collin::measures::seconds{1} + collin::measures::seconds{1} + collin::measures::seconds{1}) == collin::measures::seconds{3});
            static_assert((collin::measures::seconds{3} - collin::measures::seconds{2}) == collin::measures::seconds{1});
            static_assert(collin::measures::seconds{100} / 10 == collin::measures::seconds{10});

            static_assert(collin::measures::seconds{60} == collin::measures::minutes{1});
            static_assert(collin::measures::seconds{3600} == collin::measures::hours{1});
            static_assert(collin::measures::seconds{86400} == collin::measures::days{1});
            static_assert(collin::measures::seconds{604800} == collin::measures::weeks{1});
            static_assert(collin::measures::seconds{2629746} == collin::measures::months{1});
            static_assert(collin::measures::seconds{31556952} == collin::measures::years{1});

            static_assert(collin::measures::minutes{60} == collin::measures::hours{1});
            static_assert(collin::measures::hours{24} == collin::measures::days{1});
            static_assert(collin::measures::days{7} == collin::measures::weeks{1});
            static_assert(collin::measures::months{12} == collin::measures::years{1});

            static_assert(collin::measures::to_chrono(collin::measures::nanoseconds{10}) == std::chrono::nanoseconds{10});
            static_assert(collin::measures::to_chrono(collin::measures::microseconds{10}) == std::chrono::microseconds{10});
            static_assert(collin::measures::to_chrono(collin::measures::milliseconds{10}) == std::chrono::milliseconds{10});
            static_assert(collin::measures::to_chrono(collin::measures::seconds{10}) == std::chrono::seconds{10});
            static_assert(collin::measures::to_chrono(collin::measures::minutes{10}) == std::chrono::minutes{10});
            static_assert(collin::measures::to_chrono(collin::measures::hours{10}) == std::chrono::hours{10});
        }
};

class dimension_unit_construction_test : public collin::test::test_case
{
    public:
        dimension_unit_construction_test()
            : collin::test::test_case{"dimension_unit_construction_test"} {}

        void operator()() override
        {
            using type = collin::measures::seconds;
            using type2 = collin::measures::kiloseconds;

            constexpr collin::measures::basic_dimension_unit<typename type::rep,
                typename type::ratio, typename type::unit_values,
                typename type::system, 1> du;

            constexpr collin::measures::basic_dimension_unit<typename type::rep,
                typename type::ratio, typename type::unit_values,
                typename type::system, 1> du2{15};

            constexpr collin::measures::basic_dimension_unit<typename type::rep,
                typename type::ratio, typename type::unit_values,
                typename type::system, 2> du_squared;

            constexpr collin::measures::basic_dimension_unit<typename type::rep,
                typename type::ratio, typename type::unit_values,
                typename type::system> du_squared_dynamic{0, 2};

            constexpr collin::measures::basic_dimension_unit<typename type::rep,
                typename type::ratio, typename type::unit_values,
                typename type::system> du3{25};

            constexpr collin::measures::basic_dimension_unit<typename type::rep,
                typename type::ratio, typename type::unit_values,
                typename type::system, 2> du_squared2{5};

            constexpr collin::measures::basic_dimension_unit<typename type2::rep,
                typename type2::ratio, typename type2::unit_values,
                typename type2::system, 2> du_squared3{1};

            constexpr collin::measures::basic_dimension_unit<typename type::rep,
                typename type::ratio, typename type::unit_values,
                typename type::system> du4{125};

            constexpr collin::measures::basic_dimension_unit<typename type::rep,
                typename type::ratio, typename type::unit_values,
                typename type::system> du_cubed{5, 3};

            constexpr collin::measures::basic_dimension_unit<typename type::rep,
                typename type::ratio, typename type::unit_values,
                typename type::system> du_cubed2{25, 3};

            static_assert(du == type{});
            static_assert(du2 == type{15});
            static_assert(du2 != type{10});
            static_assert(du != du2);
            static_assert(du_squared == du);
            static_assert(du_squared == du_squared_dynamic);
            static_assert(du2 != du_squared);
            static_assert(du < du2);
            static_assert(du2 > du);
            static_assert(du2 >= du);
            static_assert(du <= du2);

            static_assert(du_squared >= du_squared_dynamic);
            static_assert(du_squared <= du_squared_dynamic);
            static_assert(du3 == du_squared2);
            static_assert(du_squared2 == du3);
            static_assert(du3 != du4);
            static_assert(du3 != du_cubed);
            static_assert(du_squared2 != du_cubed);
            static_assert(du_cubed == du4);

            static_assert(du < du2);
            static_assert(du3 > du_squared);
            static_assert(du_squared2 < du4);
            static_assert(du4 <= du_cubed2);
            static_assert(du_cubed2 > du4);
            static_assert(du_squared < du2);
            static_assert(du_cubed2 != type{});
            static_assert(type{} != du_cubed2);
            static_assert(type{15625} == du_cubed2);
            static_assert(type{1000000} == du_squared3);
            static_assert(type{25} < du_squared3);
            static_assert(type{25} <= du_squared3);
            static_assert(du_squared3 > type{25});
            static_assert(du_squared3 >= type{25});
            static_information();
        }

    private:
        void static_information()
        {
            collin::measures::basic_dimension_unit_t<collin::measures::seconds, 1> o1{10};
            collin::measures::basic_dimension_unit_t<collin::measures::seconds, collin::measures::dynamic_dimension> o2{10, 1};

            constexpr auto size1 = sizeof(o1);
            constexpr auto size2 = sizeof(o2);
        }
};

class dimensional_unit_addition_test : public collin::test::test_case
{
    public:
        dimensional_unit_addition_test()
            : collin::test::test_case{"dimensional_unit_addition_test"} {}

        void operator()() override
        {

        }
};

class derived_unit_construction_test : public collin::test::test_case
{
    public:
        derived_unit_construction_test()
            : collin::test::test_case{"derived_unit_construction_test"} {}

        void operator()() override
        {
        }
};

class speed_construction_test : public collin::test::test_case
{
    public:
        speed_construction_test()
            : collin::test::test_case{"speed_construction_test"} {}

        void operator()() override
        {
            constexpr collin::measures::speed<collin::measures::meters> v{10};
            constexpr collin::measures::speed<collin::measures::meters> v2{50};
            constexpr collin::measures::speed<collin::measures::meters> v3{1000};
            constexpr collin::measures::speed<collin::measures::kilometers> v4{1};
            constexpr collin::measures::speed<collin::measures::exameters> v5{100};

            static_assert(v.count() == 10);
            static_assert((+v) == v);
            static_assert((-v) == decltype(v){-10});

            static_assert((v + v2) == decltype(v){60});
            static_assert((v - v2) == decltype(v){-40});
            static_assert((v * 10) == decltype(v){100});
            static_assert((v2 / 10) == decltype(v){5});

            static_assert((v3 + v4) == decltype(v3){2000});
            static_assert((v3 + v4) == decltype(v4){2});
            static_assert((v3 + v4) != decltype(v4){3});
            static_assert((v4 - v3) == decltype(v4){0});

            static_information();
        }
    private:
        void static_information()
        {
            constexpr collin::measures::speed<collin::measures::meters> v{10};
            constexpr collin::measures::speed<collin::measures::exameters> v2{100};

            constexpr auto s1 = sizeof(v);
            constexpr auto s2 = sizeof(v2);
        }
};

class speed_light_test : public collin::test::test_case
{
    public:
        speed_light_test()
            : collin::test::test_case{"speed_light_test"} {}

        void operator()() override
        {
            constexpr auto sol = collin::measures::speed_of_light_vacuum<double>;
            constexpr auto solh = collin::measures::speed<collin::measures::basic_kilometers<double>, collin::measures::milliseconds>{sol};
            constexpr auto v = solh.count();
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
    suite.add_test_case<time_suffix_test>();
    suite.add_test_case<time_values_test>();
    suite.add_test_case<dimension_unit_construction_test>();
    suite.add_test_case<derived_unit_construction_test>();
    suite.add_test_case<speed_construction_test>();
    suite.add_test_case<speed_light_test>();

    collin::test::text_test_runner runner(std::cout);
    
    return !runner.run(suite);
}