#include "collin/measures/length.hpp"
#include "collin/measures/mass.hpp"
#include <iostream>
#include <ratio>

void measures_test()
{
    static_assert(collin::measures::suffix_v<std::atto> == "a");
    static_assert(collin::measures::suffix_v<std::femto> == "f");
    static_assert(collin::measures::suffix_v<std::pico> == "p");
    static_assert(collin::measures::suffix_v<std::nano> == "n");
    static_assert(collin::measures::suffix_v<std::micro> == "µ");
    static_assert(collin::measures::suffix_v<std::milli> == "m");
    static_assert(collin::measures::suffix_v<std::centi> == "c");
    static_assert(collin::measures::suffix_v<std::deci> == "d");
    static_assert(collin::measures::suffix_v<std::deca> == "da");
    static_assert(collin::measures::suffix_v<std::hecto> == "h");
    static_assert(collin::measures::suffix_v<std::kilo> == "k");
    static_assert(collin::measures::suffix_v<std::mega> == "M");
    static_assert(collin::measures::suffix_v<std::giga> == "G");
    static_assert(collin::measures::suffix_v<std::tera> == "T");
    static_assert(collin::measures::suffix_v<std::peta> == "P");
    static_assert(collin::measures::suffix_v<std::exa> == "E");
}

void length_test()
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

    static_assert(collin::measures::meters{1} * 10 == collin::measures::decameters{1});
    static_assert(collin::measures::length_cast<collin::measures::inches>(collin::measures::meters{8239}) == collin::measures::inches(324370.078740157480314960));
    static_assert(collin::measures::length_cast<collin::measures::feet>(collin::measures::meters{1}) == collin::measures::feet(3.2808398950131233595800524934383));
    static_assert(collin::measures::length_cast<collin::measures::yards>(collin::measures::meters{1}) == collin::measures::yards(1.0936132983377077865));
    static_assert(collin::measures::length_cast<collin::measures::miles>(collin::measures::meters{1}) == collin::measures::miles(0.000621371));

    static_assert(collin::measures::length_suffix_v<collin::measures::inches> == "in");
    static_assert(collin::measures::length_suffix_v<collin::measures::feet> == "ft");
    static_assert(collin::measures::length_suffix_v<collin::measures::yards> == "yd");
    static_assert(collin::measures::length_suffix_v<collin::measures::miles> == "mi");
    static_assert(collin::measures::length_suffix_v<collin::measures::attometers> == "m");
    static_assert(collin::measures::length_suffix_v<collin::measures::femtometers> == "m");
    static_assert(collin::measures::length_suffix_v<collin::measures::picometers> == "m");
    static_assert(collin::measures::length_suffix_v<collin::measures::nanometers> == "m");
    static_assert(collin::measures::length_suffix_v<collin::measures::micrometers> == "m");
    static_assert(collin::measures::length_suffix_v<collin::measures::millimeters> == "m");
    static_assert(collin::measures::length_suffix_v<collin::measures::centimeters> == "m");
    static_assert(collin::measures::length_suffix_v<collin::measures::decimeters> == "m");
    static_assert(collin::measures::length_suffix_v<collin::measures::meters> == "m");
    static_assert(collin::measures::length_suffix_v<collin::measures::decameters> == "m");
    static_assert(collin::measures::length_suffix_v<collin::measures::hectometers> == "m");
    static_assert(collin::measures::length_suffix_v<collin::measures::kilometers> == "m");
    static_assert(collin::measures::length_suffix_v<collin::measures::megameters> == "m");
    static_assert(collin::measures::length_suffix_v<collin::measures::gigameters> == "m");
    static_assert(collin::measures::length_suffix_v<collin::measures::terameters> == "m");
    static_assert(collin::measures::length_suffix_v<collin::measures::petameters> == "m");
    static_assert(collin::measures::length_suffix_v<collin::measures::exameters> == "m");
}

void mass_test()
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

    static_assert(collin::measures::mass_cast<collin::measures::ounces>(collin::measures::grams{1}) == collin::measures::ounces{0.035274});
    static_assert(collin::measures::mass_cast<collin::measures::pounds>(collin::measures::grams{1}) == collin::measures::pounds{0.00220462});
    static_assert(collin::measures::mass_cast<collin::measures::stones>(collin::measures::grams{1}) == collin::measures::stones{0.000157473});
}

int main()
{
    measures_test();
    length_test();
    mass_test();
}