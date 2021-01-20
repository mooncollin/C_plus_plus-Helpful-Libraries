#include <iostream>
#include <ratio>
#include <sstream>

#include "cmoon/format.hpp"
#include "cmoon/measures/measure.hpp"
#include "cmoon/measures/distance.hpp"
#include "cmoon/measures/mass.hpp"
#include "cmoon/measures/time.hpp"
#include "cmoon/measures/kinematic.hpp"
#include "cmoon//measures/mechanical.hpp"
#include "cmoon/measures/temperature.hpp"
#include "cmoon/measures/constants.hpp"
#include "cmoon/test/test_case.hpp"
#include "cmoon/test/runner.hpp"
#include "cmoon/test/assert.hpp"
#include "cmoon/test/test_suite.hpp"
#include "cmoon/utility.hpp"

class measures_suffix_test : public cmoon::test::test_case
{
    public:
        measures_suffix_test()
            : cmoon::test::test_case{"measures_suffix_test"} {}

        void operator()() override
        {
            static_assert(cmoon::measures::metric_system::suffix_v<std::atto> == "a");
            static_assert(cmoon::measures::metric_system::suffix_v<std::femto> == "f");
            static_assert(cmoon::measures::metric_system::suffix_v<std::pico> == "p");
            static_assert(cmoon::measures::metric_system::suffix_v<std::nano> == "n");
            static_assert(cmoon::measures::metric_system::suffix_v<std::micro> == "u");
            static_assert(cmoon::measures::metric_system::suffix_v<std::milli> == "m");
            static_assert(cmoon::measures::metric_system::suffix_v<std::centi> == "c");
            static_assert(cmoon::measures::metric_system::suffix_v<std::deci> == "d");
            static_assert(cmoon::measures::metric_system::suffix_v<std::deca> == "da");
            static_assert(cmoon::measures::metric_system::suffix_v<std::hecto> == "h");
            static_assert(cmoon::measures::metric_system::suffix_v<std::kilo> == "k");
            static_assert(cmoon::measures::metric_system::suffix_v<std::mega> == "M");
            static_assert(cmoon::measures::metric_system::suffix_v<std::giga> == "G");
            static_assert(cmoon::measures::metric_system::suffix_v<std::tera> == "T");
            static_assert(cmoon::measures::metric_system::suffix_v<std::peta> == "P");
            static_assert(cmoon::measures::metric_system::suffix_v<std::exa> == "E");

            cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::atto>, "a");
            cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::femto>, "f");
            cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::pico>, "p");
            cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::nano>, "n");
            cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::micro>, "u");
            cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::milli>, "m");
            cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::centi>, "c");
            cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::deci>, "d");
            cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::deca>, "da");
            cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::hecto>, "h");
            cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::kilo>, "k");
            cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::mega>, "M");
            cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::giga>, "G");
            cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::tera>, "T");
            cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::peta>, "P");
            cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::exa>, "E");
        }
};

class distance_suffix_test : public cmoon::test::test_case
{
    public:
        distance_suffix_test()
            : cmoon::test::test_case{"distance_suffix_test"} {}

