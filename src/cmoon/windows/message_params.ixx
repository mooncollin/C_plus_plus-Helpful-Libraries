module;

#include <windows.h>
#include <windowsx.h>
#include <Dbt.h>
#include <CommCtrl.h>

export module cmoon.windows.message_params;

import <cstdint>;
import <type_traits>;

import cmoon.windows.input;
import cmoon.windows.gui.hit_value;
import cmoon.windows.system_message;

namespace cmoon::windows
{
	export
	template<system_message M>
	struct w_params;

	export
	template<system_message M>
	struct l_params;

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

	template<class T, class T2, class ParamsType, bool LowOrderNoexcept = true, bool HighOrderNoexcept = true>
	struct double_type
	{
		using low_order_type = T;
		using high_order_type = T2;

		[[nodiscard]] static constexpr low_order_type get_low_order_value(ParamsType param) noexcept(LowOrderNoexcept)
			requires(!std::is_pointer_v<low_order_type>)
		{
			return static_cast<low_order_type>(LOWORD(param));
		}

		[[nodiscard]] static low_order_type get_low_order_value(ParamsType param) noexcept(LowOrderNoexcept)
			requires(std::is_pointer_v<low_order_type>)
		{
			return reinterpret_cast<low_order_type>(LOWORD(param));
		}

		[[nodiscard]] constexpr static high_order_type get_high_order_value(ParamsType param) noexcept(HighOrderNoexcept)
			requires(!std::is_pointer_v<high_order_type>)
		{
			return static_cast<high_order_type>(HIWORD(param));
		}

		[[nodiscard]] static high_order_type get_high_order_value(ParamsType param) noexcept(HighOrderNoexcept)
			requires(std::is_pointer_v<high_order_type>)
		{
			return reinterpret_cast<high_order_type>(HIWORD(param));
		}

		[[nodiscard]] static constexpr std::pair<low_order_type, high_order_type> get_value(ParamsType param) noexcept(LowOrderNoexcept && HighOrderNoexcept)
			requires(!std::is_pointer_v<low_order_type> && !std::is_pointer_v<high_order_type>)
		{
			return {get_low_order_value(param),
					get_high_order_value(param)};
		}

		[[nodiscard]] static std::pair<low_order_type, high_order_type> get_value(ParamsType param) noexcept(LowOrderNoexcept && HighOrderNoexcept)
		{
			return {get_low_order_value(param),
					get_high_order_value(param)};
		}
	};

	struct xy_coordinate
		: public double_type<int, int, ::LPARAM> {};

	template<class ParamType>
	struct window_handler
		: public single_type<window_handle, ParamType> {};
	
	export
	template<>
	struct w_params<messages::wm_ask_clipboard_format_name>
	: public single_type<std::size_t, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_ask_clipboard_format_name>
		: public single_type<char*, ::LPARAM> {};
	
	export
	template<>
	struct w_params<messages::wm_change_clipboard_chain>
		: public single_type<window_handle, ::WPARAM> {};
	
	export
	template<>
	struct l_params<messages::wm_change_clipboard_chain>
		: public single_type<window_handle, ::LPARAM> {};
	
	export
	template<>
	struct w_params<messages::wm_horizontal_scroll_clipboard>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_horizontal_scroll_clipboard>
	{
		using low_order_type = scroll_bar_command;
		using high_order_type = int;

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

	export
	template<>
	struct w_params<messages::wm_paint_clipboard>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_paint_clipboard>
	{
		using type = global_lock_resource<::PAINTSTRUCT>;

		[[nodiscard]] static type get_value(::LPARAM param) noexcept
		{
			return global_resource(reinterpret_cast<::PAINTSTRUCT*>(param));
		}
	};

	export
	template<>
	struct w_params<messages::wm_render_format>
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

	export
	template<>
	struct w_params<messages::wm_size_clipboard>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_size_clipboard>
	{
		using type = global_lock_resource<::RECT>;

