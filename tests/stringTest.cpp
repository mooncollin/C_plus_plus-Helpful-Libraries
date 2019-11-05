#include <utility>
#include <tuple>
#include <string>
#include <vector>
#include "collin_string.hpp"
#include "collin_test.hpp"

using namespace std::literals;

void is_vowel_test()
{
	const auto test_values = collin::make_test_data
	(
		collin::is_vowel,
			{
				std::make_pair(std::make_tuple('a'), true),
				std::make_pair(std::make_tuple('b'), false),
				std::make_pair(std::make_tuple('c'), false),
				std::make_pair(std::make_tuple('d'), false),
				std::make_pair(std::make_tuple('e'), true),
				std::make_pair(std::make_tuple('f'), false),
				std::make_pair(std::make_tuple('g'), false),
				std::make_pair(std::make_tuple('h'), false),
				std::make_pair(std::make_tuple('i'), true),
				std::make_pair(std::make_tuple('j'), false),
				std::make_pair(std::make_tuple('k'), false),
				std::make_pair(std::make_tuple('l'), false),
				std::make_pair(std::make_tuple('m'), false),
				std::make_pair(std::make_tuple('n'), false),
				std::make_pair(std::make_tuple('o'), true),
				std::make_pair(std::make_tuple('p'), false),
				std::make_pair(std::make_tuple('q'), false),
				std::make_pair(std::make_tuple('r'), false),
				std::make_pair(std::make_tuple('s'), false),
				std::make_pair(std::make_tuple('t'), false),
				std::make_pair(std::make_tuple('u'), true),
				std::make_pair(std::make_tuple('v'), false),
				std::make_pair(std::make_tuple('w'), false),
				std::make_pair(std::make_tuple('x'), false),
				std::make_pair(std::make_tuple('y'), false),
				std::make_pair(std::make_tuple('z'), false),
			}
	);

	collin::assert_test_data(test_values, "is_vowel");
}

void is_consonant_test()
{
	const auto test_values = collin::make_test_data
	(
		collin::is_consonant,
			{
				std::make_pair(std::make_tuple('a'), false),
				std::make_pair(std::make_tuple('b'), true),
				std::make_pair(std::make_tuple('c'), true),
				std::make_pair(std::make_tuple('d'), true),
				std::make_pair(std::make_tuple('e'), false),
				std::make_pair(std::make_tuple('f'), true),
				std::make_pair(std::make_tuple('g'), true),
				std::make_pair(std::make_tuple('h'), true),
				std::make_pair(std::make_tuple('i'), false),
				std::make_pair(std::make_tuple('j'), true),
				std::make_pair(std::make_tuple('k'), true),
				std::make_pair(std::make_tuple('l'), true),
				std::make_pair(std::make_tuple('m'), true),
				std::make_pair(std::make_tuple('n'), true),
				std::make_pair(std::make_tuple('o'), false),
				std::make_pair(std::make_tuple('p'), true),
				std::make_pair(std::make_tuple('q'), true),
				std::make_pair(std::make_tuple('r'), true),
				std::make_pair(std::make_tuple('s'), true),
				std::make_pair(std::make_tuple('t'), true),
				std::make_pair(std::make_tuple('u'), false),
				std::make_pair(std::make_tuple('v'), true),
				std::make_pair(std::make_tuple('w'), true),
				std::make_pair(std::make_tuple('x'), true),
				std::make_pair(std::make_tuple('y'), true),
				std::make_pair(std::make_tuple('z'), true),
			}
	);

	collin::assert_test_data(test_values, "is_consonant");
}

void contains_vowel_test()
{
	const auto test_values = collin::make_test_data
	(
		collin::contains_vowel<std::string>,
			{
				std::make_pair(std::make_tuple("Hello"), true),
				std::make_pair(std::make_tuple("My"), false),
				std::make_pair(std::make_tuple("cool"), true),
				std::make_pair(std::make_tuple("vowels"), true),
				std::make_pair(std::make_tuple("is"), true),
				std::make_pair(std::make_tuple("ghjbnj"), false),
			}
	);

	collin::assert_test_data(test_values, "contains_vowel");
}

