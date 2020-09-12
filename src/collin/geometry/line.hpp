#pragma once

namespace collin
{
	namespace geometry
	{
		template<class Rep>
		class line
		{
			public:
				using rep = Rep;

				constexpr line() = default;
				constexpr line(const Rep& l)
					: length_{l} {}

				constexpr void length(const rep& r) noexcept
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

				template<class Rep2>
				constexpr line& operator+=(const line<Rep2>& other) noexcept
				{
					length_ += other.length();
					return *this;
				}

				template<class Rep2>
				constexpr line& operator-=(const line<Rep2>& other) noexcept
				{
					length_ -= other.length();
					return *this;
				}

				template<class Rep2>
				constexpr line& operator/=(const line<Rep2>& other) noexcept
				{
					length_ /= other.length();
					return *this;
				}

				template<class Rep2>
				constexpr line& operator%=(const line<Rep2>& other) noexcept
				{
					length_ %= other.length();
					return *this;
				}

				template<class Rep2>
				constexpr line& operator+=(const Rep2& other) noexcept
				{
					length_ += other;
					return *this;
				}

				template<class Rep2>
				constexpr line& operator-=(const Rep2& other) noexcept
				{
					length_ -= other;
					return *this;
				}

				template<class Rep2>
				constexpr line& operator/=(const Rep2& other) noexcept
				{
					length_ /= other;
					return *this;
				}

				template<class Rep2>
				constexpr line& operator%=(const Rep2& other) noexcept
				{
					length_ %= other;
					return *this;
				}

				template<class T>
				[[nodiscard]] constexpr operator T() const noexcept
				{
					return static_cast<T>(length_);
				}
			private:
				rep length_ {};
		};

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator==(const line<Rep>& lhs, const line<Rep2>& rhs) noexcept
		{
			return lhs.length() == rhs.length();
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator==(const line<Rep>& lhs, const Rep2& rhs) noexcept
		{
			return lhs.length() == rhs;
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator==(const Rep& lhs, const line<Rep2>& rhs) noexcept
		{
			return lhs == rhs.length();
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator!=(const line<Rep>& lhs, const line<Rep2>& rhs) noexcept
		{
			return !(lhs == rhs);
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator!=(const line<Rep>& lhs, const Rep2& rhs) noexcept
		{
			return !(lhs == rhs);
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator!=(const Rep& lhs, const line<Rep2>& rhs) noexcept
		{
			return !(lhs == rhs);
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator<(const line<Rep>& lhs, const line<Rep2>& rhs) noexcept
		{
			return lhs.length() < rhs.length();
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator<(const line<Rep>& lhs, const Rep2& rhs) noexcept
		{
			return lhs.length() < rhs;
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator<(const Rep& lhs, const line<Rep2>& rhs) noexcept
		{
			return lhs < rhs.length();
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator>(const line<Rep>& lhs, const line<Rep2>& rhs) noexcept
		{
			return rhs < lhs;
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator>(const line<Rep>& lhs, const Rep2& rhs) noexcept
		{
			return rhs < lhs;
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator>(const Rep& lhs, const line<Rep2>& rhs) noexcept
		{
			return rhs < lhs;
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator>=(const line<Rep>& lhs, const line<Rep2>& rhs) noexcept
		{
			return !(lhs < rhs);
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator>=(const line<Rep>& lhs, const Rep2& rhs) noexcept
		{
			return !(lhs < rhs);
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator>=(const Rep& lhs, const line<Rep2>& rhs) noexcept
		{
			return !(lhs < rhs);
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator<=(const line<Rep>& lhs, const line<Rep2>& rhs) noexcept
		{
			return !(rhs < lhs);
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator<=(const line<Rep>& lhs, const Rep2& rhs) noexcept
		{
			return !(rhs < lhs);
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool operator<=(const Rep& lhs, const line<Rep2>& rhs) noexcept
		{
			return !(rhs < lhs);
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr auto operator+(const line<Rep>& lhs, const line<Rep2>& rhs) noexcept
		{
			return line{lhs.length() + rhs.length()};
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr auto operator+(const line<Rep>& lhs, const Rep2& rhs) noexcept
		{
			return line{lhs.length() + rhs};
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr auto operator+(const Rep& lhs, const line<Rep2>& rhs) noexcept
		{
			return line{lhs + rhs.length()};
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr auto operator-(const line<Rep>& lhs, const line<Rep2>& rhs) noexcept
		{
			return line{lhs.length() - rhs.length()};
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr auto operator-(const line<Rep>& lhs, const Rep2& rhs) noexcept
		{
			return line{lhs.length() - rhs};
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr auto operator-(const Rep& lhs, const line<Rep2>& rhs) noexcept
		{
			return line{lhs - rhs.length()};
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr auto operator*(const line<Rep>& lhs, const line<Rep2>& rhs) noexcept
		{
			return line{lhs.length() * rhs.length()};
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr auto operator*(const line<Rep>& lhs, const Rep2& rhs) noexcept
		{
			return line{lhs.length() * rhs};
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr auto operator*(const Rep& lhs, const line<Rep2>& rhs) noexcept
		{
			return line{lhs * rhs.length()};
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr auto operator/(const line<Rep>& lhs, const line<Rep2>& rhs) noexcept
		{
			return line{lhs.length() / rhs.length()};
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr auto operator/(const line<Rep>& lhs, const Rep2& rhs) noexcept
		{
			return line{lhs.length() / rhs};
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr auto operator/(const Rep& lhs, const line<Rep2>& rhs) noexcept
		{
			return line{lhs / rhs.length()};
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr auto operator%(const line<Rep>& lhs, const line<Rep2>& rhs) noexcept
		{
			return line{lhs.length() % rhs.length()};
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr auto operator%(const line<Rep>& lhs, const Rep2& rhs) noexcept
		{
			return line{lhs.length() % rhs};
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr auto operator%(const Rep& lhs, const line<Rep2>& rhs) noexcept
		{
			return line{lhs % rhs.length()};
		}
	}
}