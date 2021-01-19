#pragma once

#include <memory>
#include <type_traits>
#include <mutex>
#include <map>
#include <vector>
#include <typeindex>
#include <algorithm>
#include <future>
#include <stdexcept>
#include <queue>
#include <condition_variable>
#include <concepts>

#include "cmoon/concepts.hpp"
#include "cmoon/utility.hpp"

namespace cmoon
{
	namespace net
	{
		template<cmoon::object CompletionToken, class Signature>
		class async_result
		{
			public:
				using completion_handler_type = CompletionToken;
				using return_type = void;

				explicit async_result(completion_handler_type&) {}
				async_result(const async_result&) = delete;
				async_result& operator=(const async_result&) = delete;

				return_type get() {}
		};

		namespace details
		{
			template<cmoon::object CompletionToken, class Signature>
			using deduced = typename async_result<std::decay_t<CompletionToken>, Signature>::return_type;
		}

		template<class R>
		concept async_result_specialization =
			requires
		{
			typename R::completion_handler_type;
			typename R::return_type;

		} && 
			std::move_constructible<typename R::completion_handler_type> &&
			requires(R r, typename R::completion_handler_type h)
		{
			r(h);
			{ r.get() } -> std::same_as<typename R::return_type>;
		};

		template<class CompletionToken, class Signature>
		struct async_completion
		{
			using completion_handler_type = typename async_result<std::decay_t<CompletionToken>, Signature>::completion_handler_type;

			private:
				static constexpr bool same_type = std::same_as<CompletionToken, completion_handler_type>;
			public:

				explicit async_completion(CompletionToken& t)
					requires(same_type)
					: completion_handler{t}, result{completion_handler} {}

				explicit async_completion(CompletionToken& t)
					requires(!same_type)
					: completion_handler{std::forward<CompletionToken>(t)}, result{completion_handler} {}

				async_completion(const async_completion&) = delete;
				async_completion& operator=(const async_completion&) = delete;

				std::conditional_t<same_type, completion_handler_type&, completion_handler_type> completion_handler;
				async_result<std::decay_t<CompletionToken>, Signature> result;
		};

		template<class A>
		concept proto_allocator = std::copy_constructible<A> &&
								  std::destructible<A> &&
			requires
		{
			typename std::allocator_traits<A>::template rebind_alloc<int>;
		} && cmoon::allocator<typename std::allocator_traits<A>::template rebind_alloc<int>>;

		static_assert(proto_allocator<std::allocator<int>>);

		template<class T, proto_allocator ProtoAllocator = std::allocator<void>>
		struct associated_allocator
		{
			private:
				template<class T2>
				struct type_picker
				{
					using type = ProtoAllocator;
				};

				template<class T2>
					requires(requires{ typename T2::allocator_type; })
				struct type_picker<T2>
				{
					using type = typename T2::allocator_type;
				};
			public:
				using type = typename type_picker<T>::type;

				static type get(const T& t, const ProtoAllocator& a = ProtoAllocator()) noexcept
				{
					return a;
				}

				static type get(const T& t, const ProtoAllocator& = ProtoAllocator()) noexcept
					requires(requires{ typename T::allocator_type; })
				{
					return t.get_allocator();
				}
		};

		template<class T, proto_allocator ProtoAllocator = std::allocator<void>>
		using associated_allocator_t = typename associated_allocator<T, ProtoAllocator>::type;

		template<class T>
		associated_allocator_t<T> get_associated_allocator(const T& t) noexcept
		{
			return associated_allocator<T>::get(t);
		}

		template<class T, proto_allocator ProtoAllocator = std::allocator<void>>
		associated_allocator_t<T, ProtoAllocator> get_associated_allocator(const T& t, const ProtoAllocator& a) noexcept
		{
			return associated_allocator<T, ProtoAllocator>::get(t, a);
		}

		enum class fork_event
		{
			prepare,
			parent,
			child
		};

		namespace details
		{
			class function_object
			{
				public:
					constexpr void operator()() {}
			};

			static_assert(std::move_constructible<function_object>);
		}

		class execution_context
		{
			public:
				class service
				{
					friend struct std::default_delete<service>;
				protected:
					explicit service(execution_context& owner)
						: context_{owner} {}

					service(const service&) = delete;
					service& operator=(const service&) = delete;
					virtual ~service() {}

					execution_context& context() noexcept
					{
						return context_;
					}
				private:
					friend execution_context;

					virtual void shutdown() noexcept = 0;
					virtual void notify_fork(fork_event) {}

					execution_context& context_;
				};

				execution_context() = default;
				execution_context(const execution_context&) = delete;
				execution_context& operator=(const execution_context&) = delete;

				virtual ~execution_context()
				{
					shutdown();
					destroy();
				}

				void notify_fork(fork_event e)
				{
					const auto l = [=](auto& svc) { svc->notify_fork(e); };
					if (e == fork_event::prepare)
					{
						std::for_each(std::rbegin(services), std::rend(services), l);
					}
					else
					{
						std::for_each(std::begin(services), std::end(services), l);
					}
				}

			protected:
				void shutdown() noexcept
				{
					std::for_each(std::rbegin(services), std::rend(services), [](auto& svc) {
						if (svc)
						{
							svc->shutdown();
							svc.shutdown = true;
						}
						});
				}

