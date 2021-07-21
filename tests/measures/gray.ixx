export module cmoon.tests.measures.gray;

import <sstream>;

import cmoon.test;
import cmoon.measures.derived;

namespace cmoon::tests::measures
{
	export
    class gray_string_test : public cmoon::test::test_case
    {
        public:
            gray_string_test()
                : cmoon::test::test_case{"gray_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::gray{40};
                cmoon::test::assert_equal(ss.str(), "40Gy");
            }
    };
}