        void operator()() override
        {
            std::stringstream ss;

            ss << cmoon::measures::attometers{10};
            cmoon::test::assert_equal(ss.str(), "10am");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::attometers{10}), "10am");

            ss.str("");
            ss << cmoon::measures::femtometers{10};
            cmoon::test::assert_equal(ss.str(), "10fm");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::femtometers{10}), "10fm");

            ss.str("");
            ss << cmoon::measures::picometers{10};
            cmoon::test::assert_equal(ss.str(), "10pm");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::picometers{10}), "10pm");

            ss.str("");
            ss << cmoon::measures::nanometers{10};
            cmoon::test::assert_equal(ss.str(), "10nm");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::nanometers{10}), "10nm");

            ss.str("");
            ss << cmoon::measures::micrometers{10};
            cmoon::test::assert_equal(ss.str(), "10um");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::micrometers{10}), "10um");

            ss.str("");
            ss << cmoon::measures::millimeters{10};
            cmoon::test::assert_equal(ss.str(), "10mm");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::millimeters{10}), "10mm");

            ss.str("");
            ss << cmoon::measures::centimeters{10};
            cmoon::test::assert_equal(ss.str(), "10cm");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::centimeters{10}), "10cm");

            ss.str("");
            ss << cmoon::measures::decimeters{10};
            cmoon::test::assert_equal(ss.str(), "10dm");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::decimeters{10}), "10dm");

            ss.str("");
            ss << cmoon::measures::meters{10};
            cmoon::test::assert_equal(ss.str(), "10m");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::meters{10}), "10m");

            ss.str("");
            ss << cmoon::measures::decameters{10};
            cmoon::test::assert_equal(ss.str(), "10dam");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::decameters{10}), "10dam");

            ss.str("");
            ss << cmoon::measures::hectometers{10};
            cmoon::test::assert_equal(ss.str(), "10hm");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::hectometers{10}), "10hm");

            ss.str("");
            ss << cmoon::measures::kilometers{10};
            cmoon::test::assert_equal(ss.str(), "10km");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::kilometers{10}), "10km");

            ss.str("");
            ss << cmoon::measures::megameters{10};
            cmoon::test::assert_equal(ss.str(), "10Mm");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::megameters{10}), "10Mm");

            ss.str("");
            ss << cmoon::measures::gigameters{10};
            cmoon::test::assert_equal(ss.str(), "10Gm");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::gigameters{10}), "10Gm");

            ss.str("");
            ss << cmoon::measures::terameters{10};
            cmoon::test::assert_equal(ss.str(), "10Tm");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::terameters{10}), "10Tm");

            ss.str("");
            ss << cmoon::measures::petameters{10};
            cmoon::test::assert_equal(ss.str(), "10Pm");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::petameters{10}), "10Pm");

            ss.str("");
            ss << cmoon::measures::exameters{10};
            cmoon::test::assert_equal(ss.str(), "10Em");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::exameters{10}), "10Em");

            ss.str("");
            ss << cmoon::measures::inches{10};
            cmoon::test::assert_equal(ss.str(), "10in");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::inches{10}), "10in");

            ss.str("");
            ss << cmoon::measures::feet{10};
            cmoon::test::assert_equal(ss.str(), "10ft");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::feet{10}), "10ft");

            ss.str("");
            ss << cmoon::measures::yards{10};
            cmoon::test::assert_equal(ss.str(), "10yd");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::yards{10}), "10yd");

            ss.str("");
            ss << cmoon::measures::miles{10};
            cmoon::test::assert_equal(ss.str(), "10mi");
            cmoon::test::assert_equal(cmoon::format("{}", cmoon::measures::miles{10}), "10mi");
        }

        void static_asserts()
        {
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::attometers> == "am");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::femtometers> == "fm");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::picometers> == "pm");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::nanometers> == "nm");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::micrometers> == "um");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::millimeters> == "mm");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::centimeters> == "cm");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::decimeters> == "dm");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::meters> == "m");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::decameters> == "dam");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::hectometers> == "hm");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::kilometers> == "km");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::megameters> == "Mm");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::gigameters> == "Gm");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::terameters> == "Tm");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::petameters> == "Pm");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::exameters> == "Em");

            static_assert(cmoon::measures::imperial_system::suffix_v<cmoon::measures::inches> == "in");
            static_assert(cmoon::measures::imperial_system::suffix_v<cmoon::measures::feet> == "ft");
            static_assert(cmoon::measures::imperial_system::suffix_v<cmoon::measures::yards> == "yd");
            static_assert(cmoon::measures::imperial_system::suffix_v<cmoon::measures::miles> == "mi");
        }
};

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

            static_information();
        }

    private:
        void static_information()
        {
            constexpr auto size = sizeof(cmoon::measures::meters);
        }
};

class mass_suffix_test : public cmoon::test::test_case
{
    public:
        mass_suffix_test()
            : cmoon::test::test_case{"mass_suffix_test"} {}

