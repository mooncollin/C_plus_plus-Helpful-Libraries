export module cmoon.execution.bulk_guarantee_t;

import <concepts>;
import <cstddef>;

import cmoon.property;

import cmoon.execution.executor;
import cmoon.execution.impl.executor_property;

namespace cmoon::execution
{
	export
	struct bulk_guarantee_t
	{
		private:
			enum class value { unsequenced_v, sequenced_v, parallel_v, none };
		public:
			template<class T>
			static constexpr bool is_applicable_property_v {executor<T>};

			static constexpr bool is_requirable {false};
			static constexpr bool is_preferable {false};
			using polymorphic_query_result_type = bulk_guarantee_t;
			[[nodiscard]] constexpr friend bool operator==(const bulk_guarantee_t& lhs, const bulk_guarantee_t& rhs) noexcept = default;

			constexpr bulk_guarantee_t() noexcept
				: v{value::none} {}

			struct sequenced_t
			{
				template<class T>
				static constexpr bool is_applicable_property_v {executor<T>};

				static constexpr bool is_requirable {true};
				static constexpr bool is_preferable {true};
				using polymorphic_query_result_type = bulk_guarantee_t;

				template<class Executor>
					requires(requires { inner_static_query_t<Executor, sequenced_t>::value; })
				static constexpr auto static_query_v {inner_static_query_t<Executor, sequenced_t>::value};

				[[nodiscard]] static constexpr bulk_guarantee_t value() noexcept
				{
					return bulk_guarantee_t{sequenced_t{}};
				}
			};

			struct parallel_t
			{
				template<class T>
				static constexpr bool is_applicable_property_v {executor<T>};

				static constexpr bool is_requirable {true};
				static constexpr bool is_preferable {true};
				using polymorphic_query_result_type = bulk_guarantee_t;

				template<class Executor>
				static constexpr auto static_query_v {inner_static_query<Executor, parallel_t>()};

				[[nodiscard]] static constexpr bulk_guarantee_t value() noexcept
				{
					return bulk_guarantee_t{parallel_t{}};
				}
			};

			struct unsequenced_t
			{
				template<class T>
				static constexpr bool is_applicable_property_v {executor<T>};

				static constexpr bool is_requirable {true};
				static constexpr bool is_preferable {true};
				using polymorphic_query_result_type = bulk_guarantee_t;

				template<class Executor>
					requires(requires { first_inner_static_query_t<Executor, unsequenced_t, sequenced_t, parallel_t>::value; })
				static constexpr auto static_query_v {first_inner_static_query_t<Executor, unsequenced_t, sequenced_t, parallel_t>::value};

				[[nodiscard]] static constexpr bulk_guarantee_t value() noexcept
				{
					return bulk_guarantee_t{unsequenced_t{}};
				}
			};

			constexpr bulk_guarantee_t(unsequenced_t) noexcept
				: v{value::unsequenced_v} {}

			constexpr bulk_guarantee_t(sequenced_t) noexcept
				: v{value::sequenced_v} {}

			constexpr bulk_guarantee_t(parallel_t) noexcept
				: v{value::parallel_v} {}

			static constexpr unsequenced_t unsequenced {};
			static constexpr sequenced_t sequenced {};
			static constexpr parallel_t parallel {};

			template<class Executor, class Property>
				requires(cmoon::can_query_v<Executor, sequenced_t> ||
						 cmoon::can_query_v<Executor, parallel_t> ||
						 cmoon::can_query_v<Executor, unsequenced_t>)
			friend constexpr bulk_guarantee_t query(const Executor& ex, const bulk_guarantee_t& p)
			{
				if constexpr (cmoon::can_query_v<Executor, unsequenced_t>)
				{
					return cmoon::query(ex, unsequenced_t{});
				}
				else if constexpr (cmoon::can_query_v<Executor, sequenced_t>)
				{
					return cmoon::query(ex, sequenced_t{});
				}
				else if constexpr (cmoon::can_query_v<Executor, parallel_t>)
				{
					return cmoon::query(ex, parallel_t{});
				}
			}

			template<class Executor>
				requires(requires { static_query_t<Executor, bulk_guarantee_t, unsequenced_t, sequenced_t, parallel_t>::value; })
			static constexpr auto static_query_v {static_query_t<Executor, bulk_guarantee_t, unsequenced_t, sequenced_t, parallel_t>::value};
		private:
			value v;
	};

	export
	inline constexpr bulk_guarantee_t bulk_guarantee {};
}