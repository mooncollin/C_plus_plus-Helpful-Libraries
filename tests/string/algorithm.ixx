export module cmoon.tests.string.algorithm;

import <string>;
import <vector>;
import <array>;
import <string_view>;

import cmoon.test;
import cmoon.string;

namespace cmoon::tests::string
{
    export
    class uppercase_test : public cmoon::test::test_case
    {
        public: 
            uppercase_test()
                : cmoon::test::test_case{"uppercase_test"} {}

            void operator()() final
            {
                const auto transform_f {[](std::string s) {
                    cmoon::toupper(s);
                    return s;
                }};

                cmoon::test::assert_equal(transform_f("hello!"), "HELLO!");
                cmoon::test::assert_equal(transform_f("h ello!"), "H ELLO!");
                cmoon::test::assert_equal(transform_f("helLO!"), "HELLO!");
                cmoon::test::assert_equal(transform_f("HEHOdd"), "HEHODD");
            }
    };

    export
    class lowercase_test : public cmoon::test::test_case
    {
        public:
            lowercase_test()
                : cmoon::test::test_case{"lowercase_test"} {}

            void operator()() final
            {
                const auto transform_f {[](std::string s) {
                    cmoon::tolower(s);
                    return s;
                }};

                cmoon::test::assert_equal(transform_f("hello!"), "hello!");
                cmoon::test::assert_equal(transform_f("h ello!"), "h ello!");
                cmoon::test::assert_equal(transform_f("helLO!"), "hello!");
                cmoon::test::assert_equal(transform_f("HEHOdd"), "hehodd");
            }
    };

    export
    class trim_front_test : public cmoon::test::test_case
    {
        public:
            trim_front_test()
                : cmoon::test::test_case{"trim_front_test"} {}

            void operator()() final
            {
                using namespace std::literals;

                cmoon::test::assert_equal(
                    cmoon::trim_front("Hello!"sv, "He"sv),
                    "llo!"
                );

                cmoon::test::assert_equal(
                    cmoon::trim_front("Hello!"sv, "H"sv),
                    "ello!"
                );

                cmoon::test::assert_equal(
                    cmoon::trim_front("HHHHHHello!"sv, "H"sv),
                    "ello!"
                );

                cmoon::test::assert_equal(
                    cmoon::trim_front("HeHello!"sv, "He"sv),
                    "llo!"
                );

                cmoon::test::assert_equal(
                    cmoon::trim_front("Hello!"sv, "J"sv),
                    "Hello!"
                );

                cmoon::test::assert_equal(
                    cmoon::trim_front("Hello!"sv, "!olleH"sv),
                    "Hello!"
                );

                cmoon::test::assert_equal(
                    cmoon::trim_front("Hello!"sv, "Hello!"sv),
                    ""
                );

                cmoon::test::assert_equal(
                    cmoon::trim_front("Hello!"sv, 'H'),
                    "ello!"
                );

                cmoon::test::assert_equal(
                    cmoon::trim_front("Hello!"sv, 'J'),
                    "Hello!"
                );

                cmoon::test::assert_equal(
                    cmoon::trim_front("Hello!"sv, '!'),
                    "Hello!"
                );
            }
    };

    export
    class trim_back_test : public cmoon::test::test_case
    {
        public:
            trim_back_test()
                : cmoon::test::test_case{"trim_back_test"} {}

            void operator()() final
            {
                using namespace std::literals;

                cmoon::test::assert_equal(
                    cmoon::trim_back("Hello!"sv, "o!"sv),
                    "Hell"
                );

                cmoon::test::assert_equal(
                    cmoon::trim_back("Hello!"sv, "!"sv),
                    "Hello"
                );

                cmoon::test::assert_equal(
                    cmoon::trim_back("Hello!!!!!!"sv, "!"sv),
                    "Hello"
                );

                cmoon::test::assert_equal(
                    cmoon::trim_back("Hello!o!"sv, "o!"sv),
                    "Hell"
                );

                cmoon::test::assert_equal(
                    cmoon::trim_back("Hello!"sv, "J"sv),
                    "Hello!"
                );

                cmoon::test::assert_equal(
                    cmoon::trim_back("Hello!"sv, "!olleH"sv),
                    "Hello!"
                );

                cmoon::test::assert_equal(
                    cmoon::trim_back("Hello!"sv, "Hello!"sv),
                    ""
                );

                cmoon::test::assert_equal(
                    cmoon::trim_back("Hello!"sv, '!'),
                    "Hello"
                );

                cmoon::test::assert_equal(
                    cmoon::trim_back("Hello!!!!!!!!!!"sv, '!'),
                    "Hello"
                );

                cmoon::test::assert_equal(
                    cmoon::trim_back("Hello!"sv, 'J'),
                    "Hello!"
                );

                cmoon::test::assert_equal(
                    cmoon::trim_back("Hello!"sv, 'H'),
                    "Hello!"
                );

                cmoon::test::assert_equal(
                    cmoon::trim_back("HHHH"sv, 'H'),
                    ""
                );
            }
    };
}