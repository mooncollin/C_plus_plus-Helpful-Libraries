export module cmoon.tests.measures.joule;

import <sstream>;

import cmoon.test;
import cmoon.measures.derived;

namespace cmoon::tests::measures
{
	export
    class joule_string_test : public cmoon::test::test_case
    {
        public:
            joule_string_test()
                : cmoon::test::test_case{"joule_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::joules{40};
                cmoon::test::assert_equal(ss.str(), "40J");
            }
    };
}