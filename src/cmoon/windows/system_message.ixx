module;

#include <Windows.h>
#include <windowsx.h>
#include <Dbt.h>
#include <CommCtrl.h>

export module cmoon.windows.system_message;

import <cstdint>;
import <functional>;

namespace cmoon::windows
{
	export
	struct system_message
	{
		std::uint32_t value;

		[[nodiscard]] explicit constexpr operator std::uint32_t() const noexcept
		{
			return value;
		}

		[[nodiscard]] friend constexpr bool operator==(const system_message&, const system_message&) = default;
		[[nodiscard]] friend constexpr bool operator!=(const system_message&, const system_message&) = default;
	};

	namespace messages
	{
		export
		inline constexpr system_message wm_clear {WM_CLEAR};

		export
		inline constexpr system_message wm_copy {WM_COPY};

		export
		inline constexpr system_message wm_cut {WM_CUT};

		export
		inline constexpr system_message wm_paste {WM_PASTE};

		export
		inline constexpr system_message wm_ask_clipboard_format_name {WM_ASKCBFORMATNAME};

		export
		inline constexpr system_message wm_change_clipboard_chain {WM_CHANGECBCHAIN};

		export
		inline constexpr system_message wm_clipboard_update {WM_CLIPBOARDUPDATE};

		export
		inline constexpr system_message wm_destroy_clipboard {WM_DESTROYCLIPBOARD};

		export
		inline constexpr system_message wm_draw_clipboard {WM_DRAWCLIPBOARD};

		export
		inline constexpr system_message wm_horizontal_scroll_clipboard {WM_HSCROLLCLIPBOARD};

		export
		inline constexpr system_message wm_vertical_scroll_clipboard {WM_VSCROLLCLIPBOARD};

		export
		inline constexpr system_message wm_paint_clipboard {WM_PAINTCLIPBOARD};

		export
		inline constexpr system_message wm_render_all_formats {WM_RENDERALLFORMATS};

		export
		inline constexpr system_message wm_render_format {WM_RENDERFORMAT};

		export
		inline constexpr system_message wm_size_clipboard {WM_SIZECLIPBOARD};

		export
		inline constexpr system_message wm_psd_env_stamp_rectangle {WM_PSD_ENVSTAMPRECT};

		export
		inline constexpr system_message wm_psd_full_page_rectangle {WM_PSD_FULLPAGERECT};

		export
		inline constexpr system_message wm_psd_greek_text_rectangle {WM_PSD_GREEKTEXTRECT};

		export
		inline constexpr system_message wm_psd_margin_rectangle {WM_PSD_MARGINRECT};

		export
		inline constexpr system_message wm_psd_min_margin_rectangle {WM_PSD_MINMARGINRECT};

		export
		inline constexpr system_message wm_psd_page_setup_dialog {WM_PSD_PAGESETUPDLG};

		export
		inline constexpr system_message wm_psd_ya_full_page_rectangle {WM_PSD_YAFULLPAGERECT};

		export
		inline constexpr system_message wm_dwm_colorization_color_changed {WM_DWMCOLORIZATIONCOLORCHANGED};

		export
		inline constexpr system_message wm_dwm_composition_changed {WM_DWMCOMPOSITIONCHANGED};

		export
		inline constexpr system_message wm_dwm_non_client_rendering_changed {WM_DWMNCRENDERINGCHANGED};

		export
		inline constexpr system_message wm_dwm_send_iconic_live_preview_bitmap {WM_DWMSENDICONICLIVEPREVIEWBITMAP};

		export
		inline constexpr system_message wm_dwm_send_iconic_thumbnail {WM_DWMSENDICONICTHUMBNAIL};

		export
		inline constexpr system_message wm_dwm_window_maximized_change {WM_DWMWINDOWMAXIMIZEDCHANGE};

		export
		inline constexpr system_message wm_copy_data {WM_COPYDATA};

		export
		inline constexpr system_message wm_set_cursor {WM_SETCURSOR};

		export
		inline constexpr system_message wm_device_change {WM_DEVICECHANGE};

		export
		inline constexpr system_message wm_control_color_dialog {WM_CTLCOLORDLG};

		export
		inline constexpr system_message wm_enter_idle {WM_ENTERIDLE};

		export
		inline constexpr system_message wm_get_dialog_code {WM_GETDLGCODE};

		export
		inline constexpr system_message wm_init_dialog {WM_INITDIALOG};

		export
		inline constexpr system_message wm_next_dialog_control {WM_NEXTDLGCTL};

		export
		inline constexpr system_message wm_dynamic_data_exchange_initiate {WM_DDE_INITIATE};

		export
		inline constexpr system_message wm_dynamic_data_exchange_ack {WM_DDE_ACK};

