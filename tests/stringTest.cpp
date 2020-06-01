#include <utility>
#include <tuple>
#include <string>
#include <vector>
#include <locale>
#include "collin/string.hpp"
#include "collin/test.hpp"

using namespace std::literals;

class vowel_test : public collin::test::test_case
{
    public:
        vowel_test()
            : collin::test::test_case{"vowel_test"} {}

        void operator()() override
        {
            collin::test::assert_true(collin::strings::is_vowel('a'));
            collin::test::assert_false(collin::strings::is_vowel('b'));
            collin::test::assert_false(collin::strings::is_vowel('c'));
            collin::test::assert_false(collin::strings::is_vowel('d'));
            collin::test::assert_true(collin::strings::is_vowel('e'));
            collin::test::assert_false(collin::strings::is_vowel('f'));
            collin::test::assert_false(collin::strings::is_vowel('g'));
            collin::test::assert_false(collin::strings::is_vowel('h'));
            collin::test::assert_true(collin::strings::is_vowel('i'));
            collin::test::assert_false(collin::strings::is_vowel('j'));
            collin::test::assert_false(collin::strings::is_vowel('k'));
            collin::test::assert_false(collin::strings::is_vowel('l'));
            collin::test::assert_false(collin::strings::is_vowel('m'));
            collin::test::assert_false(collin::strings::is_vowel('n'));
            collin::test::assert_true(collin::strings::is_vowel('o'));
            collin::test::assert_false(collin::strings::is_vowel('p'));
            collin::test::assert_false(collin::strings::is_vowel('q'));
            collin::test::assert_false(collin::strings::is_vowel('r'));
            collin::test::assert_false(collin::strings::is_vowel('s'));
            collin::test::assert_false(collin::strings::is_vowel('t'));
            collin::test::assert_true(collin::strings::is_vowel('u'));
            collin::test::assert_false(collin::strings::is_vowel('v'));
            collin::test::assert_false(collin::strings::is_vowel('w'));
            collin::test::assert_false(collin::strings::is_vowel('x'));
            collin::test::assert_false(collin::strings::is_vowel('y'));
            collin::test::assert_false(collin::strings::is_vowel('z'));
        }
};

class consonant_test : public collin::test::test_case
{
    public:
        consonant_test()
            : collin::test::test_case{"consonant_test"} {}

        void operator()() override
        {
            collin::test::assert_false(collin::strings::is_consonant('a'));
            collin::test::assert_true(collin::strings::is_consonant('b'));
            collin::test::assert_true(collin::strings::is_consonant('c'));
            collin::test::assert_true(collin::strings::is_consonant('d'));
            collin::test::assert_false(collin::strings::is_consonant('e'));
            collin::test::assert_true(collin::strings::is_consonant('f'));
            collin::test::assert_true(collin::strings::is_consonant('g'));
            collin::test::assert_true(collin::strings::is_consonant('h'));
            collin::test::assert_false(collin::strings::is_consonant('i'));
            collin::test::assert_true(collin::strings::is_consonant('j'));
            collin::test::assert_true(collin::strings::is_consonant('k'));
            collin::test::assert_true(collin::strings::is_consonant('l'));
            collin::test::assert_true(collin::strings::is_consonant('m'));
            collin::test::assert_true(collin::strings::is_consonant('n'));
            collin::test::assert_false(collin::strings::is_consonant('o'));
            collin::test::assert_true(collin::strings::is_consonant('p'));
            collin::test::assert_true(collin::strings::is_consonant('q'));
            collin::test::assert_true(collin::strings::is_consonant('r'));
            collin::test::assert_true(collin::strings::is_consonant('s'));
            collin::test::assert_true(collin::strings::is_consonant('t'));
            collin::test::assert_false(collin::strings::is_consonant('u'));
            collin::test::assert_true(collin::strings::is_consonant('v'));
            collin::test::assert_true(collin::strings::is_consonant('w'));
            collin::test::assert_true(collin::strings::is_consonant('x'));
            collin::test::assert_true(collin::strings::is_consonant('y'));
            collin::test::assert_true(collin::strings::is_consonant('z'));
        }
};

class contains_vowel_test : public collin::test::test_case
{
    public:
        contains_vowel_test()
            : collin::test::test_case{ "contains_vowel_test" } {}

