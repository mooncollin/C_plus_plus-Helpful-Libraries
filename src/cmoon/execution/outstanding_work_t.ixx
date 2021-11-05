export module cmoon.execution.outstanding_work_t;

import <concepts>;
import <cstddef>;

import cmoon.property;

import cmoon.execution.executor;
import cmoon.execution.impl.executor_property;

namespace cmoon::execution
{
	export
	struct outstanding_work_t
	{
		private:
			enum class value { untracked_v, tracked_v, none };
		public:
			template<class T>
			static constexpr bool is_applicable_property_v {executor<T>};

			static constexpr bool is_requirable {false};
			static constexpr bool is_preferable {false};
			using polymorphic_query_result_type = outstanding_work_t;
			[[nodiscard]] constexpr friend bool operator==(const outstanding_work_t& lhs, const outstanding_work_t& rhs) noexcept = default;

			constexpr outstanding_work_t() noexcept
				: v{value::none} {}

			struct tracked_t
			{
				static constexpr bool is_requirable {true};
				static constexpr bool is_preferable {true};
				using polymorphic_query_result_type = outstanding_work_t;

				template<class Executor>
				static constexpr auto static_query_v {inner_static_query<Executor, tracked_t>()};

				[[nodiscard]] static constexpr outstanding_work_t value() noexcept
				{
					return outstanding_work_t{tracked_t{}};
				}
			};

			struct untracked_t
			{
				static constexpr bool is_requirable {true};
				static constexpr bool is_preferable {true};
				using polymorphic_query_result_type = outstanding_work_t;

				template<class Executor>
				static constexpr auto static_query_v {first_inner_static_query<Executor, untracked_t, tracked_t>()};

				[[nodiscard]] static constexpr outstanding_work_t value() noexcept
				{
					return outstanding_work_t{untracked_t{}};
				}
			};

			constexpr outstanding_work_t(untracked_t) noexcept
				: v{value::untracked_v } {}

			constexpr outstanding_work_t(tracked_t) noexcept
				: v{value::tracked_v} {}

			static constexpr untracked_t untracked {};
			static constexpr tracked_t tracked {};

			template<class Executor, class Property>
				requires(cmoon::can_query_v<Executor, tracked_t> ||
						 cmoon::can_query_v<Executor, untracked_t>)
			friend constexpr outstanding_work_t query(const Executor& ex, const outstanding_work_t& p)
			{
				if constexpr (cmoon::can_query_v<Executor, untracked_t>)
				{
					return cmoon::query(ex, untracked_t{});
				}
				else if constexpr (cmoon::can_query_v<Executor, tracked_t>)
				{
					return cmoon::query(ex, tracked_t{});
				}
			}

			template<class Executor>
			static constexpr auto static_query_v {static_query<Executor, outstanding_work_t, untracked_t, tracked_t>()};
		private:
			value v;
	};

	export
	inline constexpr outstanding_work_t outstanding_work {};
}