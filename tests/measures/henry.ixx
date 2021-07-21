export module cmoon.tests.measures.henry;

import <sstream>;

import cmoon.test;
import cmoon.measures.derived;

namespace cmoon::tests::measures
{
	export
    class henry_string_test : public cmoon::test::test_case
    {
        public:
            henry_string_test()
                : cmoon::test::test_case{"henry_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::henrys{40};
                cmoon::test::assert_equal(ss.str(), "40H");
            }
    };
}