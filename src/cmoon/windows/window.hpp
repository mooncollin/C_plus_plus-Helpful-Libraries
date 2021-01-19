#pragma once

#include <string_view>

#include <d2d1.h>
#include <windows.h>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include "cmoon/graphics/size2D.hpp"
#include "cmoon/windows/system_info.hpp"
#include "cmoon/windows/system_message.hpp"

namespace cmoon
{
	namespace windows
	{
		using window_handle = ::HWND;

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

		enum window_style : ::DWORD
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

		enum extended_window_style : ::DWORD
		{
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

		[[nodiscard]] graphics::size2D_u get_client_window_size(window_handle handle) noexcept
		{
			::RECT rc;
			::GetClientRect(handle, std::addressof(rc));

			return {static_cast<typename graphics::size2D_u::type>(rc.right),
					static_cast<typename graphics::size2D_u::type>(rc.bottom)};
		}

		template<class DerivedType>
		class base_window
		{
			public:
				static ::LRESULT window_proc(window_handle hwnd, ::UINT msg, ::WPARAM w_param, ::LPARAM l_param)
				{
					DerivedType* p_this = nullptr;

					if (cmoon::windows::message{msg} == cmoon::windows::message::wm_non_client_create)
					{
						auto create = cmoon::windows::get_value<cmoon::windows::message::wm_non_client_create>(l_param);
						p_this = reinterpret_cast<DerivedType*>(create->lpCreateParams);
						SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<::LONG_PTR>(p_this));

						p_this->hwnd = hwnd;
					}
					else
					{
						p_this = reinterpret_cast<DerivedType*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
					}

					if (p_this != nullptr)
					{
						return p_this->handle_message(cmoon::windows::message{msg}, w_param, l_param);
					}

					return DefWindowProc(hwnd, msg, w_param, l_param);
				}

				base_window() = default;

				bool create(windows::string_view_type window_name, ::DWORD style,
					::DWORD ex_style = 0,
					int x = CW_USEDEFAULT,
					int y = CW_USEDEFAULT,
					int width = CW_USEDEFAULT,
					int height = CW_USEDEFAULT,
					window_handle parent = nullptr,
					::HMENU menu = nullptr) noexcept
				{
					::WNDCLASS wc = {0};

					wc.lpfnWndProc = DerivedType::window_proc;
					wc.hInstance = GetModuleHandle(nullptr);
					wc.lpszClassName = class_name().data();

					RegisterClass(std::addressof(wc));

					hwnd = CreateWindowEx(ex_style, class_name().data(), window_name.data(), style,
						x, y, width, height, parent, menu, GetModuleHandle(nullptr), this);

					return hwnd != nullptr;
				}

				window_handle window() const noexcept
				{
					return hwnd;
				}

				bool show_window(int cmd_show) noexcept
				{
					return ::ShowWindow(hwnd, cmd_show);
				}

				virtual windows::string_view_type class_name() const = 0;
				virtual ::LRESULT handle_message(cmoon::windows::message, ::WPARAM, ::LPARAM) = 0;
			protected:
				window_handle hwnd{nullptr};
		};
	}
}