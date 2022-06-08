module;

#pragma warning(push)
#pragma warning(disable : 5105)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <sysinfoapi.h>
#include <lmcons.h>
#pragma warning(pop)

#ifdef max
#undef max
#endif

#include <string>
#include <cstdint>
#include <utility>
#include <optional>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <memory>
#include <ranges>
#include <system_error>
#include <iterator>

export module cmoon.windows.system_info;

import cmoon.string;

import cmoon.windows.system_string;
import cmoon.windows.registry;

namespace cmoon::windows
{
	export
	inline constexpr auto max_computer_username_size = UNLEN;

	export
	inline constexpr auto max_path_size = MAX_PATH;

	export
	[[nodiscard]] string_type get_computer_name(COMPUTER_NAME_FORMAT format)
	{
		::DWORD size;
		::GetComputerNameEx(format, nullptr, std::addressof(size));

		string_type name;
		name.resize(size);

		::GetComputerNameEx(format, name.data(), std::addressof(size));
		name.erase(name.end() - 1);
		return name;
	}

	export
	[[nodiscard]] ansi_string get_computer_name_ansi(COMPUTER_NAME_FORMAT format)
	{
		::DWORD size;
		::GetComputerNameExA(format, nullptr, std::addressof(size));

		ansi_string name;
		name.resize(size);

		::GetComputerNameExA(format, name.data(), std::addressof(size));
		name.erase(name.end() - 1);
		return name;
	}

	export
	[[nodiscard]] unicode_string get_computer_name_unicode(COMPUTER_NAME_FORMAT format)
	{
		::DWORD size;
		::GetComputerNameExW(format, nullptr, std::addressof(size));

		unicode_string name;
		name.resize(size);

		::GetComputerNameExW(format, name.data(), std::addressof(size));
		name.erase(name.end() - 1);
		return name;
	}

	export
	enum class username_result : int
	{
		ok = 0,
		no_such_domain = ERROR_NO_SUCH_DOMAIN,
		none_mapped = ERROR_NONE_MAPPED
	};

	export
	[[nodiscard]] string_type get_username()
	{
		string_type name;
		::ULONG size {max_computer_username_size + 1};

		name.resize(size);
		::GetUserName(name.data(), std::addressof(size));

		name.resize(size - 1);
		return name;
	}

	export
	[[nodiscard]] ansi_string get_username_ansi()
	{
		ansi_string name;
		::ULONG size{max_computer_username_size + 1};

		name.resize(size);
		::GetUserNameA(name.data(), std::addressof(size));

		name.resize(size - 1);
		return name;
	}

	export
	[[nodiscard]] unicode_string get_username_unicode()
	{
		unicode_string name;
		::ULONG size{max_computer_username_size + 1};

		name.resize(size);
		::GetUserNameW(name.data(), std::addressof(size));

		name.resize(size - 1);
		return name;
	}

	export
	[[nodiscard]] std::optional<::MEMORYSTATUSEX> get_memory_information() noexcept
	{
		::MEMORYSTATUSEX mem {};
		mem.dwLength = sizeof(mem);

		if (!::GlobalMemoryStatusEx(std::addressof(mem)))
		{
			return {};
		}

		return mem;
	}

	template<class S, class F, std::output_iterator<S> Out>
	[[nodiscard]] void get_drive_names_impl(F f, Out out)
	{
		S drive_names;
		drive_names.resize(max_path_size);
		const auto r {f(max_path_size, drive_names.data())};
		if (r == 0)
		{
			throw std::system_error{static_cast<int>(::GetLastError()), std::system_category()};
		}
		drive_names.resize(r - 1);

		for (const auto drive : std::views::split(drive_names, '\0'))
		{
			*out = S{std::ranges::begin(drive),
					 std::ranges::end(drive)};
		}
	}

	export
	template<std::output_iterator<string_type> Out>
	[[nodiscard]] void get_drive_names(Out out)
	{
		get_drive_names_impl<string_type>(::GetLogicalDriveStrings,
										  out);
	}

	export
	template<std::output_iterator<ansi_string> Out>
	[[nodiscard]] void get_drive_names_ansi(Out out)
	{
		get_drive_names_impl<ansi_string>(::GetLogicalDriveStringsA,
										  out);
	}

	export
	template<std::output_iterator<unicode_string> Out>
	[[nodiscard]] void get_drive_names_unicode(Out out)
	{
		get_drive_names_impl<unicode_string>(::GetLogicalDriveStringsW,
										     out);
	}

	export
	[[nodiscard]] ::DWORD get_cpu_speed() noexcept
	{

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

		::DWORD mhz;
		::DWORD buf_size {sizeof(mhz)};
			
		registry_key key;
		key.open(HKEY_LOCAL_MACHINE, key_name, registry_key_option::none, registry_key_access::read);
		key.query(value_name, mhz, buf_size);
		return mhz;
	}
}