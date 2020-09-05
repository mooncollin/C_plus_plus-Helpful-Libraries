#pragma once

#include <optional>
#include <any>
#include <concepts>
#include <type_traits>
#include <stdexcept>

#include "collin/property.hpp"
#include "collin/utility.hpp"
#include "collin/concepts.hpp"
#include "collin/type_traits.hpp"

namespace collin
{
	namespace execution
	{
		class bad_executor : public std::exception
		{
			public:
				bad_executor() noexcept = default;

				const char* what() const noexcept final
				{
					return "bad executor";
				}
		};

		template<class A>
		concept proto_allocator = std::copy_constructible<A> &&
			requires
		{
			typename std::allocator_traits<A>;
		};

		namespace details
		{
			struct invocable_archetype_impl
			{
				invocable_archetype_impl() = delete;

				template<class... Args>
				auto operator()(Args&&...)
				{

				}
			};
		}

		using invocable_archetype = details::invocable_archetype_impl;

		namespace details
		{
			template<class T>
			concept is_nothrow_move_or_copy_constructible = std::is_nothrow_move_constructible_v<T> &&
															std::copy_constructible<T>;

			template<class S, class R>
			concept sender_to_impl =
				requires(S && s, R && r)
			{
				execution::submit(std::forward<S>(s), std::forward<R>(r));
			};

			template<class E, class F>
			concept executor_of_impl = std::invocable<F> &&
									   std::is_nothrow_constructible_v<E> &&
									   std::is_nothrow_destructible_v<E> &&
									   std::equality_comparable<E> &&
				requires(const E& e, F&& f)
			{
				execution::execute(e, std::forward<F>(f));
			};
		}

		template<class E>
		concept executor = details::executor_of_impl<E, execution::invocable_archetype>;

		template<class E, class F>
		concept executor_of = details::executor_of_impl<E, F>;

		template<class S>
		concept sender = std::move_constructible<std::remove_cvref_t<S>> &&
						 details::sender_to_impl<S, int>;

		template<class S, class R>
		concept sender_to = sender<S> &&	
							receiver<R> &&
							details::sender_to_impl<S, R>;


		template<class T, class E = std::exception_ptr>
		concept receiver = std::move_constructible<std::remove_cvref_t<T>> &&
			details::is_nothrow_move_or_copy_constructible<std::remove_cvref_t<T>> &&
			requires(T && t, E && e)
		{
			{ execution::set_done(std::forward<T>(t)) } noexcept;
			{ execution::set_error(std::forward<T>(t), std::forward<E>(e)) } noexcept;
		};

		template<class T, class... An>
		concept receiver_of = receiver<T> &&
			requires(T&& t, An&&... an)
		{
			execution::set_value(std::forward<T>(t), std::forward<An>(an)...);
		};

		namespace details
		{
			template<class S>
			struct sender_traits_base {};
		}

		template<class S>
		struct sender_traits : details::sender_traits_base<S> {};

		template<class S>
		requires(!std::same_as<S, std::remove_cvref_t<S>>)
			struct details::sender_traits_base<S> : sender_traits<std::remove_cvref_t<S>> {};

		template<class S>
			requires(std::same_as<S, std::remove_cvref_t<S>> && sender<S>)
		struct details::sender_traits_base<S>
		{
			template<template<class...> class Tuple, template<class...> class Variant>
			using value_types = typename S::template value_types<Tuple, Variant>;

			template<template<class...> class Variant>
			using error_types = typename S::template error_types<Variant>;

			static constexpr bool sends_done = S::sends_done;
		};

		class sink_receiver;

		namespace details
		{
			template<class Executor, class Property>
			concept property_static_query = executor<Executor> &&
				requires
			{
				Property::template static_query_v<Executor>;
			};

			template<executor Executor, class Property, class... NestedProperties>
				requires(requires{ Executor::query(std::declval<Property>()); })
			constexpr auto get_static_query_v()
			{
				return Executor::query(Property{});
			}

			template<std::size_t Current, executor Executor, class Property, class... NestedProperties>
				requires(property_static_query<Executor, std::tuple_element_t<Current, std::tuple<NestedProperties...>>>)
			constexpr auto get_static_query_v()
			{
				using current_property = std::tuple_element_t<Current, std::tuple<NestedProperties...>>;
				return current_property::static_query_v<Executor>;
			}

