#include <utility>
#include <tuple>
#include <string>
#include <vector>
#include <locale>
#include "collin/string.hpp"
#include "collin/test.hpp"

using namespace std::literals;

void is_vowel_test()
{
	const auto test_values = collin::test::make_test_data
	(
		collin::strings::is_vowel<char>,
			{
				std::make_pair(std::make_tuple('a', std::locale{}), true),
				std::make_pair(std::make_tuple('b', std::locale{}), false),
				std::make_pair(std::make_tuple('c', std::locale{}), false),
				std::make_pair(std::make_tuple('d', std::locale{}), false),
				std::make_pair(std::make_tuple('e', std::locale{}), true),
				std::make_pair(std::make_tuple('f', std::locale{}), false),
				std::make_pair(std::make_tuple('g', std::locale{}), false),
				std::make_pair(std::make_tuple('h', std::locale{}), false),
				std::make_pair(std::make_tuple('i', std::locale{}), true),
				std::make_pair(std::make_tuple('j', std::locale{}), false),
				std::make_pair(std::make_tuple('k', std::locale{}), false),
				std::make_pair(std::make_tuple('l', std::locale{}), false),
				std::make_pair(std::make_tuple('m', std::locale{}), false),
				std::make_pair(std::make_tuple('n', std::locale{}), false),
				std::make_pair(std::make_tuple('o', std::locale{}), true),
				std::make_pair(std::make_tuple('p', std::locale{}), false),
				std::make_pair(std::make_tuple('q', std::locale{}), false),
				std::make_pair(std::make_tuple('r', std::locale{}), false),
				std::make_pair(std::make_tuple('s', std::locale{}), false),
				std::make_pair(std::make_tuple('t', std::locale{}), false),
				std::make_pair(std::make_tuple('u', std::locale{}), true),
				std::make_pair(std::make_tuple('v', std::locale{}), false),
				std::make_pair(std::make_tuple('w', std::locale{}), false),
				std::make_pair(std::make_tuple('x', std::locale{}), false),
				std::make_pair(std::make_tuple('y', std::locale{}), false),
				std::make_pair(std::make_tuple('z', std::locale{}), false),
			}
	);

	collin::test::assert_test_data(test_values, "is_vowel");
}

void is_consonant_test()
{
	const auto test_values = collin::test::make_test_data
	(
		collin::strings::is_consonant<char>,
			{
				std::make_pair(std::make_tuple('a', std::locale{}), false),
				std::make_pair(std::make_tuple('b', std::locale{}), true),
				std::make_pair(std::make_tuple('c', std::locale{}), true),
				std::make_pair(std::make_tuple('d', std::locale{}), true),
				std::make_pair(std::make_tuple('e', std::locale{}), false),
				std::make_pair(std::make_tuple('f', std::locale{}), true),
				std::make_pair(std::make_tuple('g', std::locale{}), true),
				std::make_pair(std::make_tuple('h', std::locale{}), true),
				std::make_pair(std::make_tuple('i', std::locale{}), false),
				std::make_pair(std::make_tuple('j', std::locale{}), true),
				std::make_pair(std::make_tuple('k', std::locale{}), true),
				std::make_pair(std::make_tuple('l', std::locale{}), true),
				std::make_pair(std::make_tuple('m', std::locale{}), true),
				std::make_pair(std::make_tuple('n', std::locale{}), true),
				std::make_pair(std::make_tuple('o', std::locale{}), false),
				std::make_pair(std::make_tuple('p', std::locale{}), true),
				std::make_pair(std::make_tuple('q', std::locale{}), true),
				std::make_pair(std::make_tuple('r', std::locale{}), true),
				std::make_pair(std::make_tuple('s', std::locale{}), true),
				std::make_pair(std::make_tuple('t', std::locale{}), true),
				std::make_pair(std::make_tuple('u', std::locale{}), false),
				std::make_pair(std::make_tuple('v', std::locale{}), true),
				std::make_pair(std::make_tuple('w', std::locale{}), true),
				std::make_pair(std::make_tuple('x', std::locale{}), true),
				std::make_pair(std::make_tuple('y', std::locale{}), true),
				std::make_pair(std::make_tuple('z', std::locale{}), true),
			}
	);

	collin::test::assert_test_data(test_values, "is_consonant");
}