        void operator()() override
        {
            std::stringstream ss;

            ss << cmoon::measures::attograms{10};
            cmoon::test::assert_equal(ss.str(), "10ag");

            ss.str("");
            ss << cmoon::measures::femtograms{10};
            cmoon::test::assert_equal(ss.str(), "10fg");

            ss.str("");
            ss << cmoon::measures::picograms{10};
            cmoon::test::assert_equal(ss.str(), "10pg");

            ss.str("");
            ss << cmoon::measures::nanograms{10};
            cmoon::test::assert_equal(ss.str(), "10ng");

            ss.str("");
            ss << cmoon::measures::micrograms{10};
            cmoon::test::assert_equal(ss.str(), "10ug");

            ss.str("");
            ss << cmoon::measures::milligrams{10};
            cmoon::test::assert_equal(ss.str(), "10mg");

            ss.str("");
            ss << cmoon::measures::centigrams{10};
            cmoon::test::assert_equal(ss.str(), "10cg");

            ss.str("");
            ss << cmoon::measures::decigrams{10};
            cmoon::test::assert_equal(ss.str(), "10dg");

            ss.str("");
            ss << cmoon::measures::grams{10};
            cmoon::test::assert_equal(ss.str(), "10g");

            ss.str("");
            ss << cmoon::measures::decagrams{10};
            cmoon::test::assert_equal(ss.str(), "10dag");

            ss.str("");
            ss << cmoon::measures::hectograms{10};
            cmoon::test::assert_equal(ss.str(), "10hg");

            ss.str("");
            ss << cmoon::measures::kilograms{10};
            cmoon::test::assert_equal(ss.str(), "10kg");

            ss.str("");
            ss << cmoon::measures::megagrams{10};
            cmoon::test::assert_equal(ss.str(), "10Mg");

            ss.str("");
            ss << cmoon::measures::gigagrams{10};
            cmoon::test::assert_equal(ss.str(), "10Gg");

            ss.str("");
            ss << cmoon::measures::teragrams{10};
            cmoon::test::assert_equal(ss.str(), "10Tg");

            ss.str("");
            ss << cmoon::measures::petagrams{10};
            cmoon::test::assert_equal(ss.str(), "10Pg");

            ss.str("");
            ss << cmoon::measures::exagrams{10};
            cmoon::test::assert_equal(ss.str(), "10Eg");

            ss.str("");
            ss << cmoon::measures::grains{10};
            cmoon::test::assert_equal(ss.str(), "10gr");

            ss.str("");
            ss << cmoon::measures::drachm{10};
            cmoon::test::assert_equal(ss.str(), "10dr");

            ss.str("");
            ss << cmoon::measures::ounces{10};
            cmoon::test::assert_equal(ss.str(), "10oz");

            ss.str("");
            ss << cmoon::measures::pounds{10};
            cmoon::test::assert_equal(ss.str(), "10lb");

            ss.str("");
            ss << cmoon::measures::stones{10};
            cmoon::test::assert_equal(ss.str(), "10st");

            ss.str("");
            ss << cmoon::measures::quarters{10};
            cmoon::test::assert_equal(ss.str(), "10qr");

            ss.str("");
            ss << cmoon::measures::hundredweights{10};
            cmoon::test::assert_equal(ss.str(), "10cwt");

            ss.str("");
            ss << cmoon::measures::short_tons{10};
            cmoon::test::assert_equal(ss.str(), "10t");

            ss.str("");
            ss << cmoon::measures::tons{10};
            cmoon::test::assert_equal(ss.str(), "10t");
        }

        void static_asserts()
        {
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::attograms> == "ag");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::femtograms> == "fg");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::picograms> == "pg");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::nanograms> == "ng");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::micrograms> == "ug");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::milligrams> == "mg");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::centigrams> == "cg");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::decigrams> == "dg");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::grams> == "g");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::decagrams> == "dag");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::hectograms> == "hg");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::kilograms> == "kg");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::megagrams> == "Mg");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::gigagrams> == "Gg");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::teragrams> == "Tg");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::petagrams> == "Pg");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::exagrams> == "Eg");

            static_assert(cmoon::measures::imperial_system::suffix_v<cmoon::measures::grains> == "gr");
            static_assert(cmoon::measures::imperial_system::suffix_v<cmoon::measures::drachm> == "dr");
            static_assert(cmoon::measures::imperial_system::suffix_v<cmoon::measures::ounces> == "oz");
            static_assert(cmoon::measures::imperial_system::suffix_v<cmoon::measures::pounds> == "lb");
            static_assert(cmoon::measures::imperial_system::suffix_v<cmoon::measures::stones> == "st");
            static_assert(cmoon::measures::imperial_system::suffix_v<cmoon::measures::quarters> == "qr");
            static_assert(cmoon::measures::imperial_system::suffix_v<cmoon::measures::hundredweights> == "cwt");
            static_assert(cmoon::measures::imperial_system::suffix_v<cmoon::measures::short_tons> == "t");
            static_assert(cmoon::measures::imperial_system::suffix_v<cmoon::measures::tons> == "t");
        }
};

class mass_values_test : public cmoon::test::test_case
{
    public:
        mass_values_test()
            : cmoon::test::test_case{"mass_values_test"} {}

