module;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module cmoon.windows.system_metrics;

import cmoon.graphics;

namespace cmoon::windows
{
	export
	enum system_metric_index : int
	{
		arrange = SM_ARRANGE,
		clean_boot = SM_CLEANBOOT,
		num_monitors = SM_CMONITORS,
		num_mouse_buttons = SM_CMOUSEBUTTONS,
		convertible_slate_mode = SM_CONVERTIBLESLATEMODE,
		border_width = SM_CXBORDER,
		border_height = SM_CYBORDER,
		cursor_width = SM_CXCURSOR,
		cursor_height = SM_CYCURSOR,
		double_click_rect_width = SM_CXDOUBLECLK,
		double_click_rect_height = SM_CYDOUBLECLK,
		vertical_drag_pixels = SM_CXDRAG,
		horizontal_drag_pixels = SM_CYDRAG,
		fixed_frame_thickness = SM_CXFIXEDFRAME,
		border_3D_width = SM_CXEDGE,
		border_3D_height = SM_CYEDGE,
		focus_rect_width = SM_CXFOCUSBORDER,
		focus_rect_height = SM_CYFOCUSBORDER,
		fullscreen_width = SM_CXFULLSCREEN,
		fullscreen_height = SM_CYFULLSCREEN,
		horizontal_scrollbar_arrow_width = SM_CXHSCROLL,
		horizontal_scrollbar_arrow_height = SM_CYHSCROLL,
		horizontal_scrollbar_thumbbox_width = SM_CXHTHUMB,
		vertical_scrollbar_thumbbox_height = SM_CYVTHUMB,
		icon_width = SM_CXICON,
		icon_height = SM_CYICON,
		icon_spacing_width = SM_CXICONSPACING,
		icon_spacing_height = SM_CYICONSPACING,
		maximized_width = SM_CXMAXIMIZED,
		maximized_height = SM_CYMAXIMIZED,
		max_window_width = SM_CXMAXTRACK,
		max_window_height = SM_CYMAXTRACK,
		menu_checkmark_width = SM_CXMENUCHECK,
		menu_checkmark_height = SM_CYMENUCHECK,
		menu_buttons_width = SM_CXMENUSIZE,
		menu_buttons_height = SM_CYMENUSIZE,
		min_window_width = SM_CXMINTRACK,
		min_window_height = SM_CYMINTRACK,
		border_padding = SM_CXPADDEDBORDER,
		display_width = SM_CXSCREEN,
		display_height = SM_CYSCREEN,
		button_width = SM_CXSIZE,
		button_height = SM_CYSIZE,
		sizing_frame_thickness = SM_CXSIZEFRAME,
		recommended_icon_width = SM_CXSMICON,
		recommended_icon_height = SM_CYSMICON,
		caption_button_width = SM_CXSMSIZE,
		caption_button_height = SM_CYSMSIZE,
		virtual_screen_width = SM_CXVIRTUALSCREEN,
		virtual_screen_height = SM_CYVIRTUALSCREEN,
		vertical_scrollbar_width = SM_CXVSCROLL,
		vertical_scrollbar_height = SM_CYVSCROLL,
		dbcs_enabled = SM_DBCSENABLED,
		debug = SM_DEBUG,
		digitizer = SM_DIGITIZER,
		input_method_manager_enabled = SM_IMMENABLED,
		maximum_touches = SM_MAXIMUMTOUCHES,
		media_center = SM_MEDIACENTER,
		menu_drop_alignment = SM_MENUDROPALIGNMENT,
		middle_east_enabled = SM_MIDEASTENABLED,
		mouse_present = SM_MOUSEPRESENT,
		mouse_horizontal_wheel_present = SM_MOUSEHORIZONTALWHEELPRESENT,
		mouse_vertical_wheel_present = SM_MOUSEWHEELPRESENT,
		network = SM_NETWORK,
		microsoft_pen_installed = SM_PENWINDOWS,
		remote_control = SM_REMOTECONTROL,
		remote_session = SM_REMOTESESSION,
		same_display_format = SM_SAMEDISPLAYFORMAT,
		secure = SM_SECURE,
		windows_server_build_number = SM_SERVERR2,
		show_sounds = SM_SHOWSOUNDS,
		shutting_down = SM_SHUTTINGDOWN,
		slow_machine = SM_SLOWMACHINE,
		starter_edition = SM_STARTER,
		mouse_buttons_swapped = SM_SWAPBUTTON,
		system_docked = SM_SYSTEMDOCKED,
		tablet_pc = SM_TABLETPC,
		left_virtual_screen = SM_XVIRTUALSCREEN,
		top_virtual_screen = SM_YVIRTUALSCREEN
	};

