#pragma once

#include "cmoon/execution/impl/property_t.hpp"

namespace cmoon
{
	namespace execution
	{
		namespace blocking_t_nested
		{
			template<class B>
			struct possibly_t : details::nested_property_t<B, possibly_t> {};

			template<class B>
			struct always_t : details::nested_property_t<B, always_t> {};

			template<class B>
			struct never_t : details::nested_property_t<B, never_t> {};
		}

		struct blocking_t : details::property_t<blocking_t,
												blocking_t_nested::possibly_t<blocking_t>,
												blocking_t_nested::always_t<blocking_t>,
												blocking_t_nested::never_t<blocking_t>>
		{
			private:
				using base = details::property_t<blocking_t,
												blocking_t_nested::possibly_t<blocking_t>,
												blocking_t_nested::always_t<blocking_t>,
												blocking_t_nested::never_t<blocking_t>>;
			public:
				[[nodiscard]] friend constexpr bool operator==(const blocking_t& a, const blocking_t& b) noexcept = default;
				[[nodiscard]] friend constexpr bool operator!=(const blocking_t& a, const blocking_t& b) noexcept = default;

				using possibly_t = blocking_t_nested::possibly_t<blocking_t>;
				static constexpr possibly_t possibly {};

				using always_t = blocking_t_nested::always_t<blocking_t>;
				static constexpr always_t always {};

				using never_t = blocking_t_nested::never_t<blocking_t>;
				static constexpr never_t never {};

				template<class NN>
				constexpr blocking_t(const NN n) noexcept
					: base{n} {}

				constexpr blocking_t() noexcept
					: base{} {}
		};

		inline constexpr blocking_t blocking {};
	}
}