			template<std::size_t Current, executor Executor, class Property, class... NestedProperties>
				requires(!property_static_query<Executor, std::tuple_element_t<Current, std::tuple<NestedProperties...>>>)
			constexpr auto get_static_query_v()
			{
				return get_static_query_v<Current + 1, Executor, Property, NestedProperties...>();
			}

			template<executor Executor, class Property, class... NestedProperties>
				requires(!(requires{ std::declval<Executor>().query(std::declval<std::tuple_element_t<0, std::tuple<NestedProperties...>>>()); }))
			constexpr auto get_static_query_v()
			{
				return get_static_query_v<0, Executor, Property, NestedProperties...>();
			}

			template<executor Executor, class Property>
			constexpr auto get_static_query_v()
			{
				return Property{};
			}

			template<executor Executor, std::size_t Index, class Derived, class... NestedProperties>
				requires(collin::can_query_v<Executor, std::tuple_element_t<Index, std::tuple<NestedProperties...>>>)
			constexpr Derived query_helper(const Executor& ex) noexcept(noexcept(collin::query(ex, std::declval<std::tuple_element_t<Index, std::tuple<NestedProperties...>>>())))
			{
				using current_property = std::tuple_element_t<Index, std::tuple<NestedProperties...>>;
				return collin::query(ex, current_property{});
			}

			template<executor Executor, std::size_t Index, class Derived, class... NestedProperties>
				requires(!collin::can_query_v<Executor, std::tuple_element_t<Index, std::tuple<NestedProperties...>>>)
			constexpr Derived query_helper(const Executor& ex) noexcept(noexcept(query_helper<Index + 1, Derived, NestedProperties...>(ex)))
			{
				return query_helper<Index + 1, Derived, NestedProperties...>(ex);
			}

			template<class Derived, class... NestedProperties>
			struct behavioral_property_base
			{
				template<class T>
				static constexpr bool is_applicable_property_v = executor<T>;

				static constexpr bool is_requirable = false;
				static constexpr bool is_preferable = false;
				using polymorphic_query_result_type = Derived;

				template<executor Executor>
				static constexpr auto static_query_v = get_static_query_v<Executor, Derived, NestedProperties...>();

				template<executor Executor, std::same_as<Derived> Property>
				friend constexpr Derived query(const Executor& ex, const Property&) noexcept(noexcept(query_helper<0, Derived, NestProperties...>(ex)))
				{
					return query_helper<0, Derived, NestProperties...>(ex);
				}

				friend constexpr bool operator==(const Derived& a, const Derived& b);
				friend constexpr bool operator!=(const Derived& a, const Derived& b)
				{
					return !(a == b);
				}

				constexpr behavioral_property_base() = default;
			};
		}

		constexpr struct context_t
		{
			template<class T>
			static constexpr bool is_applicable_property_v = executor<T>;

			static constexpr bool is_requirable = false;
			static constexpr bool is_preferable = false;

			using polymorphic_query_result_type = std::any;

			template<class Executor>
			static constexpr auto static_query_v = Executor::query(context_t{});

		} context{};

		constexpr struct blocking_t
		{
			template<class T>
			static constexpr bool is_applicable_property_v = executor<T>;
			static constexpr bool is_requirable = false;
			static constexpr bool is_preferable = false;
			using polymorphic_query_result_type = blocking_t;

			static constexpr struct possibly_t
			{
				static constexpr bool is_requirable = true;
				static constexpr bool is_preferable = true;
				using polymorphic_query_result_type = blocking_t;

				template<executor Executor>
				static constexpr auto static_query_v = details::get_static_query_v<Executor, possibly_t>();

				static constexpr blocking_t value()
				{
					return blocking_t{possibly_t{}};
				}
			} possibly{};
			static constexpr struct always_t
			{
				static constexpr bool is_requirable = true;
				static constexpr bool is_preferable = false;

				template<class T>
				static constexpr bool is_applicable_property_v = executor<T>;

				template<class Executor>
				friend constexpr auto require(Executor ex, blocking_t::always_t)
				{
					// TODO
				}
			} always{};
			static constexpr struct never_t{} never{};

			template<executor Executor>
			static constexpr auto static_query_v = details::get_static_query_v<Executor, blocking_t, possibly_t, always_t, never_t>();

			template<executor Executor, std::same_as<blocking_t> Property>
			friend constexpr blocking_t query(const Executor& ex, const Property&) noexcept(noexcept(query_helper<0, blocking_t, possibly_t, always_t, never_t>(ex)))
			{
				return details::query_helper<0, Derived, possibly_t, always_t, never_t>(ex);
			}

			friend constexpr bool operator==(const blocking_t& a, const blocking_t& b);
			friend constexpr bool operator!=(const blocking_t& a, const blocking_t& b)
			{
				return !(a == b);
			}

			constexpr blocking_t() = default;
			constexpr blocking_t(const possibly_t) {}
			constexpr blocking_t(const always_t) {}
			constexpr blocking_t(const never_t) {}

		} blocking{};

