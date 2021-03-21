module;

#include <concepts>
#include <optional>

#include "cmoon/string_literals.hpp"

export module cmoon.format.format_specs;

namespace cmoon
{
	export
	enum class align_t
	{
		none,
		left,
		right,
		center,
		numeric,
	};

	export
	enum class sign_t
	{
		none,
		minus,
		plus,
		space,
	};

	export
	template<typename CharT>
		requires(std::same_as<CharT, char> || std::same_as<CharT, wchar_t>)
	struct basic_format_specs
	{
		constexpr basic_format_specs() noexcept = default;

		int width {0};
		int precision {-1};
		CharT type {0};
		align_t align {align_t::none};
		sign_t sign {sign_t::none};
		bool alt {false};
		CharT fill {cmoon::choose_char_literal<CharT>(CHAR_LITERALS(' '))};
	};

	export
	template<typename CharT>
	struct dynamic_format_specs
	{
		constexpr dynamic_format_specs() noexcept = default;

		std::optional<std::size_t> width {};
		std::optional<std::size_t> precision {};
	};
}