        void operator()() override
        {
            collin::test::assert_true(collin::strings::contains_vowel("Hello"));
            collin::test::assert_false(collin::strings::contains_vowel("My"));
            collin::test::assert_true(collin::strings::contains_vowel("cool"));
            collin::test::assert_true(collin::strings::contains_vowel("vowels"));
            collin::test::assert_true(collin::strings::contains_vowel("is"));
            collin::test::assert_false(collin::strings::contains_vowel("ghjbnj"));
        }
};

class contains_consonant_test : public collin::test::test_case
{
    public:
        contains_consonant_test()
            : collin::test::test_case{ "contains_consonant_test" } {}

        void operator()() override
        {
            collin::test::assert_true(collin::strings::contains_consonant("Hello"));
            collin::test::assert_true(collin::strings::contains_consonant("My"));
            collin::test::assert_true(collin::strings::contains_consonant("cool"));
            collin::test::assert_true(collin::strings::contains_consonant("vowels"));
            collin::test::assert_true(collin::strings::contains_consonant("is"));
            collin::test::assert_true(collin::strings::contains_consonant("ghjbnj"));
            collin::test::assert_false(collin::strings::contains_consonant("iiii"));
        }
};

class remove_punctuation_test : public collin::test::test_case
{
    public:
        remove_punctuation_test()
            : collin::test::test_case{ "remove_punctuation_test" } {}

        void operator()() override
        {
            const auto transform_f = [](std::string s) {
                collin::strings::remove_punctuation(s);
                return s;
            };

            collin::test::assert_equal(transform_f("!!"), "");
            collin::test::assert_equal(transform_f("!!fef"), "fef");
            collin::test::assert_equal(transform_f("Hello!"), "Hello");
            collin::test::assert_equal(transform_f("I/MfffS.OC00l!"), "IMfffSOC00l");
        }
};

class remove_spaces_test : public collin::test::test_case
{
    public:
        remove_spaces_test()
            : collin::test::test_case{ "remove_spaces_test" } {}

        void operator()() override
        {
            const auto transform_f = [](std::string s) {
                collin::strings::remove_spaces(s);
                return s;
            };

            collin::test::assert_equal(transform_f("These are words"), "Thesearewords");
            collin::test::assert_equal(transform_f("Hello !!"), "Hello!!");
            collin::test::assert_equal(transform_f("What"), "What");
            collin::test::assert_equal(transform_f("Teeee ee"), "Teeeeee");
            collin::test::assert_equal(transform_f("fff "), "fff");
        }
};

class remove_alphanumerics_test : public collin::test::test_case
{
    public:
        remove_alphanumerics_test()
            : collin::test::test_case{ "remove_alphanumerics_test" } {}

        void operator()() override
        {
            const auto transform_f = [](std::string s) {
                collin::strings::remove_alphanumerics(s);
                return s;
            };

            collin::test::assert_equal(transform_f("How Ar3 Y0u??"), "  ??");
            collin::test::assert_equal(transform_f("T$$ To^"), "$$ ^");
        }
};

class remove_blanks_test : public collin::test::test_case
{
    public:
        remove_blanks_test()
            : collin::test::test_case{ "remove_blanks_test" } {}

        void operator()() override
        {
            const auto transform_f = [](std::string s) {
                collin::strings::remove_blanks(s);
                return s;
            };

            collin::test::assert_equal(transform_f("These are words"), "Thesearewords");
            collin::test::assert_equal(transform_f("Hello\t!!"), "Hello!!");
            collin::test::assert_equal(transform_f("What"), "What");
            collin::test::assert_equal(transform_f("Teeee  \tee"), "Teeeeee");
            collin::test::assert_equal(transform_f("fff   t\t\n"), "ffft");
        }
};

class remove_hexdigits_test : public collin::test::test_case
{
    public:
        remove_hexdigits_test()
            : collin::test::test_case{ "remove_hexdigits_test" } {}

