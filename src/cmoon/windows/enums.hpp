#pragma once

#include <cstdint>

#include <windows.h>

namespace cmoon
{
	namespace windows
	{
		enum class app_command
		{
			browser_backward = APPCOMMAND_BROWSER_BACKWARD,
			browser_forward = APPCOMMAND_BROWSER_FORWARD,
			browser_refresh = APPCOMMAND_BROWSER_REFRESH,
			browser_stop = APPCOMMAND_BROWSER_STOP,
			browser_search = APPCOMMAND_BROWSER_SEARCH,
			browser_favorites = APPCOMMAND_BROWSER_FAVORITES,
			browser_home = APPCOMMAND_BROWSER_HOME,
			volume_mute = APPCOMMAND_VOLUME_MUTE,
			volume_down = APPCOMMAND_VOLUME_DOWN,
			volume_up = APPCOMMAND_VOLUME_UP,
			media_nexttrack = APPCOMMAND_MEDIA_NEXTTRACK,
			media_previoustrack = APPCOMMAND_MEDIA_PREVIOUSTRACK,
			media_stop = APPCOMMAND_MEDIA_STOP,
			media_play_pause = APPCOMMAND_MEDIA_PLAY_PAUSE,
			launch_mail = APPCOMMAND_LAUNCH_MAIL,
			launch_media_select = APPCOMMAND_LAUNCH_MEDIA_SELECT,
			launch_app1 = APPCOMMAND_LAUNCH_APP1,
			launch_app2 = APPCOMMAND_LAUNCH_APP2,
			bass_down = APPCOMMAND_BASS_DOWN,
			bass_boost = APPCOMMAND_BASS_BOOST,
			bass_up = APPCOMMAND_BASS_UP,
			treble_down = APPCOMMAND_TREBLE_DOWN,
			treble_up = APPCOMMAND_TREBLE_UP,
			microphone_volume_mute = APPCOMMAND_MICROPHONE_VOLUME_MUTE,
			microphone_volume_down = APPCOMMAND_MICROPHONE_VOLUME_DOWN,
			microphone_volume_up = APPCOMMAND_MICROPHONE_VOLUME_UP,
			help = APPCOMMAND_HELP,
			find = APPCOMMAND_FIND,
			ap_new = APPCOMMAND_NEW,
			open = APPCOMMAND_OPEN,
			close = APPCOMMAND_CLOSE,
			save = APPCOMMAND_SAVE,
			print = APPCOMMAND_PRINT,
			undo = APPCOMMAND_UNDO,
			redo = APPCOMMAND_REDO,
			copy = APPCOMMAND_COPY,
			cut = APPCOMMAND_CUT,
			paste = APPCOMMAND_PASTE,
			reply_to_mail = APPCOMMAND_REPLY_TO_MAIL,
			forward_mail = APPCOMMAND_FORWARD_MAIL,
			send_mail = APPCOMMAND_SEND_MAIL,
			spell_check = APPCOMMAND_SPELL_CHECK,
			dictate_or_command_control_toggle = APPCOMMAND_DICTATE_OR_COMMAND_CONTROL_TOGGLE,
			mic_on_off_toggle = APPCOMMAND_MIC_ON_OFF_TOGGLE,
			correction_list = APPCOMMAND_CORRECTION_LIST,
			media_play = APPCOMMAND_MEDIA_PLAY,
			media_pause = APPCOMMAND_MEDIA_PAUSE,
			media_record = APPCOMMAND_MEDIA_RECORD,
			media_fast_forward = APPCOMMAND_MEDIA_FAST_FORWARD,
			media_rewind = APPCOMMAND_MEDIA_REWIND,
			media_channel_up = APPCOMMAND_MEDIA_CHANNEL_UP,
			media_channel_down = APPCOMMAND_MEDIA_CHANNEL_DOWN,
			ap_delete = APPCOMMAND_DELETE,
			flip3d = APPCOMMAND_DWM_FLIP3D,
		};

		enum class scroll_bar_constant
		{
			horizontal = SB_HORZ,
			vertical = SB_VERT,
			control = SB_CTL,
			both = SB_BOTH
		};

		enum class scroll_bar_command
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

		enum class show_window_command
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
		};

		enum class show_window
		{
			other_unzoom = SW_OTHERUNZOOM,
			other_zoom = SW_OTHERZOOM,
			parent_closing = SW_PARENTCLOSING,
			parent_opening = SW_PARENTOPENING
		};

