#pragma once

#include <type_traits>
#include <limits>

#include "cmoon/property.hpp"
#include "cmoon/execution/impl/executor.hpp"
#include "cmoon/meta.hpp"

namespace cmoon
{
	namespace execution
	{
		namespace details
		{
			template<class BaseProperty, template<class> class N>
			struct nested_property_t;

			template<class BaseProperty, class... NestedProperties>
			struct property_t
			{
				private:
					using nested_properties = cmoon::meta::type_list<NestedProperties...>;
					static constexpr auto no_value = std::numeric_limits<typename nested_properties::index_type>::max();

					typename nested_properties::index_type value_;

					template<class Executor, std::size_t I>
					[[nodiscard]] static consteval cmoon::meta::choice_t<typename nested_properties::index_type> choose_query() noexcept
					{
						using current_type = typename nested_properties::template get_type<I>;
						if constexpr (cmoon::can_query_v<Executor, current_type>)
						{
							return {I, noexcept(cmoon::query(std::declval<Executor>(), std::declval<current_type>()))};
						}
						else
						{
							return choose_query<Executor, I + 1>();
						}
					}

					template<class Executor>
					static constexpr auto query_choice = choose_query<Executor, 0>();

					template<class Executor, std::size_t I>
						requires(!(requires(Executor e, BaseProperty p) { e.query(p); }))
					[[nodiscard]] static consteval decltype(auto) static_query_helper2() noexcept
					{
						using chosen_type = typename nested_properties::template get_type<I>;
						if constexpr (requires { chosen_type::template static_query_v<Executor>; })
						{
							return chosen_type::template static_query_v<Executor>;
						}
						else
						{
							return static_query_helper2<Executor, I + 1>();
						}
					}

					template<class Executor>
					[[nodiscard]] static consteval decltype(auto) static_query_helper() noexcept
					{
						if constexpr (requires(BaseProperty p) { Executor::query(p); })
						{
							return Executor::query(BaseProperty{});
						}
						else
						{
							return static_query_helper2<Executor, 0>();
						}
					}
				public:
					[[nodiscard]] friend constexpr bool operator==(const property_t& a, const property_t& b) = default;
					[[nodiscard]] friend constexpr bool operator!=(const property_t& a, const property_t& b) = default;

					template<class T>
					static constexpr bool is_applicable_property_v = executor<T>;

					static constexpr bool is_requirable = false;
					static constexpr bool is_preferable = false;
					using polymorphic_query_result_type = BaseProperty;

					template<class Executor>
					static constexpr auto static_query_v = static_query_helper<Executor>();

					template<class NN>
						requires(nested_properties::template contains<NN>())
					constexpr property_t(const NN) noexcept
						: value_{nested_properties::template get_index<NN>()} {}

					constexpr property_t() noexcept
						: value_{no_value} {}

					template<class Executor>
						requires(std::disjunction_v<cmoon::can_query<Executor, NestedProperties>...>)
					friend constexpr BaseProperty query(const Executor& ex, const BaseProperty& p) noexcept(query_choice<Executor>.no_throw)
					{
						using chosen_nested_type = typename nested_properties::template get_type<query_choice<Executor>.strategy>;
						return cmoon::query(ex, chosen_nested_type{});
					}

					template<class B, template<class> class N>
					friend struct nested_property_t;
			};

			template<class BaseProperty, template<class> class N>
			struct nested_property_t
			{
				private:
					using base_type = N<BaseProperty>;
					using base_nested_properies = typename BaseProperty::nested_properties;
					static constexpr auto is_first_nested = base_nested_properies::template get_index<base_type>() == 0;

					template<class Executor, std::size_t I>
						requires(is_first_nested)
					[[nodiscard]] static consteval bool other_nested_properties_can_query() noexcept
					{
						if constexpr (I >= base_nested_properies::size)
						{
							return false;
						}
						else if constexpr (cmoon::can_query_v<Executor, typename base_nested_properies::template get_type<I>>)
						{
							return true;
						}
						else
						{
							return other_nested_properties_can_query<Executor, I + 1>();
						}
					}

					template<class Executor>
						requires(is_first_nested &&
								 !(requires(Executor e, base_type p) { e.query(p); }) &&
								 !other_nested_properties_can_query<Executor, 1>)
					[[nodiscard]] static consteval decltype(auto) static_query_helper2() noexcept
					{
						return base_type{};
					}

					template<class Executor>
						requires(is_first_nested)
					[[nodiscard]] static consteval decltype(auto) static_query_helper() noexcept
					{
						if constexpr (requires(base_type p) { Executor::query(p); })
						{
							return Executor::query(base_type{});
						}
						else
						{
							return static_query_helper2<Executor>();
						}
					}

					template<class Executor>
						requires(!is_first_nested && requires(base_type p) { Executor::query(p); })
					[[nodiscard]] static consteval decltype(auto) static_query_helper() noexcept
					{
						return Executor::query(base_type{});
					}
				public:
					static constexpr bool is_requirable = true;
					static constexpr bool is_preferable = true;

					using polymorphic_query_result_type = BaseProperty;

					template<class Executor>
					static constexpr auto static_query_v = static_query_helper<Executor>();

					[[nodiscard]] static constexpr BaseProperty value() noexcept
					{
						return BaseProperty{base_type{}};
					}
			};
		}
	}
}