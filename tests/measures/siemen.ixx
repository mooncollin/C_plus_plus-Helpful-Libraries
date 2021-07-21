export module cmoon.tests.measures.siemen;

import <sstream>;

import cmoon.test;
import cmoon.measures.derived;

namespace cmoon::tests::measures
{
	export
    class siemen_string_test : public cmoon::test::test_case
    {
        public:
            siemen_string_test()
                : cmoon::test::test_case{"siemen_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::siemens{40};
                cmoon::test::assert_equal(ss.str(), "40S");
            }
    };
}