		enum class size_request
		{
			maximize_hide = SIZE_MAXHIDE,
			maximized = SIZE_MAXIMIZED,
			maximize_show = SIZE_MAXSHOW,
			minimized = SIZE_MINIMIZED,
			restored = SIZE_RESTORED
		};

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

		enum class style_change
		{
			exstyle = GWL_EXSTYLE,
			style = GWL_STYLE
		};

		enum class virtual_keys : std::uint32_t
		{
			left_mouse_button = VK_LBUTTON,
			right_mouse_button = VK_RBUTTON,
			cancel = VK_CANCEL,
			middle_mouse_button = VK_MBUTTON,
			x1_mouse_button = VK_XBUTTON1,
			x2_mouse_button = VK_XBUTTON2,
			backspace_key = VK_BACK,
			tab_key = VK_TAB,
			clear_key = VK_CLEAR,
			enter_key = VK_RETURN,
			shift_key = VK_SHIFT,
			control_key = VK_CONTROL,
			alt_key = VK_MENU,
			pause_key = VK_PAUSE,
			caps_lock_key = VK_CAPITAL,
			kana_mode = VK_KANA,
			hangul_mode = VK_HANGUL,
			junja_mode = VK_JUNJA,
			final_mode = VK_FINAL,
			hanja_mode = VK_HANJA,
			kanji_mode = VK_KANJI,
			escape_key = VK_ESCAPE,
			convert_mode = VK_CONVERT,
			nonconvert_mode = VK_NONCONVERT,
			accept_mode = VK_ACCEPT,
			mode_change = VK_MODECHANGE,
			space_key = VK_SPACE,
			page_up_key = VK_PRIOR,
			page_down_key = VK_NEXT,
			end_key = VK_END,
			home_key = VK_HOME,
			left_arrow_key = VK_LEFT,
			up_arrow_key = VK_UP,
			right_arrow_key = VK_RIGHT,
			down_arrow_key = VK_DOWN,
			select_key = VK_SELECT,
			print_key = VK_PRINT,
			execute_key = VK_EXECUTE,
			print_screen_key = VK_SNAPSHOT,
			insert_key = VK_INSERT,
			delete_key = VK_DELETE,
			help_key = VK_HELP,
			n_0_key = 0x30,
			n_1_key = 0x31,
			n_2_key = 0x32,
			n_3_key = 0x33,
			n_4_key = 0x34,
			n_5_key = 0x35,
			n_6_key = 0x36,
			n_7_key = 0x37,
			n_8_key = 0x38,
			n_9_key = 0x39,
			A_key = 0x41,
			B_key = 0x42,
			C_key = 0x43,
			D_key = 0x44,
			E_key = 0x45,
			F_key = 0x46,
			G_key = 0x47,
			H_key = 0x48,
			I_key = 0x49,
			J_key = 0x4a,
			K_key = 0x4b,
			L_key = 0x4c,
			M_key = 0x4d,
			N_key = 0x4e,
			O_key = 0x4f,
			P_key = 0x50,
			Q_key = 0x51,
			R_key = 0x52,
			S_key = 0x53,
			T_key = 0x54,
			U_key = 0x55,
			V_key = 0x56,
			W_key = 0x57,
			X_key = 0x58,
			Y_key = 0x59,
			Z_key = 0x5a,
			left_windows_key = VK_LWIN,
			right_windows_key = VK_RWIN,
			applications_key = VK_APPS,
			sleep_key = VK_SLEEP,
			numpad_0_key = VK_NUMPAD0,
			numpad_1_key = VK_NUMPAD1,
			numpad_2_key = VK_NUMPAD2,
			numpad_3_key = VK_NUMPAD3,
			numpad_4_key = VK_NUMPAD4,
			numpad_5_key = VK_NUMPAD5,
			numpad_6_key = VK_NUMPAD6,
			numpad_7_key = VK_NUMPAD7,
			numpad_8_key = VK_NUMPAD8,
			numpad_9_key = VK_NUMPAD9,
			multiply_key = VK_MULTIPLY,
			add_key = VK_ADD,
			separator_key = VK_SEPARATOR,
			subtract_key = VK_SUBTRACT,
			decimal_key = VK_DECIMAL,
			divide_key = VK_DIVIDE,
			F1_key = VK_F1,
			F2_key = VK_F2,
			F3_key = VK_F3,
			F4_key = VK_F4,
			F5_key = VK_F5,
			F6_key = VK_F6,
			F7_key = VK_F7,
			F8_key = VK_F8,
			F9_key = VK_F9,
			F10_key = VK_F10,
			F11_key = VK_F11,
			F12_key = VK_F12,
			F13_key = VK_F13,
			F14_key = VK_F14,
			F15_key = VK_F15,
			F16_key = VK_F16,
			F17_key = VK_F17,
			F18_key = VK_F18,
			F19_key = VK_F19,
			F20_key = VK_F20,
			F21_key = VK_F21,
			F22_key = VK_F22,
			F23_key = VK_F23,
			F24_key = VK_F24,
			num_lock_key = VK_NUMLOCK,
			scroll_lock_key = VK_SCROLL,
			left_shift_key = VK_LSHIFT,
			right_shift_key = VK_RSHIFT,
			left_control_key = VK_LCONTROL,
			right_control_key = VK_RCONTROL,
			left_alt_key = VK_LMENU,
			right_alt_key = VK_RMENU,
			browser_back_key = VK_BROWSER_BACK,
			browser_forward_key = VK_BROWSER_FORWARD,
			browser_refresh_key = VK_BROWSER_REFRESH,
			browser_stop_key = VK_BROWSER_STOP,
			browser_favorites_key = VK_BROWSER_FAVORITES,
			browser_home_key = VK_BROWSER_HOME,
			volume_mute_key = VK_VOLUME_MUTE,
			volume_down_key = VK_VOLUME_DOWN,
			volume_up_key = VK_VOLUME_UP,
			media_next_track_key = VK_MEDIA_NEXT_TRACK,
			media_previous_track_key = VK_MEDIA_PREV_TRACK,
			media_stop_key = VK_MEDIA_STOP,
			media_play_pause_key = VK_MEDIA_PLAY_PAUSE,
			launch_mail_key = VK_LAUNCH_MAIL,
			launch_media_key = VK_LAUNCH_MEDIA_SELECT,
			launch_app1_key = VK_LAUNCH_APP1,
			launch_app2_key = VK_LAUNCH_APP2,
			oem_1 = VK_OEM_1,
			oem_plus = VK_OEM_PLUS,
			oem_comma = VK_OEM_COMMA,
			oem_minus = VK_OEM_MINUS,
			oem_period = VK_OEM_PERIOD,
			oem_2 = VK_OEM_2,
			oem_3 = VK_OEM_3,
			oem_4 = VK_OEM_4,
			oem_5 = VK_OEM_5,
			oem_6 = VK_OEM_6,
			oem_7 = VK_OEM_7,
			oem_8 = VK_OEM_8,
			oem_102 = VK_OEM_102,
			process_key = VK_PROCESSKEY,
			packet = VK_PACKET,
			attn_key = VK_ATTN,
			crsel_key = VK_CRSEL,
			exsel_key = VK_EXSEL,
			erase_eof_key = VK_EREOF,
			play_key = VK_PLAY,
			zoom_key = VK_ZOOM,
			noname = VK_NONAME,
			pa1_key = VK_PA1,
			oem_clear = VK_OEM_CLEAR
		};

