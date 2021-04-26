export module cmoon.datetime.weekday;

import cmoon.measures.time;
import cmoon.format;
import cmoon.format.chrono;
import cmoon.format.write_string_view;

import cmoon.datetime.sys_time;

namespace cmoon::datetime
{
	class weekday_indexed;
	class weekday_last;

	export
	class weekday
	{
		public:
			constexpr weekday() = default;
			explicit constexpr weekday(unsigned wd) noexcept
				: value{wd % 7u} {}

			constexpr weekday(const sys_days& sys_days)
				: value{from_days(sys_days.time_since_epoch().count())} {}

			[[nodiscard]] constexpr bool ok() const noexcept
			{
				return value >= 0u && value <= 6u;
			}

			[[nodiscard]] constexpr unsigned c_encoding() const noexcept
			{
				return value;
			}

			[[nodiscard]] constexpr unsigned iso_encoding() const noexcept
			{
				return value == 0u ? 7u : value;
			}

			[[nodiscard]] friend constexpr weekday operator+(const weekday& wd, const cmoon::measures::days& d) noexcept
			{
				return weekday{static_cast<unsigned>(static_cast<long long>(wd.c_encoding()) + d.count())};
			}

			[[nodiscard]] friend constexpr weekday operator+(const cmoon::measures::days& d, const weekday& wd) noexcept
			{
				return weekday{static_cast<unsigned>(static_cast<long long>(wd.c_encoding()) + d.count())};
			}

			[[nodiscard]] friend constexpr weekday operator-(const weekday& wd, const cmoon::measures::days& d) noexcept
			{
				const auto r = static_cast<long long>(wd.c_encoding()) - d.count();
				const auto r2 = static_cast<unsigned>(r);
				if (r < 0)
				{
					return weekday{7u + r2};
				}
					
				return weekday{r2};
			}

			[[nodiscard]] friend constexpr cmoon::measures::days operator-(const weekday& wd1, const weekday& wd2) noexcept
			{
				return cmoon::measures::days{wd1.c_encoding() - wd2.c_encoding()};
			}

			constexpr weekday& operator+=(const cmoon::measures::days& d) noexcept
			{
				*this = *this + d;
				return *this;
			}

			constexpr weekday& operator-=(const cmoon::measures::days& d) noexcept
			{
				*this = *this - d;
				return *this;
			}

			constexpr weekday& operator++() noexcept
			{
				*this += cmoon::measures::days{1};
				return *this;
			}

			constexpr weekday operator++(int) noexcept
			{
				const auto copy = *this;
				++*this;
				return copy;
			}

			constexpr weekday& operator--() noexcept
			{
				*this -= cmoon::measures::days{1};
				return *this;
			}

			constexpr weekday operator--(int) noexcept
			{
				const auto copy = *this;
				--*this;
				return copy;
			}

			// Delay generation since
			// MSVC will complain that weekday_indexed
			// is an incomplete type.
			template<class = int>
			[[nodiscard]] constexpr weekday_indexed operator[](unsigned index) const noexcept
			{
				return weekday_indexed{*this, index};
			}

			// Delay generation since
			// MSVC will complain that weekday_last
			// is an incomplete type.
			template<class = int>
			[[nodiscard]] constexpr weekday_last operator[](last_spec) const noexcept
			{
				return weekday_last{*this};
			}

			[[nodiscard]] friend constexpr bool operator==(const weekday& x, const weekday& y) noexcept
			{
				return x.c_encoding() == y.c_encoding();
			}
		private:
			unsigned value {};

			template<std::integral T>
			[[nodiscard]] static constexpr unsigned from_days(T days)
			{
				const auto u = static_cast<unsigned>(days);
				return days >= -4 ? (u + 4) % 7 : u % 7;
			}
	};

	export
	inline constexpr weekday sunday {0};

	export
	inline constexpr weekday monday {1};

	export
	inline constexpr weekday tuesday {2};

	export
	inline constexpr weekday wednesday {3};

	export
	inline constexpr weekday thursday {4};

	export
	inline constexpr weekday friday {5};

	export
	inline constexpr weekday saturday {6};
}

namespace cmoon
{
	export
	template<class CharT>
	struct formatter<datetime::weekday, CharT> : public base_chrono_formatter<datetime::weekday, CharT>
	{
		private:
			template<class OutputIt>
			struct handler : public tm_handler<CharT, OutputIt>
			{
				handler(const handler&) = delete;
				handler& operator=(const handler&) = delete;

				handler(const std::tm& value, basic_format_context<OutputIt, CharT>& ctx)
					: tm_handler<CharT, OutputIt>{value, ctx} {}

				void on_abbr_month() { this->throw_not_supported(); }
				void on_full_month() { this->throw_not_supported(); }
				void on_dec_month(const details::numeric_system) { this->throw_not_supported(); }
				void on_iso_abbr_year() { this->throw_not_supported(); }
				void on_iso_year() { this->throw_not_supported(); }
				void on_us_date() { this->throw_not_supported(); }
				void on_iso_date() { this->throw_not_supported(); }
				void on_loc_date(const details::numeric_system) { this->throw_not_supported(); }
				void on_datetime(const details::numeric_system) { this->throw_not_supported(); }
				void on_utc_offset(const details::numeric_system) { this->throw_not_supported(); }
				void on_tz_name() { this->throw_not_supported(); }
				void on_iso_week(const details::numeric_system) { this->throw_not_supported(); }
				void on_century(const details::numeric_system) { this->throw_not_supported(); }
				void on_abbr_year(const details::numeric_system) { this->throw_not_supported(); }
				void on_offset_year() { this->throw_not_supported(); }
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
			auto format(const datetime::weekday& val, basic_format_context<OutputIt, CharT>& ctx)
			{
				const std::tm time {.tm_wday = static_cast<int>(unsigned{val})};
				handler h{time, ctx};
				parse_chrono_format(this->fmt, h);

				return write_string_view(h.str(), ctx, this->parser);
			}
	};
}