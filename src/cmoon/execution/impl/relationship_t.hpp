#pragma once

#include "cmoon/execution/impl/property_t.hpp"

namespace cmoon
{
	namespace execution
	{
		namespace relationship_t_nested
		{
			template<class B>
			struct fork_t : details::nested_property_t<B, fork_t> {};

			template<class B>
			struct continuation_t : details::nested_property_t<B, continuation_t> {};
		}

		struct relationship_t : details::property_t<relationship_t,
													relationship_t_nested::fork_t<relationship_t>,
													relationship_t_nested::continuation_t<relationship_t>>
		{
			private:
				using base = details::property_t<relationship_t,
													relationship_t_nested::fork_t<relationship_t>,
													relationship_t_nested::continuation_t<relationship_t>>;
			public:
				[[nodiscard]] friend constexpr bool operator==(const relationship_t& a, const relationship_t& b) noexcept = default;
				[[nodiscard]] friend constexpr bool operator!=(const relationship_t& a, const relationship_t& b) noexcept = default;

				using fork_t = relationship_t_nested::fork_t<relationship_t>;
				static constexpr fork_t fork {};

				using continuation_t = relationship_t_nested::continuation_t<relationship_t>;
				static constexpr continuation_t continuation {};

				template<class NN>
				constexpr relationship_t(const NN n) noexcept
					: base{n} {}

				constexpr relationship_t() noexcept
					: base{} {}
		};

		inline constexpr relationship_t relationship {};
	}
}