		export
		inline constexpr system_message wm_dynamic_data_exchange_advise {WM_DDE_ADVISE};

		export
		inline constexpr system_message wm_dynamic_data_exchange_data {WM_DDE_DATA};

		export
		inline constexpr system_message wm_dynamic_data_exchange_execute {WM_DDE_EXECUTE};

		export
		inline constexpr system_message wm_dynamic_data_exchange_poke {WM_DDE_POKE};

		export
		inline constexpr system_message wm_dynamic_data_exchange_request {WM_DDE_REQUEST};

		export
		inline constexpr system_message wm_dynamic_data_exchange_terminate {WM_DDE_TERMINATE};

		export
		inline constexpr system_message wm_dynamic_data_exchange_unadvise {WM_DDE_UNADVISE};

		export
		inline constexpr system_message wm_cancel_journal {WM_CANCELJOURNAL};

		export
		inline constexpr system_message wm_change_ui_state {WM_CHANGEUISTATE};

		export
		inline constexpr system_message wm_initialize_menu {WM_INITMENU};

		export
		inline constexpr system_message wm_query_ui_state {WM_QUERYUISTATE};

		export
		inline constexpr system_message wm_update_ui_state {WM_UPDATEUISTATE};

		export
		inline constexpr system_message wm_initialize_menu_popup {WM_INITMENUPOPUP};

		export
		inline constexpr system_message wm_menu_char {WM_MENUCHAR};

		export
		inline constexpr system_message wm_queue_sync {WM_QUEUESYNC};

		export
		inline constexpr system_message wm_menu_select {WM_MENUSELECT};

		export
		inline constexpr system_message wm_system_char {WM_SYSCHAR};

		export
		inline constexpr system_message wm_system_command {WM_SYSCOMMAND};

		export
		inline constexpr system_message wm_get_hotkey {WM_GETHOTKEY};

		export
		inline constexpr system_message wm_set_hotkey {WM_SETHOTKEY};

		export
		inline constexpr system_message wm_activate {WM_ACTIVATE};

		export
		inline constexpr system_message wm_app_command {WM_APPCOMMAND};

		export
		inline constexpr system_message wm_char {WM_CHAR};

		export
		inline constexpr system_message wm_dead_char {WM_DEADCHAR};

		export
		inline constexpr system_message wm_hotkey {WM_HOTKEY};

		export
		inline constexpr system_message wm_key_down {WM_KEYDOWN};

		export
		inline constexpr system_message wm_key_up {WM_KEYUP};

		export
		inline constexpr system_message wm_kill_focus {WM_KILLFOCUS};

		export
		inline constexpr system_message wm_set_focus {WM_SETFOCUS};

		export
		inline constexpr system_message wm_system_dead_char {WM_SYSDEADCHAR};

		export
		inline constexpr system_message wm_system_key_down {WM_SYSKEYDOWN};

		export
		inline constexpr system_message wm_system_key_up {WM_SYSKEYUP};

		export
		inline constexpr system_message wm_unicode_char {WM_UNICHAR};

		export
		inline constexpr system_message wm_command {WM_COMMAND};

		export
		inline constexpr system_message wm_context_menu {WM_CONTEXTMENU};

		export
		inline constexpr system_message wm_enter_menu_loop {WM_ENTERMENULOOP};

		export
		inline constexpr system_message wm_exit_menu_loop {WM_EXITMENULOOP};

		export
		inline constexpr system_message wm_get_title_bar_info {WM_GETTITLEBARINFOEX};

		export
		inline constexpr system_message wm_menu_command {WM_MENUCOMMAND};

		export
		inline constexpr system_message wm_menu_drag {WM_MENUDRAG};

		export
		inline constexpr system_message wm_menu_get_object {WM_MENUGETOBJECT};

		export
		inline constexpr system_message wm_menu_right_button_up {WM_MENURBUTTONUP};

		export
		inline constexpr system_message wm_next_menu {WM_NEXTMENU};

		export
		inline constexpr system_message wm_uninitialize_menu_popup {WM_UNINITMENUPOPUP};

		export
		inline constexpr system_message wm_capture_changed {WM_CAPTURECHANGED};

		export
		inline constexpr system_message wm_left_mouse_button_double_click {WM_LBUTTONDBLCLK};

		export
		inline constexpr system_message wm_left_mouse_button_down {WM_LBUTTONDOWN};

		export
		inline constexpr system_message wm_left_mouse_button_up {WM_LBUTTONUP};

		export
		inline constexpr system_message wm_middle_mouse_button_double_click {WM_MBUTTONDBLCLK};

		export
		inline constexpr system_message wm_middle_mouse_button_down {WM_MBUTTONDOWN};

