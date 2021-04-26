export module cmoon.geometry.line;

import <compare>;
import <type_traits>;

namespace cmoon::geometry
{
	template<class Rep>
	class line
	{
		public:
			using rep = Rep;

			constexpr line() = default;
			explicit constexpr line(const rep& l) noexcept(std::is_nothrow_copy_constructible_v<Rep>)
				: length_{l} {}

			constexpr void length(const rep& r) noexcept(std::is_nothrow_copy_assignable_v<Rep>)
			{
				length_ = r;
			}

			[[nodiscard]] constexpr rep length() const noexcept
			{
				return length_;
			}

			[[nodiscard]] constexpr line operator+() const noexcept
			{
				return *this;
			}

			[[nodiscard]] constexpr line operator-() const noexcept
			{
				return {-length_};
			}

			[[nodiscard]] friend constexpr line operator+(const line& lhs, const line& rhs) noexcept
			{
				return line{lhs.length() + rhs.length()};
			}

			[[nodiscard]] friend constexpr line operator-(const line& lhs, const line& rhs) noexcept
			{
				return line{lhs.length() - rhs.length()};
			}

			[[nodiscard]] friend constexpr line operator*(const line& lhs, const rep& rhs) noexcept
			{
				return line{lhs.length() * rhs};
			}

			[[nodiscard]] friend constexpr line operator/(const line& lhs, const rep& rhs) noexcept
			{
				return line{lhs.length() / rhs};
			}

			constexpr line& operator+=(const line& other) noexcept
			{
				*this = *this + other;
				return *this;
			}

			constexpr line& operator-=(const line& other) noexcept
			{
				*this = *this - other;
				return *this;
			}

			constexpr line& operator/=(const rep& other) noexcept
			{
				*this = *this / other;
				return *this;
			}

			constexpr line& operator*=(const rep& other) noexcept
			{
				*this = *this * other;
				return *this;
			}

			constexpr line& operator++() noexcept
			{
				++length_;
				return *this;
			}

			constexpr line operator++(int) noexcept
			{
				const auto c = *this;
				++*this;
				return c;
			}

			constexpr line& operator--() noexcept
			{
				--length_;
				return *this;
			}

			constexpr line operator--(int) noexcept
			{
				const auto c = *this;
				--* this;
				return c;
			}

			[[nodiscard]] friend constexpr auto operator<=>(const line&, const line&) noexcept = default;
		private:
			rep length_ {};
	};
}