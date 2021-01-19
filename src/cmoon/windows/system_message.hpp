#pragma once

#include <concepts>
#include <utility>
#include <system_error>
#include <type_traits>

#include <windows.h>
#include <windowsx.h>
#include <Dbt.h>
#include <CommCtrl.h>

#include "cmoon/windows/global_lock.hpp"
#include "cmoon/windows/enums.hpp"
#include "cmoon/windows/language.hpp"

namespace cmoon
{
	namespace windows
	{
		struct keystroke_message
		{
			std::uint16_t repeat_count: 16;
			std::uint8_t scan_code: 8;
			bool is_extended_key: 1;
		private:
			char reserved : 4;
		public:
			bool alt_context : 1;
			bool is_down : 1;
			bool is_releasing : 1;
		};

		enum message : ::UINT
		{
			wm_clear = WM_CLEAR,
			wm_copy = WM_COPY,
			wm_cut = WM_CUT,
			wm_paste = WM_PASTE,
			wm_ask_clipboard_format_name = WM_ASKCBFORMATNAME,
			wm_change_clipboard_chain = WM_CHANGECBCHAIN,
			wm_clipboard_update = WM_CLIPBOARDUPDATE,
			wm_destroy_clipboard = WM_DESTROYCLIPBOARD,
			wm_draw_clipboard = WM_DRAWCLIPBOARD,
			wm_horizontal_scroll_clipboard = WM_HSCROLLCLIPBOARD,
			wm_vertical_scroll_clipboard = WM_VSCROLLCLIPBOARD,
			wm_paint_clipboard = WM_PAINTCLIPBOARD,
			wm_render_all_formats = WM_RENDERALLFORMATS,
			wm_render_format = WM_RENDERFORMAT,
			wm_size_clipboard = WM_SIZECLIPBOARD,
			wm_psd_env_stamp_rectangle = WM_PSD_ENVSTAMPRECT,
			wm_psd_full_page_rectangle = WM_PSD_FULLPAGERECT,
			wm_psd_greek_text_rectangle = WM_PSD_GREEKTEXTRECT,
			wm_psd_margin_rectangle = WM_PSD_MARGINRECT,
			wm_psd_min_margin_rectangle = WM_PSD_MINMARGINRECT,
			wm_psd_page_setup_dialog = WM_PSD_PAGESETUPDLG,
			wm_psd_ya_full_page_rectangle = WM_PSD_YAFULLPAGERECT,
			wm_dwm_colorization_color_changed = WM_DWMCOLORIZATIONCOLORCHANGED,
			wm_dwm_composition_changed = WM_DWMCOMPOSITIONCHANGED,
			wm_dwm_non_client_rendering_changed = WM_DWMNCRENDERINGCHANGED,
			wm_dwm_send_iconic_live_preview_bitmap = WM_DWMSENDICONICLIVEPREVIEWBITMAP,
			wm_dwm_send_iconic_thumbnail = WM_DWMSENDICONICTHUMBNAIL,
			wm_dwm_window_maximized_change = WM_DWMWINDOWMAXIMIZEDCHANGE,
			wm_copy_data = WM_COPYDATA,
			wm_set_cursor = WM_SETCURSOR,
			wm_device_change = WM_DEVICECHANGE,
			wm_control_color_dialog = WM_CTLCOLORDLG,
			wm_enter_idle = WM_ENTERIDLE,
			wm_get_dialog_code = WM_GETDLGCODE,
			wm_init_dialog = WM_INITDIALOG,
			wm_next_dialog_control = WM_NEXTDLGCTL,
			wm_dynamic_data_exchange_initiate = WM_DDE_INITIATE,
			wm_dynamic_data_exchange_ack = WM_DDE_ACK,
			wm_dynamic_data_exchange_advise = WM_DDE_ADVISE,
			wm_dynamic_data_exchange_data = WM_DDE_DATA,
			wm_dynamic_data_exchange_execute = WM_DDE_EXECUTE,
			wm_dynamic_data_exchange_poke = WM_DDE_POKE,
			wm_dynamic_data_exchange_request = WM_DDE_REQUEST,
			wm_dynamic_data_exchange_terminate = WM_DDE_TERMINATE,
			wm_dynamic_data_exchange_unadvise = WM_DDE_UNADVISE,
			wm_cancel_journal = WM_CANCELJOURNAL,
			wm_change_ui_state = WM_CHANGEUISTATE,
			wm_initialize_menu = WM_INITMENU,
			wm_query_ui_state = WM_QUERYUISTATE,
			wm_update_ui_state = WM_UPDATEUISTATE,
			wm_initialize_menu_popup = WM_INITMENUPOPUP,
			wm_menu_char = WM_MENUCHAR,
			wm_queue_sync = WM_QUEUESYNC,
			wm_menu_select = WM_MENUSELECT,
			wm_system_char = WM_SYSCHAR,
			wm_system_command = WM_SYSCOMMAND,
			wm_get_hotkey = WM_GETHOTKEY,
			wm_set_hotkey = WM_SETHOTKEY,
			wm_activate = WM_ACTIVATE,
			wm_app_command = WM_APPCOMMAND,
			wm_char = WM_CHAR,
			wm_dead_char = WM_DEADCHAR,
			wm_hotkey = WM_HOTKEY,
			wm_key_down = WM_KEYDOWN,
			wm_key_up = WM_KEYUP,
			wm_kill_focus = WM_KILLFOCUS,
			wm_set_focus = WM_SETFOCUS,
			wm_system_dead_char = WM_SYSDEADCHAR,
			wm_system_key_down = WM_SYSKEYDOWN,
			wm_system_key_up = WM_SYSKEYUP,
			wm_unicode_char = WM_UNICHAR,
			wm_command = WM_COMMAND,
			wm_context_menu = WM_CONTEXTMENU,
			wm_enter_menu_loop = WM_ENTERMENULOOP,
			wm_exit_menu_loop = WM_EXITMENULOOP,
			wm_get_title_bar_info = WM_GETTITLEBARINFOEX,
			wm_menu_command = WM_MENUCOMMAND,
			wm_menu_drag = WM_MENUDRAG,
			wm_menu_get_object = WM_MENUGETOBJECT,
			wm_menu_right_button_up = WM_MENURBUTTONUP,
			wm_next_menu = WM_NEXTMENU,
			wm_uninitialize_menu_popup = WM_UNINITMENUPOPUP,
			wm_capture_changed = WM_CAPTURECHANGED,
			wm_left_mouse_button_double_click = WM_LBUTTONDBLCLK,
			wm_left_mouse_button_down = WM_LBUTTONDOWN,
			wm_left_mouse_button_up = WM_LBUTTONUP,
			wm_middle_mouse_button_double_click = WM_MBUTTONDBLCLK,
			wm_middle_mouse_button_down = WM_MBUTTONDOWN,
			wm_middle_mouse_button_up = WM_MBUTTONUP,
			wm_mouse_activate = WM_MOUSEACTIVATE,
			wm_mouse_hover = WM_MOUSEHOVER,
			wm_mouse_horizontal_wheel = WM_MOUSEHWHEEL,
			wm_mouse_leave = WM_MOUSELEAVE,
			wm_mouse_move = WM_MOUSEMOVE,
			wm_mouse_wheel = WM_MOUSEWHEEL,
			wm_non_client_hit_test = WM_NCHITTEST,
			wm_non_client_left_button_double_click = WM_NCLBUTTONDBLCLK,
			wm_non_client_left_button_down = WM_NCLBUTTONDOWN,
			wm_non_client_left_button_up = WM_NCLBUTTONUP,
			wm_non_client_middle_button_double_click = WM_NCMBUTTONDBLCLK,
			wm_non_client_middle_button_down = WM_NCMBUTTONDOWN,
			wm_non_client_middle_button_up = WM_NCMBUTTONUP,
			wm_non_client_mouse_hover = WM_NCMOUSEHOVER,
			wm_non_client_mouse_leave = WM_NCMOUSELEAVE,
			wm_non_client_mouse_move = WM_NCMOUSEMOVE,
			wm_non_client_right_button_double_click = WM_NCRBUTTONDBLCLK,
			wm_non_client_right_button_down = WM_NCRBUTTONDOWN,
			wm_non_client_right_button_up = WM_NCRBUTTONUP,
			wm_non_client_x_button_double_click = WM_NCXBUTTONDBLCLK,
			wm_non_client_x_button_down = WM_NCXBUTTONDOWN,
			wm_non_client_x_button_up = WM_NCXBUTTONUP,
			wm_right_button_double_click = WM_RBUTTONDBLCLK,
			wm_right_button_down = WM_RBUTTONDOWN,
			wm_right_button_up = WM_RBUTTONUP,
			wm_x_button_double_click = WM_XBUTTONDBLCLK,
			wm_x_button_down = WM_XBUTTONDOWN,
			wm_x_button_up = WM_XBUTTONUP,
			wm_mdi_activate = WM_MDIACTIVATE,
			wm_mdi_cascade = WM_MDICASCADE,
			wm_mdi_create = WM_MDICREATE,
			wm_mdi_destroy = WM_MDIDESTROY,
			wm_mdi_get_active = WM_MDIGETACTIVE,
			wm_mdi_icon_arrange = WM_MDIICONARRANGE,
			wm_mdi_maximize = WM_MDIMAXIMIZE,
			wm_mdi_next = WM_MDINEXT,
			wm_mdi_refresh_menu = WM_MDIREFRESHMENU,
			wm_mdi_restore = WM_MDIRESTORE,
			wm_mdi_set_menu = WM_MDISETMENU,
			wm_mdi_tile = WM_MDITILE,
			wm_input = WM_INPUT,
			wm_input_device_change = WM_INPUT_DEVICE_CHANGE,
			wm_control_color_scrollbar = WM_CTLCOLORSCROLLBAR,
			wm_horizontal_scroll = WM_HSCROLL,
			wm_vertical_scroll = WM_VSCROLL,
			wm_timer = WM_TIMER,
			mn_get_menu_handle = MN_GETHMENU,
			wm_erase_background = WM_ERASEBKGND,
			wm_get_font = WM_GETFONT,
			wm_get_text = WM_GETTEXT,
			wm_get_text_length = WM_GETTEXTLENGTH,
			wm_set_font = WM_SETFONT,
			wm_set_icon = WM_SETICON,
			wm_set_text = WM_SETTEXT,
			wm_activate_app = WM_ACTIVATEAPP,
			wm_cancel_mode = WM_CANCELMODE,
			wm_child_activate = WM_CHILDACTIVATE,
			wm_close = WM_CLOSE,
			wm_compacting = WM_COMPACTING,
			wm_create = WM_CREATE,
			wm_destroy = WM_DESTROY,
			wm_dpi_changed = WM_DPICHANGED,
			wm_enable = WM_ENABLE,
			wm_enter_size_move = WM_ENTERSIZEMOVE,
			wm_exit_size_move = WM_EXITSIZEMOVE,
			wm_get_icon = WM_GETICON,
			wm_get_min_max_info = WM_GETMINMAXINFO,
			wm_input_language_change = WM_INPUTLANGCHANGE,
			wm_input_language_change_request = WM_INPUTLANGCHANGEREQUEST,
			wm_move = WM_MOVE,
			wm_moving = WM_MOVING,
			wm_non_client_activate = WM_NCACTIVATE,
			wm_non_client_calculate_size = WM_NCCALCSIZE,
			wm_non_client_create = WM_NCCREATE,
			wm_non_client_destroy = WM_NCDESTROY,
			wm_null = WM_NULL,
			wm_query_drag_icon = WM_QUERYDRAGICON,
			wm_query_open = WM_QUERYOPEN,
			wm_quit = WM_QUIT,
			wm_show_window = WM_SHOWWINDOW,
			wm_size = WM_SIZE,
			wm_sizing = WM_SIZING,
			wm_style_changed = WM_STYLECHANGED,
			wm_paint = WM_PAINT,
			wm_display_change = WM_DISPLAYCHANGE,
			wm_style_changing = WM_STYLECHANGING,
			wm_theme_changed = WM_THEMECHANGED,
			wm_user_changed = WM_USERCHANGED,
			wm_window_position_changed = WM_WINDOWPOSCHANGED,
			wm_window_position_changing = WM_WINDOWPOSCHANGING,
			cdn_file_ok = CDN_FILEOK,
			cdn_folder_change = CDN_FOLDERCHANGE,
			cdn_help = CDN_HELP,
			cdn_include_item = CDN_INCLUDEITEM,
			cdn_init_done = CDN_INITDONE,
			cdn_sel_change = CDN_SELCHANGE,
			cdn_share_violation = CDN_SHAREVIOLATION,
			cdn_type_change = CDN_TYPECHANGE,
		};

