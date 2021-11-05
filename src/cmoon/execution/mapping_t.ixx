export module cmoon.execution.mapping_t;

import <concepts>;
import <cstddef>;

import cmoon.property;

import cmoon.execution.executor;
import cmoon.execution.impl.executor_property;

namespace cmoon::execution
{
	export
	struct mapping_t
	{
		private:
			enum class value { thread_v, new_thread_v, other_v, none };
		public:
			template<class T>
			static constexpr bool is_applicable_property_v {executor<T>};

			static constexpr bool is_requirable {false};
			static constexpr bool is_preferable {false};
			using polymorphic_query_result_type = mapping_t;
			[[nodiscard]] constexpr friend bool operator==(const mapping_t& lhs, const mapping_t& rhs) noexcept = default;

			constexpr mapping_t() noexcept
				: v{value::none} {}

			struct new_thread_t
			{
				static constexpr bool is_requirable {true};
				static constexpr bool is_preferable {true};
				using polymorphic_query_result_type = mapping_t;

				template<class Executor>
				static constexpr auto static_query_v {inner_static_query<Executor, new_thread_t>()};

				[[nodiscard]] static constexpr mapping_t value() noexcept
				{
					return mapping_t{new_thread_t{}};
				}
			};

			struct other_t
			{
				static constexpr bool is_requirable {true};
				static constexpr bool is_preferable {true};
				using polymorphic_query_result_type = mapping_t;

				template<class Executor>
				static constexpr auto static_query_v {inner_static_query<Executor, other_t>()};

				[[nodiscard]] static constexpr mapping_t value() noexcept
				{
					return mapping_t{other_t{}};
				}
			};

			struct thread_t
			{
				static constexpr bool is_requirable {true};
				static constexpr bool is_preferable {true};
				using polymorphic_query_result_type = mapping_t;

				template<class Executor>
				static constexpr auto static_query_v {first_inner_static_query<Executor, thread_t, new_thread_t, other_t>()};

				[[nodiscard]] static constexpr mapping_t value() noexcept
				{
					return mapping_t{thread_t{}};
				}
			};

			constexpr mapping_t(thread_t) noexcept
				: v{value::thread_v} {}

			constexpr mapping_t(new_thread_t) noexcept
				: v{value::new_thread_v} {}

			constexpr mapping_t(other_t) noexcept
				: v{value::other_v} {}

			static constexpr thread_t thread {};
			static constexpr new_thread_t new_thread {};
			static constexpr other_t other {};

			template<class Executor, class Property>
				requires(cmoon::can_query_v<Executor, new_thread_t> ||
						 cmoon::can_query_v<Executor, other_t> ||
						 cmoon::can_query_v<Executor, thread_t>)
			friend constexpr mapping_t query(const Executor& ex, const mapping_t& p)
			{
				if constexpr (cmoon::can_query_v<Executor, thread_t>)
				{
					return cmoon::query(ex, thread_t{});
				}
				else if constexpr (cmoon::can_query_v<Executor, new_thread_t>)
				{
					return cmoon::query(ex, new_thread_t{});
				}
				else if constexpr (cmoon::can_query_v<Executor, other_t>)
				{
					return cmoon::query(ex, other_t{});
				}
			}

			template<class Executor>
			static constexpr auto static_query_v {static_query<Executor, mapping_t, thread_t, new_thread_t, other_t>()};
		private:
			value v;
	};

	export
	inline constexpr mapping_t mapping {};
}