				void destroy() noexcept
				{
					while (!services.empty())
					{
						services.pop_back();
					}
					key_type_mappings.clear();
				}

			private:
				template<class Service>
				friend typename Service::key_type& use_service(execution_context& ctx);

				template<class Service>
				friend bool has_service(const execution_context& ctx) noexcept;

				template<class Service, class... Args>
				friend Service& make_service(execution_context& ctx, Args&&... args);

				struct service_tracker
				{
					explicit service_tracker(std::unique_ptr<service> other)
						: svc{std::move(other)} {}

					std::unique_ptr<service> svc{nullptr};
					bool shutdown{false};

					service* operator->()
					{
						return svc.get();
					}

					inline explicit operator bool() const noexcept
					{
						return !shutdown;
					}
				};

				template<class Service, class... Args>
				void add_service(Args&&... args)
				{
					using key = typename Service::key_type;
					static_assert(std::derived_from<key, execution_context::service>);
					static_assert(std::derived_from<key, Service>);

					auto ptr = std::make_unique<Service>(*this, std::forward<Args>(args)...);
					services.emplace_back(std::move(ptr));
					key_type_mappings[std::type_index{ typeid(Service::key_type) }] = services.back().svc.get();
				}

				template<class Service>
				inline bool has_service() const noexcept
				{
					using key = typename Service::key_type;
					static_assert(std::derived_from<key, execution_context::service>);
					static_assert(std::derived_from<key, Service>);

					return key_type_mappings.contains(std::type_index{ typeid(key) });
				}

				std::vector<service_tracker> services;
				std::map<std::type_index, service*> key_type_mappings;
				std::mutex mut;
		};

		template<class X>
		struct is_executor;

		template<class X>
		concept execution_context_concept = std::derived_from<X, execution_context> &&
			requires
		{
			typename X::executor_type;
		} /*&& is_executor_v<typename X::executor_type> */ &&
			requires(X x)
		{
			x.~X();
			{ x.get_executor() } -> std::same_as<typename X::executor_type>;
		};

		class service_already_exists : public std::logic_error
		{
			public:
				service_already_exists(const std::string& what_arg)
					: std::logic_error{what_arg} {}

				service_already_exists(const char* what_arg)
					: std::logic_error{what_arg} {}
		};

		template<class Service>
		typename Service::key_type& use_service(execution_context& ctx)
		{
			using key = typename Service::key_type;
			static_assert(std::derived_from<key, execution_context::service>);
			static_assert(std::derived_from<key, Service>);

			std::scoped_lock lock{ctx.mut};

			auto svc_it = ctx.key_type_mappings.find(std::type_index{ typeid(typename Service::key_type) });
			if (svc_it != ctx.key_type_mappings.end())
			{
				return static_cast<key&>(*(svc_it->second));
			}

			ctx.add_service<Service>();
			return static_cast<key&>(*(ctx.services.back().svc.get()));
		}

		template<class Service, class... Args>
		Service& make_service(execution_context& ctx, Args&&... args)
		{
			using key = typename Service::key_type;
			static_assert(std::derived_from<key, execution_context::service>);
			static_assert(std::derived_from<key, Service>);

			std::scoped_lock<std::mutex>{ctx.mut};

			if (ctx.has_service<Service>())
			{
				throw service_already_exists{"Service already exists"};
			}

			ctx.add_service<Service>(std::forward<Args>(args)...);
			return static_cast<Service&>(*(ctx.services.back().svc.get()));
		}

		template<class Service>
		bool has_service(const execution_context& ctx) noexcept
		{
			using key = typename Service::key_type;
			static_assert(std::derived_from<key, execution_context::service>);
			static_assert(std::derived_from<key, Service>);

			std::scoped_lock<std::mutex>{ctx.mut};
			return ctx.has_service<Service>();
		}

		namespace details
		{
			template<class T>
			concept execution_context_concept_decay = execution_context_concept<std::decay_t<T>>;
		}

		template<class X>
		struct is_executor : std::false_type {};

		template<class X>
		requires(
			std::is_nothrow_copy_constructible_v<X> &&
			std::is_nothrow_move_constructible_v<X> &&
			std::destructible<X> &&
			requires(X x1, const X x2, X&& mx1, details::function_object&& f, const std::allocator<int>& a, X u)
		{
			{ x1 == x2 } noexcept -> std::convertible_to<bool>;
			{ x1 != x2 } noexcept -> std::convertible_to<bool>;
			{ x1.on_work_started() } noexcept;
			{ x1.on_work_finished() } noexcept;
			x1.dispatch(std::move(f), a);
			x1.post(std::move(f), a);
			x1.defer(std::move(f), a);
		} && (
			requires(X x1)
		{
			{ x1.context() } noexcept -> details::execution_context_concept_decay;
		} || requires(X x1)
		{
			{ x1.context() } noexcept -> std::same_as<execution_context&>;
		})
		)
		struct is_executor<X> : std::true_type {};

		template<class T>
		constexpr bool is_executor_v = is_executor<T>::value;

		struct executor_arg_t {};
		constexpr executor_arg_t executor_arg = executor_arg_t{};

		template<class T, class Executor>
		struct uses_executor : std::false_type {};

		template<class T, class Executor>
			requires(requires { typename T::executor_type; } && std::is_convertible_v<Executor, typename T::executor_type> && is_executor_v<Executor>)
		struct uses_executor<T, Executor> : std::true_type {};

