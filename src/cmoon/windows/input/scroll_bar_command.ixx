module;

#include <Windows.h>

export module cmoon.windows.input.scroll_bar_command;

namespace cmoon::windows
{
	export
	enum class scroll_bar_command : std::uint32_t
	{
		lineup = SB_LINEUP,
		lineleft = SB_LINELEFT,
		linedown = SB_LINEDOWN,
		lineright = SB_LINERIGHT,
		pageup = SB_PAGEUP,
		pageleft = SB_PAGELEFT,
		pagedown = SB_PAGEDOWN,
		pageright = SB_PAGERIGHT,
		thumbposition = SB_THUMBPOSITION,
		thumbtrack = SB_THUMBTRACK,
		top = SB_TOP,
		left = SB_LEFT,
		bottom = SB_BOTTOM,
		right = SB_RIGHT,
		endscroll = SB_ENDSCROLL,
	};
}