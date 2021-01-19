#pragma once

#include <version>
#include <limits>
#include <climits>
#include <type_traits>
#include <cstdint>
#include <concepts>

namespace cmoon
{
	constexpr auto bits_in_byte = CHAR_BIT;

	template<typename T>
	constexpr auto bits_in_type = sizeof(T) * bits_in_byte;

	enum class compiler_e
	{
		clang,
		como,
		compaq,
		gcc,
		intel,
		msvc,
		mingw32,
		mingw64,
		not_available
	};

	constexpr compiler_e compiler =
	#if defined(__clang__)
	compiler_e::clang
	#elif defined(__como__)
	compiler_e::como
	#elif defined(__DECCXX__)
	compiler_e::compaq
	#elif defined(__GNUC__)
	compiler_e::gcc
	#elif defined(__INTEL_COMPILER__)
	compiler_e::intel
	#elif defined(_MSC_VER)
	compiler_e::msvc
	#elif defined(__MINGW64__)
	compiler_e::mingw64
	#elif defined(__WINGW32__)
	compiler_e::mingw32
	#else
	compiler_e::not_available
	#endif
	;

	enum class operating_system_e
	{
		aix,
		android,
		amigaos,
		beos,
		bluegene,
		freebsd,
		netbsd,
		openbsd,
		convexos,
		cygwin,
		dragonfly,
		gnu,
		linux,
		mac,
		minix,
		msdos,
		win16,
		win32,
		win64,
		wince,
		not_available
	};

	constexpr operating_system_e operating_system =
	#if defined(_AIX)
	operating_system_e::aix
	#elif defined(__ANDROID__)
	operating_system_e::android
	#elif defined(__amigaos__)
	operating_system_e::amigaos
	#elif defined(__BEOS__)
	operating_system_e::beos
	#elif defined(__bg__)
	operating_system_e::bluegene
	#elif defined(__FreeBSD__)
	operating_system_e::freebsd
	#elif defined(__NetBSD__)
	operating_system_e::netbsd
	#elif defined(__OpenBSD__)
	operating_system_e::openbsd
	#elif defined(__convex__)
	operating_system_e::convexos
	#elif defined(__CYGWIN__)
	operating_system_e::cygwin
	#elif defined(__DragonFly__)
	operating_system_e::dragonfly
	#elif defined(macintosh) || defined(__APPLE__)
	operating_system_e::mac
	#elif defined(__minix)
	operating_system_e::minix
	#elif defined(__gnu_linux__)
	operating_system_e::gnu
	#elif defined(__linux__)
	operating_system_e::linux
	#elif defined(__MSDOS__)
	operating_system_e::msdos
	#elif defined(_WIN32_WCE)
	operating_system_e::wince
	#elif defined(_WIN64)
	operating_system_e::win64
	#elif defined(_WIN32)
	operating_system_e::win32
	#elif defined(_WIN16)
	operating_system_e::win16
	#else
	operating_system_e::not_available
	#endif
	;

	constexpr bool win32_api = (operating_system == operating_system_e::win32) ||
								(operating_system == operating_system_e::win64);

	constexpr bool win_unicode = 
	#ifdef UNICODE
		true
	#else
		false
	#endif
	;

	#pragma warning(disable:6285)
	constexpr bool posix_api = (operating_system == operating_system_e::aix) ||
								(operating_system == operating_system_e::android) ||
								(operating_system == operating_system_e::beos) ||
								(operating_system == operating_system_e::freebsd) ||
								(operating_system == operating_system_e::netbsd) ||
								(operating_system == operating_system_e:: openbsd) ||
								(operating_system == operating_system_e::cygwin) ||
								(operating_system == operating_system_e::dragonfly) ||
								(operating_system == operating_system_e::mac) ||
								(operating_system == operating_system_e::minix) ||
								(operating_system == operating_system_e::gnu) ||
								(operating_system == operating_system_e::linux);
	#pragma warning(default:6285)


