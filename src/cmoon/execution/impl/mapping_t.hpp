#pragma once

#include "cmoon/execution/impl/property_t.hpp"

namespace cmoon
{
	namespace execution
	{
		namespace mapping_t_nested
		{
			template<class B>
			struct thread_t : details::nested_property_t<B, thread_t> {};

			template<class B>
			struct new_thread_t : details::nested_property_t<B, new_thread_t> {};

			template<class B>
			struct other_t : details::nested_property_t<B, other_t> {};
		}

		struct mapping_t : details::property_t<mapping_t,
											   mapping_t_nested::thread_t<mapping_t>,
											   mapping_t_nested::new_thread_t<mapping_t>,
											   mapping_t_nested::other_t<mapping_t>>
		{
			private:
				using base = details::property_t<mapping_t,
											   mapping_t_nested::thread_t<mapping_t>,
											   mapping_t_nested::new_thread_t<mapping_t>,
											   mapping_t_nested::other_t<mapping_t>>;
			public:
				[[nodiscard]] friend constexpr bool operator==(const mapping_t& a, const mapping_t& b) noexcept = default;
				[[nodiscard]] friend constexpr bool operator!=(const mapping_t& a, const mapping_t& b) noexcept = default;

				using thread_t = mapping_t_nested::thread_t<mapping_t>;
				static constexpr thread_t thread {};

				using new_thread_t = mapping_t_nested::new_thread_t<mapping_t>;
				static constexpr new_thread_t new_thread {};

				using other_t = mapping_t_nested::other_t<mapping_t>;
				static constexpr other_t other {};

				template<class NN>
				constexpr mapping_t(const NN n) noexcept
					: base{n} {}

				constexpr mapping_t() noexcept
					: base{} {}
		};

		inline constexpr mapping_t mapping {};
	}
}