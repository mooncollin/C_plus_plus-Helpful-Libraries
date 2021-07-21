module;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module cmoon.windows.gui.show_window;

namespace cmoon::windows
{
	export
	enum class show_window_command : int
	{
		hide = SW_HIDE,
		show_normal = SW_SHOWNORMAL,
		normal = SW_NORMAL,
		show_minimized = SW_SHOWMINIMIZED,
		show_maximized = SW_SHOWMAXIMIZED,
		maximize = SW_MAXIMIZE,
		show_no_activate = SW_SHOWNOACTIVATE,
		show = SW_SHOW,
		minimize = SW_MINIMIZE,
		show_min_no_active = SW_SHOWMINNOACTIVE,
		show_na = SW_SHOWNA,
		restore = SW_RESTORE,
		show_default = SW_SHOWDEFAULT,
		force_minimize = SW_FORCEMINIMIZE,
		max = SW_MAX,
		other_unzoom = SW_OTHERUNZOOM,
		other_zoom = SW_OTHERZOOM,
		parent_closing = SW_PARENTCLOSING,
		parent_opening = SW_PARENTOPENING
	};
}