        void operator()() override
        {
            const auto transform_f = [](std::string s) {
                collin::strings::remove_hexdigits(s);
                return s;
            };

            collin::test::assert_equal(transform_f("A"), "");
            collin::test::assert_equal(transform_f("B"), "");
            collin::test::assert_equal(transform_f("C"), "");
            collin::test::assert_equal(transform_f("D"), "");
            collin::test::assert_equal(transform_f("E"), "");
            collin::test::assert_equal(transform_f("F"), "");
            collin::test::assert_equal(transform_f("1"), "");
            collin::test::assert_equal(transform_f("2"), "");
            collin::test::assert_equal(transform_f("3"), "");
            collin::test::assert_equal(transform_f("4"), "");
            collin::test::assert_equal(transform_f("5"), "");
            collin::test::assert_equal(transform_f("6"), "");
            collin::test::assert_equal(transform_f("7"), "");
            collin::test::assert_equal(transform_f("8"), "");
            collin::test::assert_equal(transform_f("9"), "");
            collin::test::assert_equal(transform_f("0"), "");
            collin::test::assert_equal(transform_f("Auihel88 *"), "uihl *");
        }
};

class remove_upper_test : public collin::test::test_case
{
    public:
        remove_upper_test()
            : collin::test::test_case{ "remove_upper_test" } {}

        void operator()() override
        {
            const auto transform_f = [](std::string s) {
                collin::strings::remove_upper(s);
                return s;
            };

            collin::test::assert_equal(transform_f("SUP"), "");
            collin::test::assert_equal(transform_f("Sup"), "up");
            collin::test::assert_equal(transform_f("SUP !!"), " !!");
        }
};

class remove_lower_test : public collin::test::test_case
{
    public:
        remove_lower_test()
            : collin::test::test_case{"remove_lower_test"} {}

        void operator()() override
        {
            const auto transform_f = [](std::string s) {
                collin::strings::remove_lower(s);
                return s;
            };

            collin::test::assert_equal(transform_f("SUP"), "SUP");
            collin::test::assert_equal(transform_f("Sup"), "S");
            collin::test::assert_equal(transform_f("SUP f!!"), "SUP !!");
        }
};

class remove_character_test : public collin::test::test_case
{
    public:
        remove_character_test()
            : collin::test::test_case{"remove_character_test"} {}

        void operator()() override
        {
            const auto transform_f = [](std::string s, char c) {
                collin::strings::remove_character(s, c);
                return s;
            };

            collin::test::assert_equal(transform_f("cccc", 'c'), "");
            collin::test::assert_equal(transform_f("ccccFFF", 'c'), "FFF");
            collin::test::assert_equal(transform_f("Hello!", 'l'), "Heo!");
            collin::test::assert_equal(transform_f("Tee Tee", 'T'), "ee ee");
        }
};

class uppercase_test : public collin::test::test_case
{
    public: 
        uppercase_test()
            : collin::test::test_case{"uppercase_test"} {}

        void operator()() override
        {
            const auto transform_f = [](std::string s) {
                collin::strings::uppercase(s);
                return s;
            };

            collin::test::assert_equal(transform_f("hello!"), "HELLO!");
            collin::test::assert_equal(transform_f("h ello!"), "H ELLO!");
            collin::test::assert_equal(transform_f("helLO!"), "HELLO!");
            collin::test::assert_equal(transform_f("HEHOdd"), "HEHODD");
        }
};

class lowercase_test : public collin::test::test_case
{
    public:
        lowercase_test()
            : collin::test::test_case{"lowercase_test"} {}

        void operator()() override
        {
            const auto transform_f = [](std::string s) {
                collin::strings::lowercase(s);
                return s;
            };

            collin::test::assert_equal(transform_f("hello!"), "hello!");
            collin::test::assert_equal(transform_f("h ello!"), "h ello!");
            collin::test::assert_equal(transform_f("helLO!"), "hello!");
            collin::test::assert_equal(transform_f("HEHOdd"), "hehodd");
        }
};

class split_test : public collin::test::test_case
{
    public:
        split_test()
            : collin::test::test_case{"split_test"} {}