		template<message GM>
		struct w_params;

		template<message GM>
		struct l_params;

		namespace details
		{
			template<class T, class ParamsType, bool IsNoexcept = true>
			struct single_type
			{
				using type = T;

				[[nodiscard]] static constexpr type get_value(ParamsType param) noexcept(IsNoexcept)
					requires(!std::is_pointer_v<T>)
				{
					return static_cast<type>(param);
				}

				[[nodiscard]] static type get_value(ParamsType param) noexcept(IsNoexcept)
					requires(std::is_pointer_v<T>)
				{
					return reinterpret_cast<T>(param);
				}
			};

			template<class T, class T2, class ParamsType, bool IsNoexcept1 = true, bool IsNoexcept2 = true>
			struct double_type
			{
				using low_order_type = T;
				using high_order_type = T2;

				[[nodiscard]] static constexpr low_order_type get_low_order_value(ParamsType param) noexcept(IsNoexcept1)
					requires(!std::is_pointer_v<low_order_type>)
				{
					return static_cast<low_order_type>(LOWORD(param));
				}

				[[nodiscard]] static low_order_type get_low_order_value(ParamsType param) noexcept(IsNoexcept1)
					requires(std::is_pointer_v<low_order_type>)
				{
					return reinterpret_cast<low_order_type>(LOWORD(param));
				}

				[[nodiscard]] constexpr static high_order_type get_high_order_value(ParamsType param) noexcept(IsNoexcept2)
					requires(!std::is_pointer_v<high_order_type>)
				{
					return static_cast<high_order_type>(HIWORD(param));
				}

				[[nodiscard]] static high_order_type get_high_order_value(ParamsType param) noexcept(IsNoexcept2)
					requires(std::is_pointer_v<high_order_type>)
				{
					return reinterpret_cast<high_order_type>(HIWORD(param));
				}

				[[nodiscard]] static constexpr std::pair<low_order_type, high_order_type> get_value(ParamsType param) noexcept(IsNoexcept1 && IsNoexcept2)
					requires(!std::is_pointer_v<low_order_type> && !std::is_pointer_v<high_order_type>)
				{
					return {get_low_order_value(param),
							get_high_order_value(param)};
				}

				[[nodiscard]] static std::pair<low_order_type, high_order_type> get_value(ParamsType param) noexcept(IsNoexcept1 && IsNoexcept2)
				{
					return {get_low_order_value(param),
							get_high_order_value(param)};
				}
			};

			struct xy_coordinate
				: public double_type<int, int, ::LPARAM> {};

			template<class ParamType>
			struct window_handle
				: public single_type<::HWND, ParamType> {};
		}

		template<>
		struct w_params<message::wm_ask_clipboard_format_name>
		: public details::single_type<std::size_t, ::WPARAM> {};

		template<>
		struct l_params<message::wm_ask_clipboard_format_name>
			: public details::single_type<char*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_change_clipboard_chain>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_change_clipboard_chain>
			: public details::single_type<::HWND, ::LPARAM> {};

		template<>
		struct w_params<message::wm_horizontal_scroll_clipboard>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_horizontal_scroll_clipboard>
		{
			using high_order_type = int;

			enum class low_order_type : std::uint32_t
			{
				end_scroll = SB_ENDSCROLL,
				left = SB_LEFT,
				right = SB_RIGHT,
				line_left = SB_LINELEFT,
				line_right = SB_LINERIGHT,
				page_left = SB_PAGELEFT,
				page_right = SB_PAGERIGHT,
				thumb_position = SB_THUMBPOSITION
			};

			[[nodiscard]] static constexpr low_order_type get_low_order_value(::LPARAM param) noexcept
			{
				return static_cast<low_order_type>(LOWORD(param));
			}