		template<class T, class Executor>
		constexpr bool uses_executor_v = uses_executor<T, Executor>::value;

		class system_executor;

		template<class T, class Executor = system_executor>
			requires(is_executor_v<Executor>)
		struct associated_executor
		{
			private:
				template<class T2>
				struct type_picker
				{
					using type = Executor;
				};

				template<class T2>
					requires(requires{ typename T2::executor_type; })
				struct type_picker<T2>
				{
					using type = typename T2::executor_type;
				};
			public:
				using type = typename type_picker<T>::type;

				static type get(const T& t, const Executor& e = Executor()) noexcept
				{
					return e;
				}

				static type get(const T& t, const Executor & = Executor()) noexcept
					requires(requires { typename T::executor_type; })
				{
					return t.get_executor();
				}
		};

		template<class T, class Executor = system_executor>
		using associated_executor_t = typename associated_executor<T, Executor>::type;

		template<class T>
		associated_executor_t<T> get_associated_executor(const T& t) noexcept
		{
			return associated_executor<T>::get(t);
		}

		template<class T, class Executor>
			requires(is_executor_v<Executor>)
		associated_executor_t<T, Executor> get_associated_executor(const T& t, const Executor& ex) noexcept
		{
			return associated_executor<T, Executor>::get(t, ex);
		}

		template<class T, class ExecutionContext>
			requires(std::is_convertible_v<ExecutionContext&, execution_context&>)
		associated_executor_t<T, typename ExecutionContext::executor_type> get_associated_executor(const T& t, ExecutionContext& ctx) noexcept
		{
			return get_associated_executor(t, ctx.get_executor());
		}

		template<class T, class Executor>
			requires(is_executor_v<Executor>)
		class executor_binder
		{
			public:
				using target_type = T;
				using executor_type = Executor;

				executor_binder(T t, const Executor& ex)
					: ex_{ex}, target_{std::move(t), ex_} {}

				executor_binder(T t, const Executor& ex)
					requires(uses_executor_v<T, Executor> && std::is_constructible_v<T, executor_arg_t, T&&, const Executor&>)
					: ex_{ex}, target_{executor_arg, std::move(t), ex_} {}

				executor_binder(const executor_binder&) = default;
				executor_binder(executor_binder&&) = default;

				template<class U, class OtherExecutor>
					requires(is_executor_v<OtherExecutor> && std::is_convertible_v<U, T> && std::is_convertible_v<OtherExecutor, Executor>)
				executor_binder(const executor_binder<U, OtherExecutor>& other)
					: ex_{other.get_executor()}, target_{other.get(), ex_} {}

				template<class U, class OtherExecutor>
					requires(is_executor_v<OtherExecutor> && std::is_convertible_v<U, T> && std::is_convertible_v<OtherExecutor, Executor> && uses_executor_v<U, OtherExecutor> && std::is_constructible_v<T, executor_arg_t, U&&, const OtherExecutor&>)
				executor_binder(executor_binder<U, OtherExecutor>&& other)
					: ex_{other.get_executor()}, target_{executor_arg, std::move(other.get()), ex_} {}

				template<class U, class OtherExecutor>
					requires(is_executor_v<OtherExecutor> && std::is_convertible_v<U, T>)
				executor_binder(executor_arg_t, const Executor& ex, const executor_binder<U, OtherExecutor>& other)
					: ex_{ex}, target_{executor_arg, other.get(), ex_} {}

				template<class U, class OtherExecutor>
					requires(is_executor_v<OtherExecutor> && std::is_convertible_v<U, T>)
				executor_binder(executor_arg_t, const Executor& ex, executor_binder<U, OtherExecutor>&& other)
					: ex_{ex}, target_{executor_arg, std::move(other.get()), ex_} {}

				~executor_binder() noexcept = default;

				T& get() noexcept
				{
					return target_;
				}

				const T& get() const noexcept
				{
					return target_;
				}

				executor_type get_executor() const noexcept
				{
					return ex_;
				}

				template<class... Args>
				std::invoke_result_t<T&(Args&&...)> operator()(Args&&... args)
				{
					return std::invoke(get(), std::forward<Args>(args)...);
				}

				template<class... Args>
				std::invoke_result_t<const T&(Args&&...)> operator()(Args&&... args) const
				{
					return std::invoke(get(), std::forward<Args>(args)...);
				}
			private:
				Executor ex_;
				T target_;
		};

		template<class T, class Executor, class Signature>
			requires(is_executor_v<Executor>)
		class async_result<executor_binder<T, Executor>, Signature>
		{
			public:
				using completion_handler_type = executor_binder<typename async_result<T, Signature>::completion_handler_type, Executor>;
				using return_type = typename async_result<T, Signature>::return_type;

				explicit async_result(completion_handler_type& h)
					: target_{h.get()} {}

				async_result(const async_result&) = delete;
				async_result& operator=(const async_result&) = delete;

				return_type get()
				{
					target_.get();
				}
			private:
				async_result<T, Signature> target_;
		};

		template<class T, class Executor, proto_allocator ProtoAllocator>
			requires(is_executor_v<Executor>)
		struct associated_allocator<executor_binder<T, Executor>, ProtoAllocator>
		{
			using type = associated_allocator_t<T, ProtoAllocator>;