void contains_vowel_test()
{
	const auto test_values = collin::test::make_test_data
	(
		static_cast<bool(*)(const std::string&, const std::locale&)>(collin::strings::contains_vowel),
			{
				std::make_pair(std::make_tuple("Hello", std::locale{}), true),
				std::make_pair(std::make_tuple("My", std::locale{}), false),
				std::make_pair(std::make_tuple("cool", std::locale{}), true),
				std::make_pair(std::make_tuple("vowels", std::locale{}), true),
				std::make_pair(std::make_tuple("is", std::locale{}), true),
				std::make_pair(std::make_tuple("ghjbnj", std::locale{}), false),
			}
	);

	collin::test::assert_test_data(test_values, "contains_vowel");
}

void contains_consonant_test()
{
	const auto test_values = collin::test::make_test_data
	(
		static_cast<bool(*)(const std::string&, const std::locale&)>(collin::strings::contains_consonant<std::string>),
			{
				std::make_pair(std::make_tuple("Hello", std::locale{}), true),
				std::make_pair(std::make_tuple("My", std::locale{}), true),
				std::make_pair(std::make_tuple("cool", std::locale{}), true),
				std::make_pair(std::make_tuple("vowels", std::locale{}), true),
				std::make_pair(std::make_tuple("is", std::locale{}), true),
				std::make_pair(std::make_tuple("ghjbnj", std::locale{}), true),
				std::make_pair(std::make_tuple("iiii", std::locale{}), false),
			}
	);

	collin::test::assert_test_data(test_values, "contains_consonant");
}

void remove_punctuation_test()
{
	const auto transform_f = [] (std::string s) {
		collin::strings::remove_punctuation(s);
		return s;
	};

	const auto test_values = collin::test::make_test_data
	(
		transform_f,
			{
				std::make_pair(std::make_tuple("!!"), ""s),
				std::make_pair(std::make_tuple("!!fef"), "fef"s),
				std::make_pair(std::make_tuple("Hello!"), "Hello"s),
				std::make_pair(std::make_tuple("I/MfffS.OC00l!"), "IMfffSOC00l"s)
			}
	);

	collin::test::assert_test_data(test_values, "remove_punctuation");
}