			[[nodiscard]] static constexpr high_order_type get_high_order_value(::LPARAM param) noexcept
			{
				return static_cast<high_order_type>(HIWORD(param));
			}

			[[nodiscard]] static constexpr std::pair<low_order_type, high_order_type> get_value(::LPARAM param) noexcept
			{
				return {get_low_order_value(param),
						get_high_order_value(param)};
			}
		};

		template<>
		struct w_params<message::wm_paint_clipboard>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_paint_clipboard>
		{
			using type = details::global_lock_resource<::PAINTSTRUCT>;

			[[nodiscard]] static type get_value(::LPARAM param) noexcept
			{
				return global_resource(reinterpret_cast<::PAINTSTRUCT*>(param));
			}
		};

		template<>
		struct w_params<message::wm_render_format>
		{
			enum class type : std::uint32_t
			{
				bitmap = CF_BITMAP,
				dib = CF_DIB,
				dibv5 = CF_DIBV5,
				dif = CF_DIF,
				dsp_bitmap = CF_DSPBITMAP,
				dsp_metafile_picture = CF_DSPMETAFILEPICT,
				dsp_text = CF_DSPTEXT,
				enhanced_metafile = CF_ENHMETAFILE,
				gdi_object_first = CF_GDIOBJFIRST,
				gdi_object_last = CF_GDIOBJLAST,
				handle_drop = CF_HDROP,
				locale = CF_LOCALE,
				metafile_picture = CF_METAFILEPICT,
				oem_text = CF_OEMTEXT,
				owner_display = CF_OWNERDISPLAY,
				palette = CF_PALETTE,
				pen_data = CF_PENDATA,
				private_first = CF_PRIVATEFIRST,
				private_last = CF_PRIVATELAST,
				riff = CF_RIFF,
				sylk = CF_SYLK,
				unicode_text = CF_UNICODETEXT,
				wave = CF_WAVE
			};

			[[nodiscard]] static constexpr type get_value(::WPARAM param) noexcept
			{
				return static_cast<type>(param);
			}
		};

		template<>
		struct w_params<message::wm_size_clipboard>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_size_clipboard>
		{
			using type = details::global_lock_resource<::RECT>;

			[[nodiscard]] static type get_value(::LPARAM param) noexcept
			{
				return global_resource(reinterpret_cast<::RECT*>(param));
			}
		};

		template<>
		struct w_params<message::wm_vertical_scroll_clipboard>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_vertical_scroll_clipboard>
		{
			using high_order_type = int;
			enum class low_order_type : std::uint32_t
			{
				bottom = SB_BOTTOM,
				end_scroll = SB_ENDSCROLL,
				line_down = SB_LINEDOWN,
				line_up = SB_LINEUP,
				page_down = SB_PAGEDOWN,
				page_up = SB_PAGEUP,
				thumb_position = SB_THUMBPOSITION,
				top = SB_TOP
			};

			[[nodiscard]] static constexpr low_order_type get_low_order_value(::LPARAM param) noexcept
			{
				return static_cast<low_order_type>(LOWORD(param));
			}

			[[nodiscard]] static constexpr high_order_type get_high_order_value(::LPARAM param) noexcept
			{
				return static_cast<high_order_type>(HIWORD(param));
			}

			[[nodiscard]] static constexpr std::pair<low_order_type, high_order_type> get_value(::LPARAM param) noexcept
			{
				return {get_low_order_value(param),
						get_high_order_value(param)};
			}
		};

		template<>
		struct l_params<message::cdn_file_ok>
			: public details::single_type<::OFNOTIFY*, ::LPARAM> {};

		template<>
		struct l_params<message::cdn_folder_change>
			: public details::single_type<::OFNOTIFY*, ::LPARAM> {};

		template<>
		struct l_params<message::cdn_help>
			: public details::single_type<::OFNOTIFY*, ::LPARAM> {};

		template<>
		struct l_params<message::cdn_include_item>
			: public details::single_type<::OFNOTIFYEX*, ::LPARAM> {};

		template<>
		struct l_params<message::cdn_init_done>
			: public details::single_type<::OFNOTIFY*, ::LPARAM> {};

		template<>
		struct l_params<message::cdn_sel_change>
			: public details::single_type<::OFNOTIFY*, ::LPARAM> {};

		template<>
		struct l_params<message::cdn_share_violation>
			: public details::single_type<::OFNOTIFY*, ::LPARAM> {};

		template<>
		struct l_params<message::cdn_type_change>
			: public details::single_type<::OFNOTIFY*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_psd_env_stamp_rectangle>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_psd_env_stamp_rectangle>
			: public details::single_type<::RECT*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_psd_full_page_rectangle>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_psd_full_page_rectangle>
			: public details::single_type<::RECT*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_psd_greek_text_rectangle>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_psd_greek_text_rectangle>
			: public details::single_type<::RECT*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_psd_margin_rectangle>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_psd_margin_rectangle>
			: public details::single_type<::RECT*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_psd_min_margin_rectangle>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_psd_min_margin_rectangle>
			: public details::single_type<::RECT*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_psd_page_setup_dialog>
		{
			using low_order_type = std::uint32_t;
			enum class high_order_type : std::uint32_t
			{
				paper_landscape_dot_matrix = 0x0001,
				paper_landscape_HPPCL = 0x0003,
				paper_portrait_dot_matrix = 0x0005,
				paper_portrait_HPPCL = 0x0007,
				envelope_landscape_HPPCL = 0x000b,
				envelope_portrait_dot_matrix = 0x000d,
				envelope_landscape_dot_matrix = 0x0019,
				envelope_portrait_HPPCL = 0x001f
			};

			[[nodiscard]] static constexpr low_order_type get_low_order_value(::WPARAM param) noexcept
			{
				return static_cast<low_order_type>(LOWORD(param));
			}

			[[nodiscard]] static constexpr high_order_type get_high_order_value(::WPARAM param) noexcept
			{
				return static_cast<high_order_type>(HIWORD(param));
			}

			[[nodiscard]] static constexpr std::pair<low_order_type, high_order_type> get_value(::WPARAM param) noexcept
			{
				return {get_low_order_value(param),
						get_high_order_value(param)};
			}
		};

		template<>
		struct l_params<message::wm_psd_page_setup_dialog>
			: public details::single_type<::PAGESETUPDLG*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_psd_ya_full_page_rectangle>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_psd_ya_full_page_rectangle>
			: public details::single_type<::RECT*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_set_cursor>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_set_cursor>
		{
			enum class low_order_type : std::int32_t
			{
				border = 18,
				bottom = 15,
				bottom_left = 16,
				bottom_right = 17,
				caption = 2,
				client = 1,
				close = 20,
				error = -2,
				grow_box = 4,
				help = 21,
				horizontal_scroll = 6,
				left = 10,
				menu = 5,
				max_button = 9,
				min_button = 8,
				reduce = 8,
				nowhere = 0,
				right = 11,
				size = 4,
				sys_menu = 3,
				top = 12,
				top_left = 13,
				top_right = 14,
				transparent = -1,
				vertical_scroll = 7,
				zoom = 9
			};

			using high_order_type = mouse_keyboard;

			[[nodiscard]] static constexpr low_order_type get_low_order_value(::LPARAM param) noexcept
			{
				return static_cast<low_order_type>(LOWORD(param));
			}

			[[nodiscard]] static constexpr high_order_type get_high_order_value(::LPARAM param) noexcept
			{
				return static_cast<high_order_type>(HIWORD(param));
			}

			[[nodiscard]] static constexpr std::pair<low_order_type, high_order_type> get_value(::LPARAM param) noexcept
			{
				return {get_low_order_value(param),
						get_high_order_value(param)};
			}
		};

		template<>
		struct w_params<message::wm_copy_data>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_copy_data>
			: public details::single_type<::COPYDATASTRUCT*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_dwm_colorization_color_changed>
			: public details::single_type<std::uint32_t, ::WPARAM> {};

		template<>
		struct l_params<message::wm_dwm_colorization_color_changed>
			: public details::single_type<bool, ::LPARAM> {};

		template<>
		struct w_params<message::wm_dwm_non_client_rendering_changed>
			: public details::single_type<bool, ::WPARAM> {};