			static type get(const executor_binder<T, Executor>& b, const ProtoAllocator& a = ProtoAllocator()) noexcept
			{
				return associated_allocator<T, ProtoAllocator>::get(b.get(), a);
			}
		};

		template<class T, class Executor, class Executor1>
			requires(is_executor_v<Executor> && is_executor_v<Executor1>)
		struct associated_executor<executor_binder<T, Executor>, Executor1>
		{
			using type = Executor;

			static type get(const executor_binder<T, Executor>& b, const Executor1& = Executor1()) noexcept
			{
				return b.get_executor();
			}
		};

		template<class Executor, class T>
			requires(is_executor_v<Executor>)
		executor_binder<std::decay_t<T>, Executor> bind_executor(const Executor& ex, T&& t)
		{
			return executor_binder<std::decay_t<T>, Executor>(std::forward<T>(t), ex);
		}

		template<class ExecutionContext, class CompletionToken>
			requires(std::is_convertible_v<ExecutionContext&, execution_context&>)
		executor_binder<std::decay_t<CompletionToken>, typename ExecutionContext::executor_type> bind_executor(ExecutionContext& ctx, CompletionToken&& t)
		{
			return bind_executor(ctx.get_executor(), std::forward<CompletionToken>(t));
		}

		template<class Executor>
			requires(is_executor_v<Executor>)
		class executor_work_guard
		{
			public:
				using executor_type = Executor;

				explicit executor_work_guard(const executor_type& ex) noexcept
					: ex_{ex}, owns_{true}
				{
					ex_.on_work_started();
				}

				executor_work_guard(const executor_work_guard& other) noexcept
					: ex_{other.ex_}, owns_{other.owns_}
				{
					if (owns_)
					{
						ex_.on_work_started();
					}
				}

				executor_work_guard(executor_work_guard&& other) noexcept
					: ex_{std::move(other.ex_)}, owns_{std::exchange(other.owns_, false)} {}

				executor_work_guard& operator=(const executor_work_guard&) = delete;

				~executor_work_guard()
				{
					if (owns_)
					{
						ex_.on_work_finished();
					}
				}

				executor_type get_executor() const noexcept
				{
					return ex_;
				}

				bool owns_work() const noexcept
				{
					return owns_;
				}

				void reset() noexcept
				{
					if (owns_)
					{
						ex_.on_work_finished();
					}

					owns_ = false;
				}
			private:
				Executor ex_;
				bool owns_;
		};

		template<class Executor>
			requires(is_executor_v<Executor>)
		executor_work_guard<Executor> make_work_guard(const Executor& ex)
		{
			return executor_work_guard<Executor>(ex);
		}

		template<class ExecutionContext>
			requires(std::is_convertible_v<ExecutionContext&, execution_context&>)
		executor_work_guard<typename ExecutionContext::executor_type> make_work_guard(ExecutionContext& ctx)
		{
			return make_work_guard(ctx.get_executor());
		}

		template<class T>
			requires(!is_executor_v<T> && !std::is_convertible_v<T&, execution_context&>)
		executor_work_guard<associated_executor_t<T>> make_work_guard(const T& t)
		{
			return make_work_guard(get_associated_executor(t));
		}

		template<class T, class U>
		auto make_work_guard(const T& t, U&& u) -> decltype(make_work_guard(get_associated_executor(t, std::forward<U>(u))))
		{
			return make_work_guard(get_associated_executor(t, std::forward<U>(u)));
		}

		class system_context : public execution_context
		{
			public:
				using executor_type = system_executor;

				system_context() = delete;
				system_context(const system_context&) = delete;
				system_context& operator=(const system_context&) = delete;

				~system_context()
				{
					stop();
					join();
				}

				executor_type get_executor() noexcept;

				void stop()
				{
					stopped_ = true;
				}

				bool stopped() const noexcept
				{
					return stopped_;
				}

				void join()
				{
					while (!jobs.empty())
					{
						jobs.back().wait();
						jobs.pop_back();
					}
				}
			private:
				friend class system_executor;

				struct private_constructor_t {};
				static constexpr private_constructor_t private_constructor{};

				system_context(private_constructor_t) {}

				std::vector<std::future<void>> jobs;
				bool stopped_{false};	
		};

		class system_executor
		{
			public:
				system_executor() {}
				system_executor(const system_executor&) noexcept = default;
				system_executor(system_executor&&) noexcept = default;

				system_context& context() const noexcept
				{
					static system_context ctx{system_context::private_constructor};
					return ctx;
				}

				void on_work_started() const noexcept {}
				void on_work_finished() const noexcept {}

				template<class Func, proto_allocator ProtoAllocator>
				void dispatch(Func&& f, const ProtoAllocator&) const
				{
					cmoon::decay_copy(std::forward<Func>(f))();
				}

				template<class Func, proto_allocator ProtoAllocator>
				void post(Func&& f, const ProtoAllocator&) const
				{
					if (!context().stopped())
					{
						auto f1 = cmoon::decay_copy(std::forward<Func>(f));
						context().jobs.push_back(std::async(std::launch::async, [f2 = std::move(f1)]() { f2(); }));
					}
				}

