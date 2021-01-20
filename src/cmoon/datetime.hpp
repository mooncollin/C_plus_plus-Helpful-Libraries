#pragma once

#include <cstdint>
#include <limits>
#include <concepts>
#include <compare>
#include <chrono>
#include <string>
#include <vector>
#include <forward_list>
#include <mutex>

#include "cmoon/measures/time.hpp"
#include "cmoon/math.hpp"
#include "cmoon/format.hpp"

namespace cmoon
{
	namespace datetime
	{
		template<class Duration>
		using sys_time = std::chrono::time_point<std::chrono::system_clock, Duration>;

		using sys_seconds = sys_time<std::chrono::seconds>;
		using sys_days = sys_time<measures::chrono_t<measures::days>>;

		struct last_spec
		{
			explicit constexpr last_spec() = default;
		};

		constexpr last_spec last {};

		[[nodiscard]] constexpr bool is_am(const measures::hours& h) noexcept
		{
			return measures::hours{0} <= h && h <= measures::hours{11};
		}

		[[nodiscard]] constexpr bool is_pm(const measures::hours& h) noexcept
		{
			return measures::hours{12} <= h && h <= measures::hours{23};
		}

		[[nodiscard]] constexpr measures::hours make12(const measures::hours& h) noexcept
		{
			switch (h.count())
			{
				case 0:
					return measures::hours{12};
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:
				case 19:
				case 20:
				case 21:
				case 22:
				case 23:
					return h - measures::hours{12};
				default:
					return h;
			}
		}

		[[nodiscard]] constexpr measures::hours make24(const measures::hours& h, const bool is_pm) noexcept
		{
			if (is_pm)
			{
				switch (h.count())
				{
					case 12:
						return h;
					default:
						return h + measures::hours{12};
				}
			}
			else
			{
				switch (h.count())
				{
					case 12:
						return measures::hours{0};
					default:
						return h;
				}
			}
		}

		template<class Duration>
		class hh_mm_ss;

		namespace details
		{
			template<class Duration>
			class base_hh_mm_ss
			{
				using precision = Duration;
				public:
					constexpr base_hh_mm_ss() = default;
					explicit constexpr base_hh_mm_ss(precision since_midnight)
						: value_{since_midnight} {}

					[[nodiscard]] constexpr explicit operator precision() const noexcept
					{
						return value_;
					}

					constexpr precision to_duration() const noexcept
					{
						return value_;
					}

					[[nodiscard]] constexpr measures::hours hours() const noexcept
					{
						return measures::unit_cast<measures::hours>(value_);
					}

					[[nodiscard]] constexpr measures::minutes minutes() const noexcept
						requires(!std::same_as<precision, measures::hours>)
					{
						return measures::unit_cast<measures::minutes>(value_) - measures::unit_cast<measures::minutes>(hours());
					}

					[[nodiscard]] constexpr measures::seconds seconds() const noexcept
						requires(!std::same_as<precision, measures::minutes> && !std::same_as<precision, measures::hours>)
					{
						return measures::unit_cast<measures::seconds>(value_) - measures::unit_cast<measures::seconds>(minutes()) - measures::unit_cast<measures::seconds>(hours());
					}

					[[nodiscard]] constexpr precision subseconds() const noexcept
						requires(!std::same_as<precision, measures::seconds> && !std::same_as<precision, measures::minutes> && !std::same_as<precision, measures::hours>)
					{
						return value_ - measures::unit_cast<precision>(seconds()) - measures::unit_cast<precision>(minutes()) - measures::unit_cast<precision>(hours());
					}
				private:
					precision value_;
			};
		}

		template<>
		class hh_mm_ss<measures::hours> : public details::base_hh_mm_ss<measures::hours>
		{
			using precision = measures::hours;

			public:
				constexpr hh_mm_ss() = default;
				explicit constexpr hh_mm_ss(precision since_midnight)
					: details::base_hh_mm_ss<measures::hours>{since_midnight} {}
		};

		template<>
		class hh_mm_ss<measures::minutes> : public details::base_hh_mm_ss<measures::minutes>
		{
			using precision = measures::minutes;

			public:
				constexpr hh_mm_ss() = default;
				explicit constexpr hh_mm_ss(precision since_midnight)
					: details::base_hh_mm_ss<measures::minutes>{since_midnight} {}
		};

		template<>
		class hh_mm_ss<measures::seconds> : public details::base_hh_mm_ss<measures::seconds>
		{
			using precision = measures::seconds;

			public:
				constexpr hh_mm_ss() = default;
				explicit constexpr hh_mm_ss(precision since_midnight)
					: details::base_hh_mm_ss<measures::seconds>{since_midnight} {}
		};

		template<class Rep, class Period>
			requires(!std::floating_point<Rep> && !std::convertible_to<measures::time<Rep, measures::metric_system, Period>, measures::seconds>)
		class hh_mm_ss<measures::time<Rep, measures::metric_system, Period, 1>> : public details::base_hh_mm_ss<measures::time<Rep, measures::metric_system, Period, 1>>
		{
			using precision = measures::time<Rep, measures::metric_system, Period, 1>;

			public:
				constexpr hh_mm_ss() = default;
				explicit constexpr hh_mm_ss(precision since_midnight)
					: details::base_hh_mm_ss<measures::time<Rep, measures::metric_system, Period, 1>>{since_midnight} {}
		};

		class month
		{
			public:
				constexpr explicit month() noexcept = default;
				constexpr explicit month(unsigned v) noexcept
					: value{v} {}

				[[nodiscard]] friend constexpr month operator+(const month& m, const measures::months& ms) noexcept
				{
					const auto result = static_cast<std::intmax_t>(m.value) + (ms.count() - 1);

					return month{static_cast<unsigned>((result % 12) + 1)};
				}

				[[nodiscard]] friend constexpr month operator+(const measures::months& ms, const month& m) noexcept
				{
					return m + ms;
				}

				[[nodiscard]] friend constexpr month operator-(const month& m, const measures::months& ms) noexcept
				{
					return m + -ms;
				}

				[[nodiscard]] friend constexpr measures::months operator-(const month& m1, const month& m2) noexcept
				{
					if (m1.value > m2.value)
					{
						return measures::months{m1.value - m2.value};
					}
					else
					{
						return -measures::months{m2.value - m1.value};
					}
				}

				constexpr month& operator+=(const measures::months& ms) noexcept
				{
					*this = *this + ms;
					return *this;
				}

				constexpr month& operator-=(const measures::months& ms) noexcept
				{
					*this = *this - ms;
					return *this;
				}

