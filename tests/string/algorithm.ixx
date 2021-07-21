export module cmoon.tests.string.algorithm;

import <string>;
import <vector>;
import <array>;

import cmoon.test;
import cmoon.string;

namespace cmoon::tests::string
{
    export
	class remove_punctuation_test : public cmoon::test::test_case
    {
        public:
            remove_punctuation_test()
                : cmoon::test::test_case{ "remove_punctuation_test" } {}

            void operator()() override
            {
                const auto transform_f = [](std::string s) {
                    cmoon::erase_punctuation(s);
                    return s;
                };

                cmoon::test::assert_equal(transform_f("!!"), "");
                cmoon::test::assert_equal(transform_f("!!fef"), "fef");
                cmoon::test::assert_equal(transform_f("Hello!"), "Hello");
                cmoon::test::assert_equal(transform_f("I/MfffS.OC00l!"), "IMfffSOC00l");
            }
    };

    export
    class remove_spaces_test : public cmoon::test::test_case
    {
        public:
            remove_spaces_test()
                : cmoon::test::test_case{ "remove_spaces_test" } {}

            void operator()() override
            {
                const auto transform_f = [](std::string s) {
                    cmoon::erase_spaces(s);
                    return s;
                };

                cmoon::test::assert_equal(transform_f("These are words"), "Thesearewords");
                cmoon::test::assert_equal(transform_f("Hello !!"), "Hello!!");
                cmoon::test::assert_equal(transform_f("What"), "What");
                cmoon::test::assert_equal(transform_f("Teeee ee"), "Teeeeee");
                cmoon::test::assert_equal(transform_f("fff "), "fff");
            }
    };

    export
    class remove_alphanumerics_test : public cmoon::test::test_case
    {
        public:
            remove_alphanumerics_test()
                : cmoon::test::test_case{ "remove_alphanumerics_test" } {}

            void operator()() override
            {
                const auto transform_f = [](std::string s) {
                    cmoon::erase_alphanumerics(s);
                    return s;
                };

                cmoon::test::assert_equal(transform_f("How Ar3 Y0u??"), "  ??");
                cmoon::test::assert_equal(transform_f("T$$ To^"), "$$ ^");
            }
    };

    export
    class remove_blanks_test : public cmoon::test::test_case
    {
        public:
            remove_blanks_test()
                : cmoon::test::test_case{ "remove_blanks_test" } {}

            void operator()() override
            {
                const auto transform_f = [](std::string s) {
                    cmoon::erase_blanks(s);
                    return s;
                };

                cmoon::test::assert_equal(transform_f("These are words"), "Thesearewords");
                cmoon::test::assert_equal(transform_f("Hello\t!!"), "Hello!!");
                cmoon::test::assert_equal(transform_f("What"), "What");
                cmoon::test::assert_equal(transform_f("Teeee  \tee"), "Teeeeee");
                cmoon::test::assert_equal(transform_f("fff   t\t\n"), "ffft");
            }
    };

    export
    class remove_hexdigits_test : public cmoon::test::test_case
    {
        public:
            remove_hexdigits_test()
                : cmoon::test::test_case{ "remove_hexdigits_test" } {}

            void operator()() override
            {
                const auto transform_f = [](std::string s) {
                    cmoon::erase_hexdigits(s);
                    return s;
                };

                cmoon::test::assert_equal(transform_f("A"), "");
                cmoon::test::assert_equal(transform_f("B"), "");
                cmoon::test::assert_equal(transform_f("C"), "");
                cmoon::test::assert_equal(transform_f("D"), "");
                cmoon::test::assert_equal(transform_f("E"), "");
                cmoon::test::assert_equal(transform_f("F"), "");
                cmoon::test::assert_equal(transform_f("1"), "");
                cmoon::test::assert_equal(transform_f("2"), "");
                cmoon::test::assert_equal(transform_f("3"), "");
                cmoon::test::assert_equal(transform_f("4"), "");
                cmoon::test::assert_equal(transform_f("5"), "");
                cmoon::test::assert_equal(transform_f("6"), "");
                cmoon::test::assert_equal(transform_f("7"), "");
                cmoon::test::assert_equal(transform_f("8"), "");
                cmoon::test::assert_equal(transform_f("9"), "");
                cmoon::test::assert_equal(transform_f("0"), "");
                cmoon::test::assert_equal(transform_f("Auihel88 *"), "uihl *");
            }
    };

    export
    class remove_upper_test : public cmoon::test::test_case
    {
        public:
            remove_upper_test()
                : cmoon::test::test_case{ "remove_upper_test" } {}

            void operator()() override
            {
                const auto transform_f = [](std::string s) {
                    cmoon::erase_upper(s);
                    return s;
                };

                cmoon::test::assert_equal(transform_f("SUP"), "");
                cmoon::test::assert_equal(transform_f("Sup"), "up");
                cmoon::test::assert_equal(transform_f("SUP !!"), " !!");
            }
    };

