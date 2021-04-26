export module cmoon.language;

import <concepts>;
import <string_view>;
import <concepts>;
import <iterator>;
import <algorithm>;
import <functional>;

namespace cmoon::language
{
	export
	struct language {};

	export
	template<class Language, class CharT>
		requires(std::derived_from<Language, language>)
	bool is_vowel(CharT c);

	export
	template<class Language, class CharT>
	bool is_consonant(CharT c);

	export
	template<class Language, std::input_iterator InputIterator>
		requires(std::derived_from<Language, language>)
	bool contains_vowel(InputIterator first, InputIterator last)
	{
		return std::any_of(first, last, [](const auto c) { return is_vowel<Language>(c); });
	}

	export
	template<class Language, std::ranges::input_range Range, class Proj = std::identity>
		requires(std::derived_from<Language, language>)
	bool contains_vowel(Range&& r, Proj proj = {})
	{
		return std::ranges::any_of(std::forward<Range>(r), [](const auto c) { return is_vowel<Language>(c); }, proj);
	}

	export
	template<class Language, std::input_iterator InputIterator>
		requires(std::derived_from<Language, language>)
	bool contains_consonant(InputIterator first, InputIterator last)
	{
		return std::any_of(first, last, [](const auto c) { return is_consonant<Language>(c); });
	}

	export
	template<class Language, std::ranges::input_range Range, class Proj = std::identity>
		requires(std::derived_from<Language, language>)
	bool contains_consonant(Range&& r, Proj proj = {})
	{
		return std::ranges::any_of(std::forward<Range>(r), [](const auto c) { return is_consonant<Language>(c); }, proj);
	}
}