export module cmoon.execution.blocking_t;

import cmoon.execution.propert_t;

namespace cmoon::execution
{
	template<class B>
	struct possibly_t : nested_property_t<B, possibly_t> {};

	template<class B>
	struct always_t : nested_property_t<B, always_t> {};

	template<class B>
	struct never_t : nested_property_t<B, never_t> {};

	export
	struct blocking_t : property_t<blocking_t,
								   possibly_t<blocking_t>,
								   always_t<blocking_t>,
								   never_t<blocking_t>>
	{
		private:
			using base = property_t<blocking_t,
									possibly_t<blocking_t>,
									always_t<blocking_t>,
									never_t<blocking_t>>;
		public:
			[[nodiscard]] friend constexpr bool operator==(const blocking_t& a, const blocking_t& b) noexcept = default;
			[[nodiscard]] friend constexpr bool operator!=(const blocking_t& a, const blocking_t& b) noexcept = default;

			using possibly_t = possibly_t<blocking_t>;
			static constexpr possibly_t possibly {};

			using always_t = always_t<blocking_t>;
			static constexpr always_t always {};

			using never_t = never_t<blocking_t>;
			static constexpr never_t never {};

			template<class NN>
			constexpr blocking_t(const NN n) noexcept
				: base{n} {}

			constexpr blocking_t() noexcept
				: base{} {}
	};

	export
	inline constexpr blocking_t blocking {};
}