module;

#pragma warning(disable : 4005)

#pragma warning(push)
#pragma warning(disable : 5105)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <sysinfoapi.h>
#include <lmcons.h>
#pragma warning(pop)

#include <utility>

export module cmoon.windows.registry;

import cmoon.windows.system_string;

namespace cmoon::windows
{
	export
	enum class registry_key_option : ::DWORD
	{
		none = 0,
		open_link = REG_OPTION_OPEN_LINK
	};

	export
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

	export
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

	export
	class registry_key
	{
		public:
			registry_key() noexcept = default;
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

			::LSTATUS open(::HKEY root, string_view_type name, registry_key_option options, registry_key_access access) noexcept
			{
				const auto r {::RegOpenKeyEx(root, name.data(), static_cast<::DWORD>(options), static_cast<::REGSAM>(access), std::addressof(key_))};
				if (r != ERROR_SUCCESS)
				{
					key_ = nullptr;
				}

				return r;
			}

			template<class T>
			std::pair<::LSTATUS, registry_value_type> query(string_view_type name, T& buf, ::DWORD& size) noexcept
			{
				::DWORD type;
				const auto r {::RegQueryValueEx(key_, name.data(), nullptr, std::addressof(type), reinterpret_cast<::LPBYTE>(std::addressof(buf)), std::addressof(size))};
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
				const auto r {::RegCloseKey(key_)};
				key_ = nullptr;
				return r;
			}
				
		private:
			::HKEY key_ {nullptr};
	};
}