        void operator()() override
        {
        }

        void static_asserts()
        {
            static_assert(cmoon::measures::grams{1} == cmoon::measures::attograms{1000000000000000000});
            static_assert(cmoon::measures::grams{1} == cmoon::measures::femtograms{1000000000000000});
            static_assert(cmoon::measures::grams{1} == cmoon::measures::picograms{1000000000000});
            static_assert(cmoon::measures::grams{1} == cmoon::measures::nanograms{1000000000});
            static_assert(cmoon::measures::grams{1} == cmoon::measures::micrograms{1000000});
            static_assert(cmoon::measures::grams{1} == cmoon::measures::milligrams{1000});
            static_assert(cmoon::measures::grams{1} == cmoon::measures::centigrams{100});
            static_assert(cmoon::measures::grams{1} == cmoon::measures::decigrams{10});
            static_assert(cmoon::measures::grams{1} == cmoon::measures::grams{1});
            static_assert(cmoon::measures::grams{10} == cmoon::measures::decagrams{1});
            static_assert(cmoon::measures::grams{1000} == cmoon::measures::kilograms{1});
            static_assert(cmoon::measures::grams{1000000} == cmoon::measures::megagrams{1});
            static_assert(cmoon::measures::grams{1000000000} == cmoon::measures::gigagrams{1});
            static_assert(cmoon::measures::grams{1000000000000} == cmoon::measures::teragrams{1});
            static_assert(cmoon::measures::grams{1000000000000000} == cmoon::measures::petagrams{1});
            static_assert(cmoon::measures::grams{1000000000000000000} == cmoon::measures::exagrams{1});

            static_assert(cmoon::measures::grains{7000} == cmoon::measures::pounds{1});
            static_assert(cmoon::measures::drachm{256} == cmoon::measures::pounds{1});
            static_assert(cmoon::measures::ounces{16} == cmoon::measures::pounds{1});
            static_assert(cmoon::measures::pounds{1} == cmoon::measures::pounds{1});
            static_assert(cmoon::measures::stones{1} == cmoon::measures::pounds{14});
            static_assert(cmoon::measures::quarters{1} == cmoon::measures::pounds{28});
            static_assert(cmoon::measures::hundredweights{1} == cmoon::measures::pounds{112});
            static_assert(cmoon::measures::short_tons{1} == cmoon::measures::pounds{2000});
            static_assert(cmoon::measures::tons{1} == cmoon::measures::pounds{2240});

            static_assert(cmoon::measures::system_cast<cmoon::measures::pounds>(cmoon::measures::short_tons{10}) == cmoon::measures::pounds{20000});
            static_assert(cmoon::measures::system_cast<cmoon::measures::grams>(cmoon::measures::pounds{1}) == cmoon::measures::grams{453});
            static_assert(cmoon::measures::system_cast<cmoon::measures::pounds>(cmoon::measures::kilograms{10}) == cmoon::measures::pounds{22});
        }
};

class time_suffix_test : public cmoon::test::test_case
{
    public:
        time_suffix_test()
            : cmoon::test::test_case{"time_suffix_test"} {}