void remove_spaces_test()
{
	const auto transform_f = [] (std::string s) {
		collin::strings::remove_spaces(s);
		return s;
	};

	const auto test_values = collin::test::make_test_data
	(
		transform_f,
			{
				std::make_pair(std::make_tuple("These are words"), "Thesearewords"s),
				std::make_pair(std::make_tuple("Hello !!"), "Hello!!"s),
				std::make_pair(std::make_tuple("What"), "What"s),
				std::make_pair(std::make_tuple("Teeee ee"), "Teeeeee"s),
				std::make_pair(std::make_tuple("fff "), "fff"s)
			}
	);

	collin::test::assert_test_data(test_values, "remove_spaces");
}

 void remove_alphanumerics_test()
 {
 	const auto transform_f = [] (std::string s) {
 		collin::strings::remove_alphanumerics(s);
 		return s;
 	};

 	const auto test_values = collin::test::make_test_data
 	(
 		transform_f,
 			{
 				std::make_pair(std::make_tuple("How Ar3 Y0u??"), "  ??"s),
 				std::make_pair(std::make_tuple("T$$ To^"), "$$ ^"s),
 			}
 	);

 	collin::test::assert_test_data(test_values, "remove_alphanumerics");
 }

 void remove_blanks_test()
 {
 	const auto transform_f = [] (std::string s) {
 		collin::strings::remove_blanks(s);
 		return s;
 	};

 	const auto test_values = collin::test::make_test_data
 	(
 		transform_f,
 			{
 				std::make_pair(std::make_tuple("These are words"), "Thesearewords"s),
 				std::make_pair(std::make_tuple("Hello\t!!"), "Hello!!"s),
 				std::make_pair(std::make_tuple("What"), "What"s),
 				std::make_pair(std::make_tuple("Teeee  \tee"), "Teeeeee"s),
 				std::make_pair(std::make_tuple("fff   t\t\n"), std::isblank('\n', std::locale{}) ? "ffft"s : "ffft\n"s)
 			}
 	);

 	collin::test::assert_test_data(test_values, "remove_blanks");
 }

 void remove_hexdigits_test()
 {
 	const auto transform_f = [] (std::string s) {
 		collin::strings::remove_hexdigits(s);
 		return s;
 	};

 	const auto test_values = collin::test::make_test_data
 	(
 		transform_f,
 		 {
 			 std::make_pair(std::make_tuple("A"), ""s),
 			 std::make_pair(std::make_tuple("B"), ""s),
 			 std::make_pair(std::make_tuple("C"), ""s),
 			 std::make_pair(std::make_tuple("D"), ""s),
 			 std::make_pair(std::make_tuple("E"), ""s),
 			 std::make_pair(std::make_tuple("F"), ""s),
 			 std::make_pair(std::make_tuple("1"), ""s),
 			 std::make_pair(std::make_tuple("2"), ""s),
 			 std::make_pair(std::make_tuple("3"), ""s),
 			 std::make_pair(std::make_tuple("4"), ""s),
 			 std::make_pair(std::make_tuple("5"), ""s),
 			 std::make_pair(std::make_tuple("6"), ""s),
 			 std::make_pair(std::make_tuple("7"), ""s),
 			 std::make_pair(std::make_tuple("8"), ""s),
 			 std::make_pair(std::make_tuple("9"), ""s),
 			 std::make_pair(std::make_tuple("0"), ""s),
 			 std::make_pair(std::make_tuple("Auihel88 *"), "uihl *"s),
 		 }
 	);

 	collin::test::assert_test_data(test_values, "remove_hexdigits");
 }

 void remove_upper_test()
 {
 	const auto transform_f = [] (std::string s) {
 		collin::strings::remove_upper(s);
 		return s;
 	};

 	const auto test_values = collin::test::make_test_data
 	(
 		transform_f,
 			{
 				std::make_pair(std::make_tuple("SUP"), ""s),
 				std::make_pair(std::make_tuple("Sup"), "up"s),
 				std::make_pair(std::make_tuple("SUP !!"), " !!"s),
 			}
 	);

 	collin::test::assert_test_data(test_values, "remove_upper");
 }

 void remove_lower_test()
 {
 	const auto transform_f = [] (std::string s) {
 		collin::strings::remove_lower(s);
 		return s;
 	};

 	const auto test_values = collin::test::make_test_data
 	(
 		transform_f,
 			{
 				std::make_pair(std::make_tuple("SUP"), "SUP"s),
 				std::make_pair(std::make_tuple("Sup"), "S"s),
 				std::make_pair(std::make_tuple("SUP f!!"), "SUP !!"s),
 			}
 	);

 	collin::test::assert_test_data(test_values, "remove_lower");
 }

 void remove_character_test()
 {
 	const auto transform_f = [] (std::string s, char c) {
 		collin::strings::remove_character(s, c);
 		return s;
 	};

 	const auto test_values = collin::test::make_test_data
 	(
 		transform_f,
 			{
 				std::make_pair(std::make_tuple("cccc", 'c'), ""s),
 				std::make_pair(std::make_tuple("ccccFFF", 'c'), "FFF"s),
 				std::make_pair(std::make_tuple("Hello!", 'l'), "Heo!"s),
 				std::make_pair(std::make_tuple("Tee Tee", 'T'), "ee ee"s),
 			}
 	);

 	collin::test::assert_test_data(test_values, "remove_character");
 }

 void uppercase_test()
 {
 	const auto transform_f = [] (std::string s) {
 		collin::strings::uppercase(s);
 		return s;
 	};
	
 	const auto test_values = collin::test::make_test_data
 	(
 		transform_f,
 			{
 				std::make_pair(std::make_tuple("hello!"), "HELLO!"s),
 				std::make_pair(std::make_tuple("h ello!"), "H ELLO!"s),
 				std::make_pair(std::make_tuple("helLO!"), "HELLO!"s),
 				std::make_pair(std::make_tuple("HEHOdd"), "HEHODD"s),
 			}
 	);

 	collin::test::assert_test_data(test_values, "uppercase");
 }

 void lowercase_test()
 {
 	const auto transform_f = [] (std::string s) {
 		collin::strings::lowercase(s);
 		return s;
 	};
	
 	const auto test_values = collin::test::make_test_data
 	(
 		transform_f,
 			{
 				std::make_pair(std::make_tuple("hello!"), "hello!"s),
 				std::make_pair(std::make_tuple("h ello!"), "h ello!"s),
 				std::make_pair(std::make_tuple("helLO!"), "hello!"s),
 				std::make_pair(std::make_tuple("HEHOdd"), "hehodd"s),
 			}
 	);

 	collin::test::assert_test_data(test_values, "lowercase");
 }

 template<typename T>
 auto get_split_numeric_values()
 {
 	return collin::test::make_test_data
 	(
 		collin::strings::split<T, std::string>,
 			{
 				std::make_pair(std::make_tuple("1 2 3 4", " ", -1), std::vector<T> {1, 2, 3, 4}),
 				std::make_pair(std::make_tuple("1\t2\t3\t4", "\t", -1), std::vector<T> {1, 2, 3, 4}),
 				std::make_pair(std::make_tuple("1fe2fe3fe4", "fe", -1), std::vector<T> {1, 2, 3, 4}),
 				std::make_pair(std::make_tuple("1|2|3|4", "\\|", -1), std::vector<T> {1, 2, 3, 4})
 			}
 	);
 }

 void split_test()
 {
 	std::error_code ec;
 	const auto test_string_values = collin::test::make_test_data
 	(
 		collin::strings::split<std::string, std::string>,
 			{
 				std::make_pair(std::make_tuple("Here are words", " ", -1), std::vector<std::string> {"Here", "are", "words"}),
 				std::make_pair(std::make_tuple("Here!are!words", "!", -1), std::vector<std::string> {"Here", "are", "words"}),
 				std::make_pair(std::make_tuple("Here are words", " ", 1), std::vector<std::string> {"Here", "are words"}),
 				std::make_pair(std::make_tuple("Here are words", " ", 2), std::vector<std::string> {"Here", "are", "words"}),
 				std::make_pair(std::make_tuple("Here are words", " ", 3), std::vector<std::string> {"Here", "are", "words"}),
 				std::make_pair(std::make_tuple("Here are words", " ", 99), std::vector<std::string> {"Here", "are", "words"}),
 				std::make_pair(std::make_tuple("Here are words", " ", 0), std::vector<std::string> {"Here are words"})
 			}
 	);

 	const auto test_int_values = get_split_numeric_values<int>();
 	const auto test_long_values = get_split_numeric_values<long>();
 	const auto test_unsigned_long_values = get_split_numeric_values<unsigned long>();
 	const auto test_long_int_values = get_split_numeric_values<long int>();
 	const auto test_float_values = get_split_numeric_values<float>();
 	const auto test_double_values = get_split_numeric_values<double>();
 	const auto test_long_double_values = get_split_numeric_values<long double>();
 	const auto test_long_long_values = get_split_numeric_values<long long>();
 	const auto test_unsigned_long_long_values = get_split_numeric_values<unsigned long long>();

 	collin::test::assert_test_data(test_string_values, "split Strings");
 	collin::test::assert_test_data(test_int_values, "split Ints");
 	collin::test::assert_test_data(test_long_values, "split Longs");
 	collin::test::assert_test_data(test_unsigned_long_values, "split Unsigned Longs");
 	collin::test::assert_test_data(test_long_int_values, "split Long Ints");
 	collin::test::assert_test_data(test_float_values, "split Floats");
 	collin::test::assert_test_data(test_double_values, "split Doubles");
 	collin::test::assert_test_data(test_long_double_values, "split Long Doubles");
 	collin::test::assert_test_data(test_long_long_values, "split Long Longs");
 	collin::test::assert_test_data(test_unsigned_long_long_values, "split Unsigned Long Longs");
 }

 void join_test()
 {
 	using join_vector_func_t = std::string (*)(const std::vector<const char*>&, std::string_view);

 	const auto join_vector_func = static_cast<join_vector_func_t>(collin::strings::join);

 	const auto test_string_values = collin::test::make_test_data
 	(
 		join_vector_func,
 			{
 				std::make_pair(std::make_tuple(std::vector {"These", "are", "words"}, " "s), "These are words"s),
 				std::make_pair(std::make_tuple(std::vector {"These", "are", "words"}, "\n"s), "These\nare\nwords"s)
 			}
 	);

 	const auto words = std::vector {"Look", "at", "these", "strings"};

 	using join_it_func_t = std::string (*)(typename decltype(words)::const_iterator, typename decltype(words)::const_iterator, std::string_view);

 	const auto join_it_func = static_cast<join_it_func_t>(collin::strings::join);

 	const auto test_it_values = collin::test::make_test_data
 	(
 		join_it_func,
 			{
 				std::make_pair(std::make_tuple(std::begin(words), std::end(words), " "s), "Look at these strings"s),
 				std::make_pair(std::make_tuple(std::begin(words) + 1, std::end(words), "$$$$"s), "at$$$$these$$$$strings"s),
 			}
 	);


 	collin::test::assert_test_data(test_string_values, "join Strings");
 	collin::test::assert_test_data(test_it_values, "join Iterators");
 }

 template<class T>
 using from_string_func_t = T(*)(std::string_view, const std::locale&);

 void from_string_test()
 {
 	std::error_code ec;
 	const auto int_test_values = collin::test::make_test_data
 	(
 		static_cast<from_string_func_t<int>>(collin::strings::from_string),
 		 	{
 				std::make_pair(std::make_tuple("1", std::locale{}), 1),
 				std::make_pair(std::make_tuple("1223", std::locale{}), 1223),
 				std::make_pair(std::make_tuple("-2391", std::locale{}), -2391),
 				std::make_pair(std::make_tuple("37943", std::locale{}), 37943),
 				std::make_pair(std::make_tuple("0", std::locale{}), 0),
 			}
 	);

    const auto double_test_values = collin::test::make_test_data
    (
        static_cast<from_string_func_t<double>>(collin::strings::from_string),
        {
            std::make_pair(std::make_tuple("1.5", std::locale{}), 1.5),
            std::make_pair(std::make_tuple("-2.23478", std::locale{}), -2.23478),
            std::make_pair(std::make_tuple("0", std::locale{}), 0.0),
            std::make_pair(std::make_tuple("1", std::locale{}), 1.0),
        }
    );

 	collin::test::assert_test_data(int_test_values, "from string Integers");
    collin::test::assert_test_data(double_test_values, "from string Doubles");
 }

int main()
{
	is_vowel_test();
	is_consonant_test();
	contains_vowel_test();
	contains_consonant_test();
	remove_punctuation_test();
	remove_spaces_test();
	remove_alphanumerics_test();
	remove_blanks_test();
	remove_hexdigits_test();
	remove_upper_test();
	remove_lower_test();
	remove_character_test();
	uppercase_test();
	lowercase_test();
	split_test();
	join_test();
	from_string_test();
}