module;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module cmoon.windows.gui.icon_size;

namespace cmoon::windows
{
	export
	enum class icon_size : int
	{
		icon_big = ICON_BIG,
		icon_small = ICON_SMALL,
		icon_small2 = ICON_SMALL2
	};
}