		enum class mouse_keyboard : std::uint32_t
		{
			left_mouse_button = MK_LBUTTON,
			right_mouse_button = MK_RBUTTON,
			shift = MK_SHIFT,
			control = MK_CONTROL,
			middle_mouse_button = MK_MBUTTON,
			x1_mouse_button = MK_XBUTTON1,
			x2_mouse_button = MK_XBUTTON2,
		};

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

		enum class xbutton_value : unsigned int
		{
			x_button1 = XBUTTON1,
			x_button2 = XBUTTON2
		};

		enum class multiple_document_cascade_value : unsigned int
		{
			tile_skip_disabled = MDITILE_SKIPDISABLED,
			tile_z_order = MDITILE_ZORDER
		};

		enum class multiple_document_tiling_value : unsigned int
		{
			tile_horizontal = MDITILE_HORIZONTAL,
			tile_vertical = MDITILE_VERTICAL
		};

		enum class raw_input_code_value : int
		{
			input = RIM_INPUT,
			input_sink = RIM_INPUTSINK
		};

		enum class register_device_status : int
		{
			arrival = GIDC_ARRIVAL,
			removal = GIDC_REMOVAL
		};

		enum class icon_size : int
		{
			icon_big = ICON_BIG,
			icon_small = ICON_SMALL,
			icon_small2 = ICON_SMALL2
		};
	}
}