		constexpr struct blocking_adaptation_t
		{
			static constexpr struct disallowed_t{} disallowed{};
			static constexpr struct allowed_t
			{
				static constexpr bool is_requirable = true;
				static constexpr bool is_preferable = false;

				template<class T>
				static constexpr bool is_applicable_property_v = executor<T>;

				template<class Executor>
				friend constexpr auto require(Executor ex, blocking_adaptation_t::allowed_t)
				{
					// TODO
				}
			} allowed{};
		} blocking_adaptation{};

		constexpr struct relationship_t
		{
			static constexpr struct fork_t{} fork{};
			static constexpr struct continuation_t{} continuation{};
		} relationship{};

		constexpr struct outstanding_work_t
		{
			static constexpr struct untracked_t{} untracked{};
			static constexpr struct tracked_t{} tracked{};
		} outstanding_work{};

		constexpr struct bulk_guarantee_t
		{
			static constexpr struct unsequenced_t{} unsequenced{};
			static constexpr struct sequenced_t{} sequenced{};
			static constexpr struct parallel_t{} parallel{};
		} bulk_guarantee{};

		constexpr struct mapping_t
		{
			static constexpr struct thread_t{} thread{};
			static constexpr struct new_thread_t{} new_thread{};
			static constexpr struct other_t{} other{};
		} mapping{};

		// TODO: Figure out what this is trying to do
		template<proto_allocator ProtoAllocator>
		struct allocator_t
		{
			template<class T>
			static constexpr bool is_applicable_property_v = executor<T>;

			static constexpr bool is_requirable = true;
			static constexpr bool is_preferable = true;

			template<class Executor>
			static constexpr auto static_query_v = Executor::query(allocator_t);

			template<proto_allocator OtherProtoAllocator>
			allocator_t<OtherProtoAllocator> operator()(const OtherProtoAllocator& a) const
			{
				return allocator_t{a};
			}

			constexpr ProtoAllocator value() const
			{
				return a_;
			}
			private:
				ProtoAllocator a_;
		};

		template<executor Executor>
		struct executor_shape
		{
			private:
				template<class T>
				using helper = typename T::shape_type;

			public:
				using type = collin::type_traits::detected_or_t<
					std::size_t, 
					helper,
					decltype(collin::require(std::declval<const Executor&>(), execution::bulk))
				>;

			static_assert(std::integral<type>, "shape type must be an integral type");
		};

		template<executor Executor>
		using executor_shape_t = typename executor_shape<Executor>::type;

		template<executor Executor>
		struct executor_index
		{
			private:
				template<class T>
				using helper = typename T::index_type;

			public:
				using type = collin::type_traits::detected_or_t<
					executor_shape_t<Executor>,
					helper,
					decltype(collin::require(std::declval<const Executor&>(), execution::bulk))
				>;

				static_assert(std::integral<type>, "index type must be an integral type");
		};

		template<executor Executor>
		using executor_index_t = typename executor_index<Executor>::type;