void contains_consonant_test()
{
	const auto test_values = collin::make_test_data
	(
		collin::contains_consonant<std::string>,
			{
				std::make_pair(std::make_tuple("Hello"), true),
				std::make_pair(std::make_tuple("My"), true),
				std::make_pair(std::make_tuple("cool"), true),
				std::make_pair(std::make_tuple("vowels"), true),
				std::make_pair(std::make_tuple("is"), true),
				std::make_pair(std::make_tuple("ghjbnj"), true),
				std::make_pair(std::make_tuple("iiii"), false),
			}
	);

	collin::assert_test_data(test_values, "contains_consonant");
}

void remove_punctuation_test()
{
	const auto transform_f = [] (std::string s) {
		collin::remove_punctuation(s);
		return s;
	};

	const auto test_values = collin::make_test_data
	(
		transform_f,
			{
				std::make_pair(std::make_tuple("!!"), ""s),
				std::make_pair(std::make_tuple("!!fef"), "fef"s),
				std::make_pair(std::make_tuple("Hello!"), "Hello"s),
				std::make_pair(std::make_tuple("I/MfffS.OC00l!"), "IMfffSOC00l"s)
			}
	);

	collin::assert_test_data(test_values, "remove_punctuation");
}

void remove_spaces_test()
{
	const auto transform_f = [] (std::string s) {
		collin::remove_spaces(s);
		return s;
	};

	const auto test_values = collin::make_test_data
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

	collin::assert_test_data(test_values, "remove_spaces");
}

void remove_alphanumerics_test()
{
	const auto transform_f = [] (std::string s) {
		collin::remove_alphanumerics(s);
		return s;
	};

	const auto test_values = collin::make_test_data
	(
		transform_f,
			{
				std::make_pair(std::make_tuple("How Ar3 Y0u??"), "  ??"s),
				std::make_pair(std::make_tuple("T$$ To^"), "$$ ^"s),
			}
	);

	collin::assert_test_data(test_values, "remove_alphanumerics");
}

void remove_blanks_test()
{
	const auto transform_f = [] (std::string s) {
		collin::remove_blanks(s);
		return s;
	};

	const auto test_values = collin::make_test_data
	(
		transform_f,
			{
				std::make_pair(std::make_tuple("These are words"), "Thesearewords"s),
				std::make_pair(std::make_tuple("Hello\t!!"), "Hello!!"s),
				std::make_pair(std::make_tuple("What"), "What"s),
				std::make_pair(std::make_tuple("Teeee  \tee"), "Teeeeee"s),
				std::make_pair(std::make_tuple("fff   t\t\n"), "ffft\n"s)
			}
	);

	collin::assert_test_data(test_values, "remove_blanks");
}

void remove_hexdigits_test()
{
	const auto transform_f = [] (std::string s) {
		collin::remove_hexdigits(s);
		return s;
	};

	const auto test_values = collin::make_test_data
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

	collin::assert_test_data(test_values, "remove_hexdigits");
}

void remove_upper_test()
{
	const auto transform_f = [] (std::string s) {
		collin::remove_upper(s);
		return s;
	};

	const auto test_values = collin::make_test_data
	(
		transform_f,
			{
				std::make_pair(std::make_tuple("SUP"), ""s),
				std::make_pair(std::make_tuple("Sup"), "up"s),
				std::make_pair(std::make_tuple("SUP !!"), " !!"s),
			}
	);

	collin::assert_test_data(test_values, "remove_upper");
}

void remove_lower_test()
{
	const auto transform_f = [] (std::string s) {
		collin::remove_lower(s);
		return s;
	};

	const auto test_values = collin::make_test_data
	(
		transform_f,
			{
				std::make_pair(std::make_tuple("SUP"), "SUP"s),
				std::make_pair(std::make_tuple("Sup"), "S"s),
				std::make_pair(std::make_tuple("SUP f!!"), "SUP !!"s),
			}
	);

	collin::assert_test_data(test_values, "remove_lower");
}