		template<>
		struct l_params<message::wm_dwm_send_iconic_thumbnail>
			: public details::double_type<std::uint32_t, std::uint32_t, ::LPARAM> {};

		template<>
		struct w_params<message::wm_dwm_window_maximized_change>
			: public details::single_type<bool, ::WPARAM> {};

		template<>
		struct w_params<message::wm_device_change>
		{
			enum class type : std::uint32_t
			{
				config_change_canceled = DBT_CONFIGCHANGECANCELED,
				config_changed = DBT_CONFIGCHANGED,
				custom_event = DBT_CUSTOMEVENT,
				device_arrival = DBT_DEVICEARRIVAL,
				device_query_remove = DBT_DEVICEQUERYREMOVE,
				device_query_remove_failed = DBT_DEVICEQUERYREMOVEFAILED,
				device_remove_complete = DBT_DEVICEREMOVECOMPLETE,
				device_remove_pending = DBT_DEVICEREMOVEPENDING,
				device_type_specific = DBT_DEVICETYPESPECIFIC,
				device_nodes_changed = DBT_DEVNODES_CHANGED,
				query_change_config = DBT_QUERYCHANGECONFIG,
				user_defined = DBT_USERDEFINED
			};

			[[nodiscard]] static constexpr type get_value(::WPARAM param) noexcept
			{
				return static_cast<type>(param);
			}
		};
		
		template<>
		struct l_params<message::wm_device_change>
			: public details::single_type<::LPARAM, ::LPARAM> {};

		template<>
		struct w_params<message::wm_control_color_dialog>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_control_color_dialog>
			: public details::single_type<::HWND, ::LPARAM> {};

		template<>
		struct w_params<message::wm_enter_idle>
		{
			enum class type : std::uint32_t
			{
				dialog_box = MSGF_DIALOGBOX,
				menu = MSGF_MENU
			};

			[[nodiscard]] static constexpr type get_value(::WPARAM param) noexcept
			{
				return static_cast<type>(param);
			}
		};

		template<>
		struct l_params<message::wm_enter_idle>
			: public details::single_type<::HWND, ::LPARAM> {};

		template<>
		struct w_params<message::wm_get_dialog_code>
			: public details::single_type<virtual_keys, ::WPARAM> {};

		template<>
		struct l_params<message::wm_get_dialog_code>
			: public details::single_type<::MSG*, ::LPARAM> {};

		// TODO: The type depends on the run-time value of l_param. Not sure
		// how this should be handled, if at all.
		template<>
		struct w_params<message::wm_next_dialog_control>
			: public details::single_type<::WPARAM, ::WPARAM> {};

		template<>
		struct l_params<message::wm_next_dialog_control>
			: public details::single_type<bool, ::LPARAM> {};

		template<>
		struct w_params<message::wm_dynamic_data_exchange_initiate>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_dynamic_data_exchange_initiate>
			: public details::double_type<::ATOM, ::ATOM, ::LPARAM> {};

		template<>
		struct w_params<message::wm_dynamic_data_exchange_ack>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_dynamic_data_exchange_ack>
			: public details::double_type<WORD, WORD, ::LPARAM> {};

		template<>
		struct w_params<message::wm_dynamic_data_exchange_advise>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_dynamic_data_exchange_advise>
		{
			using low_order_type = details::global_lock_resource<::DDEADVISE>;
			using high_order_type = ::ATOM;

			[[nodiscard]] static low_order_type get_low_order_value(::LPARAM param) noexcept
			{
				return global_resource(reinterpret_cast<::DDEADVISE*>(LOWORD(param)));
			}

			[[nodiscard]] static constexpr high_order_type get_high_order_value(::LPARAM param) noexcept
			{
				return static_cast<high_order_type>(HIWORD(param));
			}

			[[nodiscard]] static std::pair<low_order_type, high_order_type> get_value(::LPARAM param) noexcept
			{
				return {get_low_order_value(param),
						get_high_order_value(param)};
			}
		};

		template<>
		struct w_params<message::wm_dynamic_data_exchange_data>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_dynamic_data_exchange_data>
		{
			using low_order_type = details::global_lock_resource<::DDEDATA>;
			using high_order_type = ::ATOM;

			[[nodiscard]] static low_order_type get_low_order_value(::LPARAM param) noexcept
			{
				return global_resource(reinterpret_cast<::DDEDATA*>(LOWORD(param)));
			}

			[[nodiscard]] static constexpr high_order_type get_high_order_value(::LPARAM param) noexcept
			{
				return static_cast<high_order_type>(HIWORD(param));
			}

			[[nodiscard]] static std::pair<low_order_type, high_order_type> get_value(::LPARAM param) noexcept
			{
				return {get_low_order_value(param),
						get_high_order_value(param)};
			}
		};

		template<>
		struct w_params<message::wm_dynamic_data_exchange_execute>
			: public details::single_type<::HWND, ::WPARAM> {};

		// TODO: The type depends on the character encoding of the windowss
		// invoved. Not sure how to handle this, or if it should be handled at all.
		template<>
		struct l_params<message::wm_dynamic_data_exchange_execute>
			: public details::single_type<::LPARAM, ::LPARAM> {};

		template<>
		struct w_params<message::wm_dynamic_data_exchange_poke>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_dynamic_data_exchange_poke>
		{
			using low_order_type = details::global_lock_resource<::DDEPOKE>;
			using high_order_type = ::ATOM;

			[[nodiscard]] static low_order_type get_low_order_value(::LPARAM param) noexcept
			{
				return global_resource(reinterpret_cast<::DDEPOKE*>(LOWORD(param)));
			}

			[[nodiscard]] static constexpr high_order_type get_high_order_value(::LPARAM param) noexcept
			{
				return static_cast<high_order_type>(HIWORD(param));
			}

			[[nodiscard]] static std::pair<low_order_type, high_order_type> get_value(::LPARAM param) noexcept
			{
				return {get_low_order_value(param),
						get_high_order_value(param)};
			}
		};

		template<>
		struct w_params<message::wm_dynamic_data_exchange_request>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_dynamic_data_exchange_request>
			: public details::double_type<std::uint32_t, ::ATOM, ::LPARAM> {};

		template<>
		struct w_params<message::wm_dynamic_data_exchange_terminate>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct w_params<message::wm_dynamic_data_exchange_unadvise>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_dynamic_data_exchange_unadvise>
			: public details::double_type<std::uint32_t, ::ATOM, ::LPARAM> {};

		template<>
		struct w_params<message::wm_change_ui_state>
		{
			enum class low_order_type : std::uint32_t
			{
				clear = UIS_CLEAR,
				initialize = UIS_INITIALIZE,
				set = UIS_SET
			};

			enum class high_order_type : std::uint32_t
			{
				active = UISF_ACTIVE,
				hide_accelerators = UISF_HIDEACCEL,
				hide_focus = UISF_HIDEFOCUS
			};

			[[nodiscard]] static constexpr low_order_type get_low_order_value(::WPARAM param) noexcept
			{
				return static_cast<low_order_type>(LOWORD(param));
			}

			[[nodiscard]] static constexpr high_order_type get_high_order_value(::WPARAM param) noexcept
			{
				return static_cast<high_order_type>(HIWORD(param));
			}

			[[nodiscard]] static constexpr std::pair<low_order_type, high_order_type> get_value(::WPARAM param) noexcept
			{
				return {get_low_order_value(param),
						get_high_order_value(param)};
			}
		};

		template<>
		struct w_params<message::wm_initialize_menu>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct w_params<message::wm_update_ui_state>
		{
			enum class low_order_type : std::uint32_t
			{
				clear = UIS_CLEAR,
				initialize = UIS_INITIALIZE,
				set = UIS_SET
			};

			enum class high_order_type : std::uint32_t
			{
				active = UISF_ACTIVE,
				hide_accelerators = UISF_HIDEACCEL,
				hide_focus = UISF_HIDEFOCUS
			};

			[[nodiscard]] static constexpr low_order_type get_low_order_value(::WPARAM param) noexcept
			{
				return static_cast<low_order_type>(LOWORD(param));
			}

			[[nodiscard]] static constexpr high_order_type get_high_order_value(::WPARAM param) noexcept
			{
				return static_cast<high_order_type>(HIWORD(param));
			}

