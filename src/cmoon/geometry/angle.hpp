#pragma once

#include <numbers>
#include <cmath>
#include <concepts>
#include <compare>

#include "cmoon/math.hpp"

namespace cmoon
{
	namespace geometry
	{
		enum class angle_type
		{
			turn,
			radian,
			degree,
			grad,
			minute_arc,
			second_arc
		};

		class angle
		{
			public:
				[[nodiscard]] static constexpr angle zero() noexcept
				{
					return angle{{0, 1}, angle_type::radian};
				}

				[[nodiscard]] static constexpr angle quarter() noexcept
				{
					return angle{{1, 2}, angle_type::radian};
				}

				[[nodiscard]] static constexpr angle half() noexcept
				{
					return angle{{1, 1}, angle_type::radian};
				}

				[[nodiscard]] static constexpr angle three_quarters() noexcept
				{
					return angle{{3, 2}, angle_type::radian};
				}

				[[nodiscard]] static constexpr angle full() noexcept
				{
					return angle{{2, 1}, angle_type::radian};
				}

				constexpr angle() = default;
				constexpr explicit angle(const cmoon::basic_rational<std::intmax_t>& amount, angle_type t)
					: angle{convert_type(amount, t)} {}

				template<std::integral T>
				constexpr explicit angle(T amount, angle_type t)
					: angle{convert_type(cmoon::basic_rational<std::intmax_t>{static_cast<std::intmax_t>(amount)}, t)} {}

				constexpr explicit angle(long double amount, angle_type t)
					: angle{convert_type(amount, t)} {}

				template<class Rep = long double>
				[[nodiscard]] constexpr Rep turns() const noexcept
				{
					return static_cast<Rep>(amount_ / 2);
				}

				template<class Rep = long double>
				[[nodiscard]] constexpr Rep radians() const noexcept
				{
					return static_cast<Rep>(amount_) * std::numbers::pi_v<Rep>;
				}

				template<class Rep = long double>
				[[nodiscard]] constexpr Rep degrees() const noexcept
				{
					return static_cast<Rep>(amount_ * 180);
				}

				template<class Rep = long double>
				[[nodiscard]] constexpr Rep grads() const noexcept
				{
					return static_cast<Rep>(amount_ * 200);
				}

				template<class Rep = long double>
				[[nodiscard]] constexpr Rep minute_arcs() const noexcept
				{
					return static_cast<Rep>(amount_ * 10'800);
				}

				template<class Rep = long double>
				[[nodiscard]] constexpr Rep second_arcs() const noexcept
				{
					return static_cast<Rep>(amount_ * 648'000);
				}

				constexpr angle& operator--() noexcept
				{
					--amount_;
					return *this;
				}

				constexpr angle operator--(int) noexcept
				{
					return angle{amount_--};
				}

				constexpr angle& operator++() noexcept
				{
					++amount_;
					return *this;
				}

				constexpr angle operator++(int) noexcept
				{
					return angle{amount_++};
				}

				[[nodiscard]] friend constexpr angle operator+(const angle& lhs, const angle& rhs) noexcept
				{
					return angle{lhs.amount_ + rhs.amount_};
				}

				[[nodiscard]] friend constexpr angle operator-(const angle& lhs, const angle& rhs) noexcept
				{
					return angle{lhs.amount_ - rhs.amount_};
				}

				template<std::integral T>
				[[nodiscard]] friend constexpr angle operator*(const angle& lhs, const T& rhs) noexcept
				{
					return angle{lhs.amount_ * rhs};
				}

				template<std::integral T>
				[[nodiscard]] friend constexpr angle operator/(const angle& lhs, const T& rhs) noexcept
				{
					return angle{lhs.amount_ / rhs};
				}

				constexpr angle& operator+=(const angle& a) noexcept
				{
					*this = *this + a;
					return *this;
				}

				constexpr angle& operator-=(const angle& a) noexcept
				{
					*this = *this - a;
					return *this;
				}

				template<std::integral T>
				constexpr angle& operator*=(const T& t) noexcept
				{
					*this = *this * t;
					return *this;
				}

				template<std::integral T>
				constexpr angle& operator/=(const T& t) noexcept
				{
					*this = *this / t;
					return *this;
				}

				[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const angle&, const angle&) noexcept = default;
			private:
				constexpr explicit angle(const cmoon::basic_rational<std::intmax_t>& amount)
					: amount_{cmoon::canonical(amount)} {}

				static constexpr angle convert_type(long double f, angle_type from) noexcept
				{
					if (from == angle_type::radian)
					{
						return angle{cmoon::basic_rational<std::intmax_t>{f / std::numbers::pi_v<long double>}};
					}

					const cmoon::basic_rational<std::intmax_t> amount{f};
					switch (from)
					{
						case angle_type::turn:
							return angle{amount * 2};
						case angle_type::degree:
							return angle{amount / 180};
						case angle_type::grad:
							return angle{amount / 200};
						case angle_type::minute_arc:
							return angle{amount / 10'800};
						case angle_type::second_arc:
							return angle{amount / 648'000};
					}
				}

				static constexpr angle convert_type(const cmoon::basic_rational<std::intmax_t>& amount, angle_type from) noexcept
				{
					switch (from)
					{
						case angle_type::radian:
							return angle{amount};
						case angle_type::turn:
							return angle{amount * 2};
						case angle_type::degree:
							return angle{amount / 180};
						case angle_type::grad:
							return angle{amount / 200};
						case angle_type::minute_arc:
							return angle{amount / 10'800};
						case angle_type::second_arc:
							return angle{amount / 648'000};
					}
				}

				cmoon::basic_rational<std::intmax_t> amount_ {0};
		};

		[[nodiscard]] constexpr bool right_angle(const angle& a) noexcept
		{
			return a == angle::quarter();
		}

		[[nodiscard]] constexpr bool acute_angle(const angle& a) noexcept
		{
			return a < angle::quarter();
		}

		[[nodiscard]] constexpr bool obtuse_angle(const angle& a) noexcept
		{
			return a > angle::quarter();
		}

		[[nodiscard]] constexpr bool complementary_angles(const angle& lhs, const angle& rhs) noexcept
		{
			return right_angle(lhs + rhs);
		}

		[[nodiscard]] constexpr bool supplementary_angles(const angle& lhs, const angle& rhs) noexcept
		{
			return (lhs + rhs) == angle::half();
		}

		[[nodiscard]] constexpr angle supplementary_angle(const angle& a) noexcept
		{
			return angle::half() - a;
		}

		[[nodiscard]] constexpr bool explementary_angles(const angle& lhs, const angle& rhs) noexcept
		{
			return (lhs + rhs) == angle::full();
		}

		[[nodiscard]] auto sin(const angle& a) noexcept
		{
			return std::sin(a.radians());
		}

		template<class T>
		[[nodiscard]] angle asin(T t) noexcept
		{
			return angle{std::asin(t), angle_type::radian};
		}

		[[nodiscard]] auto cos(const angle& a) noexcept
		{
			return std::cos(a.radians());
		}

		template<class T>
		[[nodiscard]] angle acos(T t) noexcept
		{
			return angle{std::acos(t), angle_type::radian};
		}

		[[nodiscard]] auto tan(const angle& a) noexcept
		{
			return std::tan(a.radians());
		}

		template<class T>
		[[nodiscard]] angle atan(T t) noexcept
		{
			return angle{std::atan(t), angle_type::radian};
		}

		template<class T, class F>
		[[nodiscard]] angle atan2(T t, F f) noexcept
		{
			return angle{std::atan2(t, f), angle_type::radian};
		}
	}
}