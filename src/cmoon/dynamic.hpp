#pragma once
#include <filesystem>
#include <exception>
#include <system_error>
#include <string_view>
#include <memory>
#include <vector>

#ifdef _WIN32
	#include <Windows.h>
#else
	#include <dlfcn.h>
	#include <errno.h>
#endif

namespace cmoon
{
	class DynamicModule
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

			DynamicModule(std::filesystem::path module_path, flag_type flags={})
				: module_path_(std::move(module_path)), flags_(flags)
			{
				load_module(false);
			}

			DynamicModule(const DynamicModule& other)
				: module_path_(other.module_path_), flags_(other.flags_)
			{
				if(this != &other)
				{
					load_module(true);
				}
			}
			DynamicModule(DynamicModule&&) = default;
			DynamicModule& operator=(const DynamicModule& other)
			{
				if(this != &other)
				{
					module_path_ = other.module_path_;
					flags_ = other.flags_;
					load_module(true);
				}
			}
			DynamicModule& operator=(DynamicModule&&) = default;

			friend bool operator==(const DynamicModule&, const DynamicModule&);

			~DynamicModule()
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
			T get_function(std::string_view symbol)
			{
				#ifdef _WIN32
				const auto f = ::GetProcAddress(handle_, symbol.data());
				if(f == NULL)
				{
					throw std::system_error(std::error_code{static_cast<int>(::GetLastError()), std::system_category()});
				}
				return reinterpret_cast<T>(f);
				#else
				::dlerror();
				const auto f = ::dlsym(handle_, symbol.data());
				const auto error = ::dlerror();
				if(error != NULL)
				{
					throw std::system_error({::errno(), std::system_category()}, error);
				}
				return reinterpret_cast<T>(f);
				#endif
			}

		private:
			std::filesystem::path module_path_;
			flag_type flags_;
			handle_type handle_ = NULL;

			void load_module(bool copy)
			{
				const auto path_string = module_path_.string();
				#ifdef _WIN32
				if(copy)
				{
					::GetModuleHandleExA(flags_, path_string.c_str(), &handle_);
				}
				else
				{
					handle_ = ::LoadLibraryExA(path_string.c_str(), NULL, flags_);
				}
				if(handle_ == NULL)
				{
					throw std::system_error(std::error_code{static_cast<int>(::GetLastError()), std::system_category()});
				}
				#else
				::dlerror();
				handle_ = ::dlopen(module_path_.c_str(), flags_);
				const auto error = ::dlerror();
				if(error != NULL)
				{
					throw std::system_error({::errno(), std::system_category()}, error);
				}
				#endif
			}

			void unload_module()
			{
				#ifdef _WIN32
				::FreeLibrary(handle_);
				#else
				::dlerror();
				::dlclose(handle_);
				#endif
			}
	};

	bool operator==(const DynamicModule& first, const DynamicModule& second)
	{
		return first.handle_ == second.handle_;
	}

	bool operator!=(const DynamicModule& first, const DynamicModule& second)
	{
		return !(first == second);
	}

	template<class T, bool Automatic_Free>
	class PluginManager
	{
		public:
			using function_type = T;
			using plugin_type = std::invoke_result_t<function_type>;

			PluginManager(std::string_view init_function_name)
				: initializer_function_name_(init_function_name) {}
			PluginManager(const PluginManager&) = delete;
			PluginManager(PluginManager&&) = default;
			PluginManager& operator=(const PluginManager&) = delete;
			PluginManager& operator=(PluginManager&&) = default;
			~PluginManager()
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

			void add_plugin(DynamicModule&& plugin)
			{
				plugin_modules_.push_back(plugin);
			}

			std::vector<DynamicModule>& plugins_modules()
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
			std::vector<DynamicModule> plugin_modules_;
			std::vector<plugin_type> plugins_;
	};
}