#pragma comment(lib, "d2d1")

#include <string_view>
#include <concepts>
#include <algorithm>
#include <cstdint>
#include <array>
#include <sstream>
#include <iostream>

#include <windows.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include <d2d1.h>

#include "cmoon/graphics/color.hpp"
#include "cmoon/graphics/circle.hpp"
#include "cmoon/graphics/rectangle.hpp"
#include "cmoon/graphics/size2D.hpp"
#include "cmoon/platform.hpp"

#include "cmoon/windows/scene.hpp"
#include "cmoon/windows/window.hpp"
#include "cmoon/windows/comptr.hpp"
#include "cmoon/windows/system_message.hpp"
#include "cmoon/windows/message_box.hpp"
#include "cmoon/windows/system_metrics.hpp"
#include "cmoon/windows/system_info.hpp"


class scene : public cmoon::windows::graphics_scene
{
	public:

		void calculate_layout() override
		{
			const auto size = render_target->GetSize();

			const auto x = size.width / 2.0f;
			const auto y = size.height / 2.0f;
			const auto radius = std::min(x, y);

			circle = cmoon::graphics::circle_f{{x, y}, radius};

			const auto pt1 = cmoon::graphics::point2D_f{
				circle.point.x,
				circle.point.y - (circle.radius * 0.9f)
			};

			const auto pt2 = cmoon::graphics::point2D_f{
				circle.point.x,
				circle.point.y - circle.radius
			};

			for (std::size_t i{0}; i < 12; ++i)
			{
				const auto mat = D2D1::Matrix3x2F::Rotation((360.0f / 12) * i, circle.point);

				ticks[i * 2] = mat.TransformPoint(pt1);
				ticks[i * 2 + 1] = mat.TransformPoint(pt2);
			}
		}

		void discard_device_dependent_resources() override
		{
			fill.release();
			stroke.release();
		}

		::HRESULT create_device_independent_resources() override
		{
			return S_OK;
		}

		void discard_device_independent_resources() override {}

		::HRESULT create_device_dependent_resources() override
		{
			::ID2D1SolidColorBrush* b;
			auto hr = render_target->CreateSolidColorBrush(
				cmoon::graphics::colors::blanched_almond,
				D2D1::BrushProperties(),
				std::addressof(b)
			);

			if (SUCCEEDED(hr))
			{
				fill.reset(b);

				hr = render_target->CreateSolidColorBrush(
					cmoon::graphics::color(0, 0, 0),
					D2D1::BrushProperties(),
					std::addressof(b)
				);

				if (SUCCEEDED(hr))
				{
					stroke.reset(b);
				}
			}

			return hr;
		}

		void render_scene() override
		{
			render_target->Clear(cmoon::graphics::colors::lavender_blush);

			render_target->FillEllipse(circle, fill.get());
			render_target->DrawEllipse(circle, stroke.get());

			for (std::size_t i{0}; i < 12; ++i)
			{
				render_target->DrawLine(ticks[i * 2], ticks[i * 2 + 1], stroke.get(), 2.0f);
			}

			::SYSTEMTIME time;
			::GetLocalTime(std::addressof(time));

			const auto hour_angle = (360.0f / 12) * (time.wHour) + (time.wMinute * 0.5f);
			const auto minute_angle = (360.0f / 60) * time.wMinute;

			const auto second_angle = (360.0f / 60) * time.wSecond + (360.0f / 60000) * time.wMilliseconds;

			draw_clock_hand(0.6f, hour_angle, 6);
			draw_clock_hand(0.85f, minute_angle, 4);
			draw_clock_hand(0.85f, second_angle, 1);

			render_target->SetTransform(D2D1::Matrix3x2F::Identity());
		}
	private:
		cmoon::windows::unique_com_ptr<::ID2D1SolidColorBrush> fill;
		cmoon::windows::unique_com_ptr<::ID2D1SolidColorBrush> stroke;
		cmoon::graphics::circle_f circle {};
		std::array<cmoon::graphics::point2D_f, 24> ticks;

