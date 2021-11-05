export module cmoon.execution.relationship_t;

import <concepts>;
import <cstddef>;

import cmoon.property;

import cmoon.execution.executor;
import cmoon.execution.impl.executor_property;

namespace cmoon::execution
{
	export
	struct relationship_t
	{
		private:
			enum class value { fork_v, continuation_v, none };
		public:
			template<class T>
			static constexpr bool is_applicable_property_v {executor<T>};

			static constexpr bool is_requirable {false};
			static constexpr bool is_preferable {false};
			using polymorphic_query_result_type = relationship_t;
			[[nodiscard]] constexpr friend bool operator==(const relationship_t& lhs, const relationship_t& rhs) noexcept = default;

			constexpr relationship_t() noexcept
				: v{value::none} {}

			struct continuation_t
			{
				static constexpr bool is_requirable {true};
				static constexpr bool is_preferable {true};
				using polymorphic_query_result_type = relationship_t;

				template<class Executor>
				static constexpr auto static_query_v {inner_static_query<Executor, continuation_t>()};

				[[nodiscard]] static constexpr relationship_t value() noexcept
				{
					return relationship_t{continuation_t{}};
				}
			};

			struct fork_t
			{
				static constexpr bool is_requirable {true};
				static constexpr bool is_preferable {true};
				using polymorphic_query_result_type = relationship_t;

				template<class Executor>
				static constexpr auto static_query_v {first_inner_static_query<Executor, fork_t, continuation_t>()};

				[[nodiscard]] static constexpr relationship_t value() noexcept
				{
					return relationship_t{fork_t{}};
				}
			};

			constexpr relationship_t(fork_t) noexcept
				: v{value::fork_v} {}

			constexpr relationship_t(continuation_t) noexcept
				: v{value::continuation_v} {}

			static constexpr fork_t fork {};
			static constexpr continuation_t continuation {};

			template<class Executor, class Property>
				requires(cmoon::can_query_v<Executor, continuation_t> ||
						 cmoon::can_query_v<Executor, fork_t>)
			friend constexpr relationship_t query(const Executor& ex, const relationship_t& p)
			{
				if constexpr (cmoon::can_query_v<Executor, fork_t>)
				{
					return cmoon::query(ex, fork_t{});
				}
				else if constexpr (cmoon::can_query_v<Executor, continuation_t>)
				{
					return cmoon::query(ex, continuation_t{});
				}
			}

			template<class Executor>
			static constexpr auto static_query_v {static_query<Executor, relationship_t, fork_t, continuation_t>()};
		private:
			value v;
	};

	export
	inline constexpr relationship_t relationship {};
}