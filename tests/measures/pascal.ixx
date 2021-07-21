export module cmoon.tests.measures.pascal;

import <sstream>;

import cmoon.test;
import cmoon.measures.derived;

namespace cmoon::tests::measures
{
	export
    class pascal_string_test : public cmoon::test::test_case
    {
        public:
            pascal_string_test()
                : cmoon::test::test_case{"pascal_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::pascals{40};
                cmoon::test::assert_equal(ss.str(), "40Pa");
            }
    };
}