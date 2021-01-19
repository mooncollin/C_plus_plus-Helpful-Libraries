#pragma once

#include <cstdint>
#include <string>
#include <charconv>
#include <iostream>
#include <iomanip>

#ifdef _WIN32
	#include <d2d1.h>
#endif

namespace cmoon
{
	namespace graphics
	{
		class color
		{
			public:
				using rgb_float_t = float;
				using rgb_int_t = std::int32_t;
				using alpha_t = float;

				constexpr color() = default;

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
				[[nodiscard]] constexpr operator ::D2D1_COLOR_F() const
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

		std::ostream& operator<<(std::ostream& os, const color& c)
		{
			const auto before_flags = os.flags();

			os << std::hex << c.to_int();

			os.flags(before_flags);
			return os;
		}

		namespace colors
		{
			static constexpr color alice_blue {0xF0F8FF};
			static constexpr color antique_white {0xFAEBD7};
			static constexpr color aqua {0x00FFFF};
			static constexpr color aquamarine {0x7FFFD4};
			static constexpr color azure {0xF0FFFF};
			static constexpr color beige {0xF5F5DC};
			static constexpr color bisque {0xFFE4C4};
			static constexpr color black {0x000000};
			static constexpr color blanched_almond {0xFFEBCD};
			static constexpr color blue {0x0000FF};
			static constexpr color blue_violet {0x8A2BE2};
			static constexpr color brown {0xA52A2A};
			static constexpr color burly_wood {0xDEB887};
			static constexpr color cadet_blue {0x5F9EA0};
			static constexpr color chartreuse {0x7FFF00};
			static constexpr color chocolate {0xD2691E};
			static constexpr color coral {0xFF7F50};
			static constexpr color cornflower_blue {0x6495ED};
			static constexpr color cornsilk {0xFFF8DC};
			static constexpr color crimson {0xDC143C};
			static constexpr color cyan {0x00FFFF};
			static constexpr color dark_blue {0x00008B};
			static constexpr color dark_cyan {0x008B8B};
			static constexpr color dark_goldenrod {0xB8860B};
			static constexpr color dark_gray {0xA9A9A9};
			static constexpr color dark_green {0x006400};
			static constexpr color dark_khaki {0xBDB76B};
			static constexpr color dark_magenta {0x8B008B};
			static constexpr color dark_olive_green {0x556B2F};
			static constexpr color dark_orange {0xFF8C00};
			static constexpr color dark_orchid {0x9932CC};
			static constexpr color dark_red {0x8B0000};
			static constexpr color dark_salmon {0xE9967A};
			static constexpr color dark_sea_green {0x8FBC8F};
			static constexpr color dark_slate_blue {0x483D8B};
			static constexpr color dark_slate_gray {0x2F4F4F};
			static constexpr color dark_turquoise {0x00CED1};
			static constexpr color dark_violet {0x9400D3};
			static constexpr color deep_pink {0xFF1493};
			static constexpr color deep_sky_blue {0x00BFFF};
			static constexpr color dim_gray {0x696969};
			static constexpr color dodger_blue {0x1E90FF};
			static constexpr color firebrick {0xB22222};
			static constexpr color floral_white {0xFFFAF0};
			static constexpr color forest_green {0x228B22};
			static constexpr color fuchsia {0xFF00FF};
			static constexpr color gainsboro {0xDCDCDC};
			static constexpr color ghost_white {0xF8F8FF};
			static constexpr color gold {0xFFD700};
			static constexpr color goldenrod {0xDAA520};
			static constexpr color gray {0x808080};
			static constexpr color green {0x008000};
			static constexpr color green_yellow {0xADFF2F};
			static constexpr color honeydew {0xF0FFF0};
			static constexpr color hot_pink {0xFF69B4};
			static constexpr color indian_red {0xCD5C5C};
			static constexpr color indigo {0x4B0082};
			static constexpr color ivory {0xFFFFF0};
			static constexpr color khaki {0xF0E68C};
			static constexpr color lavender {0xE6E6FA};
			static constexpr color lavender_blush {0xFFF0F5};
			static constexpr color lawn_green {0x7CFC00};
			static constexpr color lemon_chiffon {0xFFFACD};
			static constexpr color light_blue {0xADD8E6};
			static constexpr color light_coral {0xF08080};
			static constexpr color light_cyan {0xE0FFFF};
			static constexpr color light_goldenrod_yellow {0xFAFAD2};
			static constexpr color light_green {0x90EE90};
			static constexpr color light_gray {0xD3D3D3};
			static constexpr color light_pink {0xFFB6C1};
			static constexpr color light_salmon {0xFFA07A};
			static constexpr color light_sea_green {0x20B2AA};
			static constexpr color light_sky_blue {0x87CEFA};
			static constexpr color light_slate_gray {0x778899};
			static constexpr color light_steel_blue {0xB0C4DE};
			static constexpr color light_yellow {0xFFFFE0};
			static constexpr color lime {0x00FF00};
			static constexpr color lime_green {0x32CD32};
			static constexpr color linen {0xFAF0E6};
			static constexpr color magenta {0xFF00FF};
			static constexpr color maroon {0x800000};
			static constexpr color medium_aquamarine {0x66CDAA};
			static constexpr color medium_blue {0x0000CD};
			static constexpr color medium_orchid {0xBA55D3};
			static constexpr color medium_purple {0x9370DB};
			static constexpr color medium_sea_green {0x3CB371};
			static constexpr color medium_slate_blue {0x7B68EE};
			static constexpr color medium_spring_green {0x00FA9A};
			static constexpr color medium_turquoise {0x48D1CC};
			static constexpr color medium_violet_red {0xC71585};
			static constexpr color midnight_blue {0x191970};
			static constexpr color mint_cream {0xF5FFFA};
			static constexpr color misty_rose {0xFFE4E1};
			static constexpr color moccasin {0xFFE4B5};
			static constexpr color navajo_white {0xFFDEAD};
			static constexpr color navy {0x000080};
			static constexpr color old_lace {0xFDF5E6};
			static constexpr color olive {0x808000};
			static constexpr color olive_drab {0x6B8E23};
			static constexpr color orange {0xFFA500};
			static constexpr color orange_red {0xFF4500};
			static constexpr color orchid {0xDA70D6};
			static constexpr color pale_goldenrod {0xEEE8AA};
			static constexpr color pale_green {0x98FB98};
			static constexpr color pale_turquoise {0xAFEEEE};
			static constexpr color pale_violet_red {0xDB7093};
			static constexpr color papaya_whip {0xFFEFD5};
			static constexpr color peach_puff {0xFFDAB9};
			static constexpr color peru {0xCD853F};
			static constexpr color pink {0xFFC0CB};
			static constexpr color plum {0xDDA0DD};
			static constexpr color powder_blue {0xB0E0E6};
			static constexpr color purple {0x800080};
			static constexpr color red {0xFF0000};
			static constexpr color rosy_brown {0xBC8F8F};
			static constexpr color royal_blue {0x4169E1};
			static constexpr color saddle_brown {0x8B4513};
			static constexpr color salmon {0xFA8072};
			static constexpr color sandy_brown {0xF4A460};
			static constexpr color sea_green {0x2E8B57};
			static constexpr color sea_shell {0xFFF5EE};
			static constexpr color sienna {0xA0522D};
			static constexpr color silver {0xC0C0C0};
			static constexpr color sky_blue {0x87CEEB};
			static constexpr color slate_blue {0x6A5ACD};
			static constexpr color slate_gray {0x708090};
			static constexpr color snow {0xFFFAFA};
			static constexpr color spring_green {0x00FF7F};
			static constexpr color steel_blue {0x4682B4};
			static constexpr color tan {0xD2B48C};
			static constexpr color teal {0x008080};
			static constexpr color thistle {0xD8BFD8};
			static constexpr color tomato {0xFF6347};
			static constexpr color turquoise {0x40E0D0};
			static constexpr color violet {0xEE82EE};
			static constexpr color wheat {0xF5DEB3};
			static constexpr color white {0xFFFFFF};
			static constexpr color white_smoke {0xF5F5F5};
			static constexpr color yellow {0xFFFF00};
			static constexpr color yellow_green {0x9ACD32};
		}
	}
}