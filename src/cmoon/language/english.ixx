export module cmoon.language.english;

import <iterator>;
import <string_view>;
import <string>;
import <ranges>;

import cmoon.language;

namespace cmoon::language
{
    export
	class english: public language {};

    export
	template<class CharT>
	bool is_vowel<english>(CharT c) noexcept
	{
		switch (c)
        {
            case static_cast<CharT>('a'):
            case static_cast<CharT>('e'):
            case static_cast<CharT>('i'):
            case static_cast<CharT>('o'):
            case static_cast<CharT>('u'):
            case static_cast<CharT>('A'):
            case static_cast<CharT>('E'):
            case static_cast<CharT>('I'):
            case static_cast<CharT>('O'):
            case static_cast<CharT>('U'): 
                return true;
            default:
                return false;
        }
	}

    export
    template<class CharT>
    bool is_consonant<english>(CharT c) noexcept
    {
        return !is_vowel(c);
    }
}