module;

#ifdef _WIN32
	#pragma comment(lib, "d2d1")
	#include <d2d1.h>
#endif

#include <cstdint>
#include <string>
#include <charconv>
#include <iostream>
#include <iomanip>

export module cmoon.graphics.color;

namespace cmoon::graphics
{
	export
	class color
	{
		public:
			using rgb_float_t = float;
			using rgb_int_t = std::int32_t;
			using alpha_t = float;

			constexpr color() noexcept = default;

			constexpr color(const color&) noexcept = default;
			constexpr color(color&&) noexcept = default;

			constexpr color& operator=(const color&) noexcept = default;
			constexpr color& operator=(color&&) noexcept = default;

			constexpr color(rgb_int_t rgb, alpha_t a = 1.0) noexcept
				: red_{static_cast<rgb_float_t>((rgb & red_mask) >> red_shift) / range},
					green_{static_cast<rgb_float_t>((rgb & green_mask) >> green_shift) / range},
					blue_{static_cast<rgb_float_t>((rgb & blue_mask) >> blue_shift) / range},
					alpha_{a} {}

			constexpr color(rgb_float_t red, rgb_float_t green, rgb_float_t blue, alpha_t a = 1.0) noexcept
				: red_{red}, green_{green}, blue_{blue}, alpha_{a} {}

			#ifdef _WIN32
			constexpr color(const ::D2D1_COLOR_F& c) noexcept
				: red_{c.r}, green_{c.g}, blue_{c.b}, alpha_{c.a} {}

			constexpr color& operator=(const ::D2D1_COLOR_F& c) noexcept
			{
				red_ = c.r;
				green_ = c.g;
				blue_ = c.b;
				alpha_ = c.a;

				return *this;
			}
			#endif

			[[nodiscard]] constexpr rgb_float_t red() const noexcept
			{
				return red_;
			}

			[[nodiscard]] constexpr rgb_float_t green() const noexcept
			{
				return green_;
			}

			[[nodiscard]] constexpr rgb_float_t blue() const noexcept
			{
				return blue_;
			}

			[[nodiscard]] constexpr alpha_t alpha() const noexcept
			{
				return alpha_;
			}

			constexpr void red(rgb_float_t r) noexcept
			{
				red_ = r;
			}

			constexpr void green(rgb_float_t g) noexcept
			{
				green_ = g;
			}

			constexpr void blue(rgb_float_t b) noexcept
			{
				blue_ = b;
			}

			constexpr void alpha(alpha_t a) noexcept
			{
				alpha_ = a;
			}

			constexpr void change_color(rgb_int_t rgb) noexcept
			{
				red_ = static_cast<rgb_float_t>((rgb & red_mask) >> red_shift) / range;
				green_ = static_cast<rgb_float_t>((rgb & green_mask) >> green_shift) / range;
				blue_ = static_cast<rgb_float_t>((rgb & blue_mask) >> blue_shift) / range;
			}

			[[nodiscard]] constexpr rgb_int_t to_int() const noexcept
			{
				return (static_cast<rgb_int_t>(red_ * range) << red_shift) +
						(static_cast<rgb_int_t>(green_ * range) << green_shift) +
						(static_cast<rgb_int_t>(blue_ * range) << blue_shift);
			}

			[[nodiscard]] std::string to_hex_string() const
			{
				std::string result;
				result.resize(6);

				std::to_chars(result.data(), result.data() + 2, static_cast<rgb_int_t>(red_ * range), 16);
				std::to_chars(result.data() + 2, result.data() + 4, static_cast<rgb_int_t>(green_ * range), 16);
				std::to_chars(result.data() + 4, result.data() + 6, static_cast<rgb_int_t>(blue_ * range), 16);

				return result;
			}

			#ifdef _WIN32
			[[nodiscard]] constexpr operator ::D2D1_COLOR_F() const noexcept
			{
				return {.r = red_, .g = green_, .b = blue_, .a = alpha_};
			}
			#endif
		private:
			rgb_float_t red_{};
			rgb_float_t green_{};
			rgb_float_t blue_{};
			alpha_t alpha_{1.0};

			static constexpr rgb_float_t range {255};

			static constexpr rgb_int_t red_shift {16};
			static constexpr rgb_int_t green_shift {8};
			static constexpr rgb_int_t blue_shift {0};

			static constexpr rgb_int_t red_mask {0xff << red_shift};
			static constexpr rgb_int_t green_mask {0xff << green_shift};
			static constexpr rgb_int_t blue_mask {0xff << blue_shift};
	};

	export
	template<class CharT, class Traits>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const color& c)
	{
		const auto before_flags {os.flags()};

		os << std::hex << c.to_int();

		os.flags(before_flags);
		return os;
	}
}