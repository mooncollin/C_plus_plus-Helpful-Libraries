export module cmoon.net.execution.execution_context;

import <stdexcept>;
import <map>;
import <typeindex>;
import <typeinfo>;
import <memory>;
import <vector>;
import <ranges>;

import cmoon.memory;

import cmoon.net.execution.fork_event;

namespace cmoon::net
{
	export
	class service_already_exists : public logic_error {};

	export
	class execution_context
	{
		public:
			class service
			{
				protected:
					explicit service(execution_context& owner)
						: context_{owner} {}

					service(const service&) = delete;
					service& operator=(const service&) = delete;
					virtual ~service() = default;

					execution_context& context() noexcept
					{
						return context_;
					}
				private:
					virtual void shutdown() noexcept = 0;
					virtual void notify_fork(fork_event e) {}

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
				switch (e)
				{
					case fork_event::prepare:
						for (auto& service : services | std::views::reverse)
						{
							service->notify_fork(e);
						}
						break;
					default:
						for (auto& service : services)
						{
							service->notify_fork(e);
						}
						break;
				}
			}

		protected:
			void shutdown() noexcept
			{
				for (auto& service : services | std::views::reverse)
				{
					service->shutdown();
				}
			}

			void destroy() noexcept
			{
				for (auto& service : services | std::views::reverse)
				{
					service->destroy();
				}
				services.clear();
				service_type_mapping.clear();
			}
		private:
			std::vector<std::unique_ptr<service>> services;
			std::map<std::type_index, cmoon::observer_ptr<service>> service_type_mapping;

			template<class Service>
			friend typename Service& use_service(execution_context& ctx);

			template<class Service, class... Args>
			friend Service& make_service(execution_context& ctx, Args&&... args);

			template<class Service>
			friend bool has_service(const execution_context& ctx) noexcept;
	};

	export 
	template<class Service>
	typename Service& use_service(execution_context& ctx)
	{
		const auto it = ctx.service_type_mapping.find(typeid(Service));
		if (it == std::end(ctx.service_type_mapping))
		{
			const auto& svc = ctx.services.emplace_back(std::make_unique_ptr<Service>(ctx));
			service_type_mapping[typeid(Service)] = cmoon::observer_ptr{svc.get()};

			return *dynamic_cast<Service*>(svc.get());
		}
		else
		{
			return *dynamic_cast<Service*>(it->second.get());
		}
	}

	export
	template<class Service>
	bool has_service(const execution_context& ctx) noexcept
	{
		return ctx.service_type_mapping.contains(typeid(Service));
	}

	export
	template<class Service, class... Args>
	Service& make_service(execution_context& ctx, Args&&... args)
	{
		if (has_service<Service>(ctx))
		{
			throw service_already_exists{};
		}

		const auto& svc = ctx.services.emplace_back(std::make_unique_ptr<Service>(ctx, std::forward<Args>(args)...));
		service_type_mapping[typeid(Service)] = cmoon::observer_ptr{svc.get()};
		return *dynamic_cast<Service*>(svc.get());
	}

	
}