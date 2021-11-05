export module cmoon.execution.blocking_t;

import <concepts>;
import <cstddef>;
import <atomic>;

import cmoon.property;

import cmoon.execution.executor;
import cmoon.execution.execute;
import cmoon.execution.bulk_execute;
import cmoon.execution.then;
import cmoon.execution.schedule;
import cmoon.execution.impl.executor_property;
import cmoon.execution.blocking_adaptation_t;

namespace cmoon::execution
{
	export
	struct blocking_t
	{
		private:
			enum class value { possibly_v, always_v, never_v, none };
		public:
			template<class T>
			static constexpr bool is_applicable_property_v {executor<T>};

			static constexpr bool is_requirable {false};
			static constexpr bool is_preferable {false};
			using polymorphic_query_result_type = blocking_t;
			[[nodiscard]] constexpr friend bool operator==(const blocking_t& lhs, const blocking_t& rhs) noexcept = default;

			constexpr blocking_t() noexcept
				: v{value::none} {}

			struct always_t
			{
				static constexpr bool is_requirable {true};
				static constexpr bool is_preferable {false};
				using polymorphic_query_result_type = blocking_t;

				template <class T>
				static constexpr bool is_applicable_property_v = executor<T>;

				template<class Executor>
					requires(cmoon::can_query_v<Executor, blocking_adaptation_t>)// && blocking_adaptation_t::static_query_v<Executor> == blocking_adaptation.allowed)
				friend constexpr auto require(Executor ex, always_t)
				{
					struct EX1
					{
						Executor ex;

						[[nodiscard]] static constexpr auto query(const auto& p)
						{
							return Executor::query(p);
						}

						[[nodiscard]] static constexpr auto query(const blocking_t& p)
						{
							return always_t{};
						}

						[[nodiscard]] constexpr auto require(const auto& p)
						{
							return *this;
						}

						[[nodiscard]] constexpr auto require(const blocking_adaptation_t::disallowed_t& p)
						{
							return ex;
						}

						void execute(auto&& f)
						{
							std::atomic<bool> finished {false};

							auto s = execution::schedule(ex) |
									 then(std::forward<decltype(f)>(f)) |
									 then([&finished] { finished = true; });

							execution::execute(ex, s);
							finished.wait(false);
						}

						void bulk_execute(auto&& f, auto n) requires(std::convertible_to<decltype(n), std::size_t>)
						{
							std::atomic<bool> finished {false};

							auto s = execution::bulk_execute(ex, std::forward<decltype(f)>(f), n) |
									 then([&finished] { finished = true; });

							finished.wait(false);
						}
					};

					return EX1{ex};
				}

				template<class Executor>
					requires(requires { inner_static_query_t<Executor, always_t>::value; })
				static constexpr auto static_query_v {inner_static_query_t<Executor, always_t>::value};

				[[nodiscard]] static constexpr blocking_t value() noexcept
				{
					return blocking_t{always_t{}};
				}
			};

			struct never_t
			{
				static constexpr bool is_requirable {true};
				static constexpr bool is_preferable {true};
				using polymorphic_query_result_type = blocking_t;

				template<class Executor>
				static constexpr auto static_query_v {inner_static_query<Executor, never_t>()};

				[[nodiscard]] static constexpr blocking_t value() noexcept
				{
					return blocking_t{never_t{}};
				}
			};

			struct possibly_t
			{
				static constexpr bool is_requirable {true};
				static constexpr bool is_preferable {true};
				using polymorphic_query_result_type = blocking_t;

				template<class Executor>
				static constexpr auto static_query_v {first_inner_static_query<Executor, possibly_t, always_t, never_t>()};

				[[nodiscard]] static constexpr blocking_t value() noexcept
				{
					return blocking_t{possibly_t{}};
				}
			};

			constexpr blocking_t(possibly_t) noexcept
				: v{value::possibly_v} {}

			constexpr blocking_t(always_t) noexcept
				: v{value::always_v} {}

			constexpr blocking_t(never_t) noexcept
				: v{value::never_v} {}

			static constexpr possibly_t possibly {};
			static constexpr always_t always {};
			static constexpr never_t never {};

			template<class Executor, class Property>
				requires(cmoon::can_query_v<Executor, always_t> ||
						 cmoon::can_query_v<Executor, never_t> ||
						 cmoon::can_query_v<Executor, possibly_t>)
			friend constexpr blocking_t query(const Executor& ex, const blocking_t& p)
			{
				if constexpr (cmoon::can_query_v<Executor, possibly_t>)
				{
					return cmoon::query(ex, possibly_t{});
				}
				else if constexpr (cmoon::can_query_v<Executor, always_t>)
				{
					return cmoon::query(ex, always_t{});
				}
				else if constexpr (cmoon::can_query_v<Executor, never_t>)
				{
					return cmoon::query(ex, never_t{});
				}
			}

			template<class Executor>
			static constexpr auto static_query_v {static_query<Executor, blocking_t, possibly_t, always_t, never_t>()};
		private:
			value v;
	};

	export
	inline constexpr blocking_t blocking {};
}