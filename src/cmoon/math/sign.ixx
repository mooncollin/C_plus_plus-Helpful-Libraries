export module cmoon.math.sign;

import <concepts>;

import cmoon.concepts;

namespace cmoon
{
	template<cmoon::arithmetic T>
	class signage
	{
		public:
			explicit constexpr signage(bool val) noexcept
				: is_positive{val} {}

			[[nodiscard]] constexpr operator T() const noexcept
			{
				return is_positive ? T{1} : T{-1};
			}

			template<std::convertible_to<T> T2>
			[[nodiscard]] explicit constexpr operator T2() const noexcept
			{
				return is_positive ? T2{1} : T2{-1};
			}

			[[nodiscard]] constexpr bool negative() const noexcept
			{
				return !is_positive;
			}

			[[nodiscard]] constexpr bool positive() const noexcept
			{
				return is_positive;
			}

			template<class T2>
			[[nodiscard]] friend constexpr bool operator==(const signage& lhs, const signage<T2>& rhs) noexcept
			{
				return lhs.positive() == rhs.positive();
			}

			template<class T2>
			[[nodiscard]] friend constexpr bool operator==(const signage& lhs, const signage<T2>& rhs) noexcept
			{
				return !(lhs == rhs);
			}
		private:
			bool is_positive;
	};

	template<cmoon::unsigned_arithmetic T>
	class signage<T>
	{
		public:
			explicit signage(bool) noexcept {}

			[[nodiscard]] constexpr operator T() const noexcept
			{
				return T{1};
			}

			template<std::convertible_to<T> T2>
			[[nodiscard]] explicit constexpr operator T2() const noexcept
			{
				return T2{1};
			}

			[[nodiscard]] constexpr bool negative() const noexcept
			{
				return false;
			}

			[[nodiscard]] constexpr bool positive() const noexcept
			{
				return true;
			}

			template<class T2>
			[[nodiscard]] friend constexpr bool operator==(const signage& lhs, const signage<T2>& rhs) noexcept
			{
				return lhs.positive() == rhs.postive();
			}

			template<class T2>
			[[nodiscard]] friend constexpr bool operator==(const signage& lhs, const signage<T2>& rhs) noexcept
			{
				return !(lhs == rhs);
			}
	};

	export
	template<cmoon::arithmetic T>
	inline constexpr signage<T> positive {true};

	export
	template<cmoon::signed_arithmetic T>
	inline constexpr signage<T> negative {false};

	export
	template<cmoon::arithmetic T>
	[[nodiscard]] constexpr signage<T> sign(const T& val) noexcept
	{
		if constexpr (cmoon::unsigned_arithmetic<T>)
		{
			return positive<T>;
		}
		else
		{
			return val < T{0} ? negative<T> : positive<T>;
		}
	}

	export
	template<cmoon::arithmetic T>
	[[nodiscard]] constexpr bool is_positive(const T& val) noexcept
	{
		return sign(val).positive();
	}

	export
	template<cmoon::arithmetic T>
	[[nodiscard]] constexpr bool is_negative(const T& val) noexcept
	{
		return sign(val).negative();
	}
}