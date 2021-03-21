#pragma once

#include <string>
#include <cstdint>
#include <utility>
#include <optional>
#include <vector>
#include <algorithm>
#include <type_traits>

#include <sysinfoapi.h>
#include <winbase.h>
#include <lmcons.h>
#include <windows.h>

#ifdef max
#undef max
#endif

#include "cmoon/platform.hpp"

namespace cmoon
{
	namespace windows
	{
		using ansi_string = std::string;
		using unicode_string = std::wstring;
		using string_type = std::conditional_t<win_unicode, unicode_string, ansi_string>;
		using string_view_type = std::conditional_t<win_unicode, std::wstring_view, std::string_view>;

		constexpr auto max_username_size = UNLEN;
		constexpr auto max_path_size = MAX_PATH;

		[[nodiscard]] string_type get_computer_name(COMPUTER_NAME_FORMAT format)
		{
			::DWORD size {};
			::GetComputerNameEx(format, nullptr, std::addressof(size));

			string_type name;
			name.resize(size);

			::GetComputerNameEx(format, name.data(), std::addressof(size));
			name.erase(name.end() - 1);
			return name;
		}

		[[nodiscard]] ansi_string get_computer_name_ansi(COMPUTER_NAME_FORMAT format)
		{
			::DWORD size {};
			::GetComputerNameExA(format, nullptr, std::addressof(size));

			ansi_string name;
			name.resize(size);

			::GetComputerNameExA(format, name.data(), std::addressof(size));
			name.erase(name.end() - 1);
			return name;
		}

		[[nodiscard]] unicode_string get_computer_name_unicode(COMPUTER_NAME_FORMAT format)
		{
			::DWORD size {};
			::GetComputerNameExW(format, nullptr, std::addressof(size));

			unicode_string name;
			name.resize(size);

			::GetComputerNameExW(format, name.data(), std::addressof(size));
			name.erase(name.end() - 1);
			return name;
		}

		enum class username_result : int
		{
			ok = 0,
			no_such_domain = ERROR_NO_SUCH_DOMAIN,
			none_mapped = ERROR_NONE_MAPPED
		};

		[[nodiscard]] string_type get_username()
		{
			string_type name;
			::ULONG size {max_username_size + 1};

			name.resize(size);
			::GetUserName(name.data(), std::addressof(size));

			name.resize(size - 1);
			return name;
		}

		[[nodiscard]] ansi_string get_username_ansi()
		{
			ansi_string name;
			::ULONG size{max_username_size + 1};

			name.resize(size);
			::GetUserNameA(name.data(), std::addressof(size));

			name.resize(size - 1);
			return name;
		}

		[[nodiscard]] unicode_string get_username_unicode()
		{
			unicode_string name;
			::ULONG size{max_username_size + 1};

			name.resize(size);
			::GetUserNameW(name.data(), std::addressof(size));

			name.resize(size - 1);
			return name;
		}

		[[nodiscard]] std::optional<::MEMORYSTATUSEX> get_memory_information() noexcept
		{
			::MEMORYSTATUSEX mem {};
			mem.dwLength = sizeof(mem);

			if (const auto r = ::GlobalMemoryStatusEx(std::addressof(mem)); r == 0)
			{
				return {};
			}

			return mem;
		}

		[[nodiscard]] std::vector<string_type> get_drive_names()
		{
			string_type drive_names;
			drive_names.resize(max_path_size);
			const auto r = ::GetLogicalDriveStrings(max_path_size, drive_names.data());
			std::vector<string_type> names;

			std::size_t last_index {0};
			std::size_t index {drive_names.find('\0', last_index)};

			while (index != string_type::npos && drive_names[last_index] != '\0')
			{
				names.emplace_back(drive_names.begin() + last_index, drive_names.begin() + index);
				last_index = index + 1;
				index = drive_names.find('\0', last_index);
			}

			return names;
		}

		[[nodiscard]] std::vector<ansi_string> get_drive_names_ansi()
		{
			ansi_string drive_names;
			drive_names.resize(max_path_size);
			const auto r = ::GetLogicalDriveStringsA(max_path_size, drive_names.data());
			std::vector<string_type> names;

			std::size_t last_index{0};
			std::size_t index{drive_names.find('\0', last_index)};

			while (index != ansi_string::npos && drive_names[last_index] != '\0')
			{
				names.emplace_back(drive_names.begin() + last_index, drive_names.begin() + index);
				last_index = index + 1;
				index = drive_names.find('\0', last_index);
			}

			return names;
		}

