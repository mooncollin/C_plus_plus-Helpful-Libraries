module;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module cmoon.windows.window_handle;

namespace cmoon::windows
{
	export
	using window_handle = ::HWND;
}