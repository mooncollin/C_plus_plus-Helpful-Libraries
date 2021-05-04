export module cmoon.execution.mapping_t;

import cmoon.execution.property_t;

namespace cmoon::execution
{
	template<class B>
	struct thread_t : nested_property_t<B, thread_t> {};

	template<class B>
	struct new_thread_t : nested_property_t<B, new_thread_t> {};

	template<class B>
	struct other_t : nested_property_t<B, other_t> {};

	export
	struct mapping_t : property_t<mapping_t,
								  thread_t<mapping_t>,
								  new_thread_t<mapping_t>,
								  other_t<mapping_t>>
	{
		private:
			using base = property_t<mapping_t,
									thread_t<mapping_t>,
									new_thread_t<mapping_t>,
									other_t<mapping_t>>;
		public:
			[[nodiscard]] friend constexpr bool operator==(const mapping_t& a, const mapping_t& b) noexcept = default;
			[[nodiscard]] friend constexpr bool operator!=(const mapping_t& a, const mapping_t& b) noexcept = default;

			using thread_t = thread_t<mapping_t>;
			static constexpr thread_t thread {};

			using new_thread_t = new_thread_t<mapping_t>;
			static constexpr new_thread_t new_thread {};

			using other_t = other_t<mapping_t>;
			static constexpr other_t other {};

			template<class NN>
			constexpr mapping_t(const NN n) noexcept
				: base{n} {}

			constexpr mapping_t() noexcept
				: base{} {}
	};

	export
	inline constexpr mapping_t mapping {};
}