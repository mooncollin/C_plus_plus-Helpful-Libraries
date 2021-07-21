export module cmoon.tests.measures.suffix;

import cmoon.test;
import cmoon.measures;

namespace cmoon::tests::measures
{
    export
	class suffix_test : public cmoon::test::test_case
    {
        public:
            suffix_test()
                : cmoon::test::test_case{"suffix_test"} {}

            void operator()() override
            {
                static_assert(cmoon::measures::metric_system::suffix_v<std::atto, char> == "a");
                static_assert(cmoon::measures::metric_system::suffix_v<std::femto, char> == "f");
                static_assert(cmoon::measures::metric_system::suffix_v<std::pico, char> == "p");
                static_assert(cmoon::measures::metric_system::suffix_v<std::nano, char> == "n");
                static_assert(cmoon::measures::metric_system::suffix_v<std::micro, char> == "u");
                static_assert(cmoon::measures::metric_system::suffix_v<std::milli, char> == "m");
                static_assert(cmoon::measures::metric_system::suffix_v<std::centi, char> == "c");
                static_assert(cmoon::measures::metric_system::suffix_v<std::deci, char> == "d");
                static_assert(cmoon::measures::metric_system::suffix_v<std::deca, char> == "da");
                static_assert(cmoon::measures::metric_system::suffix_v<std::hecto, char> == "h");
                static_assert(cmoon::measures::metric_system::suffix_v<std::kilo, char> == "k");
                static_assert(cmoon::measures::metric_system::suffix_v<std::mega, char> == "M");
                static_assert(cmoon::measures::metric_system::suffix_v<std::giga, char> == "G");
                static_assert(cmoon::measures::metric_system::suffix_v<std::tera, char> == "T");
                static_assert(cmoon::measures::metric_system::suffix_v<std::peta, char> == "P");
                static_assert(cmoon::measures::metric_system::suffix_v<std::exa, char> == "E");

                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::atto, char>, "a");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::femto, char>, "f");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::pico, char>, "p");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::nano, char>, "n");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::micro, char>, "u");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::milli, char>, "m");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::centi, char>, "c");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::deci, char>, "d");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::deca, char>, "da");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::hecto, char>, "h");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::kilo, char>, "k");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::mega, char>, "M");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::giga, char>, "G");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::tera, char>, "T");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::peta, char>, "P");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::exa, char>, "E");

                static_assert(cmoon::measures::metric_system::suffix_v<std::atto, wchar_t> == L"a");
                static_assert(cmoon::measures::metric_system::suffix_v<std::femto, wchar_t> == L"f");
                static_assert(cmoon::measures::metric_system::suffix_v<std::pico, wchar_t> == L"p");
                static_assert(cmoon::measures::metric_system::suffix_v<std::nano, wchar_t> == L"n");
                static_assert(cmoon::measures::metric_system::suffix_v<std::micro, wchar_t> == L"u");
                static_assert(cmoon::measures::metric_system::suffix_v<std::milli, wchar_t> == L"m");
                static_assert(cmoon::measures::metric_system::suffix_v<std::centi, wchar_t> == L"c");
                static_assert(cmoon::measures::metric_system::suffix_v<std::deci, wchar_t> == L"d");
                static_assert(cmoon::measures::metric_system::suffix_v<std::deca, wchar_t> == L"da");
                static_assert(cmoon::measures::metric_system::suffix_v<std::hecto, wchar_t> == L"h");
                static_assert(cmoon::measures::metric_system::suffix_v<std::kilo, wchar_t> == L"k");
                static_assert(cmoon::measures::metric_system::suffix_v<std::mega, wchar_t> == L"M");
                static_assert(cmoon::measures::metric_system::suffix_v<std::giga, wchar_t> == L"G");
                static_assert(cmoon::measures::metric_system::suffix_v<std::tera, wchar_t> == L"T");
                static_assert(cmoon::measures::metric_system::suffix_v<std::peta, wchar_t> == L"P");
                static_assert(cmoon::measures::metric_system::suffix_v<std::exa, wchar_t> == L"E");

                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::atto, wchar_t>, L"a");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::femto, wchar_t>, L"f");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::pico, wchar_t>, L"p");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::nano, wchar_t>, L"n");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::micro, wchar_t>, L"u");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::milli, wchar_t>, L"m");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::centi, wchar_t>, L"c");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::deci, wchar_t>, L"d");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::deca, wchar_t>, L"da");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::hecto, wchar_t>, L"h");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::kilo, wchar_t>, L"k");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::mega, wchar_t>, L"M");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::giga, wchar_t>, L"G");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::tera, wchar_t>, L"T");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::peta, wchar_t>, L"P");
                cmoon::test::assert_equal(cmoon::measures::metric_system::suffix_v<std::exa, wchar_t>, L"E");
            }
    };
}