    export
    class remove_lower_test : public cmoon::test::test_case
    {
        public:
            remove_lower_test()
                : cmoon::test::test_case{"remove_lower_test"} {}

            void operator()() override
            {
                const auto transform_f = [](std::string s) {
                    cmoon::erase_lower(s);
                    return s;
                };

                cmoon::test::assert_equal(transform_f("SUP"), "SUP");
                cmoon::test::assert_equal(transform_f("Sup"), "S");
                cmoon::test::assert_equal(transform_f("SUP f!!"), "SUP !!");
            }
    };

    export
    class remove_character_test : public cmoon::test::test_case
    {
        public:
            remove_character_test()
                : cmoon::test::test_case{"remove_character_test"} {}

            void operator()() override
            {
                const auto transform_f = [](std::string s, char c) {
                    cmoon::erase_character(s, c);
                    return s;
                };

                cmoon::test::assert_equal(transform_f("cccc", 'c'), "");
                cmoon::test::assert_equal(transform_f("ccccFFF", 'c'), "FFF");
                cmoon::test::assert_equal(transform_f("Hello!", 'l'), "Heo!");
                cmoon::test::assert_equal(transform_f("Tee Tee", 'T'), "ee ee");
            }
    };

    export
    class uppercase_test : public cmoon::test::test_case
    {
        public: 
            uppercase_test()
                : cmoon::test::test_case{"uppercase_test"} {}

            void operator()() override
            {
                const auto transform_f = [](std::string s) {
                    cmoon::uppercase(s);
                    return s;
                };

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

            void operator()() override
            {
                const auto transform_f = [](std::string s) {
                    cmoon::lowercase(s);
                    return s;
                };

                cmoon::test::assert_equal(transform_f("hello!"), "hello!");
                cmoon::test::assert_equal(transform_f("h ello!"), "h ello!");
                cmoon::test::assert_equal(transform_f("helLO!"), "hello!");
                cmoon::test::assert_equal(transform_f("HEHOdd"), "hehodd");
            }
    };

    export
    class split_test : public cmoon::test::test_case
    {
        public:
            split_test()
                : cmoon::test::test_case{"split_test"} {}

            void operator()() override
            {
                const auto v = cmoon::split(std::string_view{"Here are words"}, std::string_view{" "});
                cmoon::test::assert_equal(cmoon::split(std::string_view{"Here are words"}, std::string_view{" "}), std::vector<std::string_view>{ "Here", "are", "words"});
                cmoon::test::assert_equal(cmoon::split(std::string_view{"Here!are!words"}, std::string_view{"!"}), std::vector<std::string_view>{ "Here", "are", "words" });
                cmoon::test::assert_equal(cmoon::split(std::string_view{"Here are words"}, std::string_view{" "}, 1), std::vector<std::string_view>{ "Here", "are words" });
                cmoon::test::assert_equal(cmoon::split(std::string_view{"Here are words"}, std::string_view{" "}, 2), std::vector<std::string_view>{ "Here", "are", "words" });
                cmoon::test::assert_equal(cmoon::split(std::string_view{"Here are words"}, std::string_view{" "}, 3), std::vector<std::string_view>{ "Here", "are", "words" });
                cmoon::test::assert_equal(cmoon::split(std::string_view{"Here are words"}, std::string_view{" "}, 99), std::vector<std::string_view>{ "Here", "are", "words" });
                cmoon::test::assert_equal(cmoon::split(std::string_view{"Here are words"}, std::string_view{" "}, 0), std::vector<std::string_view>{ "Here are words" });

                std::array<std::string_view, 3> results;
                cmoon::split(std::string_view{"I like,csv,files"}, std::string_view{","}, std::begin(results), 3);

                cmoon::test::assert_equal(results, std::array<std::string_view, 3>{"I like", "csv", "files"});
            }
    };

    export
    class join_test : public cmoon::test::test_case
    {
        public:
            join_test()
                : cmoon::test::test_case{"join_test"} {}

            void operator()() override
            {
                cmoon::test::assert_equal(cmoon::join(std::vector{"These", "are", "words"}, std::string_view{" "}), std::string_view{"These are words"}, std::string_view{"join strings space delimiter"});
                cmoon::test::assert_equal(cmoon::join(std::vector{"These", "are", "words"}, std::string_view{"\n"}), std::string_view{"These\nare\nwords"}, std::string_view{"join strings newline delimiter"});

                const std::vector words {"Look", "at", "these", "strings"};

                cmoon::test::assert_equal(cmoon::join(std::begin(words), std::end(words), std::string_view{" "}), std::string_view{"Look at these strings"}, std::string_view{"join iterators space delimiter"});
                cmoon::test::assert_equal(cmoon::join(std::begin(words), std::end(words), std::string_view{"$$$$"}), std::string_view{"Look$$$$at$$$$these$$$$strings"}, std::string_view{"join iterators '$' delimiter"});
            }
    };
}