		export
		inline constexpr system_message wm_middle_mouse_button_up {WM_MBUTTONUP};

		export
		inline constexpr system_message wm_mouse_activate {WM_MOUSEACTIVATE};

		export
		inline constexpr system_message wm_mouse_hover {WM_MOUSEHOVER};

		export
		inline constexpr system_message wm_mouse_horizontal_wheel {WM_MOUSEHWHEEL};

		export
		inline constexpr system_message wm_mouse_leave {WM_MOUSELEAVE};

		export
		inline constexpr system_message wm_mouse_move {WM_MOUSEMOVE};

		export
		inline constexpr system_message wm_mouse_wheel {WM_MOUSEWHEEL};

		export
		inline constexpr system_message wm_non_client_hit_test {WM_NCHITTEST};

		export
		inline constexpr system_message wm_non_client_left_button_double_click {WM_NCLBUTTONDBLCLK};

		export
		inline constexpr system_message wm_non_client_left_button_down {WM_NCLBUTTONDOWN};

		export
		inline constexpr system_message wm_non_client_left_button_up {WM_NCLBUTTONUP};

		export
		inline constexpr system_message wm_non_client_middle_button_double_click {WM_NCMBUTTONDBLCLK};

		export
		inline constexpr system_message wm_non_client_middle_button_down {WM_NCMBUTTONDOWN};

		export
		inline constexpr system_message wm_non_client_middle_button_up {WM_NCMBUTTONUP};

		export
		inline constexpr system_message wm_non_client_mouse_hover {WM_NCMOUSEHOVER};

		export
		inline constexpr system_message wm_non_client_mouse_leave {WM_NCMOUSELEAVE};

		export
		inline constexpr system_message wm_non_client_mouse_move {WM_NCMOUSEMOVE};

		export
		inline constexpr system_message wm_non_client_right_button_double_click {WM_NCRBUTTONDBLCLK};

		export
		inline constexpr system_message wm_non_client_right_button_down {WM_NCRBUTTONDOWN};

		export
		inline constexpr system_message wm_non_client_right_button_up {WM_NCRBUTTONUP};

		export
		inline constexpr system_message wm_non_client_x_button_double_click {WM_NCXBUTTONDBLCLK};

		export
		inline constexpr system_message wm_non_client_x_button_down {WM_NCXBUTTONDOWN};

		export
		inline constexpr system_message wm_non_client_x_button_up {WM_NCXBUTTONUP};

		export
		inline constexpr system_message wm_right_button_double_click {WM_RBUTTONDBLCLK};

		export
		inline constexpr system_message wm_right_button_down {WM_RBUTTONDOWN};

		export
		inline constexpr system_message wm_right_button_up {WM_RBUTTONUP};

		export
		inline constexpr system_message wm_x_button_double_click {WM_XBUTTONDBLCLK};

		export
		inline constexpr system_message wm_x_button_down {WM_XBUTTONDOWN};

		export
		inline constexpr system_message wm_x_button_up {WM_XBUTTONUP};

		export
		inline constexpr system_message wm_mdi_activate {WM_MDIACTIVATE};

		export
		inline constexpr system_message wm_mdi_cascade {WM_MDICASCADE};

		export
		inline constexpr system_message wm_mdi_create {WM_MDICREATE};

		export
		inline constexpr system_message wm_mdi_destroy {WM_MDIDESTROY};

		export
		inline constexpr system_message wm_mdi_get_active {WM_MDIGETACTIVE};

		export
		inline constexpr system_message wm_mdi_icon_arrange {WM_MDIICONARRANGE};

		export
		inline constexpr system_message wm_mdi_maximize {WM_MDIMAXIMIZE};

		export
		inline constexpr system_message wm_mdi_next {WM_MDINEXT};

		export
		inline constexpr system_message wm_mdi_refresh_menu {WM_MDIREFRESHMENU};

		export
		inline constexpr system_message wm_mdi_restore {WM_MDIRESTORE};

		export
		inline constexpr system_message wm_mdi_set_menu {WM_MDISETMENU};

		export
		inline constexpr system_message wm_mdi_tile {WM_MDITILE};

		export
		inline constexpr system_message wm_input {WM_INPUT};

		export
		inline constexpr system_message wm_input_device_change {WM_INPUT_DEVICE_CHANGE};

		export
		inline constexpr system_message wm_control_color_scrollbar {WM_CTLCOLORSCROLLBAR};

		export
		inline constexpr system_message wm_horizontal_scroll {WM_HSCROLL};

		export
		inline constexpr system_message wm_vertical_scroll {WM_VSCROLL};

		export
		inline constexpr system_message wm_timer {WM_TIMER};