        void operator()() override
        {
            std::stringstream ss;

            ss << cmoon::measures::attoseconds{10};
		    cmoon::test::assert_equal(ss.str(), "10as");

		    ss.str("");
            ss << cmoon::measures::femtoseconds{10};
		    cmoon::test::assert_equal(ss.str(), "10fs");

		    ss.str("");
            ss << cmoon::measures::picoseconds{10};
		    cmoon::test::assert_equal(ss.str(), "10ps");

		    ss.str("");
            ss << cmoon::measures::nanoseconds{10};
		    cmoon::test::assert_equal(ss.str(), "10ns");

		    ss.str("");
            ss << cmoon::measures::microseconds{10};
		    cmoon::test::assert_equal(ss.str(), "10us");

		    ss.str("");
            ss << cmoon::measures::milliseconds{10};
		    cmoon::test::assert_equal(ss.str(), "10ms");

		    ss.str("");
            ss << cmoon::measures::centiseconds{10};
		    cmoon::test::assert_equal(ss.str(), "10cs");

		    ss.str("");
            ss << cmoon::measures::deciseconds{10};
		    cmoon::test::assert_equal(ss.str(), "10ds");

		    ss.str("");
            ss << cmoon::measures::seconds{10};
		    cmoon::test::assert_equal(ss.str(), "10s");

		    ss.str("");
            ss << cmoon::measures::decaseconds{10};
		    cmoon::test::assert_equal(ss.str(), "10das");

		    ss.str("");
            ss << cmoon::measures::hectoseconds{10};
		    cmoon::test::assert_equal(ss.str(), "10hs");

		    ss.str("");
            ss << cmoon::measures::kiloseconds{10};
		    cmoon::test::assert_equal(ss.str(), "10ks");

		    ss.str("");
            ss << cmoon::measures::megaseconds{10};
		    cmoon::test::assert_equal(ss.str(), "10Ms");

		    ss.str("");
            ss << cmoon::measures::gigaseconds{10};
		    cmoon::test::assert_equal(ss.str(), "10Gs");

		    ss.str("");
            ss << cmoon::measures::teraseconds{10};
		    cmoon::test::assert_equal(ss.str(), "10Ts");

		    ss.str("");
            ss << cmoon::measures::petaseconds{10};
		    cmoon::test::assert_equal(ss.str(), "10Ps");

		    ss.str("");
            ss << cmoon::measures::exaseconds{10};
		    cmoon::test::assert_equal(ss.str(), "10Es");

		    ss.str("");
            ss << cmoon::measures::minutes{10};
		    cmoon::test::assert_equal(ss.str(), "10min");

		    ss.str("");
            ss << cmoon::measures::hours{10};
		    cmoon::test::assert_equal(ss.str(), "10h");

		    ss.str("");
            ss << cmoon::measures::days{10};
		    cmoon::test::assert_equal(ss.str(), "10d");

		    ss.str("");
            ss << cmoon::measures::weeks{10};
		    cmoon::test::assert_equal(ss.str(), "10w");

		    ss.str("");
            ss << cmoon::measures::months{10};
		    cmoon::test::assert_equal(ss.str(), "10mo");

		    ss.str("");
            ss << cmoon::measures::years{10};
		    cmoon::test::assert_equal(ss.str(), "10y");
        }

        void static_asserts()
        {
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::attoseconds> == "as");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::femtoseconds> == "fs");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::picoseconds> == "ps");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::nanoseconds> == "ns");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::microseconds> == "us");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::milliseconds> == "ms");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::centiseconds> == "cs");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::deciseconds> == "ds");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::seconds> == "s");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::decaseconds> == "das");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::hectoseconds> == "hs");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::kiloseconds> == "ks");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::megaseconds> == "Ms");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::gigaseconds> == "Gs");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::teraseconds> == "Ts");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::petaseconds> == "Ps");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::exaseconds> == "Es");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::minutes> == "min");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::hours> == "h");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::days> == "d");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::weeks> == "w");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::months> == "mo");
            static_assert(cmoon::measures::metric_system::suffix_v<cmoon::measures::years> == "y");
        }
};

class time_values_test : public cmoon::test::test_case
{
    public:
        time_values_test()
            : cmoon::test::test_case{"time_values_test"} {}

        void operator()() override
        {

        }

