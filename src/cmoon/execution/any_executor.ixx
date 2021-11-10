export module cmoon.execution.any_executor;

import <memory>;
import <type_traits>;
import <functional>;

import cmoon.property;
import cmoon.meta;
import cmoon.utility;

import cmoon.execution.executor;
import cmoon.execution.execute;
import cmoon.execution.bad_executor;

namespace cmoon::execution
{
	template<class Property>
	concept has_polymorphic_query_result_type = 
		requires
	{
		typename Property::polymorphic_query_result_type;
	};

	template<class P, class... PN>
	constexpr bool contains_property = std::disjunction_v<std::same_as<P, PN>...>;

	template<class P>
	struct convertible_helper
	{
		template<class P2>
		struct inner_t
		{
			static constexpr auto value {std::convertible_to<P, P2>};
		};
	};

	template<class P, class... PN>
		requires(cmoon::meta::type_list<PN...>::template can_find<convertible_helper<P>::inner_t>)
	using find_convertible_property = typename cmoon::meta::type_list<PN...>::template find<convertible_helper<P>::inner_t>;

	template<class E, class P>
	concept require_enable = !P::is_requirable || cmoon::can_require_v<E, P>;

	template<class E, class P>
	concept prefer_enable = !P::is_preferable || cmoon::can_prefer_v<E, P>;

	template<class E, class P>
	concept query_enable = (P::is_requirable && P::is_preferable) || cmoon::can_query_v<E, P>;

	export
	template<has_polymorphic_query_result_type... SupportableProperties>
	class any_executor
	{
		public:
			using properties = cmoon::meta::type_list<SupportableProperties...>;

			any_executor() noexcept = default;
			any_executor(std::nullptr_t) noexcept {}
			any_executor(const any_executor&) noexcept = default;
			any_executor(any_executor&&) noexcept = default;

			template<class... OtherSupportableProperties>
				requires(std::conjunction_v<contains_property<OtherSupportableProperties, SupportableProperties...>...>)
			any_executor(any_executor<OtherSupportableProperties...> e)
				: exec{std::move(e.exec)} {}

			template<executor E>
				requires(std::conjunction_v<require_enable<E, SupportableProperties>...> &&
						 std::conjunction_v<prefer_enable<E, SupportableProperties>...> &&
						 std::conjunction_v<query_enable<E, SupportableProperties>...>)
			any_executor(E e)
				: exec{create(transform_executor(std::move(e)))} {}

			any_executor& operator=(const any_executor&) noexcept = default;
			any_executor& operator=(any_executor&&) noexcept = default;
			any_executor& operator=(std::nullptr_t) noexcept
			{
				exec.reset();
			}

			template<executor E>
				requires(std::conjunction_v<require_enable<E, SupportableProperties>...> &&
						 std::conjunction_v<prefer_enable<E, SupportableProperties>...> &&
						 std::conjunction_v<query_enable<E, SupportableProperties>...>)
			any_executor& operator=(E e)
			{
				exec = create(transform_executor(std::move(e)));
				return *this;
			}

			~any_executor() noexcept = default;

			void swap(any_executor& other) noexcept
			{
				std::swap(exec, other.exec);
			}

			template<class Property>
				requires(find_convertible_property<Property, SupportableProperties...>::is_requirable)
			auto require(Property) const
			{
				using chosen_prop = find_convertible_property<Property, SupportableProperties...>;
				constexpr auto prop_loc = properties::template index<chosen_prop>;
				using changed_properties = typename properties::template sub_list<0, prop_loc>::template concatenate<
					typename properties::template sub_list<prop_loc + 1>
				>;

				return typename changed_properties::template complete_type<execution::any_executor>{*this};
			}

			template<class Property>
				requires(find_convertible_property<Property, SupportableProperties>)
			typename Property::polymorphic_query_result_type query(Property) const
			{
				// TODO: How do we do a query(e, p), if e is type-erased?
				return typename Property::polymorphic_query_result_type{};
			}

			template<class F>
			void execute(F&& f) const
			{
				std::function<void()> f2 {[f1 = cmoon::decay_copy(std::forward<F>(f))] {
					std::invoke(std::move(f1));
				}};

				exec->execute(std::move(f2));
			}

			[[nodiscard]] explicit operator bool() const noexcept
			{
				return static_cast<bool>(exec);
			}

			[[nodiscard]] friend bool operator==(const any_executor& lhs, const any_executor& rhs) noexcept = default;
			[[nodiscard]] friend bool operator!=(const any_executor& lhs, const any_executor& rhs) noexcept = default;

			[[nodiscard]] friend bool operator==(const any_executor& lhs, std::nullptr_t) noexcept
			{
				return !lhs;
			}

			[[nodiscard]] friend bool operator!=(const any_executor& lhs, std::nullptr_t) noexcept
			{
				return static_cast<bool>(lhs);
			}

			[[nodiscard]] friend bool operator==(std::nullptr_t, const any_executor& rhs) noexcept
			{
				return rhs == nullptr;
			}

			[[nodiscard]] friend bool operator!=(std::nullptr_t, const any_executor& rhs) noexcept
			{
				return rhs != nullptr;
			}
		private:
			struct interface_t
			{
				virtual void execute(std::function<void()>&& f) const = 0;
				virtual ~interface_t() {}
			};

			template<executor E>
			struct concrete_t : public interface_t
			{
				E e_;

				void execute(std::function<void()>&& f) const override
				{
					cmoon::execution::execute(e_, std::move(f));
				}
			};

			std::shared_ptr<interface_t> exec;

			template<executor E>
			static std::shared_ptr<interface_t> create(E e)
			{
				return std::make_shared<concrete_t<E>>(e);
			}

			template<executor E>
			static auto transform_executor_helper(E e)
			{
				return std::move(e);
			}

			template<executor E, class P, class... PN>
			static auto transform_executor_helper(E e)
			{
				if constexpr (cmoon::can_require_v<E, P>)
				{
					return transform_executor_helper<PN...>(cmoon::require(std::move(e), P{}));
				}
				else if constexpr (cmoon::can_prefer_v<E, P>)
				{
					return transform_executor_helper<PN...>(cmoon::prefer(std::move(e), P{}));
				}
				else
				{
					return transform_executor_helper<PN...>(std::move(e));
				}
			}

			template<executor E>
			static auto transform_executor(E e)
			{
				return transform_executor_helper<SupportableProperties...>(e);
			}
	};

	template<class... SupportableProperties>
	void swap(any_executor<SupportableProperties...>& lhs, any_executor<SupportableProperties...>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	template<class Property, class... SupportableProperties>
		requires(find_convertible_property<Property, SupportableProperties...>::is_preferable)
	auto prefer(const any_executor<SupportableProperties...>& e, Property p)
	{
		using properties = typename any_executor<SupportableProperties...>::properties;

		using chosen_prop = find_convertible_property<Property, SupportableProperties...>;
		constexpr auto prop_loc = properties::template index<chosen_prop>;
		using changed_properties = typename properties::template sub_list<0, prop_loc>::template concatenate<
			typename properties::template sub_list<prop_loc + 1>
		>;

		return typename changed_properties::template complete_type<execution::any_executor>{e};
	}
}