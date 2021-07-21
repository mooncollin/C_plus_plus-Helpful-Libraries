export module cmoon.tests.measures.tesla;

import <sstream>;

import cmoon.test;
import cmoon.measures.derived;

namespace cmoon::tests::measures
{
	export
    class tesla_string_test : public cmoon::test::test_case
    {
        public:
            tesla_string_test()
                : cmoon::test::test_case{"tesla_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::teslas{40};
                cmoon::test::assert_equal(ss.str(), "40T");
            }
    };
}