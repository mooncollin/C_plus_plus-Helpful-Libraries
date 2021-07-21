export module cmoon.tests.measures.coulomb;

import <sstream>;

import cmoon.test;
import cmoon.measures.derived;

namespace cmoon::tests::measures
{
	export
    class coulomb_string_test : public cmoon::test::test_case
    {
        public:
            coulomb_string_test()
                : cmoon::test::test_case{"coulomb_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::coulombs{40};
                cmoon::test::assert_equal(ss.str(), "40C");
            }
    };
}