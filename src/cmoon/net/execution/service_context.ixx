export module cmoon.net.execution.service_context;

import <stdexcept>;
import <exception>;
import <deque>;
import <map>;
import <memory>;
import <typeindex>;
import <typeinfo>;
import <concepts>;
import <type_traits>;
import <utility>;
import <mutex>;
import <shared_mutex>;
import <ranges>;

namespace cmoon::net
{
	export
	class service_already_exists : public std::logic_error
	{
		public:
			service_already_exists()
				: std::logic_error{"Service already exists"} {}
	};

	struct wrapper
	{
		virtual ~wrapper() noexcept = default;
	};

	template<std::destructible T>
	struct wrapper_impl : public wrapper
	{
		template<class... Args>
			requires(std::constructible_from<T, Args...>)
		wrapper_impl(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
			: value{std::forward<Args>(args)...} {}

		T value;
	};

	export
	template<class Allocator = std::allocator<wrapper>>
	class service_context
	{
		using wrapper_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<wrapper>;
		using deque_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<wrapper*>;
		using map_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<std::pair<const std::type_index, wrapper*>>;

		public:
			service_context(const Allocator& a = {})
				: alloc{a}, services{a}, service_type_mapping{a} {}

			template<std::default_initializable Service>
			Service& use_service()
			{
				std::unique_lock l {services_m};
				const auto it {service_type_mapping.find(typeid(Service))};
				if (it == std::ranges::end(service_type_mapping))
				{
					return create_service<Service>(std::move(l));
				}

				return *dynamic_cast<Service*>(it->second);
			}

			template<class Service>
			[[nodiscard]] bool has_service() const noexcept
			{
				std::shared_lock l {services_m};
				return service_type_mapping.contains(typeid(Service));
			}

			template<class Service, class... Args>
				requires(std::constructible_from<Service, Args...>)
			Service& make_service(Args&&... args)
			{
				std::unique_lock l {services_m};
				if (service_type_mapping.contains(typeid(Service)))
				{
					throw service_already_exists{};
				}

				return create_service<Service>(std::move(l), std::forward<Args>(args)...);
			}

			void shutdown() noexcept
			{
				std::scoped_lock l {services_m};
				while (!services.empty())
				{
					auto next {services.back()};
					std::destroy_at(next);
					alloc.deallocate(next, 1);
					services.pop_back();
				}
				service_type_mapping.clear();
			}

			~service_context() noexcept
			{
				shutdown();
			}
		private:
			[[no_unique_address]] wrapper_allocator alloc;
			std::deque<wrapper*, deque_allocator> services;
			std::map<std::type_index, wrapper*, std::less<std::type_index>, map_allocator> service_type_mapping;
			mutable std::shared_mutex services_m;

			template<class Service, class... Args>
			Service& create_service(std::unique_lock<std::shared_mutex> l, Args&&... args)
			{
				l.unlock();
				typename std::allocator_traits<Allocator>::template rebind_alloc<wrapper_impl<Service>> service_alloc {alloc};
				auto svc {service_alloc.allocate(1)};

				try
				{
					std::construct_at(svc, std::forward<Args>(args)...);
				}
				catch (...)
				{
					service_alloc.deallocate(svc, 1);
					std::rethrow_exception(std::current_exception());
				}

				try
				{
					l.lock();
					services.push_back(svc);
				}
				catch (...)
				{
					l.unlock();
					std::destroy_at(svc);
					service_alloc.deallocate(svc, 1);
					std::rethrow_exception(std::current_exception());
				}

				try
				{
					service_type_mapping[typeid(Service)] = svc;
				}
				catch (...)
				{
					services.pop_back();
					l.unlock();
					std::destroy_at(svc);
					service_alloc.deallocate(svc, 1);
					std::rethrow_exception(std::current_exception());
				}

				return svc->value;
			}
	};
}