	enum class architecture_e
	{
		alpha,
		amd64,
		arm,
		arm_thumb,
		i386,
		mips,
		powerpc,
		not_available
	};

	constexpr architecture_e architecture = 
	#if defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA)
	architecture_e::alpha
	#elif defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_AMD64)
	architecture_e::amd64
	#elif defined(__arm__) || defined(_ARM) || defined(__arm) || defined(_M_ARM)
		#if defined(__thumb__) || defined(_M_ARMT)
			architecture_e::arm_thumb
		#else
			architecture_e::arm
		#endif
	#elif defined(__i386__) || defined(__i386) || defined(_M_I86)
	architecture_e::i386
	#elif defined(__mips__)
	architecture_e::mips
	#elif defined(__powerpc) || defined(__powerpc__) || defined(_M_PPC)
	architecture_e::powerpc
	#else
	architecture_e::not_available
	#endif
	;

	enum class cpp_version_e
	{
		cpp_1998,
		cpp_2011,
		cpp_2014,
		cpp_2017,
		cpp_2020,
		not_available
	};

	constexpr cpp_version_e cpp_version = 
	#if defined(_MSVC_LANG)
		#if _MSVC_LANG > 201703L
		cpp_version_e::cpp_2020
		#elif _MSVC_LANG > 201402L
		cpp_version_e::cpp_2017
		#elif _MSVC_LANG > 201103L
		cpp_version_e::cpp_2014
		#elif _MSVC_LANG > 199711L
		cpp_version_e::cpp_2011
		#else
		cpp_version_e::cpp_1998
		#endif
	#elif defined(__cplusplus)
		#if __cplusplus > 201703L
		cpp_version_e::cpp_2020
		#elif __cplusplus > 201402L
		cpp_version_e::cpp_2017
		#elif __cplusplus > 201103L
		cpp_version_e::cpp_2014
		#elif __cplusplus > 199711L
		cpp_version_e::cpp_2011
		#else
		cpp_version_e::cpp_1998
		#endif
	#else
	cpp_version_e::not_available
	#endif
	;

	template<std::size_t Bits>
	concept int_trivial_type = Bits > 0 && Bits <= sizeof(std::intmax_t) * bits_in_byte;

	template<std::size_t Bits>
	concept uint_trivial_type = Bits > 0 && Bits <= sizeof(std::uintmax_t) * bits_in_byte;

	template<std::size_t Bits>
		requires(int_trivial_type<Bits>)
	using least_bits_int = std::conditional_t<(Bits <= 8), 
								std::int_least8_t,
							std::conditional_t<(Bits <= 16),
								std::int_least16_t,
							std::conditional_t<(Bits <= 32),
								std::int_least32_t,
							std::conditional_t<(Bits <= 64),
								std::int_least64_t,
								std::intmax_t
							>>>>;

	template<std::size_t Bits>
		requires(uint_trivial_type<Bits>)
	using least_bits_unsigned_int = std::conditional_t<(Bits <= 8), 
										std::uint_least8_t,
									std::conditional_t<(Bits <= 16),
										std::uint_least16_t,
									std::conditional_t<(Bits <= 32),
										std::uint_least32_t,
									std::conditional_t<(Bits <= 64),
										std::uint_least64_t,
										std::uintmax_t
									>>>>;

	template<std::size_t Bits>
		requires(int_trivial_type<Bits>)
	using fast_bits_int = std::conditional_t<(Bits <= 8), 
								std::int_fast8_t,
							std::conditional_t<(Bits <= 16),
								std::int_fast16_t,
							std::conditional_t<(Bits <= 32),
								std::int_fast32_t,
							std::conditional_t<(Bits <= 64),
								std::int_fast64_t,
								std::intmax_t
							>>>>;

	template<std::size_t Bits>
		requires(uint_trivial_type<Bits>)
	using fast_bits_unsigned_int = std::conditional_t<(Bits <= 8), 
										std::uint_fast8_t,
									std::conditional_t<(Bits <= 16),
										std::uint_fast16_t,
									std::conditional_t<(Bits <= 32),
										std::uint_fast32_t,
									std::conditional_t<(Bits <= 64),
										std::uint_fast64_t,
										std::uintmax_t
									>>>>;