				constexpr month& operator++() noexcept
				{
					*this += measures::months{1};
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
					*this -= measures::months{1};
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

		constexpr month january{1};
		constexpr month february{2};
		constexpr month march{3};
		constexpr month april{4};
		constexpr month may{5};
		constexpr month june{6};
		constexpr month july{7};
		constexpr month august{8};
		constexpr month september{9};
		constexpr month october{10};
		constexpr month november{11};
		constexpr month december{12};

		class year
		{
			public:
				constexpr explicit year() noexcept = default;
				constexpr explicit year(std::int64_t v) noexcept
					: value{v} {}

				[[nodiscard]] friend constexpr year operator+(const year& y, const measures::years& ys) noexcept
				{
					return year{static_cast<std::int64_t>(ys.count() + y.value)};
				}

				[[nodiscard]] friend constexpr year operator+(const measures::years& ys, const year& y) noexcept
				{
					return year{static_cast<std::int64_t>(ys.count() + y.value)};
				}

				[[nodiscard]] friend constexpr year operator-(const year& y, const measures::years& ys) noexcept
				{
					return year{static_cast<std::int64_t>(y.value - ys.count())};
				}

				[[nodiscard]] friend constexpr measures::years operator-(const year& y1, const year& y2) noexcept
				{
					return measures::years{y1.value - y2.value};
				}

				constexpr year& operator+=(const measures::years& ys) noexcept
				{
					*this = *this + ys;
					return *this;
				}

				constexpr year& operator-=(const measures::years& ys) noexcept
				{
					*this = *this - ys;
					return *this;
				}

				constexpr year& operator++() noexcept
				{
					*this += measures::years{1};
					return *this;
				}

				constexpr year operator++(int) noexcept
				{
					const auto v = *this;
					++*this;
					return v;
				}

				constexpr year& operator--() noexcept
				{
					*this -= measures::years{1};
					return *this;
				}

				constexpr year operator--(int) noexcept
				{
					const auto v = *this;
					--* this;
					return v;
				}

				[[nodiscard]] constexpr bool is_leap() const noexcept
				{
					return (value % 4 == 0 && value % 100 != 0) || value % 400 == 0;
				}

				[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const year&, const year&) = default;

				[[nodiscard]] explicit constexpr operator std::int64_t() const noexcept
				{
					return value;
				}

				[[nodiscard]] explicit constexpr operator int() const noexcept
				{
					return static_cast<int>(value);
				}

			private:
				std::int64_t value {};
		};

		[[nodiscard]] constexpr measures::days days_in_month(const month& m, const year& y = year{2000}) noexcept
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
					return measures::days{31};
				case static_cast<unsigned>(april):
				case static_cast<unsigned>(june):
				case static_cast<unsigned>(september):
				case static_cast<unsigned>(november):
					return measures::days{30};
				case static_cast<unsigned>(february):
					return y.is_leap() ? measures::days{29} : measures::days{28};
				default:
					return measures::days{0};
			}
		}

		class day_of_month
		{
			public:
				constexpr explicit day_of_month() noexcept = default;
				constexpr explicit day_of_month(unsigned v) noexcept
					: value{v} {}

				[[nodiscard]] friend constexpr day_of_month operator+(const day_of_month& d, const measures::days& ds) noexcept
				{
					return day_of_month{static_cast<unsigned>(ds.count() + d.value)};
				}

				[[nodiscard]] friend constexpr day_of_month operator+(const measures::days& ds, const day_of_month& d) noexcept
				{
					return day_of_month{static_cast<unsigned>(ds.count() + d.value)};
				}

				[[nodiscard]] friend constexpr day_of_month operator-(const day_of_month& d, const measures::days& ds) noexcept
				{
					return day_of_month{static_cast<unsigned>(d.value - ds.count())};
				}

				[[nodiscard]] friend constexpr measures::days operator-(const day_of_month& d1, const day_of_month& d2) noexcept
				{
					return measures::days{d1.value - d2.value};
				}

				constexpr day_of_month& operator+=(const measures::days& ds) noexcept
				{
					*this = *this + ds;
					return *this;
				}

				constexpr day_of_month& operator-=(const measures::days& ds) noexcept
				{
					*this = *this - ds;
					return *this;
				}