			[[nodiscard]] static constexpr std::pair<low_order_type, high_order_type> get_value(::WPARAM param) noexcept
			{
				return {get_low_order_value(param),
						get_high_order_value(param)};
			}
		};

		template<>
		struct w_params<message::wm_initialize_menu_popup>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_initialize_menu_popup>
			: public details::double_type<std::int32_t, bool, ::LPARAM> {};

		template<>
		struct w_params<message::wm_menu_char>
		{
			using low_order_type = std::uint32_t;

			enum class high_order_type : std::uint32_t
			{
				popup = MF_POPUP,
				system_menu = MF_SYSMENU
			};

			[[nodiscard]] static constexpr low_order_type get_low_order_value(::WPARAM param) noexcept
			{
				return static_cast<low_order_type>(LOWORD(param));
			}

			[[nodiscard]] static constexpr high_order_type get_high_order_value(::WPARAM param) noexcept
			{
				return static_cast<high_order_type>(HIWORD(param));
			}

			[[nodiscard]] static constexpr std::pair<low_order_type, high_order_type> get_value(::WPARAM param) noexcept
			{
				return {get_low_order_value(param),
						get_high_order_value(param)};
			}
		};

		template<>
		struct l_params<message::wm_menu_char>
			: public details::single_type<::HWND, ::LPARAM> {};

		template<>
		struct w_params<message::wm_menu_select>
		{
			using low_order_type = std::size_t;

			enum class high_order_type : std::uint32_t
			{
				bitmap = MF_BITMAP,
				checked = MF_CHECKED,
				disabled = MF_DISABLED,
				grayed = MF_GRAYED,
				highlight = MF_HILITE,
				mouse_select = MF_MOUSESELECT,
				owner_draw = MF_OWNERDRAW,
				popup = MF_POPUP,
				system_menu = MF_SYSMENU
			};

			[[nodiscard]] static constexpr low_order_type get_low_order_value(::WPARAM param) noexcept
			{
				return static_cast<low_order_type>(LOWORD(param));
			}

			[[nodiscard]] static constexpr high_order_type get_high_order_value(::WPARAM param) noexcept
			{
				return static_cast<high_order_type>(HIWORD(param));
			}

			[[nodiscard]] static constexpr std::pair<low_order_type, high_order_type> get_value(::WPARAM param) noexcept
			{
				return {get_low_order_value(param),
						get_high_order_value(param)};
			}
		};

		template<>
		struct l_params<message::wm_menu_select>
			: public details::single_type<::HWND, ::LPARAM> {};

		template<>
		struct w_params<message::wm_system_char>
			: public details::single_type<std::uint32_t, ::WPARAM> {};

		template<>
		struct l_params<message::wm_system_char>
		{
			using type = keystroke_message;

			[[nodiscard]] static type get_value(::LPARAM param) noexcept
			{
				return *reinterpret_cast<type*>(std::addressof(param));
			}
		};

		template<>
		struct w_params<message::wm_system_command>
		{
			enum class type : std::uint32_t
			{
				close = SC_CLOSE,
				context_help = SC_CONTEXTHELP,
				sc_default = SC_DEFAULT,
				hotkey = SC_HOTKEY,
				horizontal_scroll = SC_HSCROLL,
				is_secure = SCF_ISSECURE,
				key_menu = SC_KEYMENU,
				maximize = SC_MAXIMIZE,
				minimize = SC_MINIMIZE,
				monitor_power = SC_MONITORPOWER,
				mouse_menu = SC_MOUSEMENU,
				move = SC_MOVE,
				next_window = SC_NEXTWINDOW,
				previous_window = SC_PREVWINDOW,
				restore = SC_RESTORE,
				screensave = SC_SCREENSAVE,
				size = SC_SIZE,
				task_list = SC_TASKLIST,
				vertical_scroll = SC_VSCROLL
			};

			[[nodiscard]] static constexpr type get_value(::WPARAM param) noexcept
			{
				return static_cast<type>(param);
			}
		};

		template<>
		struct l_params<message::wm_system_command>
			: public details::double_type<std::uint32_t, std::uint32_t, ::LPARAM> {};

		template<>
		struct w_params<message::wm_set_hotkey>
		{
			using low_order_type = std::uint32_t;

			enum class high_order_type : std::uint32_t
			{
				alt = HOTKEYF_ALT,
				control = HOTKEYF_CONTROL,
				extended = HOTKEYF_EXT,
				shift = HOTKEYF_SHIFT
			};

			[[nodiscard]] static constexpr low_order_type get_low_order_value(::WPARAM param) noexcept
			{
				return static_cast<low_order_type>(LOWORD(param));
			}

			[[nodiscard]] static constexpr high_order_type get_high_order_value(::WPARAM param) noexcept
			{
				return static_cast<high_order_type>(HIWORD(param));
			}

			[[nodiscard]] static constexpr std::pair<low_order_type, high_order_type> get_value(::WPARAM param) noexcept
			{
				return {get_low_order_value(param),
						get_high_order_value(param)};
			}
		};

		template<>
		struct w_params<message::wm_activate>
		{
			using high_order_type = bool;

			enum class low_order_type : std::uint8_t
			{
				active = WA_ACTIVE,
				click_active = WA_CLICKACTIVE,
				inactive = WA_INACTIVE
			};

			[[nodiscard]] static constexpr low_order_type get_low_order_value(::WPARAM param) noexcept
			{
				return static_cast<low_order_type>(LOWORD(param));
			}

			[[nodiscard]] static constexpr high_order_type get_high_order_value(::WPARAM param) noexcept
			{
				return static_cast<high_order_type>(HIWORD(param));
			}

			[[nodiscard]] static constexpr std::pair<low_order_type, high_order_type> get_value(::WPARAM param) noexcept
			{
				return {get_low_order_value(param),
						get_high_order_value(param)};
			}
		};

		template<>
		struct l_params<message::wm_activate>
			: public details::single_type<::HWND, ::LPARAM> {};

		template<>
		struct w_params<message::wm_app_command>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_app_command>
		{
			struct app_command
			{
				enum class cmd_value
				{
					bass_boost = APPCOMMAND_BASS_BOOST,
					bass_down = APPCOMMAND_BASS_DOWN,
					bass_up = APPCOMMAND_BASS_UP,
					browser_backward = APPCOMMAND_BROWSER_BACKWARD,
					browser_favorites = APPCOMMAND_BROWSER_FAVORITES,
					browser_forward = APPCOMMAND_BROWSER_FORWARD,
					browser_home = APPCOMMAND_BROWSER_HOME,
					browser_refresh = APPCOMMAND_BROWSER_REFRESH,
					browser_search = APPCOMMAND_BROWSER_SEARCH,
					browser_stop = APPCOMMAND_BROWSER_STOP,
					close = APPCOMMAND_CLOSE,
					copy = APPCOMMAND_COPY,
					correction_list = APPCOMMAND_CORRECTION_LIST,
					cut = APPCOMMAND_CUT,
					dictate_or_command_control_toggle = APPCOMMAND_DICTATE_OR_COMMAND_CONTROL_TOGGLE,
					find = APPCOMMAND_FIND,
					forward_mail = APPCOMMAND_FORWARD_MAIL,
					help = APPCOMMAND_HELP,
					launch_app1 = APPCOMMAND_LAUNCH_APP1,
					launch_app2 = APPCOMMAND_LAUNCH_APP2,
					launch_mail = APPCOMMAND_LAUNCH_MAIL,
					launch_media_select = APPCOMMAND_LAUNCH_MEDIA_SELECT,
					media_channel_down = APPCOMMAND_MEDIA_CHANNEL_DOWN,
					media_channel_up = APPCOMMAND_MEDIA_CHANNEL_UP,
					media_fast_forward = APPCOMMAND_MEDIA_FAST_FORWARD,
					media_next_track = APPCOMMAND_MEDIA_NEXTTRACK,
					media_pause = APPCOMMAND_MEDIA_PAUSE,
					media_play = APPCOMMAND_MEDIA_PLAY,
					media_play_pause = APPCOMMAND_MEDIA_PLAY_PAUSE,
					media_previous_track = APPCOMMAND_MEDIA_PREVIOUSTRACK,
					media_record = APPCOMMAND_MEDIA_RECORD,
					media_rewind = APPCOMMAND_MEDIA_REWIND,
					media_stop = APPCOMMAND_MEDIA_STOP,
					mic_toggle = APPCOMMAND_MIC_ON_OFF_TOGGLE,
					mic_volume_down = APPCOMMAND_MICROPHONE_VOLUME_DOWN,
					mic_volume_mute = APPCOMMAND_MICROPHONE_VOLUME_MUTE,
					mic_volume_up = APPCOMMAND_MICROPHONE_VOLUME_UP,
					ap_new = APPCOMMAND_NEW,
					open = APPCOMMAND_OPEN,
					paste = APPCOMMAND_PASTE,
					print = APPCOMMAND_PRINT,
					redo = APPCOMMAND_REDO,
					reply_to_mail = APPCOMMAND_REPLY_TO_MAIL,
					spell_check = APPCOMMAND_SPELL_CHECK,
					treble_down = APPCOMMAND_TREBLE_DOWN,
					treble_up = APPCOMMAND_TREBLE_UP,
					undo = APPCOMMAND_UNDO,
					volume_down = APPCOMMAND_VOLUME_DOWN,
					volume_mute = APPCOMMAND_VOLUME_MUTE,
					volume_up = APPCOMMAND_VOLUME_UP
				} cmd;

