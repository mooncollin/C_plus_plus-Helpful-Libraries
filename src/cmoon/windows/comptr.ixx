module;

#include <Unknwn.h>

export module cmoon.windows.comptr;

import <type_traits>;
import <memory>;
import <concepts>;

namespace cmoon::windows
{
	template<class T>
	concept com_enabled = std::derived_from<T, ::IUnknown>;

	template<com_enabled COMObject>
	struct default_com_deleter
	{
		void operator()(COMObject* ptr) const
		{
			ptr->Release();
		}
	};

	export
	template<com_enabled COMObject>
	using unique_com_ptr = std::unique_ptr<COMObject,
											default_com_deleter<COMObject>>;

	export
	template<com_enabled COMObject>
	class shared_com_ptr : public std::shared_ptr<COMObject>
	{
		using base = std::shared_ptr<COMObject>;
		using default_deleter = default_com_deleter<COMObject>;

		public:
			shared_com_ptr() noexcept
				: base{nullptr, default_deleter{}} {}

			shared_com_ptr(std::nullptr_t) noexcept
				: base{nullptr, default_deleter{}} {}

			template<class Deleter>
			shared_com_ptr(std::nullptr_t, Deleter d)
				: base{nullptr, d} {}

			shared_com_ptr(COMObject* obj)
				: base{obj, default_deleter{}} {}

			template<class Deleter>
			shared_com_ptr(COMObject* obj, Deleter d)
				: base{obj, d} {}
	};
}