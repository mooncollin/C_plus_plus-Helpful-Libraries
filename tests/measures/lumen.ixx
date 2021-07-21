export module cmoon.tests.measures.lumen;

import <sstream>;

import cmoon.test;
import cmoon.measures.derived;

namespace cmoon::tests::measures
{
	export
    class lumen_string_test : public cmoon::test::test_case
    {
        public:
            lumen_string_test()
                : cmoon::test::test_case{"lumen_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::lumens{40};
                cmoon::test::assert_equal(ss.str(), "40lm");
            }
    };
}