export module cmoon.tests.measures.farad;

import <sstream>;

import cmoon.test;
import cmoon.measures.derived;

namespace cmoon::tests::measures
{
	export
    class farad_string_test : public cmoon::test::test_case
    {
        public:
            farad_string_test()
                : cmoon::test::test_case{"farad_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::farads{40};
                cmoon::test::assert_equal(ss.str(), "40F");
            }
    };
}