#pragma once

#include <array>
#include <cstdint>

namespace cmoon
{
	namespace emulate
	{
		struct memory_6502 : std::array<std::uint8_t, 1024 * 64>
		{
			private:
				using base = std::array<std::uint8_t, 1024 * 64>;
			public:
				constexpr memory_6502() noexcept
					: base{} {}

				constexpr [[nodiscard]] const std::uint8_t& operator[](const std::uint16_t address) const noexcept
				{
					return static_cast<const base&>(*this)[static_cast<std::size_t>(address)];
				}

				constexpr std::uint8_t& operator[](const std::uint16_t address) noexcept
				{
					return static_cast<base&>(*this)[static_cast<std::size_t>(address)];
				}
		};
	}
}