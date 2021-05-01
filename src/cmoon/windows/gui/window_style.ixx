module;

#include <Windows.h>
#include <WinUser.h>

export module cmoon.windows.gui.window_style;

namespace cmoon::windows
{
	export
	enum class window_class_style : ::UINT
	{
		byte_align_client = CS_BYTEALIGNCLIENT,
		byte_align_window = CS_BYTEALIGNWINDOW,
		class_device_context = CS_CLASSDC,
		double_clicks = CS_DBLCLKS,
		drop_shadow = CS_DROPSHADOW,
		global_class = CS_GLOBALCLASS,
		horizontal_redraw = CS_HREDRAW,
		no_close = CS_NOCLOSE,
		own_device_context = CS_OWNDC,
		parent_device_context = CS_PARENTDC,
		save_bits = CS_SAVEBITS,
		vertical_redraw = CS_VREDRAW
	};

	export
	enum class window_style : ::DWORD
	{
		border = WS_BORDER,
		caption = WS_CAPTION,
		child = WS_CHILD,
		child_window = WS_CHILDWINDOW,
		clip_children = WS_CLIPCHILDREN,
		clip_siblings = WS_CLIPSIBLINGS,
		disabled = WS_DISABLED,
		dlg_frame = WS_DLGFRAME,
		group = WS_GROUP,
		horizontal_scroll_bar = WS_HSCROLL,
		iconic = WS_ICONIC,
		maximize = WS_MAXIMIZE,
		maximize_box = WS_MAXIMIZEBOX,
		minimize = WS_MINIMIZE,
		minimize_box = WS_MINIMIZEBOX,
		overlapped = WS_OVERLAPPED,
		overlapped_window = WS_OVERLAPPEDWINDOW,
		popup = WS_POPUP,
		popup_window = WS_POPUPWINDOW,
		size_box = WS_SIZEBOX,
		system_menu = WS_SYSMENU,
		tab_stop = WS_TABSTOP,
		tiled = WS_TILED,
		tiled_window = WS_TILEDWINDOW,
		visible = WS_VISIBLE,
		vertical_scroll_bar = WS_VSCROLL
	};

	export
	enum class extended_window_style : ::DWORD
	{
		left_to_right_reading = WS_EX_LTRREADING,
		accept_files = WS_EX_ACCEPTFILES,
		app_window = WS_EX_APPWINDOW,
		client_edge = WS_EX_CLIENTEDGE,
		composited = WS_EX_COMPOSITED,
		context_help = WS_EX_CONTEXTHELP,
		control_parent = WS_EX_CONTROLPARENT,
		dlg_modal_frame = WS_EX_DLGMODALFRAME,
		layered = WS_EX_LAYERED,
		rtl_layout = WS_EX_LAYOUTRTL,
		left = WS_EX_LEFT,
		left_scrollbar = WS_EX_LEFTSCROLLBAR,
		mdi_child = WS_EX_MDICHILD,
		no_activate = WS_EX_NOACTIVATE,
		no_inherit_layout = WS_EX_NOINHERITLAYOUT,
		no_parent_notify = WS_EX_NOPARENTNOTIFY,
		no_redirection_bitmap = WS_EX_NOREDIRECTIONBITMAP,
		right = WS_EX_RIGHT,
		right_scrollbar = WS_EX_RIGHTSCROLLBAR,
		rtl_reading = WS_EX_RTLREADING,
		static_edge = WS_EX_STATICEDGE,
		tool_window = WS_EX_TOOLWINDOW,
		topmost = WS_EX_TOPMOST,
		transparent = WS_EX_TRANSPARENT,
		window_edge = WS_EX_WINDOWEDGE,
		overlapped_window_ex = WS_EX_OVERLAPPEDWINDOW,
		palette_window = WS_EX_PALETTEWINDOW
	};
}