module;

#include <d2d1.h>
#include <Windows.h>

export module cmoon.windows.gui.window;

import <string_view>;

import cmoon.graphics;
import cmoon.windows;

import cmoon.windows.gui.window_style;

namespace cmoon::windows
{
	export
	[[nodiscard]] graphics::size2D_u get_client_window_size(window_handle handle) noexcept
	{
		::RECT rc;
		::GetClientRect(handle, std::addressof(rc));

		return {static_cast<typename graphics::size2D_u::type>(rc.right),
				static_cast<typename graphics::size2D_u::type>(rc.bottom)};
	}

	export
	template<class DerivedType>
	class base_window
	{
		public:
			static ::LRESULT window_proc(window_handle hwnd, ::UINT msg, ::WPARAM w_param, ::LPARAM l_param)
			{
				DerivedType* p_this = nullptr;

				if (system_message{msg} == messages::wm_non_client_create)
				{
					auto create = windows::get_value<messages::wm_non_client_create>(l_param);
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
					return p_this->handle_message(system_message{msg}, w_param, l_param);
				}

				return DefWindowProc(hwnd, msg, w_param, l_param);
			}

			base_window() = default;

			bool create(windows::string_view_type window_name, window_style style,
				extended_window_style ex_style = extended_window_style::left_to_right_reading,
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

				hwnd = CreateWindowEx(static_cast<::DWORD>(ex_style), class_name().data(), window_name.data(), static_cast<::DWORD>(style),
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