		inline namespace unspecified
		{
			struct set_value_impl
			{
				template<class R, class... Vs>
					requires(requires(R&& r, Vs&&... as) { std::forward<R>(r).set_value(std::forward<Vs>(as)...); })
				constexpr void operator()(R&& r, Vs&&... as) const noexcept(noexcept(std::forward<R>(r).set_value(std::forward<Vs>(as)...)))
				{
					std::forward<R>(r).set_value(std::forward<Vs>(as)...);
				}
			};

			struct set_done_impl
			{
				template<class R>
					requires(requires(R&& r) { {std::forward<R>(r).set_done()} noexcept; })
				constexpr void operator()(R&& r) noexcept
				{
					std::forward<R>(r).set_done();
				}
			};

			struct set_error_impl
			{
				template<class R, class E>
					requires(requires(R&& r, E&& e) { {std::forward<R>(r).set_error(std::forward<E>(e))} noexcept; })
				constexpr void operator()(R&& r, E&& e) noexcept
				{
					std::forward<R>(r).set_error(std::forward<E>(e));
				}
			};

			template<std::invocable F>
			struct as_receiver
			{
				private:
					using invocable_type = std::remove_cvref_t<F>;
					invocable_type f;
				public:
					explicit as_receiver(invocable_type&& f)
						: f{std::move_if_noexcept(f)} {}

					explicit as_receiver(const invocable_type& f)
						: f{f} {}

					as_receiver(as_receiver&&) = default;

					void set_value()
					{
						std::invoke(f);
					}

					[[noreturn]] void set_error(std::exception_ptr) noexcept
					{
						std::terminate();
					}

					void set_done() noexcept {}
			};

			template<receiver R>
			struct as_invocable
			{
				private:
					using receiver_type = std::remove_cvref_t<R>;
					std::optional<receiver_type> r_{};

					template<class T>
					void try_init(T&& r)
					{
						try
						{
							r_.emplace(std::forward<T>(r));
						}
						catch (...)
						{
							execution::set_error(r, std::current_exception());
						}
					}
				public:
					explicit as_invocable(receiver_type&& r)
					{
						try_init(std::move_if_noexcept(r));
					}

					explicit as_invocable(const receiver_type& r)
					{
						try_init(r);
					}

					as_invocable(as_invocable&& other)
					{
						if (other.r_)
						{
							try_init(std::move_if_noexcept(other.r_.value()));
							other.r_.reset();
						}
					}

					~as_invocable()
					{
						if (r_)
						{
							execution::set_done(r_.value());
						}
					}

					void operator()()
					{
						try
						{
							execution::set_value(r_.value());
						}
						catch (...)
						{
							execution::set_error(r_.value(), std::current_exception());
						}
						r_.reset();
					}
			};

			struct submit_impl
			{
				template<sender S, receiver R>
					requires(requires(S&& s, R&& r) { std::forward<S>(s).submit(std::forward<R>(r)); })
				constexpr void operator()(S&& s, R&& r)
				{
					std::forward<S>(s).submit(std::forward<R>(r));
				}

				template<class S, class R>
					requires(executor_of<S, R>)
				constexpr void operator()(S&& s, R&& r)
				{
					execution::execute(std::forward<S>(s), as_invocable<R>{std::forward<R>(r)});
				}
			};

			struct execute_impl
			{
				template<class E, std::invocable F>
					requires(requires(E&& e, F&& f) { std::forward<E>(e).execute(std::forward<F>(f)); })
				constexpr void operator()(F&& f, E&& e)
				{
					std::forward<E>(e).execute(std::forward<F>(f));
				}

				template<class E, std::invocable F>
					requires(sender_to<E, as_receiver<F>>)
				constexpr void operator()(E&& e, F&& f)
				{
					execution::submit(std::forward<E>(e), as_receiver<F>(std::forward<F>(f)));
				}
			};

			struct schedule_impl
			{
				template<class S>
					requires(requires(S&& s) { { std::forward<S>(s).schedule() } -> sender; })
				constexpr void operator()(S&& s)
				{
					std::forward<S>(s).schedule();
				}
			};

			struct bulk_execute_impl
			{
				template<class S, class F, class N>
					requires(std::convertible_to<N, std::size_t> &&
								requires(S&& s, F&& f, N&& n) { std::forward<S>(s).bulk_execute(std::forward<F>(f), std::forward<N>(n)); collin::query(s, execution::bulk_guarantee); })
				constexpr void operator()(S&& s, F&& f, N&& n)
				{
					std::forward<S>(s).bulk_execute(std::forward<F>(f), std::forward<N>(n));
				}

				template<class S, class F, class N>
					requires(std::convertible_to<N, std::size_t> &&
								std::invocable<F, executor_index_t<std::remove_cvref_t<S>>> &&
								collin::query(std::declval<S>(), execution::bulk_guarantee) == execution::bulk_guarantee.unsequenced)
				constexpr void operator()(S&& s, F&& f, N&& n)
				{
					// TODO
					auto cf = collin::decay_copy<F>(std::forward<F>(f));
					for(std::size_t i{0}; i < n; ++i)
					{
						cf(i);
					}
				}
			};

			constexpr auto set_value = set_value_impl{};
			constexpr auto set_done = set_done_impl{};
			constexpr auto set_error = set_error_impl{};
			constexpr auto submit = submit_impl{};
			constexpr auto execute = execute_impl{};
			constexpr auto schedule = schedule_impl{};
			constexpr auto bulk_execute = bulk_execute_impl{};
		}