	template<std::intmax_t LeastValue>
	using least_value_int = std::conditional_t<(LeastValue >= std::numeric_limits<std::int_least8_t>::min() && LeastValue <= std::numeric_limits<std::int_least8_t>::max()), 
								std::int_least8_t,
							std::conditional_t<(LeastValue >= std::numeric_limits<std::int_least16_t>::min() && LeastValue <= std::numeric_limits<std::int_least16_t>::max()),
								std::int_least16_t,
							std::conditional_t<(LeastValue >= std::numeric_limits<std::int_least32_t>::min() && LeastValue <= std::numeric_limits<std::int_least32_t>::max()),
								std::int_least32_t,
							std::conditional_t<(LeastValue >= std::numeric_limits<std::int_least64_t>::min() && LeastValue <= std::numeric_limits<std::int_least64_t>::max()),
								std::int_least64_t,
								std::intmax_t
							>>>>;

	template<std::uintmax_t LeastValue>
	using least_value_unsigned_int = std::conditional_t<(LeastValue <= std::numeric_limits<std::uint_least8_t>::max()), 
										std::uint_least8_t,
										std::conditional_t<(LeastValue <= std::numeric_limits<std::uint_least16_t>::max()),
										std::uint_least16_t,
										std::conditional_t<(LeastValue <= std::numeric_limits<std::uint_least32_t>::max()),
										std::uint_least32_t,
										std::conditional_t<(LeastValue <= std::numeric_limits<std::uint_least64_t>::max()),
										std::uint_least64_t,
										std::uintmax_t
										>>>>;

	template<std::intmax_t LeastValue>
	using fast_least_value_int = std::conditional_t<(LeastValue >= std::numeric_limits<std::int_fast8_t>::min() && LeastValue <= std::numeric_limits<std::int_fast8_t>::max()), 
									std::int_fast8_t,
									std::conditional_t<(LeastValue >= std::numeric_limits<std::int_fast16_t>::min() && LeastValue <= std::numeric_limits<std::int_fast16_t>::max()),
									std::int_fast16_t,
									std::conditional_t<(LeastValue >= std::numeric_limits<std::int_fast32_t>::min() && LeastValue <= std::numeric_limits<std::int_fast32_t>::max()),
									std::int_fast32_t,
									std::conditional_t<(LeastValue >= std::numeric_limits<std::int_fast64_t>::min() && LeastValue <= std::numeric_limits<std::int_fast64_t>::max()),
									std::int_fast64_t,
									std::intmax_t
									>>>>;

	template<std::uintmax_t LeastValue>
	using fast_least_value_unsigned_int = std::conditional_t<(LeastValue <= std::numeric_limits<std::uint_fast8_t>::max()), 
											std::uint_fast8_t,
											std::conditional_t<(LeastValue <= std::numeric_limits<std::uint_fast16_t>::max()),
											std::uint_fast16_t,
											std::conditional_t<(LeastValue <= std::numeric_limits<std::uint_fast32_t>::max()),
											std::uint_fast32_t,
											std::conditional_t<(LeastValue <= std::numeric_limits<std::uint_fast64_t>::max()),
											std::uint_fast64_t,
											std::uintmax_t
											>>>>;

	template<std::size_t AmountDigits>
	concept float_trivial_type = AmountDigits <= std::numeric_limits<long double>::max_exponent10;

	template<std::size_t AmountDigits>
		requires(float_trivial_type<AmountDigits>)
	using least_digits_float = std::conditional_t<(AmountDigits <= std::numeric_limits<float>::max_exponent10),
									float,
								std::conditional_t<(AmountDigits <= std::numeric_limits<double>::max_exponent10),
									double,
									long double>>;

	template<std::floating_point F>
	using least_int_type = least_value_int<static_cast<std::intmax_t>(std::numeric_limits<F>::max())>;
}