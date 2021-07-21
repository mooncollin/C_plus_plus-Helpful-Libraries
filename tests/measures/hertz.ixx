export module cmoon.tests.measures.hertz;

import <sstream>;

import cmoon.test;
import cmoon.measures.derived;

namespace cmoon::tests::measures
{
    export
	class hertz_constructor_test : public cmoon::test::test_case
    {
        public:
            hertz_constructor_test()
                : cmoon::test::test_case{"hertz_constructor_test"} {}

            void operator()() override
            {
                constexpr cmoon::measures::hertz h1{40};
                constexpr cmoon::measures::hertz h2{40};

                static_assert(h1 == h2);
                static_assert(h1 <= h2);
                static_assert(h1 >= h2);

                cmoon::test::assert_equal(h1, h2);
                cmoon::test::assert_less_equal(h1, h2);
                cmoon::test::assert_greater_equal(h1, h2);
            }
    };

    export
    class hertz_string_test : public cmoon::test::test_case
    {
        public:
            hertz_string_test()
                : cmoon::test::test_case{"hertz_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::hertz{40};
                cmoon::test::assert_equal(ss.str(), "40Hz");
            }
    };
}