				enum class device_value : std::uint32_t
				{
					key = FAPPCOMMAND_KEY,
					mouse = FAPPCOMMAND_MOUSE,
					oem = FAPPCOMMAND_OEM
				} device;

				mouse_keyboard keys;
			};

			using type = app_command;

			[[nodiscard]] static constexpr type get_value(::LPARAM param) noexcept
			{
				const auto cmd = GET_APPCOMMAND_LPARAM(param);
				const auto device = GET_DEVICE_LPARAM(param);
				const auto keys = GET_KEYSTATE_LPARAM(param);

				return {.cmd = static_cast<app_command::cmd_value>(cmd),
						.device = static_cast<app_command::device_value>(device),
						.keys = static_cast<mouse_keyboard>(keys)};
			}
		};

		template<>
		struct w_params<message::wm_char>
			: public details::single_type<std::uint32_t, ::WPARAM> {};

		template<>
		struct l_params<message::wm_char>
		{
			using type = keystroke_message;

			[[nodiscard]] static type get_value(::LPARAM param) noexcept
			{
				return *reinterpret_cast<type*>(std::addressof(param));
			}
		};

		template<>
		struct w_params<message::wm_dead_char>
			: public details::single_type<std::uint32_t, ::WPARAM> {};

		template<>
		struct l_params<message::wm_dead_char>
		{
			using type = keystroke_message;

			[[nodiscard]] static type get_value(::LPARAM param) noexcept
			{
				return *reinterpret_cast<type*>(std::addressof(param));
			}
		};

		template<>
		struct w_params<message::wm_hotkey>
			: public details::single_type<int, ::WPARAM> {};

		template<>
		struct l_params<message::wm_hotkey>
		{
			using high_order_type = std::uint32_t;

			enum class low_order_type : std::uint32_t
			{
				alt = MOD_ALT,
				control = MOD_CONTROL,
				shift = MOD_SHIFT,
				win = MOD_WIN
			};

			[[nodiscard]] static constexpr low_order_type get_low_order_value(::LPARAM param) noexcept
			{
				return static_cast<low_order_type>(param);
			}

			[[nodiscard]] static constexpr high_order_type get_high_order_value(::LPARAM param) noexcept
			{
				return static_cast<high_order_type>(param);
			}

			[[nodiscard]] static constexpr std::pair<low_order_type, high_order_type> get_value(::LPARAM param) noexcept
			{
				return {get_low_order_value(param),
						get_high_order_value(param)};
			}
		};

		template<>
		struct w_params<message::wm_key_down>
			: public details::single_type<virtual_keys, ::WPARAM> {};

		template<>
		struct l_params<message::wm_key_down>
		{
			using type = keystroke_message;

			[[nodiscard]] static type get_value(::LPARAM param) noexcept
			{
				return *reinterpret_cast<type*>(std::addressof(param));
			}
		};

		template<>
		struct w_params<message::wm_key_up>
			: public details::single_type<virtual_keys, ::WPARAM> {};

		template<>
		struct l_params<message::wm_key_up>
		{
			using type = keystroke_message;

			[[nodiscard]] static type get_value(::LPARAM param) noexcept
			{
				return *reinterpret_cast<type*>(std::addressof(param));
			}
		};

		template<>
		struct w_params<message::wm_kill_focus>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct w_params<message::wm_set_focus>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct w_params<message::wm_system_dead_char>
			: public details::single_type<std::uint32_t, ::WPARAM> {};

		template<>
		struct l_params<message::wm_system_dead_char>
		{
			using type = keystroke_message;

			[[nodiscard]] static type get_value(::LPARAM param) noexcept
			{
				return *reinterpret_cast<type*>(std::addressof(param));
			}
		};

		template<>
		struct w_params<message::wm_system_key_down>
			: public details::single_type<virtual_keys, ::WPARAM> {};

		template<>
		struct l_params<message::wm_system_key_down>
		{
			using type = keystroke_message;

			[[nodiscard]] static type get_value(::LPARAM param) noexcept
			{
				return *reinterpret_cast<type*>(std::addressof(param));
			}
		};

		template<>
		struct w_params<message::wm_system_key_up>
			: public details::single_type<virtual_keys, ::WPARAM> {};

		template<>
		struct l_params<message::wm_system_key_up>
		{
			using type = keystroke_message;

			[[nodiscard]] static type get_value(::LPARAM param) noexcept
			{
				return *reinterpret_cast<type*>(std::addressof(param));
			}
		};

		template<>
		struct w_params<message::wm_unicode_char>
			: public details::single_type<std::uint32_t, ::WPARAM> {};

		template<>
		struct l_params<message::wm_unicode_char>
		{
			using type = keystroke_message;

			[[nodiscard]] static type get_value(::LPARAM param) noexcept
			{
				return *reinterpret_cast<type*>(std::addressof(param));
			}
		};

		template<>
		struct w_params<message::wm_command>
			: public details::double_type<decltype(LOWORD(std::declval<::WPARAM>())), std::int32_t, ::WPARAM> {};

		template<>
		struct l_params<message::wm_command>
			: public details::single_type<::LPARAM, ::LPARAM> {};

		template<>
		struct w_params<message::wm_context_menu>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_context_menu>
			: public details::double_type<std::uint32_t, std::uint32_t, ::LPARAM> {};

		template<>
		struct w_params<message::wm_enter_menu_loop>
			: public details::single_type<bool, ::WPARAM> {};

		template<>
		struct w_params<message::wm_exit_menu_loop>
			: public details::single_type<bool, ::WPARAM> {};

		template<>
		struct l_params<message::wm_get_title_bar_info>
			: public details::single_type<::TITLEBARINFOEX*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_menu_command>
			: public details::single_type<std::size_t, ::WPARAM> {};

		template<>
		struct l_params<message::wm_menu_command>
			: public details::single_type<::HWND, ::LPARAM> {};

		template<>
		struct w_params<message::wm_menu_drag>
			: public details::single_type<std::uint32_t, ::WPARAM> {};

		template<>
		struct l_params<message::wm_menu_drag>
			: public details::single_type<::HWND, ::LPARAM> {};

		template<>
		struct l_params<message::wm_menu_get_object>
			: public details::single_type<::MENUGETOBJECTINFO*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_menu_right_button_up>
			: public details::single_type<std::size_t, ::WPARAM> {};

		template<>
		struct l_params<message::wm_menu_right_button_up>
			: public details::single_type<::HWND, ::LPARAM> {};

		template<>
		struct w_params<message::wm_next_menu>
			: public details::single_type<virtual_keys, ::WPARAM> {};

		template<>
		struct l_params<message::wm_next_menu>
			: public details::single_type<::MDINEXTMENU*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_uninitialize_menu_popup>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_uninitialize_menu_popup>
		{
			using type = std::uint32_t;

			[[nodiscard]] static constexpr type get_value(::LPARAM param) noexcept
			{
				return static_cast<type>(HIWORD(param));
			}
		};

