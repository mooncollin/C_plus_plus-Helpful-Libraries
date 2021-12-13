export module cmoon.string.literals;

import <type_traits>;
import <utility>;

import cmoon.concepts;
import cmoon.utility;

namespace cmoon
{
	export
    template<class T, cmoon::string_literal... Args>
    constexpr auto choose_str_literal(Args&&... args) noexcept
    {
        using proper_type = std::add_pointer_t<std::add_const_t<std::remove_cvref_t<std::remove_pointer_t<T>>>>;
        return choose_on_type<proper_type>(std::forward<Args>(args)...);
    }

    export
    template<class T, cmoon::character... Args>
    constexpr auto choose_char_literal(const Args... args) noexcept
    {
        return choose_on_type<T>(std::forward<Args>(args)...);
    }
}