        void operator()() override
        {
            collin::test::assert_equal(collin::strings::split("Here are words", " "), std::vector<std::string_view>{ "Here", "are", "words"});
            collin::test::assert_equal(collin::strings::split("Here!are!words", "!"), std::vector<std::string_view>{ "Here", "are", "words" });
            collin::test::assert_equal(collin::strings::split("Here are words", " ", 1), std::vector<std::string_view>{ "Here", "are words" });
            collin::test::assert_equal(collin::strings::split("Here are words", " ", 2), std::vector<std::string_view>{ "Here", "are", "words" });
            collin::test::assert_equal(collin::strings::split("Here are words", " ", 3), std::vector<std::string_view>{ "Here", "are", "words" });
            collin::test::assert_equal(collin::strings::split("Here are words", " ", 99), std::vector<std::string_view>{ "Here", "are", "words" });
            collin::test::assert_equal(collin::strings::split("Here are words", " ", 0), std::vector<std::string_view>{ "Here are words" });

            std::array<std::string_view, 3> results;
            collin::strings::split("I like,csv,files", ",", std::begin(results), 3);

            collin::test::assert_equal(results, std::array<std::string_view, 3>{"I like", "csv", "files"});
        }
};

class join_test : public collin::test::test_case
{
    public:
        join_test()
            : collin::test::test_case{"join_test"} {}

        void operator()() override
        {
            collin::test::assert_equal(collin::strings::join(std::vector {"These", "are", "words"}, " "), "These are words", "join strings space delimiter");
            collin::test::assert_equal(collin::strings::join(std::vector{ "These", "are", "words" }, "\n"), "These\nare\nwords", "join strings newline delimiter");

            const std::vector words {"Look", "at", "these", "strings"};

            collin::test::assert_equal(collin::strings::join(std::begin(words), std::end(words), " "), "Look at these strings", "join iterators space delimiter");
            collin::test::assert_equal(collin::strings::join(std::begin(words), std::end(words), "$$$$"), "Look$$$$at$$$$these$$$$strings", "join iterators '$' delimiter");
        }
};

class from_string_test : public collin::test::test_case
{
    public:
        from_string_test()
            : collin::test::test_case{"from_string_test"} {}

        void operator()() override
        {
            collin::test::assert_equal(collin::strings::from_string<int>("1"), 1);
            collin::test::assert_equal(collin::strings::from_string<int>("1223"), 1223);
            collin::test::assert_equal(collin::strings::from_string<int>("-2391"), -2391);
            collin::test::assert_equal(collin::strings::from_string<int>("37943"), 37943);
            collin::test::assert_equal(collin::strings::from_string<int>("0"), 0);

            collin::test::assert_almost_equal(collin::strings::from_string<double>("1.5"), 1.5, 0.000001);
            collin::test::assert_almost_equal(collin::strings::from_string<double>("-2.23478"), -2.23478, 0.000001);
            collin::test::assert_almost_equal(collin::strings::from_string<double>("0"), 0.0, 0.000001);
            collin::test::assert_almost_equal(collin::strings::from_string<double>("1"), 1.0, 0.000001);

            collin::test::assert_equal(collin::strings::from_string<char>("c"), 'c');
            collin::test::assert_equal(collin::strings::from_string<char>("38hs9f"), '3');

            collin::test::assert_equal(collin::strings::from_string<std::string>("Hello!"), "Hello!");
            collin::test::assert_equal(collin::strings::from_string<std::string_view>("Hello!"), "Hello!");

            collin::test::assert_equal(collin::strings::from_string<bool>("1"), true);
        }
};

int main()
{
    collin::test::test_suite suite;

    suite.add_test_case<vowel_test>();
    suite.add_test_case<consonant_test>();
    suite.add_test_case<contains_vowel_test>();
    suite.add_test_case<contains_consonant_test>();
    suite.add_test_case<remove_punctuation_test>();
    suite.add_test_case<remove_spaces_test>();
    suite.add_test_case<remove_alphanumerics_test>();
    suite.add_test_case<remove_blanks_test>();
    suite.add_test_case<remove_hexdigits_test>();
    suite.add_test_case<remove_upper_test>();
    suite.add_test_case<remove_lower_test>();
    suite.add_test_case<remove_character_test>();
    suite.add_test_case<uppercase_test>();
    suite.add_test_case<lowercase_test>();
    suite.add_test_case<split_test>();
    suite.add_test_case<join_test>();
    suite.add_test_case<from_string_test>();

    collin::test::text_test_runner runner(std::cout);
    runner.run(suite);
}