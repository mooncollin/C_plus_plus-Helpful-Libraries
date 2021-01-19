#pragma once

#include <type_traits>
#include <memory>
#include <concepts>

#include "cmoon/scope.hpp"
#include "cmoon/concepts.hpp"

namespace cmoon
{
	namespace windows
	{
		template<class COMPtr>
			requires(requires(COMPtr ptr) {
				ptr->Release();
			})
		struct default_com_deleter
		{
			void operator()(COMPtr ptr) const
			{
				ptr->Release();
			}
		};

		template<class COMObject>
		using unique_com_ptr = std::unique_ptr<COMObject,
											   default_com_deleter<std::add_pointer_t<COMObject>>>;

		template<class COMObject>
		class shared_com_ptr : public std::shared_ptr<COMObject>
		{
			using base = std::shared_ptr<COMObject>;
			using default_deleter = default_com_deleter<COMObject*>;

			public:
				shared_com_ptr() noexcept
					: base{nullptr, default_deleter{}} {}

				shared_com_ptr(std::nullptr_t) noexcept
					: base{nullptr, default_deleter{}} {}

				template<class Deleter>
				shared_com_ptr(std::nullptr_t, Deleter d)
					: base{nullptr, d} {}

				shared_com_ptr(COMObject* obj)
					: base{obj, default_com_deleter<COMObject*>{}} {}

				template<class Deleter>
				shared_com_ptr(COMObject* obj, Deleter d)
					: base{obj, d} {}
		};
	}
}