module;

#include <array>
#include <span>
#include <locale>
#include <string_view>

#include "cmoon/format/forward_declarations.hpp"

export module cmoon.format.basic_format_args;
import cmoon.format.basic_format_arg;

namespace cmoon
{
	template<class Context, class... Args>
	struct format_arg_store
	{
		constexpr format_arg_store(const Args&... as)
			: args{basic_format_arg<Context>(as)...} {}

		std::array<basic_format_arg<Context>, sizeof...(Args)> args {};
	};

	export
	template<class Context>
	class basic_format_args
	{
		public:
			basic_format_args() noexcept = default;

			template<class... Args>
			basic_format_args(const format_arg_store<Context, Args...>& store) noexcept
				: data_{store.args} {}

			basic_format_arg<Context> get(std::size_t i) const noexcept
			{
				return i < data_.size() ? data_[i] : basic_format_arg<Context>{};
			}
		private:
			std::span<const basic_format_arg<Context>, std::dynamic_extent> data_ {};

			template<typename CharT, class Out>
			friend Out details::vformat_to(Out, const std::locale&, std::basic_string_view<CharT>, format_args_t<Out, CharT>&&);
	};
}