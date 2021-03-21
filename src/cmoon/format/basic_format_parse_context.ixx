module;

#include <string_view>

export module cmoon.format.basic_format_parse_context;
import cmoon.format.format_error;

namespace cmoon
{
	export
	template<class CharT>
	class basic_format_parse_context
	{
		public:
			using char_type = CharT;
			using const_iterator = typename std::basic_string_view<CharT>::const_iterator;
			using iterator = const_iterator;

			explicit constexpr basic_format_parse_context(std::basic_string_view<CharT> fmt, std::size_t num_args = 0) noexcept
				: begin_{fmt.begin()}, end_{fmt.end()}, indexing_{indexing::unknown}, next_arg_id_{0}, num_args_{num_args} {}

			basic_format_parse_context(const basic_format_parse_context&) = delete;
			basic_format_parse_context& operator=(const basic_format_parse_context&) = delete;

			constexpr const_iterator begin() const noexcept
			{
				return begin_;
			}

			constexpr const_iterator end() const noexcept
			{
				return end_;
			}

			constexpr void advance_to(const_iterator it)
			{
				begin_ = it;
			}

			constexpr std::size_t next_arg_id()
			{
				switch (indexing_)
				{
					case indexing::manual:
						throw format_error{"Mixing of automatic and manual argument indexing"};
					case indexing::unknown:
						indexing_ = indexing::automatic;
						break;
					default:
						break;
				}

				return next_arg_id_++;
			}

			constexpr void check_arg_id(std::size_t)
			{
				switch (indexing_)
				{
					case indexing::automatic:
						throw format_error{"Mixing of automatic and manual argument indexing"};
					case indexing::unknown:
						indexing_ = indexing::manual;
						break;
					default:
						break;
				}
			}
		private:
			iterator begin_;
			iterator end_;
			enum class indexing { unknown, manual, automatic };
			indexing indexing_;
			std::size_t next_arg_id_;
			std::size_t num_args_;
	};

	export
	using format_parse_context = basic_format_parse_context<char>;

	export
	using wformat_parse_context = basic_format_parse_context<wchar_t>;
}