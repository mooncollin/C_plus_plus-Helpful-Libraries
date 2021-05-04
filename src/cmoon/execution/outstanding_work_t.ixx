export module cmoon.execution.outstanding_work_t;

import cmoon.execution.property_t;

namespace cmoon::execution
{
	template<class B>
	struct untracked_t : nested_property_t<B, untracked_t> {};

	template<class B>
	struct tracked_t : nested_property_t<B, tracked_t> {};

	export
	struct outstanding_work_t : property_t<outstanding_work_t,
										   untracked_t<outstanding_work_t>,
										   tracked_t<outstanding_work_t>>
	{
		private:
			using base = property_t<outstanding_work_t,
									untracked_t<outstanding_work_t>,
									tracked_t<outstanding_work_t>>;
		public:
			[[nodiscard]] friend constexpr bool operator==(const outstanding_work_t& a, const outstanding_work_t& b) noexcept = default;
			[[nodiscard]] friend constexpr bool operator!=(const outstanding_work_t& a, const outstanding_work_t& b) noexcept = default;

			using untracked_t = untracked_t<outstanding_work_t>;
			static constexpr untracked_t untracked {};

			using tracked_t = tracked_t<outstanding_work_t>;
			static constexpr tracked_t tracked {};

			template<class NN>
			constexpr outstanding_work_t(const NN n) noexcept
				: base{n} {}

			constexpr outstanding_work_t() noexcept
				: base{} {}
	};

	export
	inline constexpr outstanding_work_t outstanding_work {};
}