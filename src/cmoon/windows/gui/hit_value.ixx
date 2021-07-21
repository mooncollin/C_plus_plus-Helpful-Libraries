module;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module cmoon.windows.gui.hit_value;

namespace cmoon::windows
{
	export
	enum class hit_value : int
	{
		border = HTBORDER,
		bottom = HTBOTTOM,
		bottom_left = HTBOTTOMLEFT,
		bottom_right = HTBOTTOMRIGHT,
		caption = HTCAPTION,
		client = HTCLIENT,
		close = HTCLOSE,
		error = HTERROR,
		grow_box = HTGROWBOX,
		help = HTHELP,
		horizontal_scroll = HTHSCROLL,
		left = HTLEFT,
		menu = HTMENU,
		maximize_button = HTMAXBUTTON,
		minimize_button = HTMINBUTTON,
		nowhere = HTNOWHERE,
		reduce = HTREDUCE,
		right = HTRIGHT,
		size = HTSIZE,
		sys_menu = HTSYSMENU,
		top = HTTOP,
		top_left = HTTOPLEFT,
		top_right = HTTOPRIGHT,
		transparent = HTTRANSPARENT,
		vertical_scroll = HTVSCROLL,
		zoom = HTZOOM
	};
}