				template<class Func, proto_allocator ProtoAllocator>
				void defer(Func&& f, const ProtoAllocator&) const
				{
					if (!context().stopped())
					{
						auto f1 = cmoon::decay_copy(std::forward<Func>(f));
						context().jobs.push_back(std::async(std::launch::deferred, [f2 = std::move(f1)]() { f2(); }));
					}
				}
		};

		system_context::executor_type system_context::get_executor() noexcept
		{
			return system_executor{};
		}

		static_assert(execution_context_concept<system_context>);

		bool operator==(const system_executor&, const system_executor&) noexcept
		{
			return true;
		}

		bool operator!=(const system_executor&, const system_executor&) noexcept
		{
			return false;
		}

		static_assert(is_executor_v<system_executor>);

		class bad_executor : public std::exception
		{
			public:
				bad_executor() noexcept {}

				const char* what() const noexcept final
				{
					return "bad executor";
				}
		};

		struct allocator_arg_t {};
		constexpr allocator_arg_t allocator_arg {};

		class executor
		{
			public:
				executor() noexcept = default;
				executor(std::nullptr_t) noexcept {}
				executor(const executor& e) noexcept = default;
				executor(executor&& e) noexcept = default;

				template<class Executor>
					requires(is_executor_v<Executor>)
				executor(Executor e)
					: target_{create<Executor>(std::move(e))} {}

				template<class Executor, class ProtoAllocator>
					requires(is_executor_v<Executor>)
				executor(allocator_arg_t, const ProtoAllocator& a, Executor e)
					: target_{create<Executor, ProtoAllocator>(std::move(e), a)} {}

				executor& operator=(const executor& e) noexcept = default;
				executor& operator=(executor&& e) noexcept = default;
				executor& operator=(std::nullptr_t) noexcept
				{
					target_.reset();
					return *this;
				}

				template<class Executor>
					requires(is_executor_v<Executor>)
				executor& operator=(Executor e)
				{
					target_ = create<Executor>(std::move(e));
					return *this;
				}

				~executor() noexcept = default;

				void swap(executor& other) noexcept
				{
					target_.swap(other.target_);
				}

				template<class Executor, class ProtoAllocator>
					requires(is_executor_v<Executor>)
				void assign(Executor e, const ProtoAllocator& a)
				{
					executor{allocator_arg, a, std::move(e)}.swap(*this);
				}

				execution_context& context() const noexcept
				{
					return target_->context();
				}

				void on_work_started() const noexcept
				{
					return target_->on_work_started();
				}

				void on_work_finished() const noexcept
				{
					return target_->on_work_finished();
				}

				template<class Func, class ProtoAllocator>
				void dispatch(Func&& f, const ProtoAllocator&) const
				{
					auto fd = cmoon::decay_copy(std::forward<Func>(f));
					target_->dispatch([f = std::move(fd)](){ f(); });
				}

				template<class Func, class ProtoAllocator>
				void post(Func&& f, const ProtoAllocator&) const
				{
					auto fd = cmoon::decay_copy(std::forward<Func>(f));
					target_->post([f = std::move(fd)](){ f(); });
				}

				template<class Func, class ProtoAllocator>
				void defer(Func&& f, const ProtoAllocator&) const
				{
					auto fd = cmoon::decay_copy(std::forward<Func>(f));
					target_->defer([f = std::move(fd)](){ f(); });
				}

				explicit operator bool() const noexcept
				{
					return target_.use_count() != 0;
				}

				const std::type_info& target_type() const noexcept
				{
					if (*this)
					{
						return target_->target_type();
					}

					return typeid(void);
				}

				template<class Executor>
				Executor* target() noexcept
				{
					return target_->target(typeid(Executor));
				}

				template<class Executor>
				const Executor* target() const noexcept
				{
					return target_->target(typeid(Executor));
				}

			private:
				struct target_interface
				{
					virtual void on_work_started() const noexcept = 0;
					virtual void on_work_finished() const noexcept = 0;
					virtual execution_context& context() const noexcept = 0;
					virtual void dispatch(std::function<void()>) const = 0;
					virtual void post(std::function<void()>) const = 0;
					virtual void defer(std::function<void()>) const = 0;
					virtual const std::type_info& target_type() const noexcept = 0;
					virtual void* target(const std::type_info&) noexcept = 0;
					virtual const void* target(const std::type_info&) const noexcept = 0;
					virtual bool operator==(const target_interface*) const noexcept = 0;
					virtual void* get_executor() noexcept = 0;
					virtual const void* get_executor() const noexcept = 0;
				};

				template<class Executor, proto_allocator ProtoAllocator>
					requires(is_executor_v<Executor>)
				struct target_impl : public target_interface
				{
					explicit target_impl(Executor&& ex, const ProtoAllocator& a)
						: ex_{std::forward<Executor>(ex)}, alloc_{a} {}

					void on_work_started() const noexcept final
					{
						return ex_.on_work_started();
					}

					void on_work_finished() const noexcept final
					{
						return ex_.on_work_finished();
					}

					execution_context& context() const noexcept final
					{
						return ex_.context();
					}

					void dispatch(std::function<void()> f) const final
					{
						ex_.dispatch(std::move(f), alloc_);
					}

					void post(std::function<void()> f) const final
					{
						ex_.post(std::move(f), alloc_);
					}

					void defer(std::function<void()> f) const final
					{
						ex_.post(std::move(f), alloc_);
					}

