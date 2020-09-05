#pragma once

#include <memory>
#include <algorithm>
#include <utility>
#include <vector>
#include <map>
#include <typeinfo>
#include <typeindex>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>
#include <functional>
#include <any>
#include <future>

#include "collin/concepts.hpp"
#include "collin/utility.hpp"

namespace collin
{
	namespace net
	{
		template<collin::concepts::object CompletionToken, class Signature>
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

		template<class R>
		concept specialized_async_result =
			requires
		{
			typename R::completion_handler_type;
			typename R::return_type;
		} && requires(R r, typename R::completion_handler_type& h)
		{
			{ r(h) };
			{ r.get() } -> collin::concepts::same<typename R::return_type>;
		};

		template<class CompletionToken, class Signature>
		struct async_completion
		{
			using completion_handler_type = typename async_result<std::decay_t<CompletionToken>, Signature>::completion_handler_type;

			explicit async_completion(CompletionToken& t)
				requires(collin::concepts::same<CompletionToken, completion_handler_type>)
				: completion_handler{t} {}

			explicit async_completion(CompletionToken& t)
				requires(!collin::concepts::same<CompletionToken, completion_handler_type>)
				: completion_handler{std::forward<CompletionToken>(t)} {}

			async_completion(const async_completion&) = delete;
			async_completion& operator=(const async_completion&) = delete;

			std::conditional_t<collin::concepts::same<CompletionToken, completion_handler_type>,
				completion_handler_type&,
				completion_handler_type> completion_handler;

			async_result<std::decay_t<CompletionToken>, Signature> result;
		};

		template<class A>
		concept proto_allocator = collin::concepts::copy_constructible<A> &&
			requires
		{
			typename std::allocator_traits<A>;
		};

		struct allocator_arg_t {};

		constexpr allocator_arg_t allocator_arg {};

		namespace details
		{
			template<class T>
			concept has_allocator_type = 
				requires
			{
				typename T::allocator_type;
			};

			template<class T, class ProtoAllocator>
			struct allocator_chooser
			{
				using type = ProtoAllocator;
			};

			template<has_allocator_type T, class ProtoAllocator>
			struct allocator_chooser<T, ProtoAllocator>
			{
				using type = typename T::allocator_type;
			};
		}

		template<class T, proto_allocator ProtoAllocator = std::allocator<void>>
		struct associated_allocator
		{
			using type = typename details::allocator_chooser<T, ProtoAllocator>::type;

			static type get(const T& t, const ProtoAllocator& a = ProtoAllocator{}) noexcept
				requires(details::has_allocator_type<T>)
			{
				return t.get_allocator();
			}

			static type get(const T& t, const ProtoAllocator& a = ProtoAllocator{}) noexcept
				requires(!details::has_allocator_type<T>)
			{
				return a;
			}
		};

		template<class X, class T, class ProtoAllocator = std::allocator<void>>
		concept specialized_associated_allocator = proto_allocator<ProtoAllocator> &&
			requires(const T t, ProtoAllocator a)
		{
			typename X::type;
			{ X::get(t) } -> collin::concepts::same<typename X::type>;
			{ X::get(t, a) } -> collin::concepts::same<typename X::type>;
			noexcept(X::get(t, a));
			noexcept(X::get(t));
		};

		template<class T, proto_allocator ProtoAllocator = std::allocator<void>>
		using associated_allocator_t = typename associated_allocator<T, ProtoAllocator>::type;

		template<class T>
		associated_allocator_t<T> get_associated_allocator(const T& t) noexcept
		{
			return associated_allocator<T>::get(t);
		}

		template<class T, class ProtoAllocator>
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
						: svc{ std::move(other) } {}

					std::unique_ptr<service> svc{ nullptr };
					bool shutdown{ false };

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
					static_assert(collin::concepts::derived_from<key, execution_context::service>);
					static_assert(collin::concepts::derived_from<key, Service>);

					auto ptr = std::make_unique<Service>(*this, std::forward<Args>(args)...);
					services.emplace_back(std::move(ptr));
					key_type_mappings[std::type_index{ typeid(Service::key_type) }] = services.back().svc.get();
				}

