export module cmoon.execution.any_executor;

import <type_traits>;
import <concepts>;

import cmoon.property;
import cmoon.utility;

import cmoon.execution.find_convertible_property;
import cmoon.execution.bad_executor;

namespace cmoon::execution
{
	export
	template<class... SupportableProperties>
	class any_executor
	{
		template<class Property>
		static constexpr auto contains_property_v = std::disjunction_v<std::same_as<Property, SupportableProperties>...>;

		template<class Executor>
		static constexpr auto require_property_check = std::conjunction_v<std::conditional_t<SupportableProperties::is_requirable,
																				std::bool_constant<cmoon::can_require_v<Executor, SupportableProperties>>,
																				std::true_type>::value...>;

		template<class Executor>
		static constexpr auto prefer_property_check = std::conjunction_v<std::conditional_t<SupportableProperties::is_preferable,
																				std::bool_constant<cmoon::can_prefer_v<Executor, SupportableProperties>>,
																				std::true_type>::value...>;

		template<class Executor>
		static constexpr auto query_property_check = std::conjunction_v<std::conditional_t<!SupportableProperties::is_requirable && !SupportableProperties::is_preferable,
																				std::bool_constant<cmoon::can_require_v<Executor, SupportableProperties>>,
																				std::true_type>::value...>;

		template<class Property, class... Props>
			requires(sizeof...(Props) >= 1)
		struct find_requirable_property;

		template<class Property, class Prop1, class PropN>
		struct find_requirable_property<Property, Prop1, PropN...> : public std::conditional_t<(std::same_as<Property, Prop1> || std::convertible_to<Property, Prop1>) && Prop1::is_requirable,
																								std::type_identity<Prop1>,
																								find_requirable_property<Property, PropN...>> {};

		template<class Property, class... Props>
			requires(sizeof...(Props) >= 1)
		struct find_preferable_property;

		template<class Property, class Prop1, class PropN>
		struct find_preferable_property<Property, Prop1, PropN...> : public std::conditional_t<(std::same_as<Property, Prop1> || std::convertible_to<Property, Prop1>) && Prop1::is_preferable,
																								std::type_identity<Prop1>,
																								find_requirable_property<Property, PropN...>> {};

		public:
			any_executor() noexcept = default;
			any_executor(std::nullptr_t) noexcept {}
			any_executor(const any_executor& e) noexcept = default;
			any_executor(any_executor&& e) noexcept = default;

			template<class... OtherSupportableProperties>
				requires(std::conjunction_v<contains_property_v<SupportableProperties>...>)
			any_executor(any_executor<OtherSupportableProperties...> e)
				: target_{std::move(e.target_)} {}

			template<class... OtherSupportableProperties>
				requires(std::negation_v<std::conjunction<contains_property_v<SupportableProperties>...>>)
			any_executor(any_executor<OtherSupportableProperties...> e) = delete;

			template<executor Executor>
				requires(require_property_check<Executor> && prefer_property_check<Executor> && query_property_check<Executor>)
			any_executor(Executor e)
				: target_{create_shared(std::move(e))} {}

			any_executor& operator=(const any_executor& e) noexcept = default;
			any_executor& operator=(any_executor&& e) noexcept = default;
			any_executor& operator=(std::nullptr_t) noexcept
			{
				target_.reset();
				return *this;
			}

			template<executor Executor>
			any_executor& operator=(Executor e)
			{
				target_ = create_shared(std::move(e));
				return *this;
			}

			~any_executor() = default;

			void swap(any_executor& other) noexcept
			{
				target_.swap(other.target_);
			}

			template<class Property>
				requires(requires { find_requirable_property<Property, SupportableProperties...>; })
			any_executor require(Property p) const
			{
				// TODO
				//return any_executor{cmoon::require(/* target of this executor */, p)};
			}

			template<class Property>
				requires(requires { find_preferable_property<Property, SupportableProperties...>; })
			any_executor prefer(Property p) const
			{
				// TODO
				//return any_executor{cmoon::prefer(/* target of this executor */, p)};
			}

