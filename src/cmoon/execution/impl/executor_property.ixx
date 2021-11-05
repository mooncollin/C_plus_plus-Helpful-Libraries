export module cmoon.execution.impl.executor_property;

import cmoon.meta;
import cmoon.property;

import cmoon.execution.executor;

namespace cmoon::execution
{
	export
	template<class Executor, class P, class... IP>
		requires(requires(P p) {
			Executor::query(p);
		})
	struct static_query_t
	{
		static constexpr auto value {Executor::query(P{})};
	};

	export
	template<class Executor, class P, class... IP>
		requires(!(requires(P p) {
			Executor::query(p);
		}) && !(requires(Executor e, P p) {
			e.query(p);
		}))
	struct static_query_t<Executor, P, IP...>
	{
		template<class IP1, class... IPN>
		[[nodiscard]] static constexpr auto helper() noexcept
		{
			if constexpr (requires {
				IP1::template static_query_v<Executor>;
			})
			{
				return IP1::template static_query_v<Executor>;
			}
			else
			{
				return helper<IPN...>();
			}
		}

		static constexpr auto value {helper<IP...>()};
	};

	export
	template<class Executor, class P, class... IP>
		requires(requires(P p) {
					Executor::query(p);
				 })
	struct first_inner_static_query_t
	{
		static constexpr auto value {Executor::query(P{})};
	};

	export
	template<class Executor, class P, class... IP>
		requires((!cmoon::can_query_v<Executor, IP> && ...) &&
				 !requires(Executor e, P p) { e.query(p); } &&
				 !requires(P p) {
					Executor::query(p);
				 })
	struct first_inner_static_query_t<Executor, P, IP...>
	{
		static constexpr auto value {P{}};
	};

	export
	template<class Executor, class P>
		requires(requires(P p) {
			Executor::query(p);
		})
	struct inner_static_query_t
	{
		static constexpr auto value {Executor::query(P{})};
	};
}