#pragma once

#include "cmoon/execution/impl/property_t.hpp"

namespace cmoon
{
	namespace execution
	{
		namespace bulk_guarantee_t_nested
		{
			template<class B>
			struct unsequenced_t : details::nested_property_t<B, unsequenced_t> {};

			template<class B>
			struct sequenced_t : details::nested_property_t<B, sequenced_t> {};

			template<class B>
			struct parallel_t : details::nested_property_t<B, parallel_t> {};
		}

		struct bulk_guarentee_t : details::property_t<bulk_guarentee_t,
													  bulk_guarantee_t_nested::unsequenced_t<bulk_guarentee_t>,
													  bulk_guarantee_t_nested::sequenced_t<bulk_guarentee_t>,
													  bulk_guarantee_t_nested::parallel_t<bulk_guarentee_t>>
		{
			private:
				using base = details::property_t<bulk_guarentee_t,
													  bulk_guarantee_t_nested::unsequenced_t<bulk_guarentee_t>,
													  bulk_guarantee_t_nested::sequenced_t<bulk_guarentee_t>,
													  bulk_guarantee_t_nested::parallel_t<bulk_guarentee_t>>;
			public:
				[[nodiscard]] friend constexpr bool operator==(const bulk_guarentee_t& a, const bulk_guarentee_t& b) noexcept = default;
				[[nodiscard]] friend constexpr bool operator!=(const bulk_guarentee_t& a, const bulk_guarentee_t& b) noexcept = default;

				using unsequenced_t = bulk_guarantee_t_nested::unsequenced_t<bulk_guarentee_t>;
				static constexpr unsequenced_t unsequenced {};

				using sequenced_t = bulk_guarantee_t_nested::sequenced_t<bulk_guarentee_t>;
				static constexpr sequenced_t sequenced {};

				using parallel_t = bulk_guarantee_t_nested::parallel_t<bulk_guarentee_t>;
				static constexpr parallel_t parallel {};

				template<class NN>
				constexpr bulk_guarentee_t(const NN n) noexcept
					: base{n} {}

				constexpr bulk_guarentee_t() noexcept
					: base{} {}
		};

		inline constexpr bulk_guarentee_t bulk_guarentee {};
	}
}