export module cmoon.tests.measures.volt;

import <sstream>;

import cmoon.test;
import cmoon.measures.derived;

namespace cmoon::tests::measures
{
	export
    class volt_string_test : public cmoon::test::test_case
    {
        public:
            volt_string_test()
                : cmoon::test::test_case{"volt_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::volts{40};
                cmoon::test::assert_equal(ss.str(), "40V");
            }
    };
}