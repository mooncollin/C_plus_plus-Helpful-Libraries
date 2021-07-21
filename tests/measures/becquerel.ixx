export module cmoon.tests.measures.becquerel;

import <sstream>;

import cmoon.test;
import cmoon.measures.derived;

namespace cmoon::tests::measures
{
	export
    class becquerel_string_test : public cmoon::test::test_case
    {
        public:
            becquerel_string_test()
                : cmoon::test::test_case{"becquerel_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::becquerels{40};
                cmoon::test::assert_equal(ss.str(), "40Bq");
            }
    };
}