		template<>
		struct l_params<message::wm_capture_changed>
			: public details::single_type<::HWND, ::LPARAM> {};

		template<>
		struct w_params<message::wm_left_mouse_button_double_click>
			: public details::single_type<mouse_keyboard, ::WPARAM> {};

		template<>
		struct l_params<message::wm_left_mouse_button_double_click>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_left_mouse_button_down>
			: public details::single_type<mouse_keyboard, ::WPARAM> {};

		template<>
		struct l_params<message::wm_left_mouse_button_down>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_left_mouse_button_up>
			: public details::single_type<mouse_keyboard, ::WPARAM> {};

		template<>
		struct l_params<message::wm_left_mouse_button_up>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_middle_mouse_button_double_click>
			: public details::single_type<mouse_keyboard, ::WPARAM> {};

		template<>
		struct l_params<message::wm_middle_mouse_button_double_click>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_middle_mouse_button_down>
			: public details::single_type<mouse_keyboard, ::WPARAM> {};

		template<>
		struct l_params<message::wm_middle_mouse_button_down>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_middle_mouse_button_up>
			: public details::single_type<mouse_keyboard, ::WPARAM> {};

		template<>
		struct l_params<message::wm_middle_mouse_button_up>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_mouse_activate>
			: public details::single_type<::HWND, ::WPARAM> {};

		template<>
		struct l_params<message::wm_mouse_activate>
			: public details::double_type<hit_value, decltype(HIWORD(std::declval<::LPARAM>())), ::LPARAM> {};

		template<>
		struct w_params<message::wm_mouse_hover>
			: public details::single_type<mouse_keyboard, ::WPARAM> {};

		template<>
		struct l_params<message::wm_mouse_hover>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_mouse_horizontal_wheel>
			: public details::double_type<mouse_keyboard, int, ::WPARAM> {};

		template<>
		struct l_params<message::wm_mouse_horizontal_wheel>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_mouse_move>
			: public details::single_type<mouse_keyboard, ::WPARAM> {};

		template<>
		struct l_params<message::wm_mouse_move>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_mouse_wheel>
			: public details::double_type<mouse_keyboard, int, ::WPARAM> {};

		template<>
		struct l_params<message::wm_mouse_wheel>
			: public details::xy_coordinate {};

		template<>
		struct l_params<message::wm_non_client_hit_test>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_non_client_left_button_double_click>
			: public details::single_type<hit_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_non_client_left_button_double_click>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_non_client_left_button_down>
			: public details::single_type<hit_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_non_client_left_button_down>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_non_client_left_button_up>
			: public details::single_type<hit_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_non_client_left_button_up>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_non_client_middle_button_double_click>
			: public details::single_type<hit_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_non_client_middle_button_double_click>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_non_client_middle_button_down>
			: public details::single_type<hit_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_non_client_middle_button_down>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_non_client_middle_button_up>
			: public details::single_type<hit_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_non_client_middle_button_up>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_non_client_mouse_hover>
			: public details::single_type<hit_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_non_client_mouse_hover>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_non_client_mouse_move>
			: public details::single_type<hit_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_non_client_mouse_move>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_non_client_right_button_double_click>
			: public details::single_type<hit_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_non_client_right_button_double_click>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_non_client_right_button_down>
			: public details::single_type<hit_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_non_client_right_button_down>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_non_client_right_button_up>
			: public details::single_type<hit_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_non_client_right_button_up>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_non_client_x_button_double_click>
			: public details::single_type<xbutton_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_non_client_x_button_double_click>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_non_client_x_button_down>
			: public details::single_type<xbutton_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_non_client_x_button_down>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_non_client_x_button_up>
			: public details::single_type<xbutton_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_non_client_x_button_up>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_right_button_double_click>
			: public details::single_type<hit_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_right_button_double_click>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_right_button_down>
			: public details::single_type<hit_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_right_button_down>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_right_button_up>
			: public details::single_type<hit_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_right_button_up>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_x_button_double_click>
			: public details::double_type<hit_value, xbutton_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_x_button_double_click>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_x_button_down>
			: public details::double_type<hit_value, xbutton_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_x_button_down>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_x_button_up>
			: public details::double_type<hit_value, xbutton_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_x_button_up>
			: public details::xy_coordinate {};

		template<>
		struct w_params<message::wm_mdi_activate>
			: public details::window_handle<::WPARAM> {};

		template<>
		struct w_params<message::wm_mdi_cascade>
			: public details::single_type<multiple_document_cascade_value, ::WPARAM> {};

		template<>
		struct l_params<message::wm_mdi_create>
			: public details::single_type<::MDICREATESTRUCT*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_mdi_destroy>
			: public details::window_handle<::WPARAM> {};

		template<>
		struct l_params<message::wm_mdi_get_active>
			: public details::single_type<bool, ::LPARAM> {};

		template<>
		struct w_params<message::wm_mdi_maximize>
			: public details::window_handle<::WPARAM> {};

		template<>
		struct w_params<message::wm_mdi_next>
			: public details::window_handle<::WPARAM> {};

		template<>
		struct l_params<message::wm_mdi_next>
			: public details::single_type<bool, ::LPARAM> {};

		template<>
		struct w_params<message::wm_mdi_restore>
			: public details::window_handle<::WPARAM> {};

		template<>
		struct w_params<message::wm_mdi_set_menu>
			: public details::window_handle<::WPARAM> {};

		template<>
		struct l_params<message::wm_mdi_set_menu>
			: public details::window_handle<::LPARAM> {};

		template<>
		struct w_params<message::wm_mdi_tile>
			: public details::single_type<multiple_document_tiling_value, ::WPARAM> {};

		template<>
		struct w_params<message::wm_input>
		{
			using type = raw_input_code_value;

			[[nodiscard]] static constexpr type get_value(::WPARAM param) noexcept
			{
				return static_cast<type>(GET_RAWINPUT_CODE_WPARAM(param));
			}
		};

		template<>
		struct l_params<message::wm_input>
			: public details::single_type<::HRAWINPUT, ::LPARAM> {};

		template<>
		struct w_params<message::wm_input_device_change>
			: public details::single_type<register_device_status, ::WPARAM> {};

		template<>
		struct l_params<message::wm_input_device_change>
			: public details::single_type<::HRAWINPUT, ::LPARAM> {};

		// TODO: Figure out what types the Microsoft Docs are referring to.
		/*template<>
		struct w_params<message::wm_control_color_scrollbar> {};

		template<>
		struct l_params<message::wm_control_color_scrollbar> {};*/

		template<>
		struct w_params<message::wm_horizontal_scroll>
			: public details::double_type<scroll_bar_command, int, ::WPARAM> {};

		// TODO: Figure out what type the Microsoft Docs is referring to.
		/*template<>
		struct l_params<message::wm_horizontal_scroll> {};*/

		template<>
		struct w_params<message::wm_vertical_scroll>
			: public details::double_type<scroll_bar_command, int, ::WPARAM> {};

		// TODO: Figure out what type the Microsoft Docs is referring to.
		/*template<>
		struct l_params<message::wm_vertical_scroll> {};*/

		template<>
		struct w_params<message::wm_timer>
			: public details::single_type<::HANDLE, ::WPARAM> {};

		template<>
		struct l_params<message::wm_timer>
			: public details::single_type<void*, ::LPARAM> {};

		// TODO: Figure out what type the Microsoft Docs is referring to.
		/*template<>
		struct w_params<messgae::wm_erase_background> {};*/

		template<>
		struct w_params<message::wm_get_text>
			: public details::single_type<std::size_t, ::WPARAM> {};

		template<>
		struct l_params<message::wm_get_text>
			: public details::single_type<char*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_set_font>
			: public details::single_type<::HFONT, ::WPARAM> {};

		template<>
		struct l_params<message::wm_set_font>
		{
			using type = bool;

			[[nodiscard]] static constexpr type get_value(::LPARAM param) noexcept
			{
				return static_cast<type>(LOWORD(param));
			}
		};

		template<>
		struct w_params<message::wm_set_icon>
			: public details::single_type<icon_size, ::WPARAM> {};

