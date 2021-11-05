export module cmoon.execution.blocking_adaptation_t;

import <concepts>;

import cmoon.property;

import cmoon.execution.executor;
import cmoon.execution.impl.executor_property;

namespace cmoon::execution
{
	export
	struct blocking_adaptation_t
	{
		private:
			enum class value { disallowed_v, allowed_v, none };
		public:
			template<class T>
			static constexpr bool is_applicable_property_v {executor<T>};

			static constexpr bool is_requirable {false};
			static constexpr bool is_preferable {false};
			using polymorphic_query_result_type = blocking_adaptation_t;
			[[nodiscard]] constexpr friend bool operator==(const blocking_adaptation_t& lhs, const blocking_adaptation_t& rhs) noexcept = default;

			constexpr blocking_adaptation_t() noexcept
				: v{value::none} {}

			struct disallowed_t;

			struct allowed_t
			{
				static constexpr bool is_requirable {true};
				static constexpr bool is_preferable {false};
				using polymorphic_query_result_type = blocking_adaptation_t;

				template <class T>
				static constexpr bool is_applicable_property_v = executor<T>;

				template<class Executor>
				friend constexpr auto require(Executor ex, allowed_t)
				{
					struct EX1
					{
						Executor ex;

						[[nodiscard]] static constexpr auto query(const auto& p)
						{
							return Executor::query(p);
						}

						[[nodiscard]] static constexpr auto query(const blocking_adaptation_t& p)
						{
							return allowed_t{};
						}

						[[nodiscard]] constexpr auto require(const auto& p)
						{
							return *this;
						}

						[[nodiscard]] constexpr auto require(const disallowed_t& p)
						{
							return ex;
						}
					};

					return EX1{ex};
				}

				template<class Executor>
					requires(requires { inner_static_query_t<Executor, allowed_t>::value; })
				static constexpr auto static_query_v {inner_static_query_t<Executor, allowed_t>::value};

				[[nodiscard]] static constexpr blocking_adaptation_t value() noexcept
				{
					return blocking_adaptation_t{allowed_t{}};
				}
			};

			struct disallowed_t
			{
				static constexpr bool is_requirable {true};
				static constexpr bool is_preferable {true};
				using polymorphic_query_result_type = blocking_adaptation_t;

				template<class Executor>
				static constexpr auto static_query_v {first_inner_static_query<Executor, disallowed_t, allowed_t>()};

				[[nodiscard]] static constexpr blocking_adaptation_t value() noexcept
				{
					return blocking_adaptation_t{disallowed_t{}};
				}
			};

			constexpr blocking_adaptation_t(disallowed_t) noexcept
				: v{value::disallowed_v} {}

			constexpr blocking_adaptation_t(allowed_t) noexcept
				: v{value::allowed_v} {}

			static constexpr disallowed_t disallowed {};
			static constexpr allowed_t allowed {};

			template<class Executor>
				requires(cmoon::can_query_v<Executor, allowed_t> ||
						  cmoon::can_query_v<Executor, disallowed_t>)
			constexpr friend blocking_adaptation_t query(const Executor& ex, const blocking_adaptation_t& p)
			{
				if constexpr (cmoon::can_query_v<Executor, disallowed_t>)
				{
					return cmoon::query(ex, disallowed_t{});
				}
				else if constexpr (cmoon::can_query_v<Executor, allowed_t>)
				{
					return cmoon::query(ex, allowed_t{});
				}
			}

			template<class Executor>
				requires(requires { static_query_t<Executor, blocking_adaptation_t, disallowed_t, allowed_t>::value; })
			static constexpr auto static_query_v {static_query_t<Executor, blocking_adaptation_t, disallowed_t, allowed_t>::value};
		private:
			value v;
	};

	export
	inline constexpr blocking_adaptation_t blocking_adaptation {};
}