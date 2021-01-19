#pragma once

#include <iterator>
#include <string_view>
#include <string>
#include <ranges>

#include "cmoon/language/language.hpp"

namespace cmoon
{
	namespace language
	{
		class english: public language {};

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

        template<class CharT>
        bool is_consonant<english>(CharT c) noexcept
        {
            return !is_vowel(c);
        }
	}
}