export module cmoon.execution.relationship_t;

import cmoon.execution.property_t;

namespace cmoon::execution
{
	template<class B>
	struct fork_t : nested_property_t<B, fork_t> {};

	template<class B>
	struct continuation_t : nested_property_t<B, continuation_t> {};

	export
	struct relationship_t : property_t<relationship_t,
									   fork_t<relationship_t>,
									   continuation_t<relationship_t>>
	{
		private:
			using base = property_t<relationship_t,
									fork_t<relationship_t>,
									continuation_t<relationship_t>>;
		public:
			[[nodiscard]] friend constexpr bool operator==(const relationship_t& a, const relationship_t& b) noexcept = default;
			[[nodiscard]] friend constexpr bool operator!=(const relationship_t& a, const relationship_t& b) noexcept = default;

			using fork_t = fork_t<relationship_t>;
			static constexpr fork_t fork {};

			using continuation_t = continuation_t<relationship_t>;
			static constexpr continuation_t continuation {};

			template<class NN>
			constexpr relationship_t(const NN n) noexcept
				: base{n} {}

			constexpr relationship_t() noexcept
				: base{} {}
	};

	export
	inline constexpr relationship_t relationship {};
}