			template<class Property>
			auto query(Property p) const
			{
				// TODO
				//if constexpr (requires { find_convertible_property<Property, SupportableProperties...>; })
				//{
					//return static_cast<Property::polymorphic_query_result_type>(cmoon::query(/* target of this executor */, p));
				//}
				//else
				//{
					//return Property::polymorphic_query_result_type{};
				//}
			}

			template<class Function>
			void execute(Function&& f) const
			{
				if (!(*this))
				{
					throw bad_executor{};
				}

				target_->execute(std::forward<Function>(f));
			}

			explicit operator bool() const noexcept
			{
				return static_cast<bool>(target_);
			}

			const type_info& target_type() const noexcept
			{
				if (*this)
				{
					return target_->type();
				}

				return typeid(void);
			}

			template<executor Executor>
			Executor* target() noexcept
			{
				if (target_type() == typeid(Executor))
				{
					return reinterpret_cast<Executor*>(target_->target());
				}

				return nullptr;
			}

			template<executor Executor>
			const Executor* target() const noexcept
			{
				if (target_type() == typeid(Executor))
				{
					return reinterpret_cast<const Executor*>(target_->target());
				}

				return nullptr;
			}

			[[nodiscard]] friend bool operator==(const any_executor& a, const any_executor& b) noexcept
			{
				return (!a && !b) ||
						(a == b) ||
						(*(a.target_) == *(b.target_));
			}

			[[nodiscard]] friend bool operator==(const any_executor& e, std::nullptr_t) noexcept
			{
				return !e;
			}

			[[nodiscard]] friend bool operator==(std::nullptr_t, const any_executor& e) noexcept
			{
				return !e;
			}

			[[nodiscard]] friend bool operator!=(const any_executor& a, const any_executor& b) noexcept
			{
				return !(a == b);
			}

			[[nodiscard]] friend bool operator!=(const any_executor& e, std::nullptr_t) noexcept
			{
				return static_cast<bool>(e);
			}

			[[nodiscard]] friend bool operator!=(std::nullptr_t, const any_executor& e) noexcept
			{
				return static_cast<bool>(e);
			}
		private:
			struct executor_interface
			{
				virtual void execute(const std::function<void()>& f) = 0;
				virtual void execute(std::function<void()>&& f) = 0;
				virtual const type_info& type() const = 0;
				virtual void* target() = 0;
				virtual const void* target() const = 0;
				virtual bool operator==(const executor_interface&) const noexcept = 0;
				virtual ~executor_interface() {}
			};

			template<class Executor>
			struct specific_executor : public executor_interface
			{
				specific_executor(Executor&& e)
					: e_{std::forward<Executor>(e)} {}

				void execute(const std::function<void()>& f) override
				{
					execution::execute(e_, f);
				}

				void execute(std::function<void()>&& f) override
				{
					execution::execute(e_, std::move(f));
				}

				const type_info& type() const override
				{
					return typeid(Executor);
				}

				void* target() override
				{
					return static_cast<void*>(std::addressof(e_));
				}

				const void* target() const override
				{
					return static_cast<const void*>(std::addressof(e_));
				}

				bool operator==(const executor_interface& other) const noexcept override
				{
					return (type() == other.type()) && e_ == *(reinterpret_cast<const Executor*>(other.target()));
				}
						
				Executor e_;
			};

			template<class Executor>
			[[nodiscard]] static std::shared_ptr<executor_interface> create_shared(Executor&& e)
			{
				return std::make_shared<specific_executor<Executor>>(std::forward<Executor>(e));
			}

			std::shared_ptr<executor_interface> target_ {nullptr};
	};

	template<class Property, class... SupportableProperties>
		requires(requires { find_convertible_property<Property, SupportableProperties...>::is_preferable; } && 
					find_convertible_property<Property, SupportableProperties...>::is_preferable)
	any_executor<SupportableProperties...> prefer(const any_executor<SupportableProperties...>& e, Property p)
	{
		// TODO
	}
}

namespace std
{
	template<class... SupportableProperties>
	void swap(cmoon::execution::any_executor<SupportableProperties...>& a, cmoon::execution::any_executor<SupportableProperties...>& b) noexcept
	{
		a.swap(b);
	}
}