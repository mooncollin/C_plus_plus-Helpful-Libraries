export module cmoon.string.character;

import <cctype>;
import <concepts>;
import <array>;

namespace cmoon
{
	export
    int hex_to_base10(int ch) noexcept
    {
        const auto uppered = std::toupper(ch);
        return uppered >= 'A' ? uppered - 'A' + 10 : uppered - '0';
    }

    export
    template<std::integral CharT>
    [[nodiscard]] constexpr int code_point_length(const CharT p) noexcept
    {
        if constexpr (sizeof(CharT) != 1)
        {
            return 1;
        }
        else
        {
            constexpr std::array lengths = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                            0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 4, 0};

            const auto len = lengths[static_cast<unsigned char>(p) >> 3];
            return len + !len;
        }
    }
}