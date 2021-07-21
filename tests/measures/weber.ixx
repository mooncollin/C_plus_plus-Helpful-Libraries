export module cmoon.tests.measures.weber;

import <sstream>;

import cmoon.test;
import cmoon.measures.derived;

namespace cmoon::tests::measures
{
	export
    class weber_string_test : public cmoon::test::test_case
    {
        public:
            watt_string_test()
                : cmoon::test::test_case{"watt_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::webers{40};
                cmoon::test::assert_equal(ss.str(), "40Wb");
            }
    };
}