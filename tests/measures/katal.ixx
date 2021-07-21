export module cmoon.tests.measures.katal;

import <sstream>;

import cmoon.test;
import cmoon.measures.derived;

namespace cmoon::tests::measures
{
	export
    class katal_string_test : public cmoon::test::test_case
    {
        public:
            katal_string_test()
                : cmoon::test::test_case{"katal_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::katals{40};
                cmoon::test::assert_equal(ss.str(), "40kat");
            }
    };
}