		[[nodiscard]] static type get_value(::LPARAM param) noexcept
		{
			return global_resource(reinterpret_cast<::RECT*>(param));
		}
	};

	export
	template<>
	struct w_params<messages::wm_vertical_scroll_clipboard>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_vertical_scroll_clipboard>
	{
		using low_order_type = scroll_bar_command;
		using high_order_type = int;

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

	export
	template<>
	struct l_params<messages::cdn_file_ok>
		: public single_type<::OFNOTIFY*, ::LPARAM> {};

	export
	template<>
	struct l_params<messages::cdn_folder_change>
		: public single_type<::OFNOTIFY*, ::LPARAM> {};

	export
	template<>
	struct l_params<messages::cdn_help>
		: public single_type<::OFNOTIFY*, ::LPARAM> {};

	export
	template<>
	struct l_params<messages::cdn_include_item>
		: public single_type<::OFNOTIFYEX*, ::LPARAM> {};

	export
	template<>
	struct l_params<messages::cdn_init_done>
		: public single_type<::OFNOTIFY*, ::LPARAM> {};

	export
	template<>
	struct l_params<messages::cdn_sel_change>
		: public single_type<::OFNOTIFY*, ::LPARAM> {};

	export
	template<>
	struct l_params<messages::cdn_share_violation>
		: public single_type<::OFNOTIFY*, ::LPARAM> {};

	export
	template<>
	struct l_params<messages::cdn_type_change>
		: public single_type<::OFNOTIFY*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_psd_env_stamp_rectangle>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_psd_env_stamp_rectangle>
		: public single_type<::RECT*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_psd_full_page_rectangle>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_psd_full_page_rectangle>
		: public single_type<::RECT*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_psd_greek_text_rectangle>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_psd_greek_text_rectangle>
		: public single_type<::RECT*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_psd_margin_rectangle>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_psd_margin_rectangle>
		: public single_type<::RECT*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_psd_min_margin_rectangle>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_psd_min_margin_rectangle>
		: public single_type<::RECT*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_psd_page_setup_dialog>
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

	export
	template<>
	struct l_params<messages::wm_psd_page_setup_dialog>
		: public single_type<::PAGESETUPDLG*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_psd_ya_full_page_rectangle>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_psd_ya_full_page_rectangle>
		: public single_type<::RECT*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_set_cursor>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_set_cursor>
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

	export
	template<>
	struct w_params<messages::wm_copy_data>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_copy_data>
		: public single_type<::COPYDATASTRUCT*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_dwm_colorization_color_changed>
		: public single_type<std::uint32_t, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_dwm_colorization_color_changed>
		: public single_type<bool, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_dwm_non_client_rendering_changed>
		: public single_type<bool, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_dwm_send_iconic_thumbnail>
		: public double_type<std::uint32_t, std::uint32_t, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_dwm_window_maximized_change>
		: public single_type<bool, ::WPARAM> {};

	export
	template<>
	struct w_params<messages::wm_device_change>
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
		
	export
	template<>
	struct l_params<messages::wm_device_change>
		: public single_type<::LPARAM, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_control_color_dialog>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_control_color_dialog>
		: public single_type<window_handle, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_enter_idle>
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

	export
	template<>
	struct l_params<messages::wm_enter_idle>
		: public single_type<window_handle, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_get_dialog_code>
		: public single_type<virtual_keys, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_get_dialog_code>
		: public single_type<::MSG*, ::LPARAM> {};

	// TODO: The type depends on the run-time value of l_param. Not sure
	// how this should be handled, if at all.
	export
	template<>
	struct w_params<messages::wm_next_dialog_control>
		: public single_type<::WPARAM, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_next_dialog_control>
		: public single_type<bool, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_dynamic_data_exchange_initiate>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_dynamic_data_exchange_initiate>
		: public double_type<::ATOM, ::ATOM, ::LPARAM> {};