				constexpr day_of_month& operator++() noexcept
				{
					*this += measures::days{1};
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
					*this -= measures::days{1};
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

		constexpr day_of_month first{1};
		constexpr day_of_month second{2};
		constexpr day_of_month third{3};
		constexpr day_of_month fourth{4};
		constexpr day_of_month fifth{5};
		constexpr day_of_month sixth{6};
		constexpr day_of_month seventh{7};
		constexpr day_of_month eighth{8};
		constexpr day_of_month ninth{9};
		constexpr day_of_month tenth{10};
		constexpr day_of_month eleventh{11};
		constexpr day_of_month twelth{12};
		constexpr day_of_month thirteenth{13};
		constexpr day_of_month fourteenth{14};
		constexpr day_of_month fifteeth{15};
		constexpr day_of_month sixteeth{16};
		constexpr day_of_month seventeeth{17};
		constexpr day_of_month eighteenth{18};
		constexpr day_of_month nineteeth{19};
		constexpr day_of_month twentieth{20};
		constexpr day_of_month twenty_first{21};
		constexpr day_of_month twenty_second{22};
		constexpr day_of_month twenty_third{23};
		constexpr day_of_month twenty_fourth{24};
		constexpr day_of_month twenty_fifth{25};
		constexpr day_of_month twenty_sixth{26};
		constexpr day_of_month twenty_seventh{27};
		constexpr day_of_month twenty_eighth{28};
		constexpr day_of_month twenty_ninth{29};
		constexpr day_of_month thirtieth{30};
		constexpr day_of_month thirty_first{31};

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

		class weekday_indexed;
		class weekday_last;

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

				[[nodiscard]] friend constexpr weekday operator+(const weekday& wd, const measures::days& d) noexcept
				{
					return weekday{static_cast<unsigned>(static_cast<long long>(wd.c_encoding()) + d.count())};
				}

				[[nodiscard]] friend constexpr weekday operator+(const measures::days& d, const weekday& wd) noexcept
				{
					return weekday{static_cast<unsigned>(static_cast<long long>(wd.c_encoding()) + d.count())};
				}

				[[nodiscard]] friend constexpr weekday operator-(const weekday& wd, const measures::days& d) noexcept
				{
					const auto r = static_cast<long long>(wd.c_encoding()) - d.count();
					const auto r2 = static_cast<unsigned>(r);
					if (r < 0)
					{
						return weekday{7u + r2};
					}
					
					return weekday{r2};
				}

				[[nodiscard]] friend constexpr measures::days operator-(const weekday& wd1, const weekday& wd2) noexcept
				{
					return measures::days{wd1.c_encoding() - wd2.c_encoding()};
				}

				constexpr weekday& operator+=(const measures::days& d) noexcept
				{
					*this = *this + d;
					return *this;
				}

				constexpr weekday& operator-=(const measures::days& d) noexcept
				{
					*this = *this - d;
					return *this;
				}

				constexpr weekday& operator++() noexcept
				{
					*this += measures::days{1};
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
					*this -= measures::days{1};
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

		class weekday_indexed
		{
			public:
				constexpr weekday_indexed() = default;
				constexpr weekday_indexed(const weekday& wd, unsigned index) noexcept
					: wd_{wd}, index_{index} {}

				[[nodiscard]] constexpr datetime::weekday weekday() const noexcept
				{
					return wd_;
				}

				[[nodiscard]] constexpr unsigned index() const noexcept
				{
					return index_;
				}

				[[nodiscard]] constexpr bool ok() const noexcept
				{
					return wd_.ok() && (index_ >= 1u && index_ <= 5u);
				}

				[[nodiscard]] friend constexpr bool operator==(const weekday_indexed& x, const weekday_indexed& y) noexcept = default;
			private:
				datetime::weekday wd_ {};
				unsigned index_ {};
		};

		class weekday_last
		{
			public:
				explicit constexpr weekday_last(const weekday& wd) noexcept
					: wd_{wd} {}

				[[nodiscard]] constexpr datetime::weekday weekday() const noexcept
				{
					return wd_;
				}

				[[nodiscard]] constexpr bool ok() const noexcept
				{
					return wd_.ok();
				}

				[[nodiscard]] friend constexpr bool operator==(const weekday_last& x, const weekday_last& y) noexcept = default;
			private:
				datetime::weekday wd_ {};
		};

		constexpr weekday sunday {0};
		constexpr weekday monday {1};
		constexpr weekday tuesday {2};
		constexpr weekday wednesday {3};
		constexpr weekday thursday {4};
		constexpr weekday friday {5};
		constexpr weekday saturday {6};

		class month_day
		{
			public:
				constexpr month_day() = default;
				constexpr month_day(const datetime::month& m, const day_of_month& d)
					: m_{m}, d_{d} {}

				[[nodiscard]] constexpr datetime::month month() const noexcept
				{
					return m_;
				}

				[[nodiscard]] constexpr day_of_month day() const noexcept
				{
					return d_;
				}

				[[nodiscard]] constexpr bool ok() const noexcept
				{
					return m_.ok() && (unsigned{d_} >= 1 && unsigned{d_} <= static_cast<unsigned>(days_in_month(m_).count()));
				}

				[[nodiscard]] friend constexpr bool operator==(const month_day& x, const month_day& y) noexcept = default;

				[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const month_day& x, const month_day& y) noexcept
				{
					const auto r1 = x.month() <=> y.month();
					if (r1 != std::strong_ordering::equal)
					{
						return r1;
					}

					return x.day() <=> y.day();
				}
			private:
				datetime::month m_ {};
				day_of_month d_ {};
		};

		class month_day_last
		{
			public:
				explicit constexpr month_day_last(const datetime::month& m) noexcept
					: m_{m} {}

				[[nodiscard]] constexpr datetime::month month() const noexcept
				{
					return m_;
				}

				[[nodiscard]] constexpr bool ok() const noexcept
				{
					return m_.ok();
				}

				[[nodiscard]] friend constexpr bool operator==(const month_day_last&, const month_day_last&) noexcept = default;
				[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const month_day_last&, const month_day_last&) noexcept = default;
			private:
				datetime::month m_ {};
		};

		class month_weekday
		{
			public:
				constexpr month_weekday(const datetime::month m, const datetime::weekday_indexed w) noexcept
					: m_{m}, w_{w} {}

				[[nodiscard]] constexpr datetime::month month() const noexcept
				{
					return m_;
				}

				[[nodiscard]] constexpr datetime::weekday_indexed weekday_indexed() const noexcept
				{
					return w_;
				}

				[[nodiscard]] constexpr bool ok() const noexcept
				{
					return m_.ok() && w_.ok();
				}

				[[nodiscard]] friend constexpr bool operator==(const month_weekday&, const month_weekday&) noexcept = default;
			private:
				datetime::month m_ {};
				datetime::weekday_indexed w_ {};
		};

		class month_weekday_last
		{
			public:
				constexpr month_weekday_last(const datetime::month& m, const datetime::weekday_last& w)
					: m_{m}, w_{w} {}

				[[nodiscard]] constexpr datetime::month month() const noexcept
				{
					return m_;
				}

				[[nodiscard]] constexpr datetime::weekday_last weekday_last() const noexcept
				{
					return w_;
				}

				[[nodiscard]] constexpr bool ok() const noexcept
				{
					return m_.ok() && w_.ok();
				}

				[[nodiscard]] friend constexpr bool operator==(const month_weekday_last&, const month_weekday_last&) noexcept = default;
			private:
				datetime::month m_ {};
				datetime::weekday_last w_;
		};

		class year_month
		{
			public:
				constexpr year_month() = default;
				constexpr year_month(const datetime::year& y, const datetime::month& m) noexcept
					: y_{y}, m_{m} {}

				[[nodiscard]] constexpr datetime::year year() const noexcept
				{
					return y_;
				}

				[[nodiscard]] constexpr datetime::month month() const noexcept
				{
					return m_;
				}

				[[nodiscard]] constexpr bool ok() const noexcept
				{
					return m_.ok();
				}

				[[nodiscard]] friend constexpr year_month operator+(const year_month& ym, const measures::years& y) noexcept
				{
					return year_month{ym.year() + y, ym.month()};
				}

				[[nodiscard]] friend constexpr year_month operator+(const measures::years& y, const year_month& ym) noexcept
				{
					return ym + y;
				}

				[[nodiscard]] friend constexpr year_month operator+(const year_month& ym, const measures::months& m) noexcept
				{
					const auto year_change = measures::unit_cast<measures::years>(m);
					const auto month_change = m - measures::unit_cast<measures::months>(year_change);
					return year_month{ym.year() + year_change, ym.month() + month_change};
				}

				[[nodiscard]] friend constexpr year_month operator+(const measures::months& m, const year_month& ym) noexcept
				{
					return ym + m;
				}

				[[nodiscard]] friend constexpr year_month operator-(const year_month& ym, const measures::years& y) noexcept
				{
					return ym + -y;
				}

				[[nodiscard]] friend constexpr year_month operator-(const year_month& ym, const measures::months& m) noexcept
				{
					return ym + -m;
				}

				[[nodiscard]] friend constexpr measures::months operator-(const year_month& ym1, const year_month& ym2) noexcept
				{
					return ym1.year() - ym2.year() + measures::months{static_cast<int>(unsigned{ym1.month()}) - static_cast<int>(unsigned{ym2.month()})};
				}

				constexpr year_month& operator+=(const measures::years& y) noexcept
				{
					*this = *this + y;
					return *this;
				}

				constexpr year_month& operator+=(const measures::months& m) noexcept
				{
					*this = *this + m;
					return *this;
				}

				constexpr year_month& operator-=(const measures::years& y) noexcept
				{
					*this = *this - y;
					return *this;
				}

				constexpr year_month& operator-=(const measures::months& m) noexcept
				{
					*this = *this - m;
					return *this;
				}

				[[nodiscard]] friend constexpr bool operator==(const year_month&, const year_month&) noexcept = default;
				[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const year_month& x, const year_month& y) noexcept
				{
					const auto r = x.year() <=> y.year();
					if (r != std::strong_ordering::equal)
					{
						return r;
					}

					return x.month() <=> y.month();
				}
			private:
				datetime::year y_ {};
				datetime::month m_ {};
		};

		namespace details
		{
			[[nodiscard]] constexpr measures::days to_days(const datetime::year& y_, const datetime::month& m_, const datetime::day_of_month& d_) noexcept
			{
				const auto y = static_cast<std::int64_t>(y_) - (m_ <= datetime::february);
				const auto m = static_cast<unsigned>(m_);
				const auto d = static_cast<unsigned>(d_);
				const auto era = (y >= 0 ? y : y - 399) / 400;
				const auto yoe = static_cast<unsigned>(y - era * 400);
				const auto doy = (153 * (m > 2 ? m - 3 : m + 9) + 2) / 5 + d - 1;
				const auto doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
				return measures::days{era * 146097 + static_cast<std::int64_t>(doe) - 719468};
			}
		}

		class year_month_day_last
		{
			public:
				constexpr year_month_day_last(const datetime::year& y, const datetime::month_day_last& mdl) noexcept
					: y_{y}, mdl_{mdl} {}

				[[nodiscard]] constexpr datetime::year year() const noexcept
				{
					return y_;
				}

				[[nodiscard]] constexpr datetime::month month() const noexcept
				{
					return mdl_.month();
				}

				[[nodiscard]] constexpr datetime::day_of_month day() const noexcept
				{
					return last_day_of_month(month(), year());
				}

				[[nodiscard]] constexpr datetime::month_day_last month_day_last() const noexcept
				{
					return mdl_;
				}

				[[nodiscard]] friend constexpr year_month_day_last operator+(const year_month_day_last& ymdl, const measures::months& m) noexcept
				{
					const auto r = year_month{ymdl.year(), ymdl.month()} + m;
					return year_month_day_last{r.year(), datetime::month_day_last{r.month()}};
				}

				[[nodiscard]] friend constexpr year_month_day_last operator+(const measures::months& m, const year_month_day_last& ymdl) noexcept
				{
					return ymdl + m;
				}

				[[nodiscard]] friend constexpr year_month_day_last operator+(const year_month_day_last& ymdl, const measures::years& y) noexcept
				{
					return year_month_day_last{ymdl.year() + y, ymdl.month_day_last()};
				}

				[[nodiscard]] friend constexpr year_month_day_last operator+(const measures::years& y, const year_month_day_last& ymdl) noexcept
				{
					return ymdl + y;
				}

				[[nodiscard]] friend constexpr year_month_day_last operator-(const year_month_day_last& ymdl, const measures::months& m) noexcept
				{
					return ymdl + -m;
				}

				[[nodiscard]] friend constexpr year_month_day_last operator-(const year_month_day_last& ymdl, const measures::years& y) noexcept
				{
					return ymdl + -y;
				}

				[[nodiscard]] constexpr bool ok() const noexcept
				{
					return month().ok();
				}

				[[nodiscard]] friend constexpr bool operator==(const year_month_day_last&, const year_month_day_last&) noexcept = default;
				[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const year_month_day_last& x, const year_month_day_last& y) noexcept
				{
					const auto r = x.year() <=> y.year();
					if (r != std::strong_ordering::equal)
					{
						return r;
					}

					return x.month() <=> y.month();
				}

				[[nodiscard]] constexpr operator sys_days() const noexcept
				{
					return sys_days{measures::to_chrono(details::to_days(year(), month(), day()))};
				}
			private:
				datetime::year y_;
				datetime::month_day_last mdl_;
		};

		class year_month_day
		{
			static constexpr auto epoch_year = datetime::year{1970};
			static constexpr auto epoch_month = january;
			static constexpr auto epoch_day = first;

			public:
				constexpr year_month_day() = default;
				constexpr year_month_day(const datetime::year& y, const datetime::month& m, const datetime::day_of_month& d)
					: y_{y}, m_{m}, d_{d} {}

				constexpr year_month_day(const year_month_day_last& ymdl) noexcept
					: y_{ymdl.year()}, m_{ymdl.month()}, d_{ymdl.day()} {}

				constexpr year_month_day(const sys_days& sys_days)
					: year_month_day{from_days(measures::from_chrono(sys_days.time_since_epoch()))} {}

				[[nodiscard]] constexpr datetime::year year() const noexcept
				{
					return y_;
				}

				[[nodiscard]] constexpr datetime::month month() const noexcept
				{
					return m_;
				}

				[[nodiscard]] constexpr datetime::day_of_month day() const noexcept
				{
					return d_;
				}

				[[nodiscard]] constexpr operator sys_days() const noexcept
				{
					return sys_days{measures::to_chrono(details::to_days(year(), month(), day()))};
				}

				[[nodiscard]] constexpr bool ok() const noexcept
				{
					return month().ok() && (day() >= 1u && day() <= last_day_of_month(month(), year()));
				}

				[[nodiscard]] friend constexpr bool operator==(const year_month_day& x, const year_month_day& y) noexcept
				{
					return x.year() == y.year() &&
						   x.month() == y.month() &&
						   x.day() == y.day();
				}

				[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const year_month_day& x, const year_month_day& y) noexcept
				{
					if (const auto r = x.year() <=> y.year(); r != std::strong_ordering::equal)
					{
						return r;
					}

					if (const auto r = x.month() <=> x.month(); r != std::strong_ordering::equal)
					{
						return r;
					}

					return x.day() <=> y.day();
				}

				[[nodiscard]] friend constexpr year_month_day operator+(const year_month_day& ymd, const measures::months& m) noexcept
				{
					const auto r = year_month{ymd.year(), ymd.month()} + m;
					return year_month_day{r.year(), r.month(), ymd.day()};
				}

				[[nodiscard]] friend constexpr year_month_day operator+(const measures::months& m, const year_month_day& ymd) noexcept
				{
					return ymd + m;
				}

				[[nodiscard]] friend constexpr year_month_day operator+(const year_month_day& ymd, const measures::years& y) noexcept
				{
					return year_month_day{ymd.year() + y, ymd.month(), ymd.day()};
				}

				[[nodiscard]] friend constexpr year_month_day operator+(const measures::years& y, const year_month_day& ymd) noexcept
				{
					return ymd + y;
				}

				[[nodiscard]] friend constexpr year_month_day operator+(const year_month_day& ymd, const measures::days& d) noexcept
				{
					const auto system_days = static_cast<sys_days>(ymd);

					return year_month_day{system_days + measures::to_chrono(d)};
				}

				[[nodiscard]] friend constexpr year_month_day operator+(const measures::days& d, const year_month_day& ymd) noexcept
				{
					return ymd + d;
				}

				[[nodiscard]] friend constexpr year_month_day operator-(const year_month_day& ymd, const measures::months& m) noexcept
				{
					return ymd + -m;
				}

				[[nodiscard]] friend constexpr year_month_day operator-(const year_month_day& ymd, const measures::years& y) noexcept
				{
					return ymd + -y;
				}

				[[nodiscard]] friend constexpr year_month_day operator-(const year_month_day& ymd, const measures::days& d) noexcept
				{
					return ymd + -d;
				}
			private:
				datetime::year y_ {};
				datetime::month m_ {};
				datetime::day_of_month d_ {};

				[[nodiscard]] static constexpr year_month_day from_days(const measures::days& dp) noexcept
				{
					const auto z = dp.count() + 719468;
					const auto era = (z >= 0 ? z : z - 146096) / 146097;
					const auto doe = static_cast<unsigned>(z - era * 146097);
					const auto yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
					const auto y = static_cast<typename measures::days::rep>(yoe) + era * 400;
					const auto doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
					const auto mp = (5 * doy + 2) / 153;
					const auto d = doy - (153 * mp + 2) / 5 + 1;
					const auto m = mp < 10 ? mp + 3 : mp - 9;
					return year_month_day{datetime::year{y + (m <= 2)}, datetime::month{m}, datetime::day_of_month{d}};
				}
		};

		class year_month_weekday
		{
			public:
				constexpr year_month_weekday() = default;
				constexpr year_month_weekday(const datetime::year& y, const datetime::month& m, const datetime::weekday_indexed& w)
					: y_{y}, m_{m}, w_{w} {}

				constexpr year_month_weekday(const sys_days& sys_days)
					: year_month_weekday{from_days(sys_days)} {}


				[[nodiscard]] constexpr datetime::year year() const noexcept
				{
					return y_;
				}

				[[nodiscard]] constexpr datetime::month month() const noexcept
				{
					return m_;
				}

				[[nodiscard]] constexpr datetime::weekday weekday() const noexcept
				{
					return w_.weekday();
				}

				[[nodiscard]] constexpr unsigned index() const noexcept
				{
					return w_.index();
				}

				[[nodiscard]] constexpr datetime::weekday_indexed weekday_indexed() const noexcept
				{
					return w_;
				}

				[[nodiscard]] constexpr bool ok() const noexcept
				{
					return month().ok() && weekday_indexed().ok();
				}

				[[nodiscard]] constexpr operator sys_days() const noexcept
				{
					return sys_days{measures::to_chrono(to_days())};
				}

				[[nodiscard]] friend constexpr bool operator==(const year_month_weekday& x, const year_month_weekday& y) noexcept = default;

				[[nodiscard]] friend constexpr year_month_weekday operator+(const year_month_weekday& ymwd, const measures::months& m) noexcept
				{
					const auto ym = year_month{ymwd.year(), ymwd.month()} + m;
					return {ym.year(), ym.month(), ymwd.weekday_indexed()};
				}

				[[nodiscard]] friend constexpr year_month_weekday operator+(const measures::months& m, const year_month_weekday& ymwd) noexcept
				{
					return ymwd + m;
				}

				[[nodiscard]] friend constexpr year_month_weekday operator+(const year_month_weekday& ymwd, const measures::years& y) noexcept
				{
					return {ymwd.year() + y, ymwd.month(), ymwd.weekday_indexed()};
				}

				[[nodiscard]] friend constexpr year_month_weekday operator+(const measures::years& y, const year_month_weekday& ymwd) noexcept
				{
					return ymwd + y;
				}

				[[nodiscard]] friend constexpr year_month_weekday operator-(const year_month_weekday& ymwd, const measures::months& m) noexcept
				{
					return ymwd + -m;
				}

				[[nodiscard]] friend constexpr year_month_weekday operator-(const year_month_weekday& ymwd, const measures::years& y) noexcept
				{
					return ymwd + -y;
				}

				constexpr year_month_weekday& operator+=(const measures::years& y) noexcept
				{
					*this = *this + y;
					return *this;
				}

				constexpr year_month_weekday& operator+=(const measures::months& m) noexcept
				{
					*this = *this + m;
					return *this;
				}

				constexpr year_month_weekday& operator-=(const measures::years& y) noexcept
				{
					*this = *this - y;
					return *this;
				}

				constexpr year_month_weekday& operator-=(const measures::months& m) noexcept
				{
					*this = *this - m;
					return *this;
				}
			private:
				datetime::year y_ {};
				datetime::month m_ {};
				datetime::weekday_indexed w_ {};

				[[nodiscard]] static constexpr year_month_weekday from_days(const std::chrono::time_point<std::chrono::system_clock, measures::chrono_t<measures::days>>& sys_days)
				{
					const auto wd = datetime::weekday{sys_days};
					const auto ymd = datetime::year_month_day{sys_days};
					return {ymd.year(), ymd.month(), wd[(static_cast<unsigned>(ymd.day()) - 1) / 7 + 1]};
				}

				[[nodiscard]] constexpr measures::days to_days() const noexcept
				{
					const auto d = sys_days{datetime::year_month_day{year(), month(), first}};
					return measures::from_chrono((d + measures::to_chrono((w_.weekday() - datetime::weekday{d} + measures::days{(w_.index() - 1) * 7}))).time_since_epoch());
				}
		};

		class year_month_weekday_last
		{
			public:
				constexpr year_month_weekday_last(const datetime::year& y, const datetime::month& m, const datetime::weekday_last& w)
					: y_{y}, m_{m}, w_{w} {}

				[[nodiscard]] constexpr datetime::year year() const noexcept
				{
					return y_;
				}

				[[nodiscard]] constexpr datetime::month month() const noexcept
				{
					return m_;
				}

				[[nodiscard]] constexpr datetime::weekday weekday() const noexcept
				{
					return w_.weekday();
				}

				[[nodiscard]] constexpr datetime::weekday_last weekday_last() const noexcept
				{
					return w_;
				}

				[[nodiscard]] constexpr operator sys_days() const noexcept
				{
					return sys_days{measures::to_chrono(to_days())};
				}

				[[nodiscard]] constexpr bool ok() const noexcept
				{
					return month().ok() && weekday().ok();
				}

				[[nodiscard]] friend constexpr bool operator==(const year_month_weekday_last&, const year_month_weekday_last&) noexcept = default;

				[[nodiscard]] friend constexpr year_month_weekday_last operator+(const year_month_weekday_last& ymwdl, const measures::months& m) noexcept
				{
					const auto ym = year_month{ymwdl.year(), ymwdl.month()} + m;
					return year_month_weekday_last{ym.year(), ym.month(), ymwdl.weekday_last()};
				}

				[[nodiscard]] friend constexpr year_month_weekday_last operator+(const measures::months& m, const year_month_weekday_last& ymwdl) noexcept
				{
					return ymwdl + m;
				}

				[[nodiscard]] friend constexpr year_month_weekday_last operator+(const year_month_weekday_last& ymwdl, const measures::years& y) noexcept
				{
					return year_month_weekday_last{ymwdl.year() + y, ymwdl.month(), ymwdl.weekday_last()};
				}

				[[nodiscard]] friend constexpr year_month_weekday_last operator+(const measures::years& y, const year_month_weekday_last& ymwdl) noexcept
				{
					return ymwdl + y;
				}

				[[nodiscard]] friend constexpr year_month_weekday_last operator-(const year_month_weekday_last& ymwdl, const measures::months& m) noexcept
				{
					return ymwdl + -m;
				}

				[[nodiscard]] friend constexpr year_month_weekday_last operator-(const year_month_weekday_last& ymwdl, const measures::years& y) noexcept
				{
					return ymwdl + -y;
				}

				constexpr year_month_weekday_last& operator+=(const measures::years& y) noexcept
				{
					*this = *this + y;
					return *this;
				}

				constexpr year_month_weekday_last& operator+=(const measures::months& m) noexcept
				{
					*this = *this + m;
					return *this;
				}

				constexpr year_month_weekday_last& operator-=(const measures::years& y) noexcept
				{
					*this = *this - y;
					return *this;
				}

				constexpr year_month_weekday_last& operator-=(const measures::months& m) noexcept
				{
					*this = *this - m;
					return *this;
				}
			private:
				datetime::year y_;
				datetime::month m_;
				datetime::weekday_last w_;

				[[nodiscard]] constexpr measures::days to_days() const noexcept
				{
					const auto d = sys_days{year_month_day_last{y_, month_day_last{m_}}};
					return measures::from_chrono((d - measures::to_chrono((datetime::weekday{d} - w_.weekday()))).time_since_epoch());
				}
		};

		[[nodiscard]] constexpr year_month operator/(const year& y, const month& m) noexcept
		{
			return year_month{y, m};
		}

		[[nodiscard]] constexpr year_month operator/(const year& y, const int m) noexcept
		{
			return year_month{y, month{static_cast<unsigned>(m)}};
		}

		[[nodiscard]] constexpr month_day operator/(const month& m, const day_of_month& d) noexcept
		{
			return month_day{m, d};
		}

		[[nodiscard]] constexpr month_day operator/(const month& m, const int d) noexcept
		{
			return month_day{m, day_of_month{static_cast<unsigned>(d)}};
		}

		[[nodiscard]] constexpr month_day operator/(const int m, const day_of_month& d) noexcept
		{
			return month_day{month{static_cast<unsigned>(m)}, d};
		}

		[[nodiscard]] constexpr month_day operator/(const day_of_month& d, const month& m) noexcept
		{
			return month_day{m, d};
		}

		[[nodiscard]] constexpr month_day operator/(const day_of_month& d, const int m) noexcept
		{
			return month_day{month{static_cast<unsigned>(m)}, d};
		}

		[[nodiscard]] constexpr month_day_last operator/(const month& m, last_spec) noexcept
		{
			return month_day_last{m};
		}

		[[nodiscard]] constexpr month_day_last operator/(const int m, last_spec) noexcept
		{
			return month_day_last{month{static_cast<unsigned>(m)}};
		}

		[[nodiscard]] constexpr month_day_last operator/(last_spec, const month& m) noexcept
		{
			return month_day_last{m};
		}

		[[nodiscard]] constexpr month_day_last operator/(last_spec, const int m) noexcept
		{
			return month_day_last{month{static_cast<unsigned>(m)}};
		}

		[[nodiscard]] constexpr month_weekday operator/(const month& m, const weekday_indexed& w) noexcept
		{
			return month_weekday{m, w};
		}

		[[nodiscard]] constexpr month_weekday operator/(const int m, const weekday_indexed& w) noexcept
		{
			return month_weekday{month{static_cast<unsigned>(m)}, w};
		}

		[[nodiscard]] constexpr month_weekday operator/(const weekday_indexed& w, const month& m) noexcept
		{
			return month_weekday{m, w};
		}

		[[nodiscard]] constexpr month_weekday operator/(const weekday_indexed& w, const int m) noexcept
		{
			return month_weekday{month{static_cast<unsigned>(m)}, w};
		}

		[[nodiscard]] constexpr month_weekday_last operator/(const month& m, const weekday_last& w) noexcept
		{
			return month_weekday_last{m, w};
		}

		[[nodiscard]] constexpr month_weekday_last operator/(const int m, const weekday_last& w) noexcept
		{
			return month_weekday_last{month{static_cast<unsigned>(m)}, w};
		}

		[[nodiscard]] constexpr month_weekday_last operator/(const weekday_last& w, const month& m) noexcept
		{
			return month_weekday_last{m, w};
		}

		[[nodiscard]] constexpr month_weekday_last operator/(const weekday_last& w, const int m) noexcept
		{
			return month_weekday_last{month{static_cast<unsigned>(m)}, w};
		}

		[[nodiscard]] constexpr year_month_day operator/(const year_month& ym, const day_of_month& d) noexcept
		{
			return year_month_day{ym.year(), ym.month(), d};
		}

		[[nodiscard]] constexpr year_month_day operator/(const year_month& ym, const int d) noexcept
		{
			return year_month_day{ym.year(), ym.month(), day_of_month{static_cast<unsigned>(d)}};
		}

		[[nodiscard]] constexpr year_month_day operator/(const year& y, const month_day& md) noexcept
		{
			return year_month_day{y, md.month(), md.day()};
		}

		[[nodiscard]] constexpr year_month_day operator/(const int y, const month_day& md) noexcept
		{
			return year_month_day{year{y}, md.month(), md.day()};
		}

		[[nodiscard]] constexpr year_month_day operator/(const month_day& md, const year& y) noexcept
		{
			return year_month_day{y, md.month(), md.day()};
		}

		[[nodiscard]] constexpr year_month_day operator/(const month_day& md, const int y) noexcept
		{
			return year_month_day{year{y}, md.month(), md.day()};
		}

		[[nodiscard]] constexpr year_month_day_last operator/(const year_month& ym, last_spec) noexcept
		{
			return year_month_day_last{ym.year(), month_day_last{ym.month()}};
		}

		[[nodiscard]] constexpr year_month_day_last operator/(const year& y, const month_day_last& mdl) noexcept
		{
			return year_month_day_last{y, mdl};
		}

		[[nodiscard]] constexpr year_month_day_last operator/(const int y, const month_day_last& mdl) noexcept
		{
			return year_month_day_last{year{y}, mdl};
		}

		[[nodiscard]] constexpr year_month_day_last operator/(const month_day_last& mdl, const year& y) noexcept
		{
			return year_month_day_last{y, mdl};
		}

		[[nodiscard]] constexpr year_month_day_last operator/(const month_day_last& mdl, const int y) noexcept
		{
			return year_month_day_last{year{y}, mdl};
		}

		[[nodiscard]] constexpr year_month_weekday operator/(const year_month& ym, const weekday_indexed& w) noexcept
		{
			return year_month_weekday{ym.year(), ym.month(), w};
		}

		[[nodiscard]] constexpr year_month_weekday operator/(const year& y, const month_weekday& mwd) noexcept
		{
			return year_month_weekday{y, mwd.month(), mwd.weekday_indexed()};
		}

		[[nodiscard]] constexpr year_month_weekday operator/(const int y, const month_weekday& mwd) noexcept
		{
			return year_month_weekday{year{y}, mwd.month(), mwd.weekday_indexed()};
		}

		[[nodiscard]] constexpr year_month_weekday operator/(const month_weekday& mwd, const year& y) noexcept
		{
			return year_month_weekday{y, mwd.month(), mwd.weekday_indexed()};
		}

		[[nodiscard]] constexpr year_month_weekday operator/(const month_weekday& mwd, const int y) noexcept
		{
			return year_month_weekday{year{y}, mwd.month(), mwd.weekday_indexed()};
		}

		[[nodiscard]] constexpr year_month_weekday_last operator/(const year_month& ym, const weekday_last& w) noexcept
		{
			return year_month_weekday_last{ym.year(), ym.month(), w};
		}

		[[nodiscard]] constexpr year_month_weekday_last operator/(const year& y, const month_weekday_last& mwdl) noexcept
		{
			return year_month_weekday_last{y, mwdl.month(), mwdl.weekday_last()};
		}

		[[nodiscard]] constexpr year_month_weekday_last operator/(const int y, const month_weekday_last& mwdl) noexcept
		{
			return year_month_weekday_last{year{y}, mwdl.month(), mwdl.weekday_last()};
		}

		[[nodiscard]] constexpr year_month_weekday_last operator/(const month_weekday_last& mwdl, const year& y) noexcept
		{
			return year_month_weekday_last{y, mwdl.month(), mwdl.weekday_last()};
		}

		[[nodiscard]] constexpr year_month_weekday_last operator/(const month_weekday_last& mwdl, const int y) noexcept
		{
			return year_month_weekday_last{year{y}, mwdl.month(), mwdl.weekday_last()};
		}

		struct sys_info
		{
			sys_seconds begin;
			sys_seconds end;
			std::chrono::seconds offset;
			std::chrono::minutes save;
			std::string abbrev;
		};

		class time_zone
		{
			public:
				std::string_view name() const noexcept
				{
					return name_;
				}

				template<class Duration>
				sys_info get_info(const sys_time<Duration>& tp) const;

				//template<class Duration>
				//sys_time<std::common_type_t<Duration, std::chrono::seconds>>
				//to_sys(const local_time<Duration>& tp) const;

				[[nodiscard]] friend bool operator==(const time_zone& x, const time_zone& y) noexcept
				{
					return x.name() == y.name();
				}

				[[nodiscard]] friend bool operator!=(const time_zone& x, const time_zone& y) noexcept
				{
					return !(x == y);
				}
			private:
				std::string name_;
				std::vector<sys_info> info_;
		};

		class time_zone_link
		{
			public:
				[[nodiscard]] std::string_view name() const noexcept
				{
					return name_;
				}

				[[nodiscard]] std::string_view target() const noexcept
				{
					return target_;
				}

				[[nodiscard]] friend bool operator==(const time_zone_link& x, const time_zone_link& y) noexcept
				{
					return x.name() == y.name();
				}

				[[nodiscard]] friend bool operator!=(const time_zone_link& x, const time_zone_link& y) noexcept
				{
					return !(x == y);
				}
			private:
				std::string name_;
				std::string target_;
		};

		struct tzdb;

		namespace details
		{
			[[nodiscard]] tzdb get_default_tzdb();
		}

		struct tzdb
		{
			public:
				std::string version;
				std::vector<time_zone> zones;

			private:
				tzdb()
					: version{"2020f"} {}

				friend tzdb details::get_default_tzdb();
		};

		namespace details
		{
			[[nodiscard]] tzdb get_default_tzdb()
			{
				return tzdb{};
			}
		}

		class tzdb_list
		{
			using container = std::forward_list<tzdb>;

			public:
				tzdb_list(const tzdb_list&) = delete;
				tzdb_list& operator=(const tzdb_list&) = delete;

				using const_iterator = typename container::const_iterator;

				const tzdb& front() const noexcept
				{
					std::scoped_lock l{m};
					return values.front();
				}

				const_iterator erase_after(const_iterator p)
				{
					std::scoped_lock l{m};
					return values.erase_after(p);
				}

				const_iterator begin() const noexcept
				{
					return values.begin();
				}

				const_iterator cbegin() const noexcept
				{
					return values.cbegin();
				}

				const_iterator end() const noexcept
				{
					return values.end();
				}

				const_iterator cend() const noexcept
				{
					return values.cend();
				}
			private:
				container values;
				mutable std::mutex m;

				tzdb_list()
				{
					values.push_front(details::get_default_tzdb());
				}

				friend tzdb_list& get_tzdb_list();
				friend const tzdb& reload_tzdb();
		};

		tzdb_list& get_tzdb_list()
		{
			static tzdb_list l;
			return l;
		}

		const tzdb& get_tzdb()
		{
			return get_tzdb_list().front();
		}

		std::string remote_version()
		{
			return get_tzdb().version;
		}

		const tzdb& reload_tzdb()
		{
			if (remote_version() == get_tzdb().version)
			{
				return get_tzdb();
			}

			auto& list = get_tzdb_list();
			std::scoped_lock l {list.m};

			// Add new tzdb to list
			return list.values.front();
		}
	}

	template<typename CharT>
	struct formatter<std::tm, CharT> : public details::base_chrono_formatter<std::tm, CharT>
	{
		public:
			template<class OutputIt>
			constexpr auto format(const std::tm& t, basic_format_context<OutputIt, CharT>& ctx)
			{
				details::tm_handler h {t, ctx};
				details::parse_chrono_format(this->fmt, h);

				return details::write_string_view(h.str(), ctx, this->parser);
			}
	};

	template<typename Duration, typename CharT>
	struct formatter<std::chrono::time_point<std::chrono::system_clock, Duration>, CharT> : public details::base_chrono_formatter<std::chrono::time_point<std::chrono::system_clock, Duration>, CharT>
	{
		private:
			template<class OutputIt>
			struct handler : public details::tm_handler<CharT, OutputIt>
			{
				handler(const handler&) = delete;
				handler& operator=(const handler&) = delete;

				handler(const std::tm& value, basic_format_context<OutputIt, CharT>& ctx)
					: details::tm_handler<CharT, OutputIt>{value, ctx} {}

				void on_tz_name()
				{
					this->on_text(cmoon::choose_str_literal<CharT>(STR_LITERALS("UTC")));
				}

				void on_utc_offset(const details::numeric_system s)
				{
					if (s == details::numeric_system::standard)
					{
						this->on_text(cmoon::choose_str_literal<CharT>(STR_LITERALS("+0000")));
					}
					else
					{
						this->on_text(cmoon::choose_str_literal<CharT>(STR_LITERALS("+00:00")));
					}
				}
			};
		public:
			template<class OutputIt>
			auto format(const std::chrono::time_point<std::chrono::system_clock, Duration>& val, basic_format_context<OutputIt, CharT>& ctx)
			{
				const auto time1 = std::chrono::system_clock::to_time_t(val);
				std::tm time2;
				::localtime_s(std::addressof(time2), std::addressof(time1));

				handler h{time2, ctx};
				details::parse_chrono_format(this->fmt, h);

				return details::write_string_view(h.str(), ctx, this->parser);
			}
	};

	template<class Duration, typename CharT>
	struct formatter<datetime::hh_mm_ss<Duration>, CharT> : public details::base_chrono_formatter<datetime::hh_mm_ss<Duration>, CharT>
	{
		private:
			template<class OutputIt>
			struct handler : public details::tm_handler<CharT, OutputIt>
			{
				public:
					handler(const handler&) = delete;
					handler& operator=(const handler&) = delete;

					handler(const std::tm& value, basic_format_context<OutputIt, CharT>& ctx)
						: details::tm_handler<CharT, OutputIt>{value, ctx} {}

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
					void on_day_of_month_space(const details::numeric_system) { this->throw_not_supported(); }
					void on_day_of_month_zero(const details::numeric_system) { this->throw_not_supported(); }
					void on_dec_month(const details::numeric_system) { this->throw_not_supported(); }
					void on_century(const details::numeric_system) { this->throw_not_supported(); }
					void on_abbr_year(const details::numeric_system) { this->throw_not_supported(); }
					void on_offset_year() { this->throw_not_supported(); }
					void on_year(const details::numeric_system) { this->throw_not_supported(); }
			};

		public:
			template<class OutputIt>
			auto format(const datetime::hh_mm_ss<Duration>& val, basic_format_context<OutputIt, CharT>& ctx)
			{
				std::tm time;
				if constexpr (std::same_as<Duration, measures::hours>)
				{
					time.tm_hour = static_cast<int>(val.hours().count());
				}
				else if constexpr (std::same_as<Duration, measures::minutes>)
				{
					time.tm_hour = static_cast<int>(val.hours().count());
					time.tm_min = static_cast<int>(val.minutes().count());
				}
				else
				{
					time.tm_hour = static_cast<int>(val.hours().count());
					time.tm_min = static_cast<int>(val.minutes().count());
					time.tm_sec = static_cast<int>(val.seconds().count());
				}

				handler h{time, ctx};
				details::parse_chrono_format(this->fmt, h);

				return details::write_string_view(h.str(), ctx, this->parser);
			}
	};

	template<class Rep, class Period, typename CharT>
	struct formatter<std::chrono::duration<Rep, Period>, CharT> : public formatter<datetime::hh_mm_ss<measures::time<Rep, measures::metric_system, Period>>, CharT>
	{
		template<class OutputIt>
		auto format(const std::chrono::duration<Rep, Period>& d, basic_format_context<OutputIt, CharT>& ctx)
		{
			const datetime::hh_mm_ss<measures::time<Rep, measures::metric_system, Period>> hhmmss {measures::from_chrono(d)};
			return formatter<datetime::hh_mm_ss<measures::time<Rep, measures::metric_system, Period>>, CharT>::format(hhmmss, ctx);
		}
	};

	template<class CharT>
	struct formatter<datetime::day_of_month, CharT> : public details::base_chrono_formatter<datetime::day_of_month, CharT>
	{
		private:
			template<class OutputIt>
			struct handler : public details::tm_handler<CharT, OutputIt>
			{
				handler(const handler&) = delete;
				handler& operator=(const handler&) = delete;

				handler(const std::tm& value, basic_format_context<OutputIt, CharT>& ctx)
					: details::tm_handler<CharT, OutputIt>{value, ctx} {}

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
				details::parse_chrono_format(this->fmt, h);

				return details::write_string_view(h.str(), ctx, this->parser);
			}
	};

	template<class CharT>
	struct formatter<datetime::day_of_year, CharT> : public details::base_chrono_formatter<datetime::day_of_year, CharT>
	{
		private:
			template<class OutputIt>
			struct handler : public details::tm_handler<CharT, OutputIt>
			{
				handler(const handler&) = delete;
				handler& operator=(const handler&) = delete;

				handler(const std::tm& value, basic_format_context<OutputIt, CharT>& ctx)
					: details::tm_handler<CharT, OutputIt>{value, ctx} {}

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
				void on_day_of_month_zero(const details::numeric_system) { this->throw_not_supported(); }
				void on_day_of_month_space(const details::numeric_system) { this->throw_not_supported(); }
			};
		public:
			template<class OutputIt>
			auto format(const datetime::day_of_year& val, basic_format_context<OutputIt, CharT>& ctx)
			{
				const std::tm time {.tm_yday = static_cast<int>(unsigned{val} - 1)};
				handler h{time, ctx};
				details::parse_chrono_format(this->fmt, h);

				return details::write_string_view(h.str(), ctx, this->parser);
			}
	};

	template<class CharT>
	struct formatter<datetime::month, CharT> : public details::base_chrono_formatter<datetime::month, CharT>
	{
		private:
			template<class OutputIt>
			struct handler : public details::tm_handler<CharT, OutputIt>
			{
				handler(const handler&) = delete;
				handler& operator=(const handler&) = delete;

				handler(const std::tm& value, basic_format_context<OutputIt, CharT>& ctx)
					: details::tm_handler<CharT, OutputIt>{value, ctx} {}

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
				details::parse_chrono_format(this->fmt, h);

				return details::write_string_view(h.str(), ctx, this->parser);
			}
	};

	template<class CharT>
	struct formatter<datetime::year, CharT> : public details::base_chrono_formatter<datetime::year, CharT>
	{
		private:
			template<class OutputIt>
			struct handler : public details::tm_handler<CharT, OutputIt>
			{
				handler(const handler&) = delete;
				handler& operator=(const handler&) = delete;

				handler(const std::tm& value, basic_format_context<OutputIt, CharT>& ctx)
					: details::tm_handler<CharT, OutputIt>{value, ctx} {}

				void on_abbr_month() { this->throw_not_supported(); }
				void on_full_month() { this->throw_not_supported(); }
				void on_dec_month(const details::numeric_system) { this->throw_not_supported(); }
				void on_abbr_weekday() { this->throw_not_supported(); }
				void on_full_weekday() { this->throw_not_supported(); }
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
			auto format(const datetime::year& val, basic_format_context<OutputIt, CharT>& ctx)
			{
				const std::tm time {.tm_year = static_cast<int>(val) - 1900};
				handler h{time, ctx};
				details::parse_chrono_format(this->fmt, h);

				return details::write_string_view(h.str(), ctx, this->parser);
			}
	};

	template<class CharT>
	struct formatter<datetime::weekday, CharT> : public details::base_chrono_formatter<datetime::weekday, CharT>
	{
		private:
			template<class OutputIt>
			struct handler : public details::tm_handler<CharT, OutputIt>
			{
				handler(const handler&) = delete;
				handler& operator=(const handler&) = delete;

				handler(const std::tm& value, basic_format_context<OutputIt, CharT>& ctx)
					: details::tm_handler<CharT, OutputIt>{value, ctx} {}

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
				details::parse_chrono_format(this->fmt, h);

				return details::write_string_view(h.str(), ctx, this->parser);
			}
	};

	template<class CharT>
	struct formatter<datetime::year_month_day, CharT> : public details::base_chrono_formatter<datetime::year_month_day, CharT>
	{
		private:
			template<class OutputIt>
			struct handler : public details::tm_handler<CharT, OutputIt>
			{
				handler(const handler&) = delete;
				handler& operator=(const handler&) = delete;

				handler(const std::tm& value, basic_format_context<OutputIt, CharT>& ctx)
					: details::tm_handler<CharT, OutputIt>{value, ctx} {}

				void on_datetime(const details::numeric_system) { this->throw_not_supported(); }
				void on_utc_offset(const details::numeric_system) { this->throw_not_supported(); }
				void on_tz_name() { this->throw_not_supported(); }
				void on_24_hour(const details::numeric_system) { this->throw_not_supported(); }
				void on_12_hour(const details::numeric_system) { this->throw_not_supported(); }
				void on_minute(const details::numeric_system) { this->throw_not_supported(); }
				void on_second(const details::numeric_system) { this->throw_not_supported(); }
				void on_loc_time(const details::numeric_system) { this->throw_not_supported(); }
			};
		public:
			template<class OutputIt>
			auto format(const datetime::year_month_day& val, basic_format_context<OutputIt, CharT>& ctx)
			{
				std::tm time {.tm_mday = static_cast<int>(unsigned{val.day()}),
							  .tm_mon = static_cast<int>(unsigned{val.month()} - 1),
							  .tm_year = static_cast<int>(val.year()) - 1900};

				std::mktime(std::addressof(time));

				handler h{time, ctx};
				details::parse_chrono_format(this->fmt, h);

				return details::write_string_view(h.str(), ctx, this->parser);
			}
	};
}