#include "collin/measures/measure.hpp"
#include "collin/measures/distance.hpp"
#include <iostream>

void distance_test()
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

    const auto i = collin::measures::distance_cast<collin::measures::inches>(collin::measures::meters{8239});
    std::cout << i << '\n';
    std::cout << collin::measures::centimeters{123} << '\n';
    std::cout << collin::measures::distance_cast<collin::measures::feet>(collin::measures::meters{1}) << '\n';
    std::cout << collin::measures::distance_cast<collin::measures::yards>(collin::measures::meters{1}) << '\n';
    std::cout << collin::measures::distance_cast<collin::measures::miles>(collin::measures::meters{1}) << '\n';
    std::cout << collin::measures::distance_cast<collin::measures::yards>(collin::measures::nanometers{100000000}) << '\n';
}

int main()
{
    distance_test();
}