		void draw_clock_hand(float length, float angle, float stroke_width)
		{
			render_target->SetTransform(D2D1::Matrix3x2F::Rotation(angle, circle.point));

			const cmoon::graphics::point2D_f end_point{
				circle.point.x,
				circle.point.y - (circle.radius * length)
			};

			render_target->DrawLine(circle.point, end_point, stroke.get(), stroke_width);
		}
};

class main_window : public cmoon::windows::base_window<main_window>
{
	public:
		main_window() = default;

		void wait_timer()
		{
			if (::MsgWaitForMultipleObjects(1, std::addressof(timer), false, INFINITE, QS_ALLINPUT) == WAIT_OBJECT_0)
			{
				::InvalidateRect(hwnd, nullptr, false);
			}
		}

		cmoon::windows::string_view_type class_name() const override
		{
			if constexpr (cmoon::win_unicode)
			{
				return L"Clock Window Class";
			}
			else
			{
				return "Clock Window Class";
			}
		}

		::LRESULT handle_message(cmoon::windows::message msg, ::WPARAM w_param, ::LPARAM l_param) override
		{
			const auto s = cmoon::windows::get_client_window_size(hwnd);
			std::cout << "X: " << s.width << " Y: " << s.height << "\n";
			switch (msg)
			{
				case cmoon::windows::message::wm_create:
					if (FAILED(scene.initialize() || !initialize_timer()))
					{
						return -1;
					}
					return 0;
				case cmoon::windows::message::wm_destroy:
					::CloseHandle(timer);
					scene.clean_up();
					::PostQuitMessage(0);
					return 0;
				case cmoon::windows::message::wm_paint:
				case cmoon::windows::message::wm_display_change:
				case cmoon::windows::message::wm_window_position_changing:
					{
						::PAINTSTRUCT ps;
						::BeginPaint(hwnd, std::addressof(ps));
						scene.render(hwnd);
						::EndPaint(hwnd, std::addressof(ps));
					}
					return 0;
				case cmoon::windows::message::wm_size:
					{
						const auto size = cmoon::windows::get_value<cmoon::windows::message::wm_size>(l_param);
						scene.resize(size);
						::InvalidateRect(hwnd, nullptr, false);
					}
					return 0;
				case cmoon::windows::message::wm_erase_background:
					return 1;
				case cmoon::windows::message::wm_key_down:
					if (cmoon::windows::get_value<cmoon::windows::message::wm_key_down>(w_param) == cmoon::windows::virtual_keys::escape_key)
					{
						const auto res = cmoon::windows::show_message_box(hwnd, "Are you sure to quit?", "Message", cmoon::windows::message_box_behavior::ok_cancel);
						if (res == cmoon::windows::message_box_result::ok)
						{
							::SendMessage(hwnd, cmoon::windows::message::wm_close, 0, 0);
						}
					}
				default:
					return ::DefWindowProc(hwnd, msg, w_param, l_param);
			}
		}
	private:
		::HANDLE timer {nullptr};
		scene scene;

		bool initialize_timer()
		{
			timer = CreateWaitableTimer(nullptr, false, nullptr);
			if (timer == nullptr)
			{
				return false;
			}

			::LARGE_INTEGER li = {0};

			if (!SetWaitableTimer(timer, std::addressof(li), (1000 / 60), nullptr, nullptr, false))
			{
				::CloseHandle(timer);
				timer = nullptr;
				return false;
			}

			return true;
		}
};

int WINAPI wWinMain(::HINSTANCE, ::HINSTANCE, ::PWSTR, int nCmdShow)
{
	if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
	{
		return 0;
	}

	main_window win;

	if (!win.create("Analog Clock", cmoon::windows::window_style::overlapped_window))
	{
		return 0;
	}

	win.show_window(nCmdShow);

	MSG msg {};

	while (cmoon::windows::message{msg.message} != cmoon::windows::message::wm_quit)
	{
		if (PeekMessage(std::addressof(msg), nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		win.wait_timer();
	}

	::CoUninitialize();
	
	return 0;
}