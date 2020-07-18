#pragma once

#include <version>

namespace collin
{
	namespace platform
	{
		enum class e_compiler
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

		constexpr e_compiler compiler =
		#if defined(__clang__)
		e_compiler::clang
		#elif defined(__como__)
		e_compiler::como
		#elif defined(__DECCXX__)
		e_compiler::compaq
		#elif defined(__GNUC__)
		e_compiler::gcc
		#elif defined(__INTEL_COMPILER__)
		e_compiler::intel
		#elif defined(_MSC_VER)
		e_compiler::msvc
		#elif defined(__MINGW64__)
		e_compiler::mingw64
		#elif defined(__WINGW32__)
		e_compiler::mingw32
		#else
		e_compiler::not_available
		#endif
		;

		enum class e_operating_system
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

		constexpr e_operating_system operating_system =
		#if defined(_AIX)
		e_operating_system::aix
		#elif defined(__ANDROID__)
		e_operating_system::android
		#elif defined(__amigaos__)
		e_operating_system::amigaos
		#elif defined(__BEOS__)
		e_operating_system::beos
		#elif defined(__bg__)
		e_operating_system::bluegene
		#elif defined(__FreeBSD__)
		e_operating_system::freebsd
		#elif defined(__NetBSD__)
		e_operating_system::netbsd
		#elif defined(__OpenBSD__)
		e_operating_system::openbsd
		#elif defined(__convex__)
		e_operating_system::convexos
		#elif defined(__CYGWIN__)
		e_operating_system::cygwin
		#elif defined(__DragonFly__)
		e_operating_system::dragonfly
		#elif defined(macintosh) || defined(__APPLE__)
		e_operating_system::mac
		#elif defined(__minix)
		e_operating_system::minix
		#elif defined(__gnu_linux__)
		e_operating_system::gnu
		#elif defined(__linux__)
		e_operating_system::linux
		#elif defined(__MSDOS__)
		e_operating_system::msdos
		#elif defined(_WIN32_WCE)
		e_operating_system::wince
		#elif defined(_WIN64)
		e_operating_system::win64
		#elif defined(_WIN32)
		e_operating_system::win32
		#elif defined(_WIN16)
		e_operating_system::win16
		#else
		e_operating_system::not_available
		#endif
		;

		constexpr bool win32_api = operating_system == e_operating_system::win32 ||
								   operating_system == e_operating_system::win64;

		constexpr bool posix_api = operating_system == e_operating_system::aix ||
								   operating_system == e_operating_system::android ||
								   operating_system == e_operating_system::beos ||
								   operating_system == e_operating_system::freebsd ||
								   operating_system == e_operating_system::netbsd ||
								   operating_system == e_operating_system:: openbsd ||
								   operating_system == e_operating_system::cygwin ||
								   operating_system == e_operating_system::dragonfly ||
								   operating_system == e_operating_system::mac ||
								   operating_system == e_operating_system::minix ||
								   operating_system == e_operating_system::gnu ||
								   operating_system == e_operating_system::linux;

		enum class e_architecture
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

		constexpr e_architecture architecture = 
		#if defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA)
		e_architecture::alpha
		#elif defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_AMD64)
		e_architecture::amd64
		#elif defined(__arm__) || defined(_ARM) || defined(__arm) || defined(_M_ARM)
			#if defined(__thumb__) || defined(_M_ARMT)
				e_architecture::arm_thumb
			#else
				e_architecture::arm
			#endif
		#elif defined(__i386__) || defined(__i386) || defined(_M_I86)
		e_architecture::i386
		#elif defined(__mips__)
		e_architecture::mips
		#elif defined(__powerpc) || defined(__powerpc__) || defined(_M_PPC)
		e_architecture::powerpc
		#else
		e_architecture::not_available
		#endif
		;

		enum class e_cpp_version
		{
			cpp_1998,
			cpp_2011,
			cpp_2014,
			cpp_2017,
			cpp_2020,
			not_available
		};

		constexpr e_cpp_version cpp_version = 
		#if defined(_MSVC_LANG)
			#if _MSVC_LANG > 201703L
			e_cpp_version::cpp_2020
			#elif _MSVC_LANG > 201402L
			e_cpp_version::cpp_2017
			#elif _MSVC_LANG > 201103L
			e_cpp_version::cpp_2014
			#elif _MSVC_LANG > 199711L
			e_cpp_version::cpp_2011
			#else
			e_cpp_version::cpp_1998
			#endif
		#elif defined(__cplusplus)
			#if __cplusplus > 201703L
			e_cpp_version::cpp_2020
			#elif __cplusplus > 201402L
			e_cpp_version::cpp_2017
			#elif __cplusplus > 201103L
			e_cpp_version::cpp_2014
			#elif __cplusplus > 199711L
			e_cpp_version::cpp_2011
			#else
			e_cpp_version::cpp_1998
			#endif
		#else
		e_cpp_version::not_available
		#endif
		;
	}
}