	export
	enum class arrange_value : int
	{
		bottom_left = ARW_BOTTOMLEFT,
		bottom_right = ARW_BOTTOMRIGHT,
		top_left = ARW_TOPLEFT,
		top_right = ARW_TOPRIGHT,
		down = ARW_DOWN,
		hide = ARW_HIDE,
		left = ARW_LEFT,
		right = ARW_RIGHT,
		up = ARW_UP
	};

	export
	enum class clean_boot_value : int
	{
		normal = 0,
		fail_safe = 1,
		fail_safe_networking = 2
	};

	export
	enum class digitizer_value : int
	{
		none = 0,
		integrated_touch = NID_INTEGRATED_TOUCH,
		external_touch = NID_EXTERNAL_TOUCH,
		integrated_pen = NID_INTEGRATED_PEN,
		external_pen = NID_EXTERNAL_PEN,
		multi_input = NID_MULTI_INPUT,
		ready = NID_READY
	};

	export
	[[nodiscard]] arrange_value system_arranged_minimized_windows() noexcept
	{
		return arrange_value{::GetSystemMetrics(system_metric_index::arrange)};
	}

	export
	[[nodiscard]] clean_boot_value clean_boot_state() noexcept
	{
		return clean_boot_value{::GetSystemMetrics(system_metric_index::clean_boot)};
	}