				template<class Service>
				inline bool has_service() const noexcept
				{
					using key = typename Service::key_type;
					static_assert(collin::concepts::derived_from<key, execution_context::service>);
					static_assert(collin::concepts::derived_from<key, Service>);

					return key_type_mappings.contains(std::type_index{ typeid(key) });
				}

				std::vector<service_tracker> services;
				std::map<std::type_index, service*> key_type_mappings;
				std::mutex mut;
		};

		template<class Service>
		typename Service::key_type& use_service(execution_context& ctx)
		{
			using key = typename Service::key_type;
			static_assert(collin::concepts::derived_from<key, execution_context::service>);
			static_assert(collin::concepts::derived_from<key, Service>);

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
			static_assert(collin::concepts::derived_from<key, execution_context::service>);
			static_assert(collin::concepts::derived_from<key, Service>);

			std::scoped_lock<std::mutex>{ctx.mut};

			if (ctx.has_service<Service>())
			{
				throw service_already_exists{};
			}

			ctx.add_service(std::forward<Args>(args)...);
			return static_cast<Service&>(*(ctx.services.back().svc.get()));
		}

		template<class Service>
		bool has_service(const execution_context& ctx) noexcept
		{
			using key = typename Service::key_type;
			static_assert(collin::concepts::derived_from<key, execution_context::service>);
			static_assert(collin::concepts::derived_from<key, Service>);

			std::scoped_lock<std::mutex>{ctx.mut};
			return ctx.has_service<Service>();
		}

		class service_already_exists : public std::logic_error {};

		template<class X>
		concept specialized_execution_context = collin::concepts::derived_from<X, execution_context> &&
			requires(X x)
		{
			typename X::executor_type;
			{ x.~X() };
			{ x.get_executor() } -> collin::concepts::same<typename X::executor_type>;
		}; // && specialized_executor<typename X::executor_type>;

		template<class X>
		concept specialized_executor = collin::concepts::copy_constructible<X> &&
			requires(X x1, const X x2, std::allocator<void>(*f)())
		{
			{ f() } -> proto_allocator;
			{ X(x1) };
			{ X(x2) };
			{ x1 == x2 } -> collin::concepts::boolean;
			{ x1 != x2 } -> collin::concepts::boolean;			
			{ x1.on_work_started() };
			{ x1.on_work_finished() };
			noexcept(x1.context());
			noexcept(x1.on_work_started());
			noexcept(x1.on_work_finished());
		} &&
			requires(const X x1, std::allocator<void>(*f)(), std::invoke_result_t<decltype(f)> a)
		{
			{ x1.dispatch(std::move(f), a) };
			{ x1.post(std::move(f), a) };
			{ x1.defer(std::move(f), a) };
		} &&
		(requires(X x1)
		{
			{ x1.context() } -> specialized_execution_context;
		} ||
		requires(X x1)
		{
			{ x1.context() } -> collin::concepts::same<execution_context&>;
		});

		struct executor_arg_t {};
		constexpr executor_arg_t executor_arg = executor_arg_t{};

		template<class T, class Executor>
		concept uses_executor = specialized_executor<Executor> &&
			requires
		{
			typename T::executor_type;
		} && collin::concepts::convertible_to<Executor, typename T::executor_type>;

		template<class T, class Executor, class... Args>
		concept uses_executor_construction = uses_executor<T, Executor> &&
											 collin::concepts::constructible<T, executor_arg_t, Args...>;

		class system_executor;

		namespace details
		{
			template<class T>
			concept has_executor_type =
				requires
			{
				typename T::executor_type;
			};

			template<class T, class Executor>
			struct executor_chooser
			{
				using type = Executor;
			};

			template<has_executor_type T, class Executor>
			struct executor_chooser<T, Executor>
			{
				using type = typename T::executor_type;
			};
		}

		template<class T, specialized_executor Executor = system_executor>
		struct associated_executor
		{
			using type = typename details::executor_chooser::type;

			static type get(const T& t, const Executor& e = Executor()) noexcept
				requires(details::has_executor_type<T>)
			{
				return t.get_executor();
			}

			static type get(const T& t, const Executor& e = Executor()) noexcept
				requires(!details::has_executor_type<T>)
			{
				return e;
			}
		};



		template<class T, specialized_executor Executor = system_executor>
		using associated_executor_t = typename associated_executor<T, Executor>::type;

		template<class T>
		associated_executor_t<T> get_associated_executor(const T& t) noexcept
		{
			return associated_executor<T>::get(t);
		}

		template<class T, specialized_executor Executor>
		associated_executor_t<T, Executor> get_associated_executor(const T& t, const Executor& ex) noexcept
		{
			return associated_executor<T, Executor>::get(t, ex);
		}

		template<class T, class ExecutionContext>
		requires(collin::concepts::convertible_to<ExecutionContext&, execution_context&>)
			associated_executor_t<T, typename ExecutionContext::executor_type>
			get_associated_executor(const T& t, ExecutionContext& ctx) noexcept
		{
			return get_associated_executor(t, ctx.get_executor());
		}

		template<class X, class T, class Executor = system_executor>
		concept specialized_associated_executor = specialized_executor<Executor> &&
			requires(const T t, Executor e)
		{
			typename X::type;
			{ X::get(t) } -> collin::concepts::same<typename X::type>;
			{ X::get(t, e) } -> collin::concepts::same<typename X::type>;
			noexcept(X::get(t, e));
			noexcept(X::get(t));
		};

		template<class T, specialized_executor Executor>
		class executor_binder
		{
			public:
				using target_type = T;
				using executor_type = Executor;

				executor_binder(T t, const Executor& ex)
					requires(uses_executor_construction<T, Executor>)
					: ex_{ex}, target_{std::move(t), ex} {}

				executor_binder(T t, const Executor& ex)
					requires(!uses_executor_construction<T, Executor>)
					: ex_{ex}, target_{std::move(t)} {}

				template<class U, specialized_executor OtherExecutor>
					requires(collin::concepts::convertible_to<U, T> && collin::concepts::convertible_to<OtherExecutor, Executor>)
				executor_binder(const executor_binder<U, OtherExecutor>& other)
					: ex_{other.get_executor()}, target_{other.get(), ex_} {}

				template<class U, specialized_executor OtherExecutor>
				requires(collin::concepts::convertible_to<U, T>&& collin::concepts::convertible_to<OtherExecutor, Executor>)
				executor_binder(executor_binder<U, OtherExecutor>&& other)
					: ex_{other.get_executor()}, target_{std::move(other.get()), ex_} {}

				template<class U, specialized_executor OtherExecutor>
					requires(collin::concepts::convertible_to<U, T>)
				executor_binder(executor_arg_t, const Executor& ex, const executor_binder<U, OtherExecutor>& other)
					: ex_{ex}, target_{other.get(), ex_} {}

				template<class U, specialized_executor OtherExecutor>
					requires(collin::concepts::convertible_to<U, T>)
				executor_binder(executor_arg_t, const Executor& ex, executor_binder<U, OtherExecutor>&& other)
					: ex_{ex}, target_{std::move(other.get()), ex_} {}

				target_type& get() noexcept
				{
					return target_;
				}

				const target_type& get() const noexcept
				{
					return target_;
				}

				executor_type get_executor() const noexcept
				{
					return ex_;
				}

				template<class... Args>
				std::invoke_result_t<target_type&(Args&&...)> operator()(Args&&... args)
				{
					return std::invoke(get(), std::forward<Args>(args)...);
				}

				template<class... Args>
				std::invoke_result_t<const target_type&(Args&&...)> operator()(Args&&... args) const
				{
					return std::invoke(get(), std::forward<Args>(args)...);
				}

			private:
				executor_type ex_;
				target_type target_;
		};

		template<class T, specialized_executor Executor, class Signature>
		class async_result<executor_binder<T, Executor>, Signature>
		{
			public:
				using completion_handler_type = executor_binder<
					typename async_result<T, Signature>::completion_handler_type,
					Executor
				>;

				using return_type = typename async_result<T, Signature>::return_type;

				explicit async_result(completion_handler_type& h)
					: target_{h.get()} {};

				async_result(const async_result&) = delete;
				async_result& operator=(const async_result&) = delete;

				return_type get()
				{
					return target_.get();
				}

			private:
				async_result<T, Signature> target_;
		};

		template<class T, specialized_executor Executor, proto_allocator ProtoAllocator>
		struct associated_allocator<executor_binder<T, Executor>, ProtoAllocator>
		{
			using type = associated_allocator_t<T, ProtoAllocator>;

			static type get(const executor_binder<T, Executor>& b, const ProtoAllocator& a = ProtoAllocator()) noexcept
			{
				return associated_allocator<T, ProtoAllocator>::get(b.get(), a);
			}
		};

		template<class T, specialized_executor Executor1, specialized_executor Executor2>
		struct associated_executor<executor_binder<T, Executor1>, Executor2>
		{
			using type = Executor2;

			static type get(const executor_binder<T, Executor2>& b, const Executor1& e = Executor1()) noexcept
			{
				b.get_executor();
			}
		};

		template<specialized_executor Executor, class T>
		executor_binder<std::decay_t<T>, Executor> bind_executor(const Executor& ex, T&& t)
		{
			return executor_binder<std::decay_t<T>, Executor>(std::forward<T>(t), ex);
		}

		template<class ExecutionContext, class CompletionToken>
			requires(collin::concepts::convertible_to<ExecutionContext&, execution_context&>)
		executor_binder<std::decay_t<CompletionToken>, typename ExecutionContext::executor_type> bind_executor(ExecutionContext& ctx, CompletionToken&& t)
		{
			return bind_executor(ctx.get_executor(), std::forward<T>(t));
		}

		template<specialized_executor Executor>
		class executor_work_guard
		{
			public:
				using executor_type = Executor;

				explicit executor_work_guard(const executor_type& ex) noexcept
					: ex{ex}, owns_{true}
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
					: ex_{std::move(other.ex_)}, owns_{other.owns_}
				{
					other.owns_ = false;
				}

				executor_work_guard& operator=(const executor_work_guard&) = delete;

				~executor_work_guard()
				{
					if (owns_)
					{
						ex_.on_work_started();
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

		template<specialized_executor Executor>
		executor_work_guard<Executor> make_work_guard(const Executor& ex)
		{
			return executor_work_guard<Executor>{ex};
		}

		template<class ExecutionContext>
			requires(collin::concepts::convertible_to<ExecutionContext&, execution_context&>)
		executor_work_guard<typename ExecutionContext::executor_type> make_work_guard(ExecutionContext& ctx)
		{
			return make_work_guard(ctx.get_executor());
		}

		template<class T>
			requires(!specialized_executor<T> && collin::concepts::convertible_to<T&, execution_context&>)
		executor_work_guard<associated_executor_t<T>> make_work_guard(const T& t)
		{
			return make_work_guard(get_associated_executor(t));
		}

		template<class T, class U>
		auto make_work_guard(const T& t, U&& u) -> decltype(make_work_guard(get_associated_executor(t, std::foward<U>(u))))
		{
			return make_work_guard(get_associated_executor(t, std::foward<U>(u)));
		}

		class bad_executor : public std::exception
		{
			public:
				bad_executor() noexcept {}

				const char* what() const noexcept override
				{
					return "bad executor";
				}
		};

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
					std::scoped_lock l{mtx_};
					stopped_ = true;
					cv_.notify_all();
				}

				bool stopped() const noexcept
				{
					std::scoped_lock l{mtx_};
					return stopped_;
				}

				void join()
				{
					thread_.join();
				}
			private:
				struct tag {};
				system_context(tag) {}
				friend system_executor;

				mutable std::mutex mtx_;
				std::thread thread_;
				std::condition_variable cv_;
				std::queue<std::function<void()>> tasks_;
				bool stopped_{false};

				static system_context& get_ctx() noexcept
				{
					static system_context ctx{{}};
					return ctx;
				}

				void run()
				{
					while (true)
					{
						std::function<void()> f;
						{
							std::unique_lock l{mtx_};
							cv_.wait(l, [this](){ return !stopped_ && !tasks_.empty();});
							if (stopped_)
							{
								return;
							}

							f = std::move(tasks_.front());
							tasks_.pop();
						}

						f();
					}
				}

				void post(std::function<void()> f)
				{
					std::scoped_lock l{mtx_};
					if (stopped_)
					{
						return;
					}

					if (!thread_.joinable())
					{
						thread_ = std::thread(&system_context::run, this);
					}
					tasks_.push(std::move(f));
					cv_.notify_one();
				}
		};
		
		class system_executor
		{
			public:
				system_executor() {}

				system_context& context() const noexcept
				{
					return system_context::get_ctx();
				}

				void on_work_started() const noexcept {}
				void on_work_finished() const noexcept {}

				template<collin::concepts::callable Func, proto_allocator ProtoAllocator>
				void dispatch(Func&& f, const ProtoAllocator& a) const
				{
					collin::decay_copy(std::forward<Func>(f))();
				}

				template<collin::concepts::callable Func, proto_allocator ProtoAllocator>
				void post(Func&& f, const ProtoAllocator& a) const
				{
					context().post(std::forward<Func>(f));
				}

				template<collin::concepts::callable Func, proto_allocator ProtoAllocator>
				void defer(Func&& f, const ProtoAllocator& a) const
				{
					post(std::forward<Func>(f), a);
				}
			private:
				friend system_context;
		};

		typename system_context::executor_type system_context::get_executor() noexcept
		{
			return typename system_context::executor_type{};
		}

		static_assert(specialized_execution_context<system_context>);

		bool operator==(const system_executor&, const system_executor&) noexcept
		{
			return true;
		}

		bool operator!=(const system_executor&, const system_executor&) noexcept
		{
			return false;
		}

		//static_assert(collin::concepts::default_constructible<system_executor> && specialized_executor<system_executor>);  // MSVC flags this as a failed assertion, because it is dumb.

		class executor
		{
			public:
				executor() noexcept = default;
				executor(const executor& e) noexcept = default;
				executor(executor&& e) noexcept = default;
				executor(nullptr_t) noexcept {}

				template<specialized_executor Executor>
				executor(Executor e)
					: target_{create<Executor>(std::move(e))} {}

				template<specialized_executor Executor, proto_allocator ProtoAllocator>
				executor(allocator_arg_t, const ProtoAllocator& a, Executor e)
					: target_{create<Executor, ProtoAllocator>(std::move(e), a)} {}

				executor& operator=(const executor& e) noexcept = default;
				executor& operator=(executor&& e) noexcept = default;
				executor& operator=(nullptr_t) noexcept
				{
					target_.reset();
					return *this;
				}

				template<specialized_executor Executor>
				executor& operator=(Executor e)
				{
					target_ = create<Executor>(std::move(e));
					return *this;
				}

				~executor() = default;

				void swap(executor& other) noexcept
				{
					target_.swap(other.target_);
				}

				template<specialized_executor Executor, proto_allocator ProtoAllocator>
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
				void dispatch(Func&& f, const ProtoAllocator& a) const
				{
					if (!*this)
					{
						throw bad_executor{};
					}
					
					target_->dispatch(std::forward<Func>(f), a);
				}

				template<class Func, class ProtoAllocator>
				void post(Func&& f, const ProtoAllocator& a) const
				{
					if (!*this)
					{
						throw bad_executor{};
					}

					target_->post(std::forward<Func>(f), a);
				}

				template<class Func, class ProtoAllocator>
				void defer(Func&& f, const ProtoAllocator& a) const
				{
					if (!*this)
					{
						throw bad_executor{};
					}

					target_->defer(std::forward<Func>(f), a);
				}

				explicit operator bool() const noexcept
				{
					return static_cast<bool>(target_);
				}

				const type_info& target_type() const noexcept
				{
					if (*this)
					{
						return target_->target_type();
					}
					
					return typeid(void);
				}

				template<specialized_executor Executor>
				Executor* target() noexcept
				{
					if(*this)
					{
						return static_cast<Executor*>(target_->get_executor());
					}

					return nullptr;
				}

				template<specialized_executor Executor>
				const Executor* target() const noexcept
				{
					if(*this)
					{
						return static_cast<const Executor*>(target->get_executor());
					}

					return nullptr;
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
					virtual const std::type_info& target_type() const = 0;
					virtual void* target(const std::type_info&) const = 0;
					virtual bool operator==(const target_interface*) const noexcept = 0;
					virtual void* get_executor() noexcept = 0;
					virtual const void* get_executor() const noexcept = 0;
				};

				template<specialized_executor Executor, proto_allocator ProtoAllocator>
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

				template<specialized_executor Executor, proto_allocator ProtoAllocator = std::allocator<void>>
				static std::shared_ptr<target_interface> create(Executor&& ex, const ProtoAllocator alloc = ProtoAllocator())
				{
					return std::allocate_shared<target_impl<Executor, ProtoAllocator>>(alloc, std::forward<Executor>(ex), alloc);
				}

				std::shared_ptr<target_interface> target_;

				friend bool operator==(const executor& a, const executor& b) noexcept
				{
					return (!a && !b) ||
						   (a.target_.get() == b.target_.get()) ||
						   (a.target_type() == b.target_type() && *a.target_.get() == b.target_.get());
				}

				friend bool operator==(const executor& a, nullptr_t) noexcept
				{
					return !a;
				}

				friend bool operator==(nullptr_t, const executor& b) noexcept
				{
					return !b;
				}

				friend bool operator!=(const executor& a, const executor& b) noexcept
				{
					return !(a == b);
				}

				friend bool operator!=(const executor& a, nullptr_t) noexcept
				{
					return static_cast<bool>(a);
				}

				friend bool operator!=(nullptr_t, const executor& b) noexcept
				{
					return static_cast<bool>(b);
				}
		};

		void swap(executor& a, executor& b) noexcept
		{
			a.swap(b);
		}

		//static_assert(collin::concepts::default_constructible<executor> &&
		//			  collin::concepts::copy_assignable<executor> &&
		//			  specialized_executor<executor>);

		template<class CompletionToken>
		auto dispatch(CompletionToken&& token)
		{
			async_completion<CompletionToken, void()> completion{std::forward<CompletionToken>(token)};

			auto ex = get_associated_executor(completion.completion_handler);
			auto alloc = get_associated_allocator(completion.completion_handler);
			ex.dispatch(std::move(completion.completion_handler), alloc);
			return completion.result.get();
		}

		namespace details
		{
			template<class Executor, class CompletionHandler>
			struct f_obj
			{
				using handler_type = CompletionHandler;
				using guard_type = executor_work_guard<Executor>;

				f_obj(handler_type&& h)
					: h{std::forward<handler_type>(h)}, w{make_work_guard(h)} {}

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

		template<specialized_executor Executor, class CompletionToken>
		auto dispatch(const Executor& ex, CompletionToken&& token)
		{
			async_completion<CompletionToken, void()> completion{std::forward<CompletionToken>(token)};
			auto alloc = get_associated_allocator(completion.completion_handler);
			details::f_obj f{std::move(completion.completion_handler)};

			ex.dispatch(std::move(f), alloc);

			return completion.result.get();
		}

		template<class ExecutionContext, class CompletionToken>
			requires(collin::concepts::convertible_to<ExecutionContext&, execution_context&>)
		auto dispatch(ExecutionContext& ctx, CompletionToken&& token)
		{
			dispatch(ctx.get_executor(), std::forward<CompletionToken>(token));
		}

		template<class CompletionToken>
		auto post(CompletionToken&& token)
		{
			async_completion<CompletionToken, void()> completion{std::forward<CompletionToken>(token)};

			auto ex = get_associated_executor(completion.completion_handler);
			auto alloc = get_associated_allocator(completion.completion_handler);

			ex.post(std::move(completion.completion_handler), alloc);
			return completion.result.get();
		}

		template<specialized_executor Executor, class CompletionToken>
		auto post(const Executor& ex, CompletionToken&& token)
		{
			async_completion<CompletionToken, void()> completion{std::forward<CompletionToken>(token)};
			auto alloc = get_associated_allocator(completion.completion_handler);
			details::f_obj f{std::move(completion.completion_handler)};

			es.post(std::move(f), alloc);

			return completion.result.get();
		}

		template<class ExecutionContext, class CompletionToken>
			requires(collin::concepts::convertible_to<ExecutionContext&, execution_context&>)
		auto post(ExecutionContext& ctx, CompletionToken&& token)
		{
			return post(ctx.get_executor(), std::forward<CompletionToken>(token));
		}

		template<class CompletionToken>
		auto defer(CompletionToken&& token)
		{
			async_completion<CompletionToken, void()> completion{std::forward<CompletionToken>(token)};

			auto ex = get_associated_executor(completion.completion_handler);
			auto alloc = get_associated_allocator(completion.completion_handler);

			ex.defer(std::move(completion.completion_handler), alloc);
			return completion.result.get();
		}

		template<specialized_executor Executor, class CompletionToken>
		auto defer(const Executor& ex, CompletionToken&& token)
		{
			async_completion<CompletionToken, void()> completion{std::forward<CompletionToken>(token)};
			auto alloc = get_associated_allocator(completion.completion_handler);
			details::f_obj f{std::move(completion.completion_handler)};

			ex.defer(std::move(f), alloc);

			return completion.result.get();
		}

		template<class ExecutionContext, class CompletionToken>
			requires(collin::concepts::convertible_to<ExecutionContext&, execution_context&>)
		auto defer(ExecutionContext& ctx, CompletionToken&& token)
		{
			return defer(ctx.get_executor(), std::forward<CompletionToken>(token));
		}

		template<specialized_executor Executor>
			requires(collin::concepts::default_constructible<Executor>)
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

				strand(const strand& other) noexcept requires(collin::concepts::copy_assignable<Executor>) = default;
				strand(strand&& other) noexcept requires(collin::concepts::move_assignable<Executor>) = default;

				template<specialized_executor OtherExecutor>
				strand(const strand<OtherExecutor>& other) noexcept
					: inner_ex_{other.inner_ex_}, state_{other.state_} {}

				template<specialized_executor OtherExecutor>
				strand(strand<OtherExecutor>&& other) noexcept
					: inner_ex_{std::move(other.inner_ex_)}, state_{std::move(other.state_)} {}

				template<specialized_executor OtherExecutor>
					requires(collin::concepts::convertible_to<OtherExecutor, Executor> && collin::concepts::copy_assignable<Executor>)
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

				template<specialized_executor OtherExecutor>
					requires(collin::concepts::convertible_to<OtherExecutor, Executor> && collin::concepts::move_assignable<Executor>)
				strand& operator=(strand<OtherExecutor>&& other) noexcept
				{
					if (this != std::addressof(other))
					{
						inner_ex_ = std::move(other.inner_ex_);
						std::scoped_lock l{other.mtx_};
						state_ = std::move(other.state_);
					}

					return *this;
				}

				~strand() = default;

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
					return inner_ex_.on_work_started();
				}

				void on_work_finished() const noexcept
				{
					return inner_ex_.on_work_finished();
				}

				template<collin::concepts::callable Func, proto_allocator ProtoAllocator>
				void dispatch(Func&& f, const ProtoAllocator& a) const
				{
					if (running_in_this_thread())
					{
						collin::decay_copy<Func>(std::forward<Func>(f))();
					}
					else
					{
						post(std::forward<Func>(f), a);
					}
				}

				template<collin::concepts::callable Func, proto_allocator ProtoAllocator>
				void post(Func&& f, const ProtoAllocator& a) const
				{
					std::scoped_lock l{state->mtx_};
					state_->tasks_.push(std::forward<Func>(f));
					state_->cv_.notify_one();
				}

				template<collin::concepts::callable Func, proto_allocator ProtoAllocator>
				void defer(Func&& f, const ProtoAllocator& a) const
				{
					post(std::forward<Func>(f), a);
				}

			private:
				Executor inner_ex_ {};

				struct state;

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
							// we are not the only holder of the state, there is a task available, or we are ready to execute the next task.
							owner_state->cv_.wait(l, [=](){ return owner_state.use_count() != 1 && !owner_state->tasks_.empty() && ready });

							// If we are the only one holding onto the shared state (all associated parent strands have
							// been destroyed) and if there are no more tasks to complete, then we are done.
							if (owner_state.use_count() == 1 && owner_state->tasks_.empty())
							{
								break;
							}

							if (!ready || !owner_state->tasks_.empty())
							{
								continue;
							}

							f = std::move(tasks_.front());
							owner_state->ready = false;
						}

						auto completion_handler = [&owner_state, f{std::move(f)}]()
						{
							f();
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

				friend bool operator==(const strand& a, const strand& b)
				{
					return a.state_->thread_.get_id() == b.state_->thread_.get_id();
				}

				friend bool operator!=(const strand& a, const strand& b)
				{
					return !(a == b);
				}

			static_assert(specialized_executor<strand<Executor>>);
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
					: alloc{a} {}

				template<proto_allocator OtherProtoAllocator>
				use_future_t<OtherProtoAllocator> rebind(const OtherProtoAllocator& a) const noexcept
				{
					return use_future_t<OtherProtoAllocator>{a};
				}

				allocator_type get_allocator() const noexcept
				{
					return alloc;
				}

				template<class F>
				auto operator()(F&& f) const
				{
					using token = details::use_future_ct<std:decay_t<F>, ProtoAllocator>;
					return token{std::forward<F>(f), alloc};
				}
			private:
				allocator_type alloc{};
		};

		namespace details
		{
			template<class Result, specialized_executor Executor>
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
					template<class Result, specialized_executor Executor>
					friend class use_future_ex;

					Func f;
					mutable std::promise<result> promise;
			};
		}

		template<class Func, class ProtoAllocator, class Result, class... Args>
		class async_result<details::use_future_ct<Func, ProtoAllocator>, Result(Args...)>
		{
			public:
				using completion_handler_type = details::use_future_ch<Func, Args...>;
				using return_type = std::future<typename completion_handler_type::result>;

				explicit async_result(completion_handler_type& h)
					: future{h.get_future()} {}

				async_result(const async_result&) = delete;
				async_result& operator=(const async_result&) = delete;
			private:
				return_type future;
		};

		namespace details
		{
			template<class Result, specialized_executor Executor>
			class use_future_ex
			{
				public:
					template<class Handler>
					use_future_ex(const Handler& h, Executor ex)
						: promise{h.promise}, ex{ex} {}

					template<collin::concepts::callable Func, proto_allocator ProtoAllocator>
					void dispatch(Func&& f)
					{
						try
						{
							ex.dispatch(std::forward<Func>(f));
						}
						catch (...)
						{
							promise.set_exception(std::current_exception());
						}
					}

					template<collin::concepts::callable Func, proto_allocator ProtoAllocator>
					void post(Func&& f)
					{
						try
						{
							ex.post(std::forward<Func>(f));
						}
						catch (...)
						{
							promise.set_exception(std::current_exception());
						}
					}

					template<collin::concepts::callable Func, proto_allocator ProtoAllocator>
					void defer(Func&& f)
					{
						try
						{
							ex.defer(std::forward<Func>(f));
						}
						catch (...)
						{
							promise.set_exception(std::current_exception());
						}
					}
				private:
					std::promise<Result> promise;
					Executor ex;
			};
		}

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
	struct uses_allocator<collin::net::executor, Allocator> : std::true_type {};
}