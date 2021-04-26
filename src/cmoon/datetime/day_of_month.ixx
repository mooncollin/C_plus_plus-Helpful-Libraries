export module cmoon.datetime.day_of_month;

import <compare>;

import cmoon.measures.time;
import cmoon.format;
import cmoon.format.chrono;
import cmoon.format.write_string_view;

namespace cmoon::datetime
{
	export
	class day_of_month
	{
		public:
			constexpr explicit day_of_month() noexcept = default;
			constexpr explicit day_of_month(unsigned v) noexcept
				: value{v} {}

			[[nodiscard]] friend constexpr day_of_month operator+(const day_of_month& d, const cmoon::measures::days& ds) noexcept
			{
				return day_of_month{static_cast<unsigned>(ds.count() + d.value)};
			}

			[[nodiscard]] friend constexpr day_of_month operator+(const cmoon::measures::days& ds, const day_of_month& d) noexcept
			{
				return day_of_month{static_cast<unsigned>(ds.count() + d.value)};
			}

			[[nodiscard]] friend constexpr day_of_month operator-(const day_of_month& d, const cmoon::measures::days& ds) noexcept
			{
				return day_of_month{static_cast<unsigned>(d.value - ds.count())};
			}

			[[nodiscard]] friend constexpr cmoon::measures::days operator-(const day_of_month& d1, const day_of_month& d2) noexcept
			{
				return cmoon::measures::days{d1.value - d2.value};
			}

			constexpr day_of_month& operator+=(const cmoon::measures::days& ds) noexcept
			{
				*this = *this + ds;
				return *this;
			}

			constexpr day_of_month& operator-=(const cmoon::measures::days& ds) noexcept
			{
				*this = *this - ds;
				return *this;
			}

			constexpr day_of_month& operator++() noexcept
			{
				*this += cmoon::measures::days{1};
				return *this;
			}

			constexpr day_of_month operator++(int) noexcept
			{
				const auto v = *this;
				++*this;
				return v;
			}

			constexpr day_of_month& operator--() noexcept
			{
				*this -= cmoon::measures::days{1};
				return *this;
			}

			constexpr day_of_month operator--(int) noexcept
			{
				const auto v = *this;
				--* this;
				return v;
			}

			[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const day_of_month&, const day_of_month&) = default;

			[[nodiscard]] constexpr bool ok() const noexcept
			{
				return value >= 1u && value <= 31u;
			}

			[[nodiscard]] constexpr operator unsigned() const noexcept
			{
				return value;
			}
		private:
			unsigned value {};
	};

	export
		class day_of_year
		{
			public:
				constexpr explicit day_of_year() noexcept = default;
				constexpr explicit day_of_year(unsigned v) noexcept
					: value{v} {}

				[[nodiscard]] friend constexpr day_of_year operator+(const day_of_year& d, const measures::days& ds) noexcept
				{
					return day_of_year{static_cast<unsigned>(ds.count() + d.value)};
				}

				[[nodiscard]] friend constexpr day_of_year operator+(const measures::days& ds, const day_of_year& d) noexcept
				{
					return day_of_year{static_cast<unsigned>(ds.count() + d.value)};
				}

				[[nodiscard]] friend constexpr day_of_year operator-(const day_of_year& d, const measures::days& ds) noexcept
				{
					return day_of_year{static_cast<unsigned>(d.value - ds.count())};
				}

				[[nodiscard]] friend constexpr measures::days operator-(const day_of_year& d1, const day_of_year& d2) noexcept
				{
					return measures::days{d1.value - d2.value};
				}

				constexpr day_of_year& operator+=(const measures::days& ds) noexcept
				{
					*this = *this + ds;
					return *this;
				}

				constexpr day_of_year& operator-=(const measures::days& ds) noexcept
				{
					*this = *this - ds;
					return *this;
				}

				constexpr day_of_year& operator++() noexcept
				{
					*this += measures::days{1};
					return *this;
				}

				constexpr day_of_year operator++(int) noexcept
				{
					const auto v = *this;
					++* this;
					return v;
				}

