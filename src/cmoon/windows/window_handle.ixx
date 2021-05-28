export module cmoon.windows.window_handle;

import <Windows.h>;

namespace cmoon::windows
{
	export
	using window_handle = ::HWND;
}