	template<>
	struct w_params<messages::wm_dynamic_data_exchange_ack>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_dynamic_data_exchange_ack>
		: public double_type<WORD, WORD, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_dynamic_data_exchange_advise>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_dynamic_data_exchange_advise>
	{
		using low_order_type = global_lock_resource<::DDEADVISE>;
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

	export
	template<>
	struct w_params<messages::wm_dynamic_data_exchange_data>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_dynamic_data_exchange_data>
	{
		using low_order_type = global_lock_resource<::DDEDATA>;
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

	export
	template<>
	struct w_params<messages::wm_dynamic_data_exchange_execute>
		: public single_type<window_handle, ::WPARAM> {};

	// TODO: The type depends on the character encoding of the windowss
	// invoved. Not sure how to handle this, or if it should be handled at all.
	export
	template<>
	struct l_params<messages::wm_dynamic_data_exchange_execute>
		: public single_type<::LPARAM, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_dynamic_data_exchange_poke>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_dynamic_data_exchange_poke>
	{
		using low_order_type = global_lock_resource<::DDEPOKE>;
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

	export
	template<>
	struct w_params<messages::wm_dynamic_data_exchange_request>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_dynamic_data_exchange_request>
		: public double_type<std::uint32_t, ::ATOM, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_dynamic_data_exchange_terminate>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct w_params<messages::wm_dynamic_data_exchange_unadvise>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_dynamic_data_exchange_unadvise>
		: public double_type<std::uint32_t, ::ATOM, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_change_ui_state>
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

	export
	template<>
	struct w_params<messages::wm_initialize_menu>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct w_params<messages::wm_update_ui_state>
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

	export
	template<>
	struct w_params<messages::wm_initialize_menu_popup>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_initialize_menu_popup>
		: public double_type<std::int32_t, bool, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_menu_char>
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

	export
	template<>
	struct l_params<messages::wm_menu_char>
		: public single_type<window_handle, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_menu_select>
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

	export
	template<>
	struct l_params<messages::wm_menu_select>
		: public single_type<window_handle, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_system_char>
		: public single_type<std::uint32_t, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_system_char>
	{
		using type = keystroke_message;

		[[nodiscard]] static type get_value(::LPARAM param) noexcept
		{
			return *reinterpret_cast<type*>(std::addressof(param));
		}
	};

	export
	template<>
	struct w_params<messages::wm_system_command>
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

	export
	template<>
	struct l_params<messages::wm_system_command>
		: public double_type<std::uint32_t, std::uint32_t, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_set_hotkey>
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

	export
	template<>
	struct w_params<messages::wm_activate>
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

	export
	template<>
	struct l_params<messages::wm_activate>
		: public single_type<window_handle, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_app_command>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_app_command>
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

	export
	template<>
	struct w_params<messages::wm_char>
		: public single_type<std::uint32_t, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_char>
	{
		using type = keystroke_message;

		[[nodiscard]] static type get_value(::LPARAM param) noexcept
		{
			return *reinterpret_cast<type*>(std::addressof(param));
		}
	};

	export
	template<>
	struct w_params<messages::wm_dead_char>
		: public single_type<std::uint32_t, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_dead_char>
	{
		using type = keystroke_message;

		[[nodiscard]] static type get_value(::LPARAM param) noexcept
		{
			return *reinterpret_cast<type*>(std::addressof(param));
		}
	};

	export
	template<>
	struct w_params<messages::wm_hotkey>
		: public single_type<int, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_hotkey>
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

	export
	template<>
	struct w_params<messages::wm_key_down>
		: public single_type<virtual_keys, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_key_down>
	{
		using type = keystroke_message;

		[[nodiscard]] static type get_value(::LPARAM param) noexcept
		{
			return *reinterpret_cast<type*>(std::addressof(param));
		}
	};

	export
	template<>
	struct w_params<messages::wm_key_up>
		: public single_type<virtual_keys, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_key_up>
	{
		using type = keystroke_message;

		[[nodiscard]] static type get_value(::LPARAM param) noexcept
		{
			return *reinterpret_cast<type*>(std::addressof(param));
		}
	};

	export
	template<>
	struct w_params<messages::wm_kill_focus>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct w_params<messages::wm_set_focus>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct w_params<messages::wm_system_dead_char>
		: public single_type<std::uint32_t, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_system_dead_char>
	{
		using type = keystroke_message;

		[[nodiscard]] static type get_value(::LPARAM param) noexcept
		{
			return *reinterpret_cast<type*>(std::addressof(param));
		}
	};

	export
	template<>
	struct w_params<messages::wm_system_key_down>
		: public single_type<virtual_keys, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_system_key_down>
	{
		using type = keystroke_message;

		[[nodiscard]] static type get_value(::LPARAM param) noexcept
		{
			return *reinterpret_cast<type*>(std::addressof(param));
		}
	};

	export
	template<>
	struct w_params<messages::wm_system_key_up>
		: public single_type<virtual_keys, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_system_key_up>
	{
		using type = keystroke_message;

		[[nodiscard]] static type get_value(::LPARAM param) noexcept
		{
			return *reinterpret_cast<type*>(std::addressof(param));
		}
	};

	export
	template<>
	struct w_params<messages::wm_unicode_char>
		: public single_type<std::uint32_t, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_unicode_char>
	{
		using type = keystroke_message;

		[[nodiscard]] static type get_value(::LPARAM param) noexcept
		{
			return *reinterpret_cast<type*>(std::addressof(param));
		}
	};

	export
	template<>
	struct w_params<messages::wm_command>
		: public double_type<decltype(LOWORD(std::declval<::WPARAM>())), std::int32_t, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_command>
		: public single_type<::LPARAM, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_context_menu>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_context_menu>
		: public double_type<std::uint32_t, std::uint32_t, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_enter_menu_loop>
		: public single_type<bool, ::WPARAM> {};

	export
	template<>
	struct w_params<messages::wm_exit_menu_loop>
		: public single_type<bool, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_get_title_bar_info>
		: public single_type<::TITLEBARINFOEX*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_menu_command>
		: public single_type<std::size_t, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_menu_command>
		: public single_type<window_handle, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_menu_drag>
		: public single_type<std::uint32_t, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_menu_drag>
		: public single_type<window_handle, ::LPARAM> {};

	export
	template<>
	struct l_params<messages::wm_menu_get_object>
		: public single_type<::MENUGETOBJECTINFO*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_menu_right_button_up>
		: public single_type<std::size_t, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_menu_right_button_up>
		: public single_type<window_handle, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_next_menu>
		: public single_type<virtual_keys, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_next_menu>
		: public single_type<::MDINEXTMENU*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_uninitialize_menu_popup>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_uninitialize_menu_popup>
	{
		using type = std::uint32_t;

		[[nodiscard]] static constexpr type get_value(::LPARAM param) noexcept
		{
			return static_cast<type>(HIWORD(param));
		}
	};

	export
	template<>
	struct l_params<messages::wm_capture_changed>
		: public single_type<window_handle, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_left_mouse_button_double_click>
		: public single_type<mouse_keyboard, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_left_mouse_button_double_click>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_left_mouse_button_down>
		: public single_type<mouse_keyboard, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_left_mouse_button_down>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_left_mouse_button_up>
		: public single_type<mouse_keyboard, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_left_mouse_button_up>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_middle_mouse_button_double_click>
		: public single_type<mouse_keyboard, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_middle_mouse_button_double_click>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_middle_mouse_button_down>
		: public single_type<mouse_keyboard, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_middle_mouse_button_down>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_middle_mouse_button_up>
		: public single_type<mouse_keyboard, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_middle_mouse_button_up>
		: public xy_coordinate {};

	template<>
	struct w_params<messages::wm_mouse_activate>
		: public single_type<window_handle, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_mouse_activate>
		: public double_type<hit_value, decltype(HIWORD(std::declval<::LPARAM>())), ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_mouse_hover>
		: public single_type<mouse_keyboard, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_mouse_hover>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_mouse_horizontal_wheel>
		: public double_type<mouse_keyboard, int, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_mouse_horizontal_wheel>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_mouse_move>
		: public single_type<mouse_keyboard, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_mouse_move>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_mouse_wheel>
		: public double_type<mouse_keyboard, int, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_mouse_wheel>
		: public xy_coordinate {};

	export
	template<>
	struct l_params<messages::wm_non_client_hit_test>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_non_client_left_button_double_click>
		: public single_type<hit_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_non_client_left_button_double_click>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_non_client_left_button_down>
		: public single_type<hit_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_non_client_left_button_down>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_non_client_left_button_up>
		: public single_type<hit_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_non_client_left_button_up>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_non_client_middle_button_double_click>
		: public single_type<hit_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_non_client_middle_button_double_click>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_non_client_middle_button_down>
		: public single_type<hit_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_non_client_middle_button_down>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_non_client_middle_button_up>
		: public single_type<hit_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_non_client_middle_button_up>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_non_client_mouse_hover>
		: public single_type<hit_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_non_client_mouse_hover>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_non_client_mouse_move>
		: public single_type<hit_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_non_client_mouse_move>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_non_client_right_button_double_click>
		: public single_type<hit_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_non_client_right_button_double_click>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_non_client_right_button_down>
		: public single_type<hit_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_non_client_right_button_down>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_non_client_right_button_up>
		: public single_type<hit_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_non_client_right_button_up>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_non_client_x_button_double_click>
		: public single_type<xbutton_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_non_client_x_button_double_click>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_non_client_x_button_down>
		: public single_type<xbutton_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_non_client_x_button_down>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_non_client_x_button_up>
		: public single_type<xbutton_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_non_client_x_button_up>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_right_button_double_click>
		: public single_type<hit_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_right_button_double_click>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_right_button_down>
		: public single_type<hit_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_right_button_down>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_right_button_up>
		: public single_type<hit_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_right_button_up>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_x_button_double_click>
		: public double_type<hit_value, xbutton_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_x_button_double_click>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_x_button_down>
		: public double_type<hit_value, xbutton_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_x_button_down>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_x_button_up>
		: public double_type<hit_value, xbutton_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_x_button_up>
		: public xy_coordinate {};

	export
	template<>
	struct w_params<messages::wm_mdi_activate>
		: public window_handler<::WPARAM> {};

	export
	template<>
	struct w_params<messages::wm_mdi_cascade>
		: public single_type<multiple_document_cascade_value, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_mdi_create>
		: public single_type<::MDICREATESTRUCT*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_mdi_destroy>
		: public window_handler<::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_mdi_get_active>
		: public single_type<bool, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_mdi_maximize>
		: public window_handler<::WPARAM> {};

	export
	template<>
	struct w_params<messages::wm_mdi_next>
		: public window_handler<::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_mdi_next>
		: public single_type<bool, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_mdi_restore>
		: public window_handler<::WPARAM> {};

	export
	template<>
	struct w_params<messages::wm_mdi_set_menu>
		: public window_handler<::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_mdi_set_menu>
		: public window_handler<::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_mdi_tile>
		: public single_type<multiple_document_tiling_value, ::WPARAM> {};

	export
	template<>
	struct w_params<messages::wm_input>
	{
		using type = raw_input_code_value;

		[[nodiscard]] static constexpr type get_value(::WPARAM param) noexcept
		{
			return static_cast<type>(GET_RAWINPUT_CODE_WPARAM(param));
		}
	};

	export
	template<>
	struct l_params<messages::wm_input>
		: public single_type<::HRAWINPUT, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_input_device_change>
		: public single_type<register_device_status, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_input_device_change>
		: public single_type<::HRAWINPUT, ::LPARAM> {};

	// TODO: Figure out what types the Microsoft Docs are referring to.
	/*template<>
	struct w_params<messages::wm_control_color_scrollbar> {};

	template<>
	struct l_params<messages::wm_control_color_scrollbar> {};*/

	export
	template<>
	struct w_params<messages::wm_horizontal_scroll>
		: public double_type<scroll_bar_command, int, ::WPARAM> {};

	// TODO: Figure out what type the Microsoft Docs is referring to.
	/*template<>
	struct l_params<messages::wm_horizontal_scroll> {};*/

	export
	template<>
	struct w_params<messages::wm_vertical_scroll>
		: public double_type<scroll_bar_command, int, ::WPARAM> {};

	// TODO: Figure out what type the Microsoft Docs is referring to.
	/*template<>
	struct l_params<messages::wm_vertical_scroll> {};*/

	export
	template<>
	struct w_params<messages::wm_timer>
		: public single_type<::HANDLE, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_timer>
		: public single_type<void*, ::LPARAM> {};

	// TODO: Figure out what type the Microsoft Docs is referring to.
	/*template<>
	struct w_params<messgae::wm_erase_background> {};*/

	export
	template<>
	struct w_params<messages::wm_get_text>
		: public single_type<std::size_t, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_get_text>
		: public single_type<char*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_set_font>
		: public single_type<::HFONT, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_set_font>
	{
		using type = bool;

		[[nodiscard]] static constexpr type get_value(::LPARAM param) noexcept
		{
			return static_cast<type>(LOWORD(param));
		}
	};

	export
	template<>
	struct w_params<messages::wm_set_icon>
		: public single_type<icon_size, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_set_icon>
		: public single_type<::HICON, ::LPARAM> {};

	export
	template<>
	struct l_params<messages::wm_set_text>
		: public single_type<char*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_activate_app>
		: public single_type<bool, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_activate_app>
		: public single_type<bool, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_compacting>
		: public single_type<std::uint16_t, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_create>
		: public single_type<::CREATESTRUCT*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_dpi_changed>
		: public double_type<int, int, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_dpi_changed>
		: public single_type<::RECT*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_enable>
		: public single_type<bool, ::WPARAM> {};

	export
	template<>
	struct w_params<messages::wm_get_icon>
		: public single_type<icon_size, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_get_icon>
		: public single_type<int, ::LPARAM> {};

	export
	template<>
	struct l_params<messages::wm_get_min_max_info>
		: public single_type<::MINMAXINFO*, ::LPARAM> {};

	// TODO
	/*template<>
	struct w_params<messages::wm_input_language_change> {};

	template<>
	struct l_params<messages::wm_input_language_change> {};*/

	export
	template<>
	struct w_params<messages::wm_input_language_change_request>
		: public single_type<input_language_change, ::WPARAM> {};

	// TODO
	/*template<>
	struct l_params<messages::wm_input_language_change_request> {};*/

	export
	template<>
	struct l_params<messages::wm_move>
		: public xy_coordinate {};

	export
	template<>
	struct l_params<messages::wm_moving>
		: public single_type<::RECT*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_non_client_activate>
		: public single_type<bool, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_non_client_activate>
		: public window_handler<::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_non_client_calculate_size>
		: public single_type<bool, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_non_client_calculate_size>
		: public single_type<::LPARAM, ::LPARAM> {};

	export
	template<>
	struct l_params<messages::wm_non_client_create>
		: public single_type<::CREATESTRUCT*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_quit>
		: public single_type<int, ::WPARAM> {};

	export
	template<>
	struct w_params<messages::wm_show_window>
		: public single_type<bool, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_show_window>
		: public single_type<show_window, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_size>
		: public single_type<size_request, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_size>
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

	export
	template<>
	struct w_params<messages::wm_sizing>
		: public single_type<window_sizing, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_sizing>
		: public single_type<::RECT*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_style_changed>
		: public single_type<style_change, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_style_changed>
		: public single_type<const ::STYLESTRUCT*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_style_changing>
		: public single_type<style_change, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_style_changing>
		: public single_type<const ::STYLESTRUCT*, ::LPARAM> {};

	export
	template<>
	struct l_params<messages::wm_window_position_changed>
		: public single_type<const ::WINDOWPOS*, ::LPARAM> {};

	export
	template<>
	struct l_params<messages::wm_window_position_changing>
		: public single_type<const ::WINDOWPOS*, ::LPARAM> {};

	export
	template<>
	struct w_params<messages::wm_display_change>
		: public single_type<std::uint32_t, ::WPARAM> {};

	export
	template<>
	struct l_params<messages::wm_display_change>
		: public double_type<std::uint32_t, std::uint32_t, ::LPARAM> {};

	template<typename T>
	concept parameter_type = std::same_as<T, ::LPARAM> || std::same_as<T, ::WPARAM>;

	template<system_message Message>
	concept has_l_params = 
		requires
	{
		typename l_params<Message>;
	};

	template<system_message Message>
	concept has_w_params =
		requires
	{
		typename w_params<Message>;
	};

	template<system_message Message>
	concept has_l_params_get_value = has_l_params<Message> &&
		requires(::LPARAM p)
	{
		l_params<Message>::get_value(p);
	};

	template<system_message Message>
	concept has_w_params_get_value = has_w_params<Message> &&
		requires(::LPARAM p)
	{
		w_params<Message>::get_value(p);
	};

	template<system_message Message>
	concept has_l_params_get_low_order_value = has_l_params<Message> &&
		requires(::LPARAM p)
	{
		l_params<Message>::get_low_order_value(p);
	};

	template<system_message Message>
	concept has_w_params_get_low_order_value = has_w_params<Message> &&
		requires(::LPARAM p)
	{
		w_params<Message>::get_low_order_value(p);
	};

	template<system_message Message>
	concept has_l_params_get_high_order_value = has_l_params<Message> &&
		requires(::LPARAM p)
	{
		l_params<Message>::get_high_order_value(p);
	};

	template<system_message Message>
	concept has_w_params_get_high_order_value = has_w_params<Message> &&
		requires(::LPARAM p)
	{
		w_params<Message>::get_high_order_value(p);
	};

	export
	template<system_message Message>
		requires(has_l_params_get_value<Message>)
	[[nodiscard]] constexpr auto get_value(::LPARAM param) noexcept(noexcept(l_params<Message>::get_value(std::declval<::LPARAM>())))
	{
		return l_params<Message>::get_value(param);
	}

	export
	template<system_message Message>
		requires(has_w_params_get_value<Message>)
	[[nodiscard]] constexpr auto get_value(::WPARAM param) noexcept(noexcept(w_params<Message>::get_value(std::declval<::WPARAM>())))
	{
		return w_params<Message>::get_value(param);
	}

	export
	template<system_message Message>
		requires(has_l_params_get_low_order_value<Message>)
	[[nodiscard]] constexpr auto get_low_order_value(::LPARAM param) noexcept(noexcept(l_params<Message>::get_low_order_value(std::declval<::LPARAM>())))
	{
		return l_params<Message>::get_low_order_value(param);
	}

	export
	template<system_message Message>
		requires(has_w_params_get_low_order_value<Message>)
	[[nodiscard]] constexpr auto get_low_order_value(::WPARAM param) noexcept(noexcept(w_params<Message>::get_low_order_value(std::declval<::WPARAM>())))
	{
		return w_params<Message>::get_low_order_value(param);
	}

	export
	template<system_message Message>
		requires(has_l_params_get_high_order_value<Message>)
	[[nodiscard]] constexpr auto get_high_order_value(::LPARAM param) noexcept(noexcept(l_params<Message>::get_high_order_value(std::declval<::LPARAM>())))
	{
		return l_params<Message>::get_high_order_value(param);
	}

	export
	template<system_message Message>
		requires(has_w_params_get_high_order_value<Message>)
	[[nodiscard]] constexpr auto get_high_order_value(::WPARAM param) noexcept(noexcept(w_params<Message>::get_high_order_value(std::declval<::WPARAM>())))
	{
		return w_params<Message>::get_high_order_value(param);
	}
}