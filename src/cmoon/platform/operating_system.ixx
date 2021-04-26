export module cmoon.platform.operating_system;

namespace cmoon
{
	export
	enum class operating_system_type
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

	export
	inline constexpr operating_system_type operating_system =
	#if defined(_AIX)
	operating_system_type::aix
	#elif defined(__ANDROID__)
	operating_system_type::android
	#elif defined(__amigaos__)
	operating_system_type::amigaos
	#elif defined(__BEOS__)
	operating_system_type::beos
	#elif defined(__bg__)
	operating_system_type::bluegene
	#elif defined(__FreeBSD__)
	operating_system_type::freebsd
	#elif defined(__NetBSD__)
	operating_system_type::netbsd
	#elif defined(__OpenBSD__)
	operating_system_type::openbsd
	#elif defined(__convex__)
	operating_system_type::convexos
	#elif defined(__CYGWIN__)
	operating_system_type::cygwin
	#elif defined(__DragonFly__)
	operating_system_type::dragonfly
	#elif defined(macintosh) || defined(__APPLE__)
	operating_system_type::mac
	#elif defined(__minix)
	operating_system_type::minix
	#elif defined(__gnu_linux__)
	operating_system_type::gnu
	#elif defined(__linux__)
	operating_system_type::linux
	#elif defined(__MSDOS__)
	operating_system_type::msdos
	#elif defined(_WIN32_WCE)
	operating_system_type::wince
	#elif defined(_WIN64)
	operating_system_type::win64
	#elif defined(_WIN32)
	operating_system_type::win32
	#elif defined(_WIN16)
	operating_system_type::win16
	#else
	operating_system_type::not_available
	#endif
	;

	export
	inline constexpr bool win32_api = (operating_system == operating_system_type::win32) ||
									  (operating_system == operating_system_type::win64);

	#pragma warning(disable:6285)
	export
	inline constexpr bool posix_api = (operating_system == operating_system_type::aix) ||
									  (operating_system == operating_system_type::android) ||
									  (operating_system == operating_system_type::beos) ||
									  (operating_system == operating_system_type::freebsd) ||
									  (operating_system == operating_system_type::netbsd) ||
									  (operating_system == operating_system_type:: openbsd) ||
									  (operating_system == operating_system_type::cygwin) ||
									  (operating_system == operating_system_type::dragonfly) ||
									  (operating_system == operating_system_type::mac) ||
									  (operating_system == operating_system_type::minix) ||
									  (operating_system == operating_system_type::gnu) ||
									  (operating_system == operating_system_type::linux);
	#pragma warning(default:6285)
}