#pragma once

#include "cmoon/execution/impl/property_t.hpp"

namespace cmoon
{
	namespace execution
	{
		namespace outstanding_work_t_nested
		{
			template<class B>
			struct untracked_t : details::nested_property_t<B, untracked_t> {};

			template<class B>
			struct tracked_t : details::nested_property_t<B, tracked_t> {};
		}

		struct outstanding_work_t : details::property_t<outstanding_work_t,
														outstanding_work_t_nested::untracked_t<outstanding_work_t>,
														outstanding_work_t_nested::tracked_t<outstanding_work_t>>
		{
			private:
				using base = details::property_t<outstanding_work_t,
														outstanding_work_t_nested::untracked_t<outstanding_work_t>,
														outstanding_work_t_nested::tracked_t<outstanding_work_t>>;
			public:
				[[nodiscard]] friend constexpr bool operator==(const outstanding_work_t& a, const outstanding_work_t& b) noexcept = default;
				[[nodiscard]] friend constexpr bool operator!=(const outstanding_work_t& a, const outstanding_work_t& b) noexcept = default;

				using untracked_t = outstanding_work_t_nested::untracked_t<outstanding_work_t>;
				static constexpr untracked_t untracked {};

				using tracked_t = outstanding_work_t_nested::tracked_t<outstanding_work_t>;
				static constexpr tracked_t tracked {};

				template<class NN>
				constexpr outstanding_work_t(const NN n) noexcept
					: base{n} {}

				constexpr outstanding_work_t() noexcept
					: base{} {}
		};

		inline constexpr outstanding_work_t outstanding_work {};
	}
}