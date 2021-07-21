module;

#include <d2d1.h>

export module cmoon.windows.gui.scene;

import <memory>;

import cmoon.graphics;

import cmoon.windows.window_handle;
import cmoon.windows.comptr;
import cmoon.windows.gui.window;

namespace cmoon::windows
{
	export
	class graphics_scene
	{
		public:
			graphics_scene() = default;

			virtual ~graphics_scene() {}

			::HRESULT initialize()
			{
				::ID2D1Factory* f;
				const auto hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, std::addressof(f));

				if (SUCCEEDED(hr))
				{
					factory.reset(f);
					create_device_independent_resources();
				}

				return hr;
			}

			void render(window_handle h)
			{
				auto hr = create_graphics_resources(h);
				if (FAILED(hr))
				{
					return;
				}

				render_target->BeginDraw();

				render_scene();

				hr = render_target->EndDraw();
				if (hr == D2DERR_RECREATE_TARGET)
				{
					discard_device_dependent_resources();
					render_target.release();
				}
			}

			::HRESULT resize(const cmoon::graphics::size2D_u32& size)
			{
				auto hr = S_OK;
				if (render_target)
				{
					hr = render_target->Resize(size);
					if (SUCCEEDED(hr))
					{
						calculate_layout();
					}
				}

				return hr;
			}

			void clean_up()
			{
				discard_device_dependent_resources();
				discard_device_independent_resources();
			}
		protected:
			unique_com_ptr<::ID2D1Factory> factory;
			unique_com_ptr<::ID2D1HwndRenderTarget> render_target;

			float scale_x {1.0f};
			float scale_y {1.0f};

			virtual ::HRESULT create_device_independent_resources() = 0;
			virtual void discard_device_independent_resources() = 0;
			virtual ::HRESULT create_device_dependent_resources() = 0;
			virtual void discard_device_dependent_resources() = 0;
			virtual void calculate_layout() = 0;
			virtual void render_scene() = 0;

			::HRESULT create_graphics_resources(window_handle h)
			{
				auto hr = S_OK;
				if (!render_target)
				{
					const auto size = get_client_window_size(h);

					::ID2D1HwndRenderTarget* target;
						
					hr = factory->CreateHwndRenderTarget(
						D2D1::RenderTargetProperties(),
						D2D1::HwndRenderTargetProperties(h, size),
						std::addressof(target)
					);

					if (SUCCEEDED(hr))
					{
						render_target.reset(target);
						hr = create_device_dependent_resources();
					}

					if (SUCCEEDED(hr))
					{
						calculate_layout();
					}
				}

				return hr;
			}

			template<class T>
			[[nodiscard]] constexpr T pixel_to_dipX(T pixels) const noexcept
			{
				return static_cast<T>(pixels / scale_x);
			}

			template<class T>
			[[nodiscard]] constexpr T pixel_to_dipY(T pixels) const noexcept
			{
				return static_cast<T>(pixels / scale_y);
			}
	};
}