		export
		inline constexpr system_message mn_get_menu_handle {MN_GETHMENU};

		export
		inline constexpr system_message wm_erase_background {WM_ERASEBKGND};

		export
		inline constexpr system_message wm_get_font {WM_GETFONT};

		export
		inline constexpr system_message wm_get_text {WM_GETTEXT};

		export
		inline constexpr system_message wm_get_text_length {WM_GETTEXTLENGTH};

		export
		inline constexpr system_message wm_set_font {WM_SETFONT};

		export
		inline constexpr system_message wm_set_icon {WM_SETICON};

		export
		inline constexpr system_message wm_set_text {WM_SETTEXT};

		export
		inline constexpr system_message wm_activate_app {WM_ACTIVATEAPP};

		export
		inline constexpr system_message wm_cancel_mode {WM_CANCELMODE};

		export
		inline constexpr system_message wm_child_activate {WM_CHILDACTIVATE};

		export
		inline constexpr system_message wm_close {WM_CLOSE};

		export
		inline constexpr system_message wm_compacting {WM_COMPACTING};

		export
		inline constexpr system_message wm_create {WM_CREATE};

		export
		inline constexpr system_message wm_destroy {WM_DESTROY};

		export
		inline constexpr system_message wm_dpi_changed {WM_DPICHANGED};

		export
		inline constexpr system_message wm_enable {WM_ENABLE};

		export
		inline constexpr system_message wm_enter_size_move {WM_ENTERSIZEMOVE};

		export
		inline constexpr system_message wm_exit_size_move {WM_EXITSIZEMOVE};

		export
		inline constexpr system_message wm_get_icon {WM_GETICON};

		export
		inline constexpr system_message wm_get_min_max_info {WM_GETMINMAXINFO};

		export
		inline constexpr system_message wm_input_language_change {WM_INPUTLANGCHANGE};

		export
		inline constexpr system_message wm_input_language_change_request {WM_INPUTLANGCHANGEREQUEST};

		export
		inline constexpr system_message wm_move {WM_MOVE};

		export
		inline constexpr system_message wm_moving {WM_MOVING};

		export
		inline constexpr system_message wm_non_client_activate {WM_NCACTIVATE};

		export
		inline constexpr system_message wm_non_client_calculate_size {WM_NCCALCSIZE};

		export
		inline constexpr system_message wm_non_client_create {WM_NCCREATE};

		export
		inline constexpr system_message wm_non_client_destroy {WM_NCDESTROY};

		export
		inline constexpr system_message wm_null {WM_NULL};

		export
		inline constexpr system_message wm_query_drag_icon {WM_QUERYDRAGICON};

		export
		inline constexpr system_message wm_query_open {WM_QUERYOPEN};

		export
		inline constexpr system_message wm_quit {WM_QUIT};

		export
		inline constexpr system_message wm_show_window {WM_SHOWWINDOW};

		export
		inline constexpr system_message wm_size {WM_SIZE};

		export
		inline constexpr system_message wm_sizing {WM_SIZING};

		export
		inline constexpr system_message wm_style_changed {WM_STYLECHANGED};

		export
		inline constexpr system_message wm_paint {WM_PAINT};

		export
		inline constexpr system_message wm_display_change {WM_DISPLAYCHANGE};

		export
		inline constexpr system_message wm_style_changing {WM_STYLECHANGING};

		export
		inline constexpr system_message wm_theme_changed {WM_THEMECHANGED};

		export
		inline constexpr system_message wm_user_changed {WM_USERCHANGED};

		export
		inline constexpr system_message wm_window_position_changed {WM_WINDOWPOSCHANGED};

		export
		inline constexpr system_message wm_window_position_changing {WM_WINDOWPOSCHANGING};

		export
		inline constexpr system_message cdn_file_ok {CDN_FILEOK};

		export
		inline constexpr system_message cdn_folder_change {CDN_FOLDERCHANGE};

		export
		inline constexpr system_message cdn_help {CDN_HELP};

		export
		inline constexpr system_message cdn_include_item {CDN_INCLUDEITEM};

		export
		inline constexpr system_message cdn_init_done {CDN_INITDONE};

		export
		inline constexpr system_message cdn_sel_change {CDN_SELCHANGE};

		export
		inline constexpr system_message cdn_share_violation {CDN_SHAREVIOLATION};

		export
		inline constexpr system_message cdn_type_change {CDN_TYPECHANGE};

	}
}

namespace std
{
	export
	template<>
	struct hash<cmoon::windows::system_message>
	{
		std::size_t operator()(const cmoon::windows::system_message& message) const noexcept
		{
			return std::hash<std::uint32_t>{}(message.value);
		}
	};
}