					const type_info& target_type() const noexcept final
					{
						return typeid(Executor);
					}

					void* target(const std::type_info& ti) noexcept final
					{
						if (ti == target_type())
						{
							return get_executor();
						}

						return nullptr;
					}

					const void* target(const std::type_info& ti) const noexcept final
					{
						if (ti == target_type())
						{
							return get_executor();
						}

						return nullptr;
					}

					bool operator==(const target_interface* tgt) const noexcept final
					{
						if (tgt->target_type() == target_type())
						{
							return *static_cast<const Executor*>(tgt->get_executor()) == ex_;
						}

						return false;
					}

					void* get_executor() noexcept final
					{
						return std::addressof(ex_);
					}

					const void* get_executor() const noexcept final
					{
						return std::addressof(ex_);
					}

					Executor ex_;
					ProtoAllocator alloc_;
				};

				template<class Executor, proto_allocator ProtoAllocator = std::allocator<void>>
				static std::shared_ptr<target_interface> create(Executor&& ex, const ProtoAllocator& alloc = ProtoAllocator())
				{
					return std::allocate_shared<target_impl<Executor, ProtoAllocator>>(alloc, std::forward<Executor>(ex), alloc);
				}

				std::shared_ptr<target_interface> target_;

				friend bool operator==(const executor& a, const executor& b) noexcept;
				friend bool operator==(const executor& a, std::nullptr_t) noexcept;
				friend bool operator==(std::nullptr_t, const executor& b) noexcept;

				friend bool operator!=(const executor& a, const executor& b) noexcept;
				friend bool operator!=(const executor& a, std::nullptr_t) noexcept;
				friend bool operator!=(std::nullptr_t, const executor& b) noexcept;
		};

		bool operator==(const executor& a, const executor& b) noexcept
		{
			return (!a && !b) ||
				   (a.target_.get() == b.target_.get()) ||
				   (a.target_ == b.target_);
		}

		bool operator==(const executor& a, std::nullptr_t) noexcept
		{
			return !a;
		}

		bool operator==(std::nullptr_t, const executor& b) noexcept
		{
			return !b;
		}

		bool operator!=(const executor& a, const executor& b) noexcept
		{
			return !(a == b);
		}

		bool operator!=(const executor& a, std::nullptr_t) noexcept
		{
			return static_cast<bool>(a);
		}

		bool operator!=(std::nullptr_t, const executor& b) noexcept
		{
			return static_cast<bool>(b);
		}

		void swap(executor& a, executor& b) noexcept
		{
			a.swap(b);
		}

		namespace details
		{
			template<class CompletionHandler>
			struct f_obj
			{
				using handler_type = CompletionHandler;
				using guard_type = decltype(make_work_guard(std::declval<handler_type>()));

				f_obj(handler_type&& handler)
					: h{std::forward<handler_type>(handler)}, w{make_work_guard(h)} {}

				void operator()()
				{
					auto internal_alloc = get_associated_allocator(h);
					w.get_executor().dispatch(std::move(h), internal_alloc);
					w.reset();
				}

				handler_type h;
				guard_type w;
			};
		}

		template<class CompletionToken>
		details::deduced<CompletionToken, void()> dispatch(CompletionToken&& token)
		{
			async_completion<CompletionToken, void()> completion{token};

			auto ex = get_associated_executor(completion.completion_handler);
			auto alloc = get_associated_allocator(completion.completion_handler);

			ex.dispatch(std::move(completion.completion_handler), alloc);
			return completion.result.get();
		}

		template<class Executor, class CompletionToken>
			requires(is_executor_v<Executor>)
		details::deduced<CompletionToken, void()> dispatch(const Executor& ex, CompletionToken&& token)
		{
			async_completion<CompletionToken, void()> completion{token};

			auto alloc = get_associated_allocator(completion.completion_handler);
			details::f_obj f{std::move(completion.completion_handler)};

			ex.dispatch(std::move(f), alloc);
			return completion.result.get();
		}

		template<class ExecutionContext, class CompletionToken>
			requires(std::is_convertible_v<ExecutionContext&, execution_context&>)
		details::deduced<CompletionToken, void()> dispatch(ExecutionContext& ctx, CompletionToken&& token)
		{
			return net::dispatch(ctx.get_executor(), std::forward<CompletionToken>(token));
		}

		template<class CompletionToken>
		details::deduced<CompletionToken, void()> post(CompletionToken&& token)
		{
			async_completion<CompletionToken, void()> completion{token};

			auto ex = get_associated_executor(completion.completion_handler);
			auto alloc = get_associated_allocator(completion.completion_handler);

			ex.post(std::move(completion.completion_handler), alloc);
			return completion.result.get();
		}

		template<class Executor, class CompletionToken>
			requires(is_executor_v<Executor>)
		details::deduced<CompletionToken, void()> post(const Executor& ex, CompletionToken&& token)
		{
			async_completion<CompletionToken, void()> completion{token};

			auto alloc = get_associated_allocator(completion.completion_handler);
			details::f_obj f{std::move(completion.completion_handler)};

			ex.post(std::move(f), alloc);
			return completion.result.get();
		}