		namespace details
		{
			template<template<template<class...> class Tuple, template<class...> class Variant> class>
			struct has_value_types;

			template<template<class...> class Variant>
			struct has_error_types;

			template<class S>
			concept has_sender_types =
				requires
			{
				typename has_value_types<S::template value_types>;
				typename has_error_types<S::template error_types>;
				typename std::bool_constant<S::sends_done>;
			};
		}

		template<class S>
		concept typed_sender = sender<S> &&
							   details::has_sender_types<sender_traits<S>>;

		template<class S>
		concept scheduler = std::copy_constructible<std::remove_cvref_t<S>> &&
							std::copy_constructible<std::remove_cvref_t<S>> &&
			requires(S&& s)
		{
			execution::schedule(std::forward<S>(s));
		};

		class sink_receiver
		{
			public:
				template<class... T>
				[[noreturn]] void set_value(T&&...) noexcept
				{
					std::terminate();
				}

				void set_done() noexcept {}
		};

		namespace details
		{
			template<typename P, typename P0, typename... Pn>
			struct find_convertible_property
			{
				using type = std::conditional_t<std::is_convertible_v<P, P0>, P, typename find_convertible_property<P, Pn...>::type>;
			};

			template<typename P, typename... Pn>
			concept contains_property = std::disjunction_v<std::same_as<P, Pn>...>;
		}

		template<class... SupportableProperties>
		class any_executor
		{
			public:
				any_executor() noexcept = default;
				any_executor(nullptr_t) noexcept {}
				any_executor(const any_executor& e) noexcept = default;
				any_executor(any_executor&& e) noexcept = default;

				template<class... OtherSupportableProperties>
					requires((details::contains_property<SupportableProperties, OtherSupportableProperties...> && ...))
				any_executor(any_executor<OtherSupportableProperties...> e)
					: target_{std::move(e.target_)} {}

				template<executor Executor>
					requires(
							 ((
							 (!SupportableProperties::is_requirable || SupportableProperties::is_requirable && collin::can_require_v<Executor, SupportableProperties>) &&
							 (!SupportableProperties::is_preferable || SupportableProperties::is_preferable && collin::can_prefer_v<Executor, SupportableProperties>) &&
							 (!SupportableProperties::is_requirable || SupportableProperties::is_requirable && collin::can_query_v<Executor, SupportableProperties>)
							 ) && ...)
							)
				any_executor(Executor e)
					: target_{create<Executor>()} {}

				any_executor& operator=(const any_executor& e) noexcept = default;
				any_executor& operator=(any_executor&& e) noexcept = default;
				any_executor& operator=(nullptr_t) noexcept
				{
					target_.reset();
					return *this;
				}

				template<executor Executor>
				any_executor& operator=(Executor e)
				{
					target_ = create<Executor>();
					return *this;
				}

				~any_executor() = default;

				void swap(any_executor& other) noexcept
				{
					target_.swap(other.target_);
				}

				template<class Property>
					requires(requires { details::find_convertible_property<Property, SupportableProperties...>::type::is_requirable; } && details::find_convertible_property<Property, SupportableProperties...>::type::is_requirable)
				any_executor require(Property) const
				{
					// TODO: Extract correct type from this executor for require()
				}

				template<class Property>
				typename Property::polymorphic_query_result_type query(Property) const
				{
					// TODO: Extract correct type from this executor for query()
				}

				template<class Function>
				void execute(Function&& f) const
				{
					if(*this)
					{
						target_->execute(std::forward<Function>(f));
					}

					throw bad_executor{};
				}

				explicit operator bool() const noexcept
				{
					return static_cast<bool>(target_);
				}

				const type_info& target_type() const noexcept
				{
					if(*this)
					{
						return target_->target_type();
					}

					return typeid(void);
				}

				template<executor Executor> Executor* target() noexcept
				{
					void* t = target_->target(typeid(Executor));
					if (t == nullptr)
					{
						return t;
					}

					return static_cast<Executor*>(t);
				}

				template<executor Executor> const Executor* target() const noexcept
				{
					const void* t = target_->target(typeid(Executor));
					if (t == nullptr)
					{
						return t;
					}

					return static_cast<const Executor*>(t);
				}
			private:
				struct target_interface
				{
					virtual void execute(std::function<void()>&&) const = 0;
					virtual void* target(const std::type_info&) const noexcept = 0;
					virtual const void* target(const std::type_info&) noexcept = 0;
					virtual const std::type_info& target_type() const noexcept = 0;
					virtual bool operator==(const target_interface* tgt) const noexcept = 0;
				};