void remove_character_test()
{
	const auto transform_f = [] (std::string s, char c) {
		collin::remove_character(s, c);
		return s;
	};

	const auto test_values = collin::make_test_data
	(
		transform_f,
			{
				std::make_pair(std::make_tuple("cccc", 'c'), ""s),
				std::make_pair(std::make_tuple("ccccFFF", 'c'), "FFF"s),
				std::make_pair(std::make_tuple("Hello!", 'l'), "Heo!"s),
				std::make_pair(std::make_tuple("Tee Tee", 'T'), "ee ee"s),
			}
	);

	collin::assert_test_data(test_values, "remove_character");
}

void uppercase_test()
{
	const auto transform_f = [] (std::string s) {
		collin::uppercase(s);
		return s;
	};
	
	const auto test_values = collin::make_test_data
	(
		transform_f,
			{
				std::make_pair(std::make_tuple("hello!"), "HELLO!"s),
				std::make_pair(std::make_tuple("h ello!"), "H ELLO!"s),
				std::make_pair(std::make_tuple("helLO!"), "HELLO!"s),
				std::make_pair(std::make_tuple("HEHOdd"), "HEHODD"s),
			}
	);

	collin::assert_test_data(test_values, "uppercase");
}

void lowercase_test()
{
	const auto transform_f = [] (std::string s) {
		collin::lowercase(s);
		return s;
	};
	
	const auto test_values = collin::make_test_data
	(
		transform_f,
			{
				std::make_pair(std::make_tuple("hello!"), "hello!"s),
				std::make_pair(std::make_tuple("h ello!"), "h ello!"s),
				std::make_pair(std::make_tuple("helLO!"), "hello!"s),
				std::make_pair(std::make_tuple("HEHOdd"), "hehodd"s),
			}
	);

	collin::assert_test_data(test_values, "lowercase");
}

template<typename T>
auto get_split_numeric_values()
{
	return collin::make_test_data
	(
		collin::split<T, std::string>,
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
	const auto test_string_values = collin::make_test_data
	(
		collin::split<std::string, std::string>,
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

	collin::assert_test_data(test_string_values, "split Strings");
	collin::assert_test_data(test_int_values, "split Ints");
	collin::assert_test_data(test_long_values, "split Longs");
	collin::assert_test_data(test_unsigned_long_values, "split Unsigned Longs");
	collin::assert_test_data(test_long_int_values, "split Long Ints");
	collin::assert_test_data(test_float_values, "split Floats");
	collin::assert_test_data(test_double_values, "split Doubles");
	collin::assert_test_data(test_long_double_values, "split Long Doubles");
	collin::assert_test_data(test_long_long_values, "split Long Longs");
	collin::assert_test_data(test_unsigned_long_long_values, "split Unsigned Long Longs");
}

void join_test()
{
	using join_vector_func_t = std::string (*)(const std::vector<const char*>&, std::string_view);

	const auto join_vector_func = static_cast<join_vector_func_t>(collin::join);

	const auto test_string_values = collin::make_test_data
	(
		join_vector_func,
			{
				std::make_pair(std::make_tuple(std::vector {"These", "are", "words"}, " "s), "These are words"s),
				std::make_pair(std::make_tuple(std::vector {"These", "are", "words"}, "\n"s), "These\nare\nwords"s)
			}
	);

	const auto words = std::vector {"Look", "at", "these", "strings"};

	using join_it_func_t = std::string (*)(typename decltype(words)::const_iterator, typename decltype(words)::const_iterator, std::string_view);

	const auto join_it_func = static_cast<join_it_func_t>(collin::join);

	const auto test_it_values = collin::make_test_data
	(
		join_it_func,
			{
				std::make_pair(std::make_tuple(std::begin(words), std::end(words), " "s), "Look at these strings"s),
				std::make_pair(std::make_tuple(std::begin(words) + 1, std::end(words), "$$$$"s), "at$$$$these$$$$strings"s),
			}
	);


	collin::assert_test_data(test_string_values, "join Strings");
	collin::assert_test_data(test_it_values, "join Iterators");
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
}