		template<class ExecutionContext, class CompletionToken>
			requires(std::is_convertible_v<ExecutionContext&, execution_context&>)
		details::deduced<CompletionToken, void()> post(ExecutionContext& ctx, CompletionToken&& token)
		{
			return net::post(ctx.get_executor(), std::forward<CompletionToken>(token));
		}

		template<class CompletionToken>
		details::deduced<CompletionToken, void()> defer(CompletionToken&& token)
		{
			async_completion<CompletionToken, void()> completion{token};

			auto ex = get_associated_executor(completion.completion_handler);
			auto alloc = get_associated_allocator(completion.completion_handler);

			ex.defer(std::move(completion.completion_handler), alloc);
			return completion.result.get();
		}

		template<class Executor, class CompletionToken>
			requires(is_executor_v<Executor>)
		details::deduced<CompletionToken, void()> defer(const Executor& ex, CompletionToken&& token)
		{
			async_completion<CompletionToken, void()> completion{token};

			auto alloc = get_associated_allocator(completion.completion_handler);
			details::f_obj f{std::move(completion.completion_handler)};

			ex.defer(std::move(f), alloc);
			return completion.result.get();
		}

		template<class ExecutionContext, class CompletionToken>
			requires(std::is_convertible_v<ExecutionContext&, execution_context&>)
		details::deduced<CompletionToken, void()> defer(ExecutionContext& ctx, CompletionToken&& token)
		{
			return net::defer(ctx.get_executor(), std::forward<CompletionToken>(token));
		}

		template<class Executor>
			requires(is_executor_v<Executor>)
		class strand
		{
			public:
				using inner_executor_type = Executor;

				strand()
					: state_{create()} {}

				explicit strand(Executor ex)
					: inner_ex_{ex}, state_{create()} {}

				template<proto_allocator ProtoAllocator>
				strand(allocator_arg_t, const ProtoAllocator& alloc, Executor ex)
					: inner_ex_{ex}, state_{create(alloc)} {}

				strand(const strand& other) noexcept requires(std::assignable_from<Executor, Executor>) = default;
				strand(strand&& other) noexcept requires(std::assignable_from<Executor, Executor>) = default;

				template<class OtherExecutor>
				strand(const strand<OtherExecutor>& other) noexcept
					: inner_ex_{other.inner_ex_}, state_{other.state_} {}

				template<class OtherExecutor>
				strand(strand<OtherExecutor>&& other) noexcept
					: inner_ex_{std::move(other.inner_ex_)}, state_{std::move(other.state_)} {}

				strand& operator=(const strand& other) noexcept
				{
					if (this != std::addressof(other))
					{
						inner_ex_ = other.inner_ex_;
						std::scoped_lock l{other.mtx_};
						state_ = other.state_;
					}
					
					return *this;
				}

				strand& operator=(strand&& other) noexcept
				{
					if (this != std::addressof(other))
					{
						inner_ex_ = std::move(other.inner_ex_);
						std::scoped_lock l{other.mtx_};
						state_ = std::move(other.state_);
					}
				}

				template<class OtherExecutor>
				strand& operator=(const strand<OtherExecutor>& other) noexcept
				{
					if (this != std::addressof(other))
					{
						inner_ex_ = other.inner_ex_;
						std::scoped_lock l{other.mtx_};
						state_ = other.state_;
					}
					
					return *this;
				}

				template<class OtherExecutor>
				strand& operator=(strand<OtherExecutor>&& other) noexcept
				{
					if (this != std::addressof(other))
					{
						inner_ex_ = std::move(other.inner_ex_);
						std::scoped_lock l{other.mtx_};
						state_ = std::move(other.state_);
					}
				}

				~strand() noexcept = default;

				inner_executor_type get_inner_executor() const noexcept
				{
					return inner_ex_;
				}

				bool running_in_this_thread() const noexcept
				{
					return state_->thread_.get_id() == std::this_thread::get_id();
				}

				execution_context& context() const noexcept
				{
					return inner_ex_.context();
				}

				void on_work_started() const noexcept
				{
					inner_ex_.on_work_started();
				}

				void on_work_finished() const noexcept
				{
					inner_ex_.on_work_finished();
				}

				template<class Func, proto_allocator ProtoAllocator>
				void dispatch(Func&& f, const ProtoAllocator& a) const
				{
					if (running_in_this_thread())
					{
						cmoon::decay_copy(std::forward<Func>(f))();
					}
					else
					{
						post(std::forward<Func>(f), a);
					}
				}

				template<class Func, proto_allocator ProtoAllocator>
				void post(Func&& f, const ProtoAllocator& a) const
				{
					std::scoped_lock l{state_->mtx_};
					if constexpr (std::is_void_v<std::invoke_result_t<Func()>>)
					{
						state_->tasks_.emplace(std::forward<Func>(f));
					}
					else
					{
						state_->tasks_.emplace([f1 = std::move(f)](){ f1(); });
					}
					
					state_->cv_.notify_one();
				}

				template<class Func, proto_allocator ProtoAllocator>
				void defer(Func&& f, const ProtoAllocator& a) const
				{
					post(std::forward<Func>(f), a);
				}

				template<class Executor>
				friend bool operator==(const strand<Executor>& a, const strand<Executor>& b)
				{
					return a.state_->thread_.get_id() == b.state_->thread_.get_id();
				}

				template<class Executor>
				friend bool operator!=(const strand<Executor>& a, const strand<Executor>& b)
				{
					return !(a == b);
				}
			private:
				Executor inner_ex_;

