module;

#ifdef _WIN32
	#include <Windows.h>
#else
	#include <dlfcn.h>
	#include <errno.h>
#endif

export module cmoon.dynamic;

import <filesystem>;
import <exception>;
import <system_error>;
import <string_view>;
import <memory>;
import <vector>;

import cmoon.platform;

namespace cmoon
{
	export
	class dynamic_module
	{
		public:
			#ifdef _WIN32
				using flag_type = DWORD;
				using handle_type = HMODULE;
				using function_type = FARPROC;
			#else	
				using flag_type = int;
				using handle_type = void*;
				using function_type = void*;
			#endif

			dynamic_module(std::filesystem::path module_path, flag_type flags={}, [[maybe_unused]] bool already_loaded = false)
				: module_path_{std::move(module_path)}, flags_{flags}, handle{nullptr}
			{
				if constexpr (cmoon::win32_api)
				{
					if (already_loaded)
					{
						load_module<true>();
					}
					else
					{
						load_module<false>();
					}
				}
				else
				{
					load_module<false>();
				}
			}

			dynamic_module(const dynamic_module& other)
				: module_path_{other.module_path_}, flags_{other.flags_}
			{
				load_module<true>();
			}

			dynamic_module(dynamic_module&&) = default;

			dynamic_module& operator=(const dynamic_module& other)
			{
				if(this != std::addressof(other))
				{
					module_path_ = other.module_path_;
					flags_ = other.flags_;
					load_module<true>();
				}

				return *this;
			}

			dynamic_module& operator=(dynamic_module&&) = default;

			friend bool operator==(const dynamic_module&, const dynamic_module&);

			~dynamic_module() noexcept
			{
				unload_module();
			}

			const std::filesystem::path& module_path() const noexcept
			{
				return module_path_;
			}

			flag_type flags() const noexcept
			{
				return flags_;
			}

			template<typename T = function_type>
			T get_function(const std::string& symbol)
			{
				#ifdef _WIN32
				const auto f = ::GetProcAddress(handle_, symbol.c_str());
				if(f == nullptr)
				{
					throw std::system_error(std::error_code{static_cast<int>(::GetLastError()), std::system_category()});
				}
				return reinterpret_cast<T>(f);
				#else
				::dlerror();
				const auto f = ::dlsym(handle_, symbol.c_str());
				const auto error = ::dlerror();
				if(error != nullptr)
				{
					throw std::system_error({::errno(), std::system_category()}, error);
				}
				return reinterpret_cast<T>(f);
				#endif
			}

		private:
			std::filesystem::path module_path_;
			flag_type flags_;
			handle_type handle_;

			template<bool Copy>
			void load_module()
			{
				const auto path_string = module_path_.string();
				#ifdef _WIN32
				if constexpr (Copy)
				{
					::GetModuleHandleExA(flags_, path_string.c_str(), &handle_);
				}
				else
				{
					handle_ = ::LoadLibraryExA(path_string.c_str(), NULL, flags_);
				}

				if(handle_ == nullptr)
				{
					throw std::system_error(std::error_code{static_cast<int>(::GetLastError()), std::system_category()});
				}
				#else
				::dlerror();
				handle_ = ::dlopen(module_path_.c_str(), flags_);
				const auto error = ::dlerror();
				if(error != nullptr)
				{
					throw std::system_error({::errno(), std::system_category()}, error);
				}
				#endif
			}

			void unload_module() const
			{
				#ifdef _WIN32
				if (!(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT & flags_))
				{
					::FreeLibrary(handle_);
				}
				#else
				::dlerror();
				::dlclose(handle_);
				#endif
			}
	};

	bool operator==(const dynamic_module& first, const dynamic_module& second)
	{
		return first.handle_ == second.handle_;
	}

	bool operator!=(const dynamic_module& first, const dynamic_module& second)
	{
		return !(first == second);
	}

	template<class T, bool Automatic_Free>
	class plugin_manager
	{
		public:
			using function_type = T;
			using plugin_type = std::invoke_result_t<function_type>;

			plugin_manager(std::string_view init_function_name)
				: initializer_function_name_(init_function_name) {}
			plugin_manager(const plugin_manager&) = delete;
			plugin_manager(plugin_manager&&) = default;
			plugin_manager& operator=(const plugin_manager&) = delete;
			plugin_manager& operator=(plugin_manager&&) = default;
			~plugin_manager()
			{
				if constexpr (Automatic_Free && std::is_pointer_v<plugin_type>)
				{
					for (auto plugin : plugins_)
					{
						if (plugin != nullptr)
						{
							if constexpr (std::is_array_v<plugin_type>)
							{
								delete[] plugin;
							}
							else
							{
								delete plugin;
							}
						}
					}
				}
			}

			void add_plugin(dynamic_module&& plugin)
			{
				plugin_modules_.push_back(plugin);
			}

			std::vector<dynamic_module>& plugins_modules()
			{
				return plugin_modules_;
			}

			std::vector<plugin_type>& plugins()
			{
				return plugins_;
			}

			const std::string& initializer_function_name() const
			{
				return initializer_function_name_;
			}

			constexpr bool is_automatic_freeing() const
			{
				return Automatic_Free;
			}

			void initialize_plugins()
			{
				for (auto& module_ : plugin_modules_)
				{
					const auto function = module_.get_function<function_type>(initializer_function_name_);
					plugins_.push_back(function());
				}
			}
		private:
			std::string initializer_function_name_;
			std::vector<dynamic_module> plugin_modules_;
			std::vector<plugin_type> plugins_;
	};
}