        void static_asserts()
        {
            static_assert(cmoon::measures::seconds{1} == cmoon::measures::attoseconds{1000000000000000000});
            static_assert(cmoon::measures::seconds{1} == cmoon::measures::femtoseconds{1000000000000000});
            static_assert(cmoon::measures::seconds{1} == cmoon::measures::picoseconds{1000000000000});
            static_assert(cmoon::measures::seconds{1} == cmoon::measures::nanoseconds{1000000000});
            static_assert(cmoon::measures::seconds{1} == cmoon::measures::microseconds{1000000});
            static_assert(cmoon::measures::seconds{1} == cmoon::measures::milliseconds{1000});
            static_assert(cmoon::measures::seconds{1} == cmoon::measures::centiseconds{100});
            static_assert(cmoon::measures::seconds{1} == cmoon::measures::deciseconds{10});
            static_assert(cmoon::measures::seconds{1} == cmoon::measures::seconds{1});
            static_assert(cmoon::measures::seconds{10} == cmoon::measures::decaseconds{1});
            static_assert(cmoon::measures::seconds{1000} == cmoon::measures::kiloseconds{1});
            static_assert(cmoon::measures::seconds{1000000} == cmoon::measures::megaseconds{1});
            static_assert(cmoon::measures::seconds{1000000000} == cmoon::measures::gigaseconds{1});
            static_assert(cmoon::measures::seconds{1000000000000} == cmoon::measures::teraseconds{1});
            static_assert(cmoon::measures::seconds{1000000000000000} == cmoon::measures::petaseconds{1});
            static_assert(cmoon::measures::seconds{1000000000000000000} == cmoon::measures::exaseconds{1});

            static_assert(cmoon::measures::system_cast<cmoon::measures::seconds>(cmoon::measures::kiloseconds{1}) == cmoon::measures::seconds{1000});

            static_assert(cmoon::measures::seconds{1} * 10 == cmoon::measures::decaseconds{1});
            static_assert((cmoon::measures::seconds{1} + cmoon::measures::seconds{1} + cmoon::measures::seconds{1}) == cmoon::measures::seconds{3});
            static_assert((cmoon::measures::seconds{3} - cmoon::measures::seconds{2}) == cmoon::measures::seconds{1});
            static_assert(cmoon::measures::seconds{100} / 10 == cmoon::measures::seconds{10});

            static_assert(cmoon::measures::seconds{60} == cmoon::measures::minutes{1});
            static_assert(cmoon::measures::seconds{3600} == cmoon::measures::hours{1});
            static_assert(cmoon::measures::seconds{86400} == cmoon::measures::days{1});
            static_assert(cmoon::measures::seconds{604800} == cmoon::measures::weeks{1});
            static_assert(cmoon::measures::seconds{2629746} == cmoon::measures::months{1});
            static_assert(cmoon::measures::seconds{31556952} == cmoon::measures::years{1});

            static_assert(cmoon::measures::minutes{60} == cmoon::measures::hours{1});
            static_assert(cmoon::measures::hours{24} == cmoon::measures::days{1});
            static_assert(cmoon::measures::days{7} == cmoon::measures::weeks{1});
            static_assert(cmoon::measures::months{12} == cmoon::measures::years{1});

            static_assert(cmoon::measures::to_chrono(cmoon::measures::nanoseconds{10}) == std::chrono::nanoseconds{10});
            static_assert(cmoon::measures::to_chrono(cmoon::measures::microseconds{10}) == std::chrono::microseconds{10});
            static_assert(cmoon::measures::to_chrono(cmoon::measures::milliseconds{10}) == std::chrono::milliseconds{10});
            static_assert(cmoon::measures::to_chrono(cmoon::measures::seconds{10}) == std::chrono::seconds{10});
            static_assert(cmoon::measures::to_chrono(cmoon::measures::minutes{10}) == std::chrono::minutes{10});
            static_assert(cmoon::measures::to_chrono(cmoon::measures::hours{10}) == std::chrono::hours{10});
        }
};

class dimension_unit_construction_test : public cmoon::test::test_case
{
    public:
        dimension_unit_construction_test()
            : cmoon::test::test_case{"dimension_unit_construction_test"} {}

        void operator()() override
        {
            using type = cmoon::measures::seconds;
            using type2 = cmoon::measures::kiloseconds;

            constexpr type s1{10};
            constexpr auto s2 = s1 * s1;
            constexpr auto s3 = (s2 / s1) / s1;
            constexpr auto s4 = 1000 / s1;

            static_assert(s2.count() == 100);
            static_assert(s2.dimension == 2);
            static_assert(s3 == 1);
            static_assert(s4.count() == 100);

            static_information();
        }

    private:
        void static_information()
        {
            constexpr auto size1 = sizeof(cmoon::measures::basic_seconds<std::intmax_t, 2>);
            constexpr auto size2 = sizeof(cmoon::measures::basic_seconds<std::intmax_t, -2>);
        }
};

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

            using derived_t = cmoon::measures::basic_derived_unit<std::intmax_t, t, t2>;
            using derived_t2 = cmoon::measures::basic_derived_unit<std::intmax_t, t3, t2>;
            using derived_t3 = cmoon::measures::basic_derived_unit<std::intmax_t,
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

int main()
{
    cmoon::test::test_suite suite;
    suite.add_test_case<measures_suffix_test>();
    suite.add_test_case<distance_suffix_test>();
    suite.add_test_case<distance_values_test>();
    suite.add_test_case<mass_suffix_test>();
    suite.add_test_case<mass_values_test>();
    suite.add_test_case<time_suffix_test>();
    suite.add_test_case<time_values_test>();
    suite.add_test_case<dimension_unit_construction_test>();
    suite.add_test_case<derived_unit_construction_test>();

    cmoon::test::text_test_runner runner(std::cout);
    
    return !runner.run(suite);
}