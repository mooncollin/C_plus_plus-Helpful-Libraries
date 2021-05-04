export module cmoon.execution.bulk_guarantee_t;

import cmoon.execution.property_t;

namespace cmoon::execution
{
	template<class B>
	struct unsequenced_t : nested_property_t<B, unsequenced_t> {};

	template<class B>
	struct sequenced_t : nested_property_t<B, sequenced_t> {};

	template<class B>
	struct parallel_t : nested_property_t<B, parallel_t> {};

	export
	struct bulk_guarentee_t : property_t<bulk_guarentee_t,
										 unsequenced_t<bulk_guarentee_t>,
										 sequenced_t<bulk_guarentee_t>,
										 parallel_t<bulk_guarentee_t>>
	{
		private:
			using base = property_t<bulk_guarentee_t,
									unsequenced_t<bulk_guarentee_t>,
									sequenced_t<bulk_guarentee_t>,
									parallel_t<bulk_guarentee_t>>;
		public:
			[[nodiscard]] friend constexpr bool operator==(const bulk_guarentee_t& a, const bulk_guarentee_t& b) noexcept = default;
			[[nodiscard]] friend constexpr bool operator!=(const bulk_guarentee_t& a, const bulk_guarentee_t& b) noexcept = default;

			using unsequenced_t = unsequenced_t<bulk_guarentee_t>;
			static constexpr unsequenced_t unsequenced {};

			using sequenced_t = sequenced_t<bulk_guarentee_t>;
			static constexpr sequenced_t sequenced {};

			using parallel_t = parallel_t<bulk_guarentee_t>;
			static constexpr parallel_t parallel {};

			template<class NN>
			constexpr bulk_guarentee_t(const NN n) noexcept
				: base{n} {}

			constexpr bulk_guarentee_t() noexcept
				: base{} {}
	};

	export
	inline constexpr bulk_guarentee_t bulk_guarentee {};
}