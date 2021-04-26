export module cmoon.datetime.month;

import <compare>;

import cmoon.measures.time;
import cmoon.format;
import cmoon.format.chrono;
import cmoon.format.write_string_view;

import cmoon.datetime.year;
import cmoon.datetime.day_of_month;

namespace cmoon::datetime
{
	export
	class month
	{
		public:
			constexpr explicit month() noexcept = default;
			constexpr explicit month(unsigned v) noexcept
				: value{v} {}

			[[nodiscard]] friend constexpr month operator+(const month& m, const cmoon::measures::months& ms) noexcept
			{
				const auto result = static_cast<std::intmax_t>(m.value) + (ms.count() - 1);

				return month{static_cast<unsigned>((result % 12) + 1)};
			}

			[[nodiscard]] friend constexpr month operator+(const cmoon::measures::months& ms, const month& m) noexcept
			{
				return m + ms;
			}

			[[nodiscard]] friend constexpr month operator-(const month& m, const cmoon::measures::months& ms) noexcept
			{
				return m + -ms;
			}

			[[nodiscard]] friend constexpr cmoon::measures::months operator-(const month& m1, const month& m2) noexcept
			{
				if (m1.value > m2.value)
				{
					return cmoon::measures::months{m1.value - m2.value};
				}
				else
				{
					return -cmoon::measures::months{m2.value - m1.value};
				}
			}

			constexpr month& operator+=(const cmoon::measures::months& ms) noexcept
			{
				*this = *this + ms;
				return *this;
			}

			constexpr month& operator-=(const cmoon::measures::months& ms) noexcept
			{
				*this = *this - ms;
				return *this;
			}

			constexpr month& operator++() noexcept
			{
				*this += cmoon::measures::months{1};
				return *this;
			}

			constexpr month operator++(int) noexcept
			{
				const auto m = *this;
				++*this;
				return m;
			}

			constexpr month& operator--() noexcept
			{
				*this -= cmoon::measures::months{1};
				return *this;
			}

			constexpr month operator--(int) noexcept
			{
				const auto m = *this;
				--* this;
				return m;
			}

			[[nodiscard]] constexpr bool ok() const noexcept
			{
				return value >= 1 && value <= 12;
			}

			[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const month&, const month&) = default;

			[[nodiscard]] constexpr explicit operator unsigned() const noexcept
			{
				return value;
			}

		private:
			unsigned value {};
	};

	export
	inline constexpr month january{1};

	export
	inline constexpr month february{2};

	export
	inline constexpr month march{3};

	export
	inline constexpr month april{4};

	export
	inline constexpr month may{5};

	export
	inline constexpr month june{6};

	export
	inline constexpr month july{7};

	export
	inline constexpr month august{8};

	export
	inline constexpr month september{9};

	export
	inline constexpr month october{10};

	export
	inline constexpr month november{11};

	export
	inline constexpr month december{12};

	export
	[[nodiscard]] constexpr cmoon::measures::days days_in_month(const month& m, const year& y = year{2000}) noexcept
	{
		switch (static_cast<unsigned>(m))
		{
			case static_cast<unsigned>(january):
			case static_cast<unsigned>(march):
			case static_cast<unsigned>(may):
			case static_cast<unsigned>(july):
			case static_cast<unsigned>(august):
			case static_cast<unsigned>(october):
			case static_cast<unsigned>(december):
				return cmoon::measures::days{31};
			case static_cast<unsigned>(april):
			case static_cast<unsigned>(june):
			case static_cast<unsigned>(september):
			case static_cast<unsigned>(november):
				return cmoon::measures::days{30};
			case static_cast<unsigned>(february):
				return y.is_leap() ? cmoon::measures::days{29} : cmoon::measures::days{28};
			default:
				return cmoon::measures::days{0};
		}
	}

	export
	[[nodiscard]] constexpr day_of_month last_day_of_month(const month& m, const year& y = year{2000}) noexcept
	{
		switch (static_cast<unsigned>(m))
		{
			case static_cast<unsigned>(january):
			case static_cast<unsigned>(march):
			case static_cast<unsigned>(may):
			case static_cast<unsigned>(july):
			case static_cast<unsigned>(august):
			case static_cast<unsigned>(october):
			case static_cast<unsigned>(december):
				return thirty_first;
			case static_cast<unsigned>(april):
			case static_cast<unsigned>(june):
			case static_cast<unsigned>(september):
			case static_cast<unsigned>(november):
				return thirtieth;
			case static_cast<unsigned>(february):
				return y.is_leap() ? twenty_ninth : twenty_eighth;
			default:
				return day_of_month{0};
		}
	}
}

namespace cmoon
{
	export
	template<class CharT>
	struct formatter<datetime::month, CharT> : public base_chrono_formatter<datetime::month, CharT>
	{
		private:
			template<class OutputIt>
			struct handler : public tm_handler<CharT, OutputIt>
			{
				handler(const handler&) = delete;
				handler& operator=(const handler&) = delete;

				handler(const std::tm& value, basic_format_context<OutputIt, CharT>& ctx)
					: tm_handler<CharT, OutputIt>{value, ctx} {}

				void on_abbr_weekday() { this->throw_not_supported(); }
				void on_full_weekday() { this->throw_not_supported(); }
				void on_iso_abbr_year() { this->throw_not_supported(); }
				void on_iso_year() { this->throw_not_supported(); }
				void on_week_sunday(const details::numeric_system) { this->throw_not_supported(); }
				void on_week_monday(const details::numeric_system) { this->throw_not_supported(); }
				void on_us_date() { this->throw_not_supported(); }
				void on_iso_date() { this->throw_not_supported(); }
				void on_loc_date(const details::numeric_system) { this->throw_not_supported(); }
				void on_datetime(const details::numeric_system) { this->throw_not_supported(); }
				void on_utc_offset(const details::numeric_system) { this->throw_not_supported(); }
				void on_tz_name() { this->throw_not_supported(); }
				void on_iso_week(const details::numeric_system) { this->throw_not_supported(); }
				void on_dec0_weekday(const details::numeric_system) { this->throw_not_supported(); }
				void on_dec1_weekday(const details::numeric_system) { this->throw_not_supported(); }
				void on_century(const details::numeric_system) { this->throw_not_supported(); }
				void on_abbr_year(const details::numeric_system) { this->throw_not_supported(); }
				void on_offset_year() { this->throw_not_supported(); }
				void on_year(const details::numeric_system) { this->throw_not_supported(); }
				void on_24_hour(const details::numeric_system) { this->throw_not_supported(); }
				void on_12_hour(const details::numeric_system) { this->throw_not_supported(); }
				void on_minute(const details::numeric_system) { this->throw_not_supported(); }
				void on_second(const details::numeric_system) { this->throw_not_supported(); }
				void on_loc_time(const details::numeric_system) { this->throw_not_supported(); }
				void on_day_of_month_zero(const details::numeric_system) { this->throw_not_supported(); }
				void on_day_of_month_space(const details::numeric_system) { this->throw_not_supported(); }
				void on_day_of_year() { this->throw_not_supported(); }
			};
		public:
			template<class OutputIt>
			auto format(const datetime::month& val, basic_format_context<OutputIt, CharT>& ctx)
			{
				const std::tm time {.tm_mon = static_cast<int>(unsigned{val})};
				handler h{time, ctx};
				parse_chrono_format(this->fmt, h);

				return write_string_view(h.str(), ctx, this->parser);
			}
	};
}