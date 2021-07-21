export module cmoon.tests.measures;
export import cmoon.tests.measures.suffix;
export import cmoon.tests.measures.distance;
export import cmoon.tests.measures.mass;
export import cmoon.tests.measures.time;
export import cmoon.tests.measures.derived_unit;
export import cmoon.tests.measures.becquerel;
export import cmoon.tests.measures.coulomb;
export import cmoon.tests.measures.farad;
export import cmoon.tests.measures.gray;
export import cmoon.tests.measures.henry;
export import cmoon.tests.measures.hertz;
export import cmoon.tests.measures.joule;
export import cmoon.tests.measures.katal;
export import cmoon.tests.measures.lumen;
export import cmoon.tests.measures.lux;
export import cmoon.tests.measures.newton;
export import cmoon.tests.measures.ohm;
export import cmoon.tests.measures.pascal;
export import cmoon.tests.measures.siemen;
export import cmoon.tests.measures.tesla;
export import cmoon.tests.measures.volt;
export import cmoon.tests.measures.watt;
export import cmoon.tests.measures.weber;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::measures>()
	{
		cmoon::test::test_suite suite{"measures library tests"};
		suite.add_test_case<measures::suffix_test>();
		suite.add_test_case<measures::distance_string_test>();
		suite.add_test_case<measures::distance_values_test>();
		suite.add_test_case<measures::mass_string_test>();
		suite.add_test_case<measures::mass_values_test>();
		suite.add_test_case<measures::time_string_test>();
		suite.add_test_case<measures::time_values_test>();
		suite.add_test_case<measures::derived_unit_construction_test>();
		suite.add_test_case<measures::becquerel_string_test>();
		suite.add_test_case<measures::coulomb_string_test>();
		suite.add_test_case<measures::farad_string_test>();
		suite.add_test_case<measures::gray_string_test>();
		suite.add_test_case<measures::henry_string_test>();
		suite.add_test_case<measures::hertz_constructor_test>();
		suite.add_test_case<measures::hertz_string_test>();
		suite.add_test_case<measures::joule_string_test>();
		suite.add_test_case<measures::katal_string_test>();
		suite.add_test_case<measures::lumen_string_test>();
		suite.add_test_case<measures::lux_string_test>();
		suite.add_test_case<measures::newton_string_test>();
		suite.add_test_case<measures::ohm_string_test>();
		suite.add_test_case<measures::pascal_string_test>();
		suite.add_test_case<measures::siemen_string_test>();
		suite.add_test_case<measures::tesla_string_test>();
		suite.add_test_case<measures::volt_string_test>();
		suite.add_test_case<measures::watt_string_test>();
		suite.add_test_case<measures::weber_string_test>();

		return std::move(suite);
	}
}