		template<>
		struct l_params<message::wm_set_icon>
			: public details::single_type<::HICON, ::LPARAM> {};

		template<>
		struct l_params<message::wm_set_text>
			: public details::single_type<char*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_activate_app>
			: public details::single_type<bool, ::WPARAM> {};

		template<>
		struct l_params<message::wm_activate_app>
			: public details::single_type<bool, ::LPARAM> {};

		template<>
		struct w_params<message::wm_compacting>
			: public details::single_type<std::uint16_t, ::WPARAM> {};

		template<>
		struct l_params<message::wm_create>
			: public details::single_type<::CREATESTRUCT*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_dpi_changed>
			: public details::double_type<int, int, ::WPARAM> {};

		template<>
		struct l_params<message::wm_dpi_changed>
			: public details::single_type<::RECT*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_enable>
			: public details::single_type<bool, ::WPARAM> {};

		template<>
		struct w_params<message::wm_get_icon>
			: public details::single_type<icon_size, ::WPARAM> {};

		template<>
		struct l_params<message::wm_get_icon>
			: public details::single_type<int, ::LPARAM> {};

		template<>
		struct l_params<message::wm_get_min_max_info>
			: public details::single_type<::MINMAXINFO*, ::LPARAM> {};

		// TODO
		/*template<>
		struct w_params<message::wm_input_language_change> {};

		template<>
		struct l_params<message::wm_input_language_change> {};*/

		template<>
		struct w_params<message::wm_input_language_change_request>
			: public details::single_type<input_language_change, ::WPARAM> {};

		// TODO
		/*template<>
		struct l_params<message::wm_input_language_change_request> {};*/

		template<>
		struct l_params<message::wm_move>
			: public details::xy_coordinate {};

		template<>
		struct l_params<message::wm_moving>
			: public details::single_type<::RECT*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_non_client_activate>
			: public details::single_type<bool, ::WPARAM> {};

		template<>
		struct l_params<message::wm_non_client_activate>
			: public details::window_handle<::LPARAM> {};

		template<>
		struct w_params<message::wm_non_client_calculate_size>
			: public details::single_type<bool, ::WPARAM> {};

		template<>
		struct l_params<message::wm_non_client_calculate_size>
			: public details::single_type<::LPARAM, ::LPARAM> {};

		template<>
		struct l_params<message::wm_non_client_create>
			: public details::single_type<::CREATESTRUCT*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_quit>
			: public details::single_type<int, ::WPARAM> {};

		template<>
		struct w_params<message::wm_show_window>
			: public details::single_type<bool, ::WPARAM> {};

		template<>
		struct l_params<message::wm_show_window>
			: public details::single_type<show_window, ::LPARAM> {};

		template<>
		struct w_params<message::wm_size>
			: public details::single_type<size_request, ::WPARAM> {};

		template<>
		struct l_params<message::wm_size>
		{
			using low_order_type = std::uint32_t;
			using high_order_type = std::uint32_t;

			[[nodiscard]] static constexpr low_order_type get_low_order_value(::LPARAM param) noexcept
			{
				return static_cast<low_order_type>(LOWORD(param));
			}

			[[nodiscard]] static constexpr high_order_type get_high_order_value(::LPARAM param) noexcept
			{
				return static_cast<high_order_type>(HIWORD(param));
			}

			[[nodiscard]] static constexpr ::D2D1_SIZE_U get_value(::LPARAM param) noexcept
			{
				return {.width = get_low_order_value(param),
						.height = get_high_order_value(param)};
			}
		};

		template<>
		struct w_params<message::wm_sizing>
			: public details::single_type<window_sizing, ::WPARAM> {};

		template<>
		struct l_params<message::wm_sizing>
			: public details::single_type<::RECT*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_style_changed>
			: public details::single_type<style_change, ::WPARAM> {};

		template<>
		struct l_params<message::wm_style_changed>
			: public details::single_type<const ::STYLESTRUCT*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_style_changing>
			: public details::single_type<style_change, ::WPARAM> {};

		template<>
		struct l_params<message::wm_style_changing>
			: public details::single_type<const ::STYLESTRUCT*, ::LPARAM> {};

		template<>
		struct l_params<message::wm_window_position_changed>
			: public details::single_type<const ::WINDOWPOS*, ::LPARAM> {};

		template<>
		struct l_params<message::wm_window_position_changing>
			: public details::single_type<const ::WINDOWPOS*, ::LPARAM> {};

		template<>
		struct w_params<message::wm_display_change>
			: public details::single_type<std::uint32_t, ::WPARAM> {};

		template<>
		struct l_params<message::wm_display_change>
			: public details::double_type<std::uint32_t, std::uint32_t, ::LPARAM> {};

		namespace details
		{
			template<typename T>
			concept parameter_type = std::same_as<T, ::LPARAM> || std::same_as<T, ::WPARAM>;

			template<message Message>
			concept has_l_params = 
				requires
			{
				typename l_params<Message>;
			};

			template<message Message>
			concept has_w_params =
				requires
			{
				typename w_params<Message>;
			};

			template<message Message>
			concept has_l_params_get_value = has_l_params<Message> &&
				requires(::LPARAM p)
			{
				l_params<Message>::get_value(p);
			};

			template<message Message>
			concept has_w_params_get_value = has_w_params<Message> &&
				requires(::LPARAM p)
			{
				w_params<Message>::get_value(p);
			};

			template<message Message>
			concept has_l_params_get_low_order_value = has_l_params<Message> &&
				requires(::LPARAM p)
			{
				l_params<Message>::get_low_order_value(p);
			};

			template<message Message>
			concept has_w_params_get_low_order_value = has_w_params<Message> &&
				requires(::LPARAM p)
			{
				w_params<Message>::get_low_order_value(p);
			};

			template<message Message>
			concept has_l_params_get_high_order_value = has_l_params<Message> &&
				requires(::LPARAM p)
			{
				l_params<Message>::get_high_order_value(p);
			};

			template<message Message>
			concept has_w_params_get_high_order_value = has_w_params<Message> &&
				requires(::LPARAM p)
			{
				w_params<Message>::get_high_order_value(p);
			};
		}

		template<message Message, std::same_as<::LPARAM> ParamType>
			requires(details::has_l_params_get_value<Message>)
		[[nodiscard]] constexpr auto get_value(ParamType param) noexcept(noexcept(l_params<Message>::get_value(std::declval<::LPARAM>())))
		{
			return l_params<Message>::get_value(param);
		}

		template<message Message, std::same_as<::WPARAM> ParamType>
			requires(details::has_w_params_get_value<Message>)
		[[nodiscard]] constexpr auto get_value(ParamType param) noexcept(noexcept(w_params<Message>::get_value(std::declval<::WPARAM>())))
		{
			return w_params<Message>::get_value(param);
		}

		template<message Message, std::same_as<::LPARAM> ParamType>
			requires(details::has_l_params_get_low_order_value<Message>)
		[[nodiscard]] constexpr auto get_low_order_value(ParamType param) noexcept(noexcept(l_params<Message>::get_low_order_value(std::declval<::LPARAM>())))
		{
			return l_params<Message>::get_low_order_value(param);
		}

		template<message Message, std::same_as<::WPARAM> ParamType>
			requires(details::has_w_params_get_low_order_value<Message>)
		[[nodiscard]] constexpr auto get_low_order_value(ParamType param) noexcept(noexcept(w_params<Message>::get_low_order_value(std::declval<::WPARAM>())))
		{
			return w_params<Message>::get_low_order_value(param);
		}

		template<message Message, std::same_as<::LPARAM> ParamType>
			requires(details::has_l_params_get_high_order_value<Message>)
		[[nodiscard]] constexpr auto get_high_order_value(ParamType param) noexcept(noexcept(l_params<Message>::get_high_order_value(std::declval<::LPARAM>())))
		{
			return l_params<Message>::get_high_order_value(param);
		}

		template<message Message, std::same_as<::WPARAM> ParamType>
			requires(details::has_w_params_get_high_order_value<Message>)
		[[nodiscard]] constexpr auto get_high_order_value(ParamType param) noexcept(noexcept(w_params<Message>::get_high_order_value(std::declval<::WPARAM>())))
		{
			return w_params<Message>::get_high_order_value(param);
		}
	}
}