		[[nodiscard]] std::vector<unicode_string> get_drive_names_unicode()
		{
			unicode_string drive_names;
			drive_names.resize(max_path_size);
			const auto r = ::GetLogicalDriveStringsW(max_path_size, drive_names.data());
			std::vector<unicode_string> names;

			std::size_t last_index{0};
			std::size_t index{drive_names.find(L'\0', last_index)};

			while (index != unicode_string::npos && drive_names[last_index] != L'\0')
			{
				names.emplace_back(drive_names.begin() + last_index, drive_names.begin() + index);
				last_index = index + 1;
				index = drive_names.find(L'\0', last_index);
			}

			return names;
		}

		enum class registry_key_option : ::DWORD
		{
			none = 0,
			open_link = REG_OPTION_OPEN_LINK
		};

		enum class registry_key_access : ::REGSAM
		{
			all_access = KEY_ALL_ACCESS,
			create_link = KEY_CREATE_LINK,
			create_sub_key = KEY_CREATE_SUB_KEY,
			enumerate_sub_keys = KEY_ENUMERATE_SUB_KEYS,
			execute = KEY_EXECUTE,
			notify = KEY_NOTIFY,
			query_value = KEY_QUERY_VALUE,
			read = KEY_READ,
			set_value = KEY_SET_VALUE,
			wow64_32 = KEY_WOW64_32KEY,
			wow64_64 = KEY_WOW64_64KEY,
			write = KEY_WRITE
		};

		enum class registry_value_type : ::DWORD
		{
			binary = REG_BINARY,
			dword = REG_DWORD,
			dword_little_endian = REG_DWORD_LITTLE_ENDIAN,
			dword_big_endian = REG_DWORD_BIG_ENDIAN,
			expand_sz = REG_EXPAND_SZ,
			link = REG_LINK,
			multi_sz = REG_MULTI_SZ,
			none = REG_NONE,
			qword = REG_QWORD,
			qword_little_endian = REG_QWORD_LITTLE_ENDIAN,
			sz = REG_SZ
		};

		class registry_key
		{
			public:
				registry_key() = default;
				registry_key(const registry_key&) = delete;
				registry_key& operator=(const registry_key&) = delete;

				registry_key(registry_key&& other) noexcept
					: key_{std::exchange(other.key_, nullptr)} {}

				registry_key& operator=(registry_key&& other) noexcept
				{
					if (this != std::addressof(other))
					{
						key_ = std::exchange(other.key_, nullptr);
					}

					return *this;
				}

				::LSTATUS open(::HKEY root, string_view_type name, registry_key_option options, registry_key_access access)
				{
					const auto r = ::RegOpenKeyEx(root, name.data(), static_cast<::DWORD>(options), static_cast<::REGSAM>(access), std::addressof(key_));
					if (r != ERROR_SUCCESS)
					{
						key_ = nullptr;
					}

					return r;
				}

				template<class T>
				std::pair<::LSTATUS, registry_value_type> query(string_view_type name, T& buf, ::DWORD& size)
				{
					::DWORD type {};
					const auto r = ::RegQueryValueEx(key_, name.data(), nullptr, std::addressof(type), reinterpret_cast<::LPBYTE>(std::addressof(buf)), std::addressof(size));
					return {r, registry_value_type{type}};
				}

				~registry_key() noexcept
				{
					if (key_ != nullptr)
					{
						::RegCloseKey(key_);
					}
				}

				::HKEY key() noexcept
				{
					return key_;
				}

				::LSTATUS close() noexcept
				{
					const auto r = ::RegCloseKey(key_);
					key_ = nullptr;
					return r;
				}
				
			private:
				::HKEY key_ {nullptr};
		};

		[[nodiscard]] ::DWORD get_cpu_speed() noexcept
		{
			::DWORD mhz {};
			::DWORD buf_size = sizeof(mhz);

			constexpr auto key_name = []() {
				if constexpr (win_unicode)
				{
					return L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";
				}
				else
				{
					return "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";
				}
			}();

			constexpr auto value_name = []() {
				if constexpr (win_unicode)
				{
					return L"~MHz";
				}
				else
				{
					return "~MHz";
				}
			}();
			
			registry_key key;
			key.open(HKEY_LOCAL_MACHINE, key_name, registry_key_option::none, registry_key_access::read);
			key.query(value_name, mhz, buf_size);
			return mhz;
		}
	}
}