				constexpr day_of_year& operator--() noexcept
				{
					*this -= measures::days{1};
					return *this;
				}

				constexpr day_of_year operator--(int) noexcept
				{
					const auto v = *this;
					--* this;
					return v;
				}

				[[nodiscard]] constexpr bool ok() const noexcept
				{
					return value >= 1u && value <= 365u;
				}

				[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const day_of_year&, const day_of_year&) = default;

				[[nodiscard]] constexpr operator unsigned() const noexcept
				{
					return value;
				}
			private:
				unsigned value {};
		};

	export
	inline constexpr day_of_month first{1};

	export
	inline constexpr day_of_month second{2};

	export
	inline constexpr day_of_month third{3};

	export
	inline constexpr day_of_month fourth{4};

	export
	inline constexpr day_of_month fifth{5};

	export
	inline constexpr day_of_month sixth{6};

	export
	inline constexpr day_of_month seventh{7};

	export
	inline constexpr day_of_month eighth{8};

	export
	inline constexpr day_of_month ninth{9};

	export
	inline constexpr day_of_month tenth{10};

	export
	inline constexpr day_of_month eleventh{11};

	export
	inline constexpr day_of_month twelth{12};

	export
	inline constexpr day_of_month thirteenth{13};

	export
	inline constexpr day_of_month fourteenth{14};

	export
	inline constexpr day_of_month fifteeth{15};

	export
	inline constexpr day_of_month sixteeth{16};

	export
	inline constexpr day_of_month seventeeth{17};

	export
	inline constexpr day_of_month eighteenth{18};

	export
	inline constexpr day_of_month nineteeth{19};

	export
	inline constexpr day_of_month twentieth{20};

	export
	inline constexpr day_of_month twenty_first{21};

	export
	inline constexpr day_of_month twenty_second{22};

	export
	inline constexpr day_of_month twenty_third{23};

	export
	inline constexpr day_of_month twenty_fourth{24};

	export
	inline constexpr day_of_month twenty_fifth{25};

	export
	inline constexpr day_of_month twenty_sixth{26};

	export
	inline constexpr day_of_month twenty_seventh{27};

	export
	inline constexpr day_of_month twenty_eighth{28};

	export
	inline constexpr day_of_month twenty_ninth{29};

	export
	inline constexpr day_of_month thirtieth{30};

	export
	inline constexpr day_of_month thirty_first{31};
}

namespace cmoon
{
	export
	template<class CharT>
	struct formatter<datetime::day_of_month, CharT> : public base_chrono_formatter<datetime::day_of_month, CharT>
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
				void on_day_of_year() { this->throw_not_supported(); }
				void on_iso_week(const details::numeric_system) { this->throw_not_supported(); }
				void on_dec0_weekday(const details::numeric_system) { this->throw_not_supported(); }
				void on_dec1_weekday(const details::numeric_system) { this->throw_not_supported(); }
				void on_dec_month(const details::numeric_system) { this->throw_not_supported(); }
				void on_century(const details::numeric_system) { this->throw_not_supported(); }
				void on_abbr_year(const details::numeric_system) { this->throw_not_supported(); }
				void on_offset_year() { this->throw_not_supported(); }
				void on_year(const details::numeric_system) { this->throw_not_supported(); }
				void on_24_hour(const details::numeric_system) { this->throw_not_supported(); }
				void on_12_hour(const details::numeric_system) { this->throw_not_supported(); }
				void on_minute(const details::numeric_system) { this->throw_not_supported(); }
				void on_second(const details::numeric_system) { this->throw_not_supported(); }
				void on_loc_time(const details::numeric_system) { this->throw_not_supported(); }
			};
		public:
			template<class OutputIt>
			auto format(const datetime::day_of_month& val, basic_format_context<OutputIt, CharT>& ctx)
			{
				const std::tm time {.tm_mday = static_cast<int>(val)};
				handler h{time, ctx};
				parse_chrono_format(this->fmt, h);

				return write_string_view(h.str(), ctx, this->parser);
			}
	};
}