	export
	[[nodiscard]] int number_display_monitors() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::num_monitors);
	}

	export
	[[nodiscard]] int number_mouse_buttons() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::num_mouse_buttons);
	}

	export
	[[nodiscard]] int slate_mode() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::convertible_slate_mode);
	}

	export
	[[nodiscard]] int window_border_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::border_width);
	}

	export
	[[nodiscard]] int window_border_height() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::border_height);
	}

	export
	[[nodiscard]] int get_cursor_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::cursor_width);
	}

	export
	[[nodiscard]] int get_cursor_height() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::cursor_height);
	}

	export
	[[nodiscard]] int double_click_rectangle_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::double_click_rect_width);
	}

	export
	[[nodiscard]] int double_click_rectangle_height() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::double_click_rect_height);
	}

	export
	[[nodiscard]] int get_vertical_drag_pixels() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::vertical_drag_pixels);
	}

	export
	[[nodiscard]] int get_horizontal_drag_pixels() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::horizontal_drag_pixels);
	}

	export
	[[nodiscard]] int get_border_3D_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::border_3D_width);
	}

	export
	[[nodiscard]] int get_border_3D_height() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::border_3D_height);
	}

	export
	[[nodiscard]] int window_fixed_frame_thickness() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::fixed_frame_thickness);
	}

	export
	[[nodiscard]] int focus_rectangle_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::focus_rect_width);
	}

	export
	[[nodiscard]] int focus_rectangle_height() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::focus_rect_height);
	}

	export
	[[nodiscard]] int fullscreen_window_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::fullscreen_width);
	}

	export
	[[nodiscard]] int fullscreen_window_height() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::fullscreen_height);
	}

	export
	[[nodiscard]] int get_horizontal_scrollbar_arrow_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::horizontal_scrollbar_arrow_width);
	}

	export
	[[nodiscard]] int get_horizontal_scrollbar_thumbbox_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::horizontal_scrollbar_thumbbox_width);
	}
	export

	[[nodiscard]] int default_icon_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::icon_width);
	}

	export
	[[nodiscard]] int default_icon_height() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::icon_height);
	}

	export
	[[nodiscard]] int get_icon_spacing_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::icon_spacing_width);
	}

	export
	[[nodiscard]] int get_icon_spacing_height() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::icon_spacing_height);
	}

	export
	[[nodiscard]] int default_width_maximized_window() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::maximized_width);
	}

	export
	[[nodiscard]] int default_height_maximized_window() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::maximized_height);
	}

	export
	[[nodiscard]] int default_max_width_window() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::max_window_width);
	}

	export
	[[nodiscard]] int default_max_height_window() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::max_window_height);
	}

	export
	[[nodiscard]] int get_menu_checkmark_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::menu_checkmark_width);
	}

	export
	[[nodiscard]] int get_menu_checkmark_height() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::menu_buttons_height);
	}

	export
	[[nodiscard]] int get_menu_buttons_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::menu_buttons_width);
	}

	export
	[[nodiscard]] int get_menu_buttons_height() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::menu_buttons_height);
	}

	export
	[[nodiscard]] int minimum_window_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::min_window_width);
	}

	export
	[[nodiscard]] int minimum_window_height() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::min_window_height);
	}

	export
	[[nodiscard]] int window_border_padding() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::border_padding);
	}

	export
	[[nodiscard]] int primary_display_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::display_width);
	}

	export
	[[nodiscard]] int primary_display_height() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::display_height);
	}

	export
	[[nodiscard]] int get_button_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::button_width);
	}

	export
	[[nodiscard]] int get_button_height() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::button_height);
	}

	export
	[[nodiscard]] int get_recommended_icon_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::recommended_icon_width);
	}

	export
	[[nodiscard]] int get_recommended_icon_height() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::recommended_icon_height);
	}

	export
	[[nodiscard]] int get_caption_button_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::caption_button_width);
	}

	export
	[[nodiscard]] int get_caption_button_height() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::caption_button_height);
	}

	export
	[[nodiscard]] int get_virtual_screen_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::virtual_screen_width);
	}

	export
	[[nodiscard]] int get_virtual_screen_height() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::virtual_screen_height);
	}

	export
	[[nodiscard]] int get_vertical_scrollbar_width() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::vertical_scrollbar_width);
	}

	export
	[[nodiscard]] int get_vertical_scrollbar_height() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::vertical_scrollbar_height);
	}

	export
	[[nodiscard]] bool is_dbcs_enabled() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::dbcs_enabled);
	}

	export
	[[nodiscard]] bool is_debug_installed() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::debug);
	}

	export
	[[nodiscard]] digitizer_value get_digitizer() noexcept
	{
		return digitizer_value{::GetSystemMetrics(system_metric_index::digitizer)};
	}

	export
	[[nodiscard]] bool is_input_method_manager_enabled() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::input_method_manager_enabled);
	}

	export
	[[nodiscard]] int number_maximum_touches() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::maximum_touches);
	}

	export
	[[nodiscard]] bool is_media_center_OS() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::media_center);
	}

	export
	[[nodiscard]] int get_menu_drop_alignment() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::menu_drop_alignment);
	}

	export
	[[nodiscard]] bool menu_drop_right_aligned() noexcept
	{
		return get_menu_drop_alignment();
	}

	export
	[[nodiscard]] bool menu_drop_left_aligned() noexcept
	{
		return get_menu_drop_alignment() == 0;
	}

	export
	[[nodiscard]] bool is_middle_east_enabled() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::middle_east_enabled);
	}

	export
	[[nodiscard]] bool mouse_installed() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::mouse_present);
	}

	export
	[[nodiscard]] bool horizontal_mouse_wheel_installed() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::mouse_horizontal_wheel_present);
	}

	export
	[[nodiscard]] bool vertical_mouse_wheel_installed() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::mouse_vertical_wheel_present);
	}

	export
	[[nodiscard]] bool is_network_present() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::network) & 1;
	}

	export
	[[nodiscard]] bool is_microsoft_pen_installed() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::microsoft_pen_installed);
	}

	export
	[[nodiscard]] bool is_remote_controlled() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::remote_control);
	}

	export
	[[nodiscard]] bool is_remote_session() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::remote_session);
	}

	export
	[[nodiscard]] bool displays_same_format() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::same_display_format);
	}

	export
	[[nodiscard]] int windows_server_2003R2_build_number() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::windows_server_build_number);
	}

	export
	[[nodiscard]] bool is_showing_sounds() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::show_sounds);
	}

	export
	[[nodiscard]] bool is_shutting_down() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::shutting_down);
	}

	export
	[[nodiscard]] bool is_slow_machine() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::slow_machine);
	}

	export
	[[nodiscard]] bool is_starter_OS() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::starter_edition);
	}

	export
	[[nodiscard]] bool is_tablet_PC() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::tablet_pc);
	}

	export
	[[nodiscard]] int virtual_screen_left_side_coordinates() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::left_virtual_screen);
	}

	export
	[[nodiscard]] int virtual_screen_top_side_coordinates() noexcept
	{
		return ::GetSystemMetrics(system_metric_index::top_virtual_screen);
	}

	export
	[[nodiscard]] cmoon::graphics::basic_size2D<int> primary_screen_size() noexcept
	{
		return {primary_display_width(), primary_display_height()};
	}
}