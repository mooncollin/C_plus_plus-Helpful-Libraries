export module cmoon.tests.measures.newton;

import <sstream>;

import cmoon.test;
import cmoon.measures.derived;

namespace cmoon::tests::measures
{
	export
    class newton_string_test : public cmoon::test::test_case
    {
        public:
            newton_string_test()
                : cmoon::test::test_case{"newton_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::newtons{40};
                cmoon::test::assert_equal(ss.str(), "40N");
            }
    };
}