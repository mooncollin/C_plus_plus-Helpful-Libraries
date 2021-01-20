#pragma once

#include <concepts>
#include <string_view>
#include <concepts>
#include <iterator>
#include <algorithm>
#include <functional>

namespace cmoon
{
	namespace language
	{
		struct language {};

		template<class Language, class CharT>
			requires(std::derived_from<Language, language>)
		bool is_vowel(CharT c);

		template<class Language, class CharT>
		bool is_consonant(CharT c);

		template<class Language, std::input_iterator InputIterator>
			requires(std::derived_from<Language, language>)
		bool contains_vowel(InputIterator first, InputIterator last)
		{
			return std::any_of(first, last, [](const auto c) { return is_vowel<Language>(c); });
		}

		template<class Language, std::ranges::input_range Range, class Proj = std::identity>
			requires(std::derived_from<Language, language>)
		bool contains_vowel(Range&& r, Proj proj = {})
		{
			return std::ranges::any_of(std::forward<Range>(r), [](const auto c) { return is_vowel<Language>(c); }, proj);
		}

		template<class Language, std::input_iterator InputIterator>
			requires(std::derived_from<Language, language>)
		bool contains_consonant(InputIterator first, InputIterator last)
		{
			return std::any_of(first, last, [](const auto c) { return is_consonant<Language>(c); });
		}

		template<class Language, std::ranges::input_range Range, class Proj = std::identity>
			requires(std::derived_from<Language, language>)
		bool contains_consonant(Range&& r, Proj proj = {})
		{
			return std::ranges::any_of(std::forward<Range>(r), [](const auto c) { return is_consonant<Language>(c); }, proj);
		}
	}
}