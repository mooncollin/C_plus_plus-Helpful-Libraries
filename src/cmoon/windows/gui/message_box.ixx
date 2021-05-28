module;

#include <Windows.h>

export module cmoon.windows.gui.message_box;

import <string_view>;
import <cstdint>;
import <concepts>;
import <type_traits>;

namespace cmoon::windows
{
	export
	enum class message_box_behavior : std::uint32_t
	{
		abort_retry_ignore = MB_ABORTRETRYIGNORE,
		cancel_try_continue = MB_CANCELTRYCONTINUE,
		help = MB_HELP,
		ok = MB_OK,
		ok_cancel = MB_OKCANCEL,
		retry_cancel = MB_RETRYCANCEL,
		yes_no = MB_YESNO,
		yes_no_cancel = MB_YESNOCANCEL,
	};

	export
	enum class message_box_icon : std::uint32_t
	{
		exclamation = MB_ICONEXCLAMATION,
		warning = MB_ICONWARNING,
		information = MB_ICONINFORMATION,
		asterisk = MB_ICONASTERISK,
		question = MB_ICONQUESTION,
		stop = MB_ICONSTOP,
		error = MB_ICONERROR,
		hand = MB_ICONHAND
	};

	export
	enum class message_box_default_button : std::uint32_t
	{
		button1 = MB_DEFBUTTON1,
		button2 = MB_DEFBUTTON2,
		button3 = MB_DEFBUTTON3,
		button4 = MB_DEFBUTTON4
	};

	export
	enum class message_box_modality : std::uint32_t
	{
		application_modal = MB_APPLMODAL,
		system_modal = MB_SYSTEMMODAL,
		task_modal = MB_TASKMODAL
	};

	export
	enum class message_box_other_options : std::uint32_t
	{
		desktop_only = MB_DEFAULT_DESKTOP_ONLY,
		right_justified = MB_RIGHT,
		right_to_left = MB_RTLREADING,
		set_foreground = MB_SETFOREGROUND,
		top_most = MB_TOPMOST,
		service_notification = MB_SERVICE_NOTIFICATION
	};

	export
	enum class message_box_result : int
	{
		abort = IDABORT,
		cancel = IDCANCEL,
		continue_ = IDCONTINUE,
		ignore = IDIGNORE,
		no = IDNO,
		ok = IDOK,
		retry = IDRETRY,
		try_again = IDTRYAGAIN,
		yes = IDYES
	};

	template<class T>
	concept is_message_box_option = std::same_as<T, message_box_behavior> ||
									std::same_as<T, message_box_icon> ||
									std::same_as<T, message_box_default_button> ||
									std::same_as<T, message_box_modality> ||
									std::same_as<T, message_box_other_options>;

	export
	class message_box_options
	{
		public:
			template<class... T>
				requires((is_message_box_option<T> && ...))
			constexpr message_box_options(T... options) noexcept
				: value_{(static_cast<std::underlying_type_t<T>>(options) | ...)} {}

			template<class... T>
				requires((is_message_box_option<T> && ...) && sizeof...(T) > 0)
			constexpr void value(T... options) noexcept
			{
				value_ = (static_cast<std::underlying_type_t<T>>(options) | ...);
			}

			[[nodiscard]] constexpr std::uint32_t value() const noexcept
			{
				return value_;
			}

			[[nodiscard]] constexpr explicit operator ::UINT() const noexcept
			{
				return static_cast<::UINT>(value_);
			}

			template<is_message_box_option T>
			[[nodiscard]] friend constexpr message_box_options operator|(const message_box_options& lhs, const T& rhs) noexcept
			{
				return message_box_option{lhs.value_ | rhs};
			}
		private:
			constexpr explicit message_box_options(std::uint32_t v)
				: value_{v} {}

			std::uint32_t value_;
	};

	export
	message_box_result show_message_box(::HWND owner, std::string_view message, std::string_view title, message_box_options options)
	{
		return message_box_result{::MessageBoxA(owner, message.data(), title.data(), static_cast<::UINT>(options))};
	}

	export
	message_box_result show_message_box(::HWND owner, std::wstring_view message, std::wstring_view title, message_box_options options)
	{
		return message_box_result{::MessageBoxW(owner, message.data(), title.data(), static_cast<::UINT>(options)))};
	}

	export
	message_box_result show_message_box(std::string_view message, std::string_view title, message_box_options options)
	{
		return show_message_box(nullptr, message, title, options);
	}

	message_box_result show_message_box(std::wstring_view message, std::wstring_view title, message_box_options options)
	{
		return show_message_box(nullptr, message, title, options);
	}
}