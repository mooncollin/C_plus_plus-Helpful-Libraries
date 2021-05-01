module;

#include <WinUser.h>

export module cmoon.windows.gui.size_request;

namespace cmoon::windows
{
	export
	enum class size_request
	{
		maximize_hide = SIZE_MAXHIDE,
		maximized = SIZE_MAXIMIZED,
		maximize_show = SIZE_MAXSHOW,
		minimized = SIZE_MINIMIZED,
		restored = SIZE_RESTORED
	};
}