				template<executor Executor>
				struct target_impl : public target_interface
				{
					explicit target_impl(Executor&& ex)
						: ex_{std::forward<Executor>(ex)} {}

					const type_info& target_type() const final
					{
						return typeid(Executor);
					}

					const void* target(const std::type_info& ti) const final
					{
						if (ti == target_type())
						{
							return std::addressof(ex_);
						}

						return nullptr;
					}

					void* target(const std::type_info& ti) final
					{
						if (ti == target_type())
						{
							return std::addressof(ex_);
						}

						return nullptr;
					}

					void execute(std::function<void()>&& f) const final
					{
						auto f1 = collin::decay_copy(std::forward<std::function<void()>>(f));
						collin::execution::execute(ex_, std::move(f1));
					}

					bool operator==(const target_interface* tgt) const noexcept final
					{
						const void* t = tgt->target(target_type());
						if (t == nullptr)
						{
							return false;
						}

						return *static_cast<const Executor*>(t) == ex_;
					}

					Executor ex_;
				};

				template<executor Executor>
				static std::unique_ptr<target_interface> create(Executor&& ex)
				{
					return std::make_unique<target_impl<Executor>>(std::forward<Executor>(ex));
				}

				std::unique_ptr<target_interface> target_;

				template<class... SupportableProperties>
				friend bool operator==(const any_executor<SupportableProperties...>& a, const any_executor<SupportableProperties...>& b) noexcept
				{
					return (!a && !b) ||
						   (a.target_.get() == b.target_.get()) ||
						   (a.target_->operator==(b.target_.get()));
				}

				friend bool operator==(const any_executor& a, nullptr_t) noexcept
				{
					return !a;
				}

				friend bool operator==(nullptr_t, const any_executor& b) noexcept
				{
					return !b;
				}

				friend bool operator!=(const any_executor& a, const any_executor& b) noexcept
				{
					return !(a == b);
				}

				friend bool operator!=(const any_executor& a, nullptr_t) noexcept
				{
					return static_cast<bool>(a);
				}

				friend bool operator!=(nullptr_t, const any_executor& b) noexcept
				{
					return static_cast<bool>(b);
				}
		};

		template<class... SupportableProperties>
		void swap(any_executor<SupportableProperties...>& a, any_executor<SupportableProperties...>& b) noexcept
		{
			a.swap(b);
		}

		template<class Property, class... SupportableProperties>
			requires(requires { details::find_convertible_property<Property, SupportableProperties...>::type::is_preferable; } && details::find_convertible_property<Property, SupportableProperties...>::type::is_preferable)
		any_executor<SupportableProperties...> prefer(const any_executor<SupportableProperties...>& e, Property p)
		{
			// TODO
		}

		template<class InnerProperty>
		struct prefer_only
		{
			InnerProperty property;

			static constexpr bool is_requirable = false;
			static constexpr bool is_preferable = InnerProperty::is_preferable;

			template<std::same_as<InnerProperty> T = InnerProperty>
				requires(requires{ typename T::polymorphic_query_result_type; })
			using polymorphic_query_result_type = typename T::polymorphic_query_result_type;

			template<executor Executor, std::same_as<InnerProperty> T = InnerProperty>
				requires(requires{ T::static_query_v<Executor>; })
			static constexpr auto static_query_v = T::static_query_v<Executor>;

			constexpr prefer_only(const InnerProperty& p)
				: property{p} {}

			constexpr auto value() const noexcept(noexcept(std::declval<const InnerProperty>().value()))
				-> decltype(std::declval<const InnerProperty>().value())
			{
				return property.value();
			}

			template<executor Executor, std::same_as<prefer_only> Property>
			friend auto prefer(Executor ex, const Property& p) noexcept(noexcept(collin::prefer(std::move(ex), std::declval<const InnerProperty>())))
				-> decltype(collin::prefer(std::move(ex), std::declval<const InnerProperty>()))
			{
				return collin::prefer(std::move(ex), p.property);
			}

			template<executor Executor, std::same_as<prefer_only> Property>
			friend constexpr auto query(Executor ex, const Property& p) noexcept(noexcept(collin::query(std::move(ex), std::declval<const InnerProperty>())))
				-> decltype(collin::query(std::move(ex), std::declval<const InnerProperty>()))
			{
				return collin::query(std::move(ex), p.property);
			}
		};
	}
}