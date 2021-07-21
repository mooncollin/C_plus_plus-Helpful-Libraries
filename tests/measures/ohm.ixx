export module cmoon.tests.measures.ohm;

import <sstream>;

import cmoon.test;
import cmoon.measures.derived;

namespace cmoon::tests::measures
{
    export
    class ohm_string_test : public cmoon::test::test_case
    {
        public:
            ohm_string_test()
                : cmoon::test::test_case{"ohm_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::ohms{40};
                cmoon::test::assert_equal(ss.str(), "40O");
            }
    };
}