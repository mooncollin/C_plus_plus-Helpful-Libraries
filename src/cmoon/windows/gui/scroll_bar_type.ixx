module;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module cmoon.windows.gui.scroll_bar_type;

namespace cmoon::windows
{
	export
	enum class scroll_bar_type
	{
		horizontal = SB_HORZ,
		vertical = SB_VERT,
		control = SB_CTL,
		both = SB_BOTH
	};
}