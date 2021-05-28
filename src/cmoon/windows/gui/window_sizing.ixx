module;

#include <Windows.h>

export module cmoon.windows.gui.window_sizing;

namespace cmoon::windows
{
	export
	enum class window_sizing
	{
		bottom = WMSZ_BOTTOM,
		bottom_left = WMSZ_BOTTOMLEFT,
		bottom_right = WMSZ_BOTTOMRIGHT,
		left = WMSZ_LEFT,
		right = WMSZ_RIGHT,
		top = WMSZ_TOP,
		top_left = WMSZ_TOPLEFT,
		top_right = WMSZ_TOPRIGHT
	};
}