export module cmoon.tests.measures.lux;

import <sstream>;

import cmoon.test;
import cmoon.measures.derived;

namespace cmoon::tests::measures
{
	export
    class lux_string_test : public cmoon::test::test_case
    {
        public:
            lux_string_test()
                : cmoon::test::test_case{"lux_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::lux{40};
                cmoon::test::assert_equal(ss.str(), "40lx");
            }
    };
}