				void run()
				{
					auto owner_state = state_;
					auto owner_ex = inner_ex_;

					while (true)
					{
						std::function<void()> f;
						{
							std::unique_lock l{owner_state->mtx_};

							// If my logic is correct, then this condition variable should only wake once
							// we are the only holder of the state, there is a task available, or we are ready to execute the next task.
							owner_state->cv_.wait(l, [=](){ return owner_state.use_count() != 1 && !owner_state->tasks_.empty() && owner_state->ready; });

							// If we are the only one holding onto the shared state (all associated parent strands have
							// been destroyed) and if there are no more tasks to complete, then we are done.
							if (owner_state.use_count() == 1 && owner_state->tasks_.empty())
							{
								break;
							}

							// If we are not ready yet to run the next task. Or
							// we are ready, but we just got done executing our last known
							// task, so we are going back to wait for more tasks or until ready.
							if (!owner_state->ready || owner_state->tasks_.empty())
							{
								continue;
							}

							f = std::move(owner_state->tasks_.front());
							owner_state->ready = false;
						}

						auto completion_handler = [&owner_state, f2 = std::move(f)]()
						{
							try
							{
								f2();
							}
							catch(...) {}
							std::scoped_lock l{owner_state->mtx_};
							owner_state->tasks_.pop();
							owner_state->ready = true;
							owner_state->cv_.notify_one();
						};

						post(owner_ex, completion_handler);
					}

					// This function should reach here once there are no tasks available and we are the
					// only owner of the shared state (all strands associated were destroyed).
					// This will effectively destroy the `owner_state` shared ptr variable
					// and since it is the last reference, the shared state will be freed.
				}

				struct state
				{
					state(strand* owner)
						: thread_{strand::run, owner}
					{
						thread_.detach();
					}

					std::thread thread_;
					std::condition_variable cv_;
					std::mutex mtx_;
					bool ready {true};
					std::queue<std::function<void()>> tasks_;
				};

				std::shared_ptr<state> state_;

				template<class Allocator = std::allocator<void>>
				static std::shared_ptr<state> create(const Allocator& alloc = Allocator())
				{
					return std::allocate_shared<state>(alloc, this);
				}
		};

		namespace details
		{
			template<class Func, class Alloc>
			struct use_future_ct
			{
				Func f;
				Alloc a;
			};
		}

		template<proto_allocator ProtoAllocator = std::allocator<void>>
		class use_future_t
		{
			public:
				using allocator_type = ProtoAllocator;

				constexpr use_future_t() noexcept(noexcept(allocator_type())) = default;
				explicit use_future_t(const allocator_type& a) noexcept
					: alloc_{a} {}

				template<class OtherProtoAllocator>
				use_future_t<OtherProtoAllocator> rebind(const OtherProtoAllocator& a) const noexcept
				{
					return use_future_t<OtherProtoAllocator>{OtherProtoAllocator{alloc_}};
				}

				allocator_type get_allocator() const noexcept
				{
					return alloc_;
				}

				template<class F>
				auto operator()(F&& f) const
				{
					using token = details::use_future_ct<std::decay_t<F>, ProtoAllocator>;
					return token{std::forward<F>(f), alloc_};
				}
			private:
				allocator_type alloc_ {};
		};

		namespace details
		{
			template<class Result, class Executor>
				requires(is_executor_v<Executor>)
			class use_future_ex;

			template<class Func, class... Args>
			class use_future_ch
			{
				public:
					using result = std::invoke_result_t<Func(std::decay_t<Args>...)>;

					template<class Allocator>
					explicit use_future_ch(details::use_future_ct<Func, Allocator>&& token)
						: f{std::move(token.f)}, promise{token.a} {}

					void operator()(Args&&... args)
					{
						try
						{
							promise.set_value(f(std::forward<Args>(args)...));
						}
						catch (...)
						{
							promise.set_exception(std::current_exception());
						}
					}

					std::future<result> get_future()
					{
						return promise.get_future();
					}

				private:
					Func f;
					mutable std::promise<result> promise;
			};
		}

		template<proto_allocator ProtoAllocator, class Result, class... Args>
		class async_result<use_future_t<ProtoAllocator>, Result(Args...)>
		{
			public:
				using completion_handler_type = details::use_future_ch<void(Args...), Args...>;
				using return_type = std::future<typename completion_handler_type::result>;

				explicit async_result(completion_handler_type& h)
					: future{h.get_future()} {}

				async_result(const async_result&) = delete;
				async_result& operator=(const async_result&) = delete;

				return_type get()
				{
					return std::move(future);
				}
			private:
				return_type future;
		};

		template<class Result, class... Args, class Signature>
		class async_result<std::packaged_task<Result(Args...)>, Signature>
		{
			public:
				using completion_handler_type = std::packaged_task<Result(Args...)>;
				using return_type = std::future<Result>;

				explicit async_result(completion_handler_type& h)
					: future{h.get_future()} {}

				async_result(const async_result&) = delete;
				async_result& operator=(const async_result&) = delete;

				return_type get()
				{
					return std::move(future);
				}
			private:
				return_type future;
		};
	}
}

namespace std
{
	template<class Allocator>
	struct uses_allocator<cmoon::net::executor, Allocator> : std::true_type {};
}