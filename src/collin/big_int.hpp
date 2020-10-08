#pragma once

#include <memory>
#include <vector>
#include <limits>
#include <string_view>
#include <iostream>
#include <charconv>
#include <algorithm>
#include <iterator>

#include "collin/concepts.hpp"

namespace collin
{
	template<class Allocator = std::allocator<std::intmax_t>>
	class big_integer
	{
		template<collin::concepts::integral T>
		[[nodiscard]] static constexpr T power_of_10(T n) noexcept
		{
			T result {1};
			while (n)
			{
				result *= T{10};
				--n;
			}

			return result;
		}

		template<typename T>
		static constexpr auto max_value = power_of_10<T>(std::numeric_limits<T>::digits10) - 1;

		using rep = int;

		static constexpr auto digits_per_slot = 8;

		public:
			big_integer() = default;

			template<collin::concepts::integral I>
			big_integer(const I val)
			{
				assign_integer(val);
			}

			big_integer(std::string_view str, const int radix=10)
			{
				assign_string(str, radix);
			}

			big_integer& operator+=(const big_integer& other)
			{
				if (positive && other.positive)
				{
					add(other);
				}
				else if (positive && !other.positive)
				{
					auto placeholder{other};
					placeholder.positive = true;
					subtract(placeholder);
				}
				else
				{
					add(other);
					positive = false;
				}

				return *this;
			}

			big_integer operator+(const big_integer& other) const;
			
			big_integer& operator-=(const big_integer& other);
			big_integer operator-(const big_integer& other) const;

			big_integer& operator*=(const big_integer& other);
			big_integer operator*(const big_integer& other) const;

			[[nodiscard]] bool operator==(const big_integer& other) const noexcept
			{
				if (mag.size() != other.mag.size())
				{
					return false;
				}

				if (positive != other.positive)
				{
					return false;
				}

				return std::equal(mag.rbegin(), mag.rend(), other.mag.rbegin());
			}

			[[nodiscard]] bool operator!=(const big_integer& other) const noexcept
			{
				return !(*this == other);
			}

			[[nodiscard]] bool operator<(const big_integer& other) const noexcept
			{
				if(mag.size() != other.mag.size() || positive != other.positive)
				{
					return mag.size() * (positive - 0.5) < other.mag.size() * (other.positive - 0.5);
				}

				auto it = mag.rbegin();
				auto it2 = other.mag.rbegin();

				if (it == mag.rend() && it2 != other.mag.rbegin())
				{
					return true;
				}
				else if (it != mag.rend() && it2 == other.mag.rbegin())
				{
					return false;
				}

				while ((it + 1) != mag.rend() && (it2 + 1) != other.mag.rend() && *it == *it2)
				{
					++it;
					++it2;
				}

				return *it < *it2;
			}

			[[nodiscard]] bool operator<=(const big_integer& other) const noexcept
			{
				return !(other < *this);
			}

			[[nodiscard]] bool operator>(const big_integer& other) const noexcept
			{
				return other < *this;
			}

			[[nodiscard]] bool operator>=(const big_integer& other) const noexcept
			{
				return !(*this < other);
			}

			big_integer& operator++();
			big_integer operator++(int);

			big_integer& operator--();
			big_integer operator--(int);

			friend std::ostream& operator<<(std::ostream& os, const big_integer& obj)
			{
				if (!obj.positive)
				{
					os.put('-');
				}

				if (obj.mag.empty())
				{
					os.put('0');
				}

				std::copy(obj.mag.rbegin(), obj.mag.rend(), std::ostream_iterator<rep>{std::cout});

				return os;
			}
		private:
			std::vector<rep> mag;
			bool positive {true};

			void add(const big_integer& other)
			{
				bool carry {false};
				if (mag.size() < other.mag.size())
				{
					mag.resize(other.mag.size(), rep{0});
				}

				for (std::size_t i {0}; i < mag.size(); ++i)
				{
					const auto other_value = i < other.mag.size() ? other.mag[i] : 0;
					const auto sum = mag[i] + other_value + static_cast<rep>(carry);
					if (sum == mag[i])
					{
						break;
					}

					carry = sum >= max_value<rep>;
					mag[i] = carry ? sum - max_value<rep> + 1 : sum;
				}

				if (carry)
				{
					mag.push_back(1);
				}
			}

			void subtract(const big_integer& other)
			{
				if (other > *this)
				{
					big_integer placeholder{other};
					placeholder.subtract(*this);
					*this = placeholder;
					positive = false;
				}
				else
				{
					big_integer placeholder{*this};
					mag.clear();
				}
			}

			template<collin::concepts::integral I>
			void assign_integer(I val)
			{
				if (val < 0)
				{
					positive = false;
					val = -val;
				}

				while (val >= max_value<rep>)
				{
					mag.push_back(val / max_value<rep>);
					val %= max_value<rep>;
				}

				mag.push_back(val);
				std::reverse(mag.begin(), mag.end());
			}

			void assign_string(std::string_view str, const int radix)
			{
				constexpr auto stride = std::numeric_limits<rep>::digits10;
				const auto end = str.data() + str.size();

				// Would use iterators, but string_view iterators
				// are not const char*, and therefore cannot be used
				// with from_chars. There is not way to convert it.
				auto b = str.data();
				auto m = b;
				std::advance(m, str.size() % stride);
				if (b == m)
				{
					std::advance(m, stride);
				}

				while (true)
				{
					rep value;
					if (std::from_chars(b, m, value, radix).ec != std::errc{})
					{
						throw std::invalid_argument{"invalid string"};
					}
					mag.push_back(value);

					if (m == end)
					{
						break;
					}

					b = m;
					std::advance(m, stride);
				}

				std::reverse(mag.begin(), mag.end());
			}
	};
}