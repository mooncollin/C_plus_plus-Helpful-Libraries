#pragma once

#include <cstddef>
#include <type_traits>
#include <algorithm>
#include <limits>
#include <memory>
#include <variant>
#include <array>
#include <vector>
#include <charconv>
#include <string_view>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <stdexcept>

#include "cmoon/multiprecision/multiprecision.hpp"
#include "cmoon/platform.hpp"

namespace cmoon
{
	namespace multiprecision
	{

		template<std::size_t MinBits = 0, signed_type SignType = signed_type::signed_magnitude, checked_type Checked = checked_type::unchecked, class Allocator = std::allocator<limb_type>>
		class big_int
		{
			template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
			friend class big_int;

			template<std::size_t min_precision, signed_type S>
			struct trivial_type_t
			{
				using type = void;
			};

			template<std::size_t min_precision, signed_type S>
				requires(cmoon::uint_trivial_type<min_precision>)
			struct trivial_type_t<min_precision, S>
			{
				using type = std::conditional_t<S == signed_type::signed_magnitude, 
									cmoon::least_bits_int<min_precision>,
									cmoon::least_bits_unsigned_int<min_precision>>;
			};

			public:
				static constexpr auto min_bits = MinBits;
				static constexpr auto sign_type = SignType;
				static constexpr auto check_type = Checked;
				static constexpr auto is_signed = sign_type == signed_type::signed_magnitude;
				static constexpr auto is_checked = check_type == checked_type::checked;

				using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<limb_type>;

				static constexpr auto fixed_precision = min_bits != 0;

				static_assert(fixed_precision ? true : !std::is_void_v<Allocator>);

				using trivial_type = typename trivial_type_t<min_bits, sign_type>::type;

				static constexpr auto trivial = !std::is_void_v<trivial_type>;

				constexpr big_int() requires(fixed_precision) = default;
				constexpr big_int(const big_int&) = default;
				constexpr big_int(big_int&&) noexcept = default;

				big_int(const allocator_type& a = allocator_type{})
					requires(!fixed_precision)
					: data_{a} {}
					
				
				template<std::integral I>
					requires(trivial)
				constexpr big_int(I val)
					: data_{val} {}

				template<std::integral I>
					requires(fixed_precision)
				constexpr big_int(I val)
				{
					assign_integer(val);
				}

				template<std::integral I>
				big_int(I val, const allocator_type& a = allocator_type{})
					requires(!fixed_precision)
					: data_{a}
				{
					assign_integer(val);
				}

				big_int(std::string_view s, int radix = 10)
				{
					assign_string(s, radix);
				}

				big_int(std::string_view s, int radix = 10, const Allocator& a = allocator_type{})
					requires(!fixed_precision)
					: data_{a}
				{
					assign_string(s, radix);
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(trivial && big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr big_int(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other)
					: data_{other.data_} {}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(trivial && big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr big_int(big_int<MinBits2, SignType2, Checked2, Allocator2>&& other)
					: data_{std::move(other.data_)} {}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
				constexpr big_int(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other)
				{
					capture(other);
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
				constexpr big_int(big_int<MinBits2, SignType2, Checked2, Allocator2>&& other)
				{
					using other_t = big_int<MinBits2, SignType2, Checked2, Allocator2>;

					capture(std::forward<other_t>(other));
				}

				constexpr big_int& operator=(const big_int&) = default;
				constexpr big_int& operator=(big_int&&) = default;

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(trivial && big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr big_int& operator=(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other)
				{
					data_ = other.data_;
					return *this;
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(trivial && big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr big_int& operator=(big_int<MinBits2, SignType2, Checked2, Allocator2>&& other)
				{
					data_ = std::move(other.data_);
					return *this;
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
				constexpr big_int& operator=(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other)
				{
					zero_out();
					capture(other);
					return *this;
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
				constexpr big_int& operator=(big_int<MinBits2, SignType2, Checked2, Allocator2>&& other)
				{
					using other_t = big_int<MinBits2, SignType2, Checked2, Allocator2>;

					zero_out();
					capture(std::forward<other_t>(other));
					return *this;
				}

				[[nodiscard]] constexpr big_int operator+() const noexcept
				{
					return *this;
				}

				[[nodiscard]] constexpr big_int operator-() const noexcept
				{
					auto result = *this;
					result.flip_sign();
					return result;
				}

				[[nodiscard]] constexpr bool positive() const noexcept
					requires(!is_signed)
				{
					return true;
				}

				[[nodiscard]] constexpr bool positive() const noexcept
					requires(is_signed)
				{
					if constexpr (trivial)
					{
						return data_ > 0;
					}
					else
					{
						return data_.positive_;
					}
				}

				template<std::integral I>
				constexpr big_int& operator+=(I val) noexcept(noexcept(add(val)) || noexcept(val.subtract(*this)) || noexcept(subtract(val)))
				{
					if (val < 0)
					{
						subtract(-val);
					}
					else if(!positive())
					{
						big_int placeholder{val};
						placeholder.subtract(*this);
						*this = std::move(placeholder);
					}
					else
					{
						add(val);
					}
					return *this;
				}
				
				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
				constexpr big_int& operator+=(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept(noexcept(add(other)) || noexcept(other.subtract(*this)) || noexcept(subtract(other)))
				{	
					if (!other.positive())
					{
						subtract(-other);
					}
					else if(!positive())
					{
						auto placeholder = other;
						placeholder.subtract(*this);
						*this = std::move(placeholder);
					}
					else
					{
						add(other);
					}
					return *this;
				}

				template<std::integral I>
				constexpr big_int& operator-=(I val) noexcept(noexcept(add(val)) || noexcept(val.subtract(*this)) || noexcept(subtract(val)))
				{
					if (val < 0)
					{
						add(-val);
					}
					else if (!positive())
					{
						add(val);
						flip_sign();
					}
					else if(*this < val)
					{
						big_int placeholder {val};
						placeholder.subtract(*this);
						placeholder.flip_sign();
						*this = std::move(placeholder);
					}
					else
					{
						subtract(val);
					}
					return *this;
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
				constexpr big_int& operator-=(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept(noexcept(add(other)) || noexcept(other.subtract(*this)) || noexcept(subtract(other)))
				{
					if (!other.positive())
					{
						add(-other);
					}
					else if (!positive())
					{
						add(other);
						flip_sign();
					}
					else if (*this < other)
					{
						auto placeholder = other;
						placeholder.subtract(*this);
						placeholder.flip_sign();
						*this = std::move(placeholder);
					}
					else
					{
						subtract(other);
					}
					return *this;
				}

				template<std::integral I>
				constexpr big_int& operator*=(I val) noexcept(noexcept(multiply(val)))
				{
					multiply(val);
					return *this;
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
				constexpr big_int& operator*=(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept(noexcept(multiply(other)))
				{
					multiply(other);
					return *this;
				}

				template<std::integral I>
				constexpr big_int& operator/=(I val) noexcept(noexcept(divide(val)))
				{
					divide(val);
					return *this;
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
				constexpr big_int& operator/=(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept(noexcept(divide(other)))
				{
					divide(other);
					return *this;
				}

				friend std::ostream& operator<<(std::ostream& os, const big_int& b)
				{
					if constexpr (trivial)
					{
						os << b.data_;
					}
					else
					{
						if (b.empty())
						{
							os << 0;
						}
						else
						{
							for (auto it = b.crbegin(); it != b.crend(); ++it)
							{
								if (*it == 0)
								{
									os << std::setfill('0') << std::setw(digits_per_block_10);
								}

								os << *it;
							}
						}
					}

					return os;
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
				[[nodiscard]] constexpr bool operator==(const big_int<MinBits2, SignType2, Checked2, Allocator2>& rhs) const noexcept
				{
					using other_t = big_int<MinBits2, SignType2, Checked2, Allocator2>;

					if constexpr (trivial)
					{
						if constexpr (other_t::trivial)
						{
							return data_ == rhs.data_;
						}
						else
						{
							return other_t{data_} == rhs;
						}
					}
					else
					{
						if constexpr (other_t::trivial)
						{
							return *this == big_int{rhs.data_};
						}
						else
						{
							return (positive() == rhs.positive()) && std::equal(cbegin(), cend(), rhs.cbegin(), rhs.cend());
						}
					}
				}

				template<std::integral I>
				[[nodiscard]] constexpr bool operator==(I rhs) const noexcept
				{
					return *this == big_int{rhs};
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
				[[nodiscard]] constexpr bool operator!=(const big_int<MinBits2, SignType2, Checked2, Allocator2>& rhs) const noexcept
				{
					return !(*this == rhs);
				}

				template<std::integral I>
				[[nodiscard]] constexpr bool operator!=(I rhs) const noexcept
				{
					return !(*this == rhs);
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
				[[nodiscard]] constexpr bool operator<(const big_int<MinBits2, SignType2, Checked2, Allocator2>& rhs) const noexcept
				{
					using other_t = big_int<MinBits2, SignType2, Checked2, Allocator2>;

					if constexpr (trivial)
					{
						if constexpr (other_t::trivial)
						{
							return data_ < rhs.data_;
						}
						else
						{
							return other_t{data_} < rhs;
						}
					}
					else
					{
						if constexpr (other_t::trivial)
						{
							return *this < big_int{rhs.data_};
						}
						else
						{
							if (positive() && !rhs.positive())
							{
								return true;
							}
							else if (!positive() && rhs.positive())
							{
								return false;
							}

							return std::lexicographical_compare(crbegin(), crend(), rhs.crbegin(), rhs.crend());
						}
					}
				}

				template<std::integral I>
				[[nodiscard]] constexpr bool operator<(I rhs) const noexcept
				{
					return *this < big_int{rhs};
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
				[[nodiscard]] constexpr bool operator>(const big_int<MinBits2, SignType2, Checked2, Allocator2>& rhs) const noexcept
				{
					return rhs < *this;
				}

				template<std::integral I>
				[[nodiscard]] constexpr bool operator>(I rhs) const noexcept
				{
					return big_int{rhs} < *this;
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
				[[nodiscard]] constexpr bool operator<=(const big_int<MinBits2, SignType2, Checked2, Allocator2>& rhs) const noexcept
				{
					return !(rhs < *this);
				}

				template<std::integral I>
				[[nodiscard]] constexpr bool operator<=(I rhs) const noexcept
				{
					return !(big_int{rhs} < *this);
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
				[[nodiscard]] constexpr bool operator>=(const big_int<MinBits2, SignType2, Checked2, Allocator2>& rhs) const noexcept
				{
					return !(*this < rhs);
				}

				template<std::integral I>
				[[nodiscard]] constexpr bool operator>=(I rhs) const noexcept
				{
					return !(*this < rhs);
				}

				constexpr void swap(big_int& other) noexcept
				{
					if (this != std::addressof(other))
					{
						if constexpr (trivial)
						{
							std::swap(data_, other.data_);
						}
						else
						{
							swap(data_, other.data_);
						}
					}
				}
			private:
				static constexpr auto internal_limb_count = fixed_precision ? (min_bits / bits_per_limb + ((min_bits % bits_per_limb) ? 1 : 0))
																			: 0;

				struct dynamic_unsigned_t
				{
					dynamic_unsigned_t(const allocator_type& a = allocator_type{})
						: data_{a} {}

					std::vector<limb_type, allocator_type> data_;

					void swap(dynamic_unsigned_t& other) noexcept
					{
						std::swap(data_, other.data_);
					}
				};

				void swap(dynamic_unsigned_t& lhs, dynamic_unsigned_t& rhs) noexcept
				{
					lhs.swap(rhs);
				}

				struct dynamic_signed_t
				{
					dynamic_signed_t(const allocator_type& a = allocator_type{})
						: data_{a} {}

					std::vector<limb_type, allocator_type> data_;
					bool positive_ {true};

					void swap(dynamic_signed_t& other) noexcept
					{
						std::swap(data_, other.data_);
						std::swap(positive_, other.positive_);
					}
				};

				void swap(dynamic_signed_t& lhs, dynamic_signed_t& rhs) noexcept
				{
					lhs.swap(rhs);
				}

				struct fixed_unsigned_t
				{
					std::array<limb_type, internal_limb_count> data_ {};
					std::size_t size_ {};

					constexpr void swap(fixed_unsigned_t& other) noexcept
					{
						std::swap(data_, other.data_);
						std::swap(size_, other.size_);
					}
				};

				constexpr void swap(fixed_unsigned_t& lhs, fixed_unsigned_t& rhs) noexcept
				{
					lhs.swap(rhs);
				}

				struct fixed_signed_t
				{
					std::array<limb_type, internal_limb_count> data_ {};
					std::size_t size_ {};
					bool positive_ {true};

					constexpr void swap(fixed_signed_t& other) noexcept
					{
						std::swap(data_, other.data_);
						std::swap(size_, other.size_);
						std::swap(positive_, other.positive_);
					}
				};

				constexpr void swap(fixed_signed_t& lhs, fixed_signed_t& rhs) noexcept
				{
					lhs.swap(rhs);
				}

				using data_type = std::conditional_t<trivial, trivial_type,
												std::conditional_t<fixed_precision,
													std::conditional_t<is_signed, fixed_signed_t, fixed_unsigned_t>,
													std::conditional_t<is_signed, dynamic_signed_t, dynamic_unsigned_t>>>;

				data_type data_ {};

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
				constexpr void capture(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept(fixed_precision && !is_checked)
				{
					using other_t = big_int<MinBits2, SignType2, Checked2, Allocator2>;
					
					if constexpr (trivial)
					{
						add(other);
					}
					else if constexpr (fixed_precision)
					{
						if constexpr (other_t::trivial)
						{
							assign_integer(other.data_);
						}
						else
						{
							data_.size_ = std::min(other.size(), data_.data_.size());
							std::copy_n(other.begin(), data_.size_, begin());
						}
					}
					else
					{
						if constexpr (other_t::trivial)
						{
							assign_integer(other.data_);
						}
						else
						{
							std::copy(other.begin(), other.end(), std::back_inserter(data_.data_));
						}
					}

					if (!!other.positive())
					{
						flip_sign();
					}
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
				constexpr void capture(big_int<MinBits2, SignType2, Checked2, Allocator2>&& other) noexcept(fixed_precision && !is_checked)
				{
					using other_t = big_int<MinBits2, SignType2, Checked2, Allocator2>;

					if constexpr (trivial)
					{
						add(std::forward<other_t>(other));
					}
					else if constexpr (fixed_precision)
					{
						if constexpr (other_t::trivial)
						{
							assign_integer(std::forward<decltype(other.data_)>(other.data_));
						}
						else
						{
							data_.size_ = std::min(other.size(), data_.data_.size());
							std::copy_n(other.begin(), data_.size_, begin());
						}
					}
					else
					{
						if constexpr (other_t::trivial)
						{
							assign_integer(std::forward<decltype(other.data_)>(other.data_));
						}
						else if constexpr(other_t::fixed_precision)
						{
							std::move(other.begin(), other.end(), std::back_inserter(data_.data_));
						}
						else
						{
							data_.data_ = std::move(other.data_.data_);
						}
					}

					if (!other.positive())
					{
						flip_sign();
					}
				}

				constexpr void flip_sign() noexcept
				{
					if constexpr (is_signed)
					{
						if constexpr (trivial)
						{
							data_ = -data_;
						}
						else
						{
							data_.positive_ = !data_.positive_;
						}
					}
				}

				[[nodiscard]] constexpr std::size_t size() const noexcept
				{
					if constexpr (trivial)
					{
						return 0;
					}
					else if constexpr(fixed_precision)
					{
						return data_.size_;
					}
					else
					{
						return data_.data_.size();
					}
				}

				[[nodiscard]] constexpr bool empty() const noexcept
				{
					return size() == 0;
				}

				constexpr void push_back(limb_type l) noexcept(fixed_precision && !is_checked)
					requires(!trivial)
				{
					if constexpr (fixed_precision)
					{
						if constexpr (is_checked)
						{
							if (data_.size_ >= data_.data_.size())
							{
								throw std::runtime_error{"Cannot expand container past size"};
							}
						}

						data_.data_[data_.size_++] = l;
					}
					else
					{
						data_.data_.push_back(l);
					}
				}

				constexpr void pop_back() noexcept(!is_checked)
					requires(!trivial)
				{
					if constexpr (fixed_precision)
					{
						if constexpr (is_checked)
						{
							if (data_.size_ == 0)
							{
								throw std::runtime_error{"Cannot shrink container"};
							}
						}
						--data_.size_;
					}
					else
					{
						if constexpr (is_checked)
						{
							if (data_.data_.size() == 0)
							{
								throw std::runtime_error{"Cannot shrink container"};
							}
						}

						data_.data_.pop_back();
					}
				}

				constexpr limb_type& back() noexcept
					requires(!trivial)
				{
					if constexpr (fixed_precision)
					{
						return data_.data_[data_.size_ - 1];
					}
					else
					{
						return data_.data_.back();
					}
				}

				constexpr const limb_type& back() const noexcept
					requires(!trivial)
				{
					if constexpr (fixed_precision)
					{
						return data_.data_[data_.size_ - 1];
					}
					else
					{
						return data_.data_.back();
					}
				}

				constexpr limb_type& front() noexcept
					requires(!trivial)
				{
					return data_.data_.front();
				}

				constexpr const limb_type& front() const noexcept
					requires(!trivial)
				{
					return data_.data_.front();
				}

				constexpr void remove_leading_zeros() noexcept
					requires(!trivial)
				{
					while (!empty() && back() == 0)
					{
						pop_back();
					}
				}

				void assign_string(std::string_view s, int radix) noexcept(fixed_precision && !is_checked)
				{
					if constexpr (trivial)
					{
						const auto r = std::from_chars(s.data(), s.data() + s.size(), data_, radix);
						if (r.ec != std::errc{})
						{
							throw std::invalid_argument{"Invalid string"};
						}
					}
					else
					{
						for (auto it = s.data() + s.size(); it > s.data(); it -= 9)
						{
							limb_type value;
							const auto start = std::distance(s.data(), it) < 9 ? s.data() : it - 9;
							const auto r = std::from_chars(start, it, value, radix);
							if (r.ec != std::errc{})
							{
								throw std::invalid_argument{"Invalid string"};
							}
							if constexpr (is_signed)
							{
								if (value < 0)
								{
									data_.positive_ = false;
									value = -value;
								}
							}

							push_back(value);
						}

						remove_leading_zeros();
					}
				}

				template<std::integral I>
				constexpr void assign_integer(I val) noexcept(fixed_precision && !is_checked)
				{
					if constexpr (is_signed)
					{
						if (val < 0)
						{
							data_.positive_ = false;
							val = -val;
						}
					}

					while (val >= max_block_10)
					{
						push_back(static_cast<limb_type>(val / max_block_10));
						val %= max_block_10;
					}

					if (val != 0)
					{
						push_back(static_cast<limb_type>(val));
					}
					std::reverse(begin(), end());
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(!trivial && !big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr void add(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept(fixed_precision && !is_checked)
				{
					limb_type carry {0};

					for(std::size_t i = 0; i < std::max(size(), other.size()) || carry; ++i)
					{
						if (i == size())
						{
							push_back(0);
						}

						data_.data_[i] += carry + (i < other.size() ? other.data_.data_[i] : 0);
						carry = data_.data_[i] >= max_block_10;
						if (carry)
						{
							data_.data_[i] -= max_block_10;
						}
					}

					if (carry)
					{
						push_back(1);
					}
				}

				template<std::integral I>
					requires(trivial)
				constexpr void add(I val) noexcept
				{
					data_ += val;
				}

				template<std::integral I>
					requires(!trivial)
				constexpr void add(I val) noexcept(fixed_precision && !is_checked)
				{
					big_int placeholder {val};
					add(placeholder);
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(trivial && big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr void add(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept
				{
					data_ += other.data_;
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(trivial && !big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr void add(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept
				{
					double_limb_type base = 1;
					for (auto& v : other.data_.data_)
					{
						data_ += v * base;
						base *= max_block_10;
					}
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(!trivial && big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr void add(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept(fixed_precision && !is_checked)
				{
					big_int placeholder {other.data_};
					add(placeholder);
				}

				template<std::integral I>
					requires(trivial)
				constexpr void subtract(I val) noexcept
				{
					data_ -= val;
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(trivial && big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr void subtract(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept
				{
					data_ -= other.data_;
				}

				template<std::integral I>
					requires(!trivial)
				constexpr void subtract(I val) noexcept(fixed_precision && !is_checked)
				{
					big_int placeholder {val};
					subtract(placeholder);
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(!trivial && big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr void subtract(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept(fixed_precision)
				{
					big_int placeholder {other.data_};
					subtract(placeholder);
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(trivial && !big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr void subtract(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept
				{
					double_limb_type base = 1;
					for(auto it = other.begin(); it != other.end(); ++it)
					{
						data_ -= *it * base;
						base *= max_block_10;
					}
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(!trivial && !big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr void subtract(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept(fixed_precision)
				{
					limb_type carry {0};

					for(std::size_t i = 0; i < other.size() || carry; ++i)
					{
						data_.data_[i] -= carry + (i < other.size() ? other.data_.data_[i] : 0);
						carry = data_.data_[i] < 0;
						if (carry)
						{
							data_.data_[i] += max_block_10;
						}
					}

					remove_leading_zeros();
				}

				template<std::integral I>
					requires(trivial)
				constexpr void multiply(I val) noexcept
				{
					data_ *= val;
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(trivial && big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr void multiply(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept
				{
					data_ *= other.data_;
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(!trivial && big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr void multiply(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept(fixed_precision && !is_checked)
				{
					multiply(other.data_);
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(trivial && !big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr void multiply(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept
				{
					double_limb_type base = 1;
					for(auto it = other.begin(); it != other.end(); ++it)
					{
						data_ *= *it * base;
						base *= max_block_10;
					}
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(!trivial && !big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr void multiply(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept(fixed_precision && is_checked)
				{
					const auto required_size = size() + other.size();
					big_int c;
					if (!c.resize(size() + other.size(), 0))
					{
						if constexpr(is_checked)
						{
							throw std::invalid_argument{ "Multiply would result in overflow" };
						}
						else
						{
							return;
						}
					}

					for (std::size_t i {0}; i < size(); ++i)
					{
						limb_type carry {0};
						for (std::size_t j {0}; j < other.size() || carry; ++j)
						{
							double_limb_type cur = c.data_.data_[i + j] + data_.data_[i] * static_cast<double_limb_type>(1) * (j < other.size() ? other.data_.data_[j] : 0) + carry;
							c.data_.data_[i + j] = static_cast<limb_type>(cur % max_block_10);
							carry = static_cast<limb_type>(cur / max_block_10);
						}
					}

					c.remove_leading_zeros();
					*this = std::move(c);
				}

				template<std::integral I>
					requires(!trivial)
				constexpr void multiply(I val) noexcept(fixed_precision && !is_checked)
				{
					if (val < max_block_10)
					{
						limb_type carry {0};
						for (std::size_t i {0}; i < size() || carry; ++i)
						{
							if (i == size())
							{
								push_back(0);
							}

							double_limb_type cur = carry + data_.data_[i] * static_cast<double_limb_type>(1) * val;
							data_.data_[i] = static_cast<limb_type>(cur % max_block_10);
							carry = static_cast<limb_type>(cur / max_block_10);
						}

						remove_leading_zeros();
					}
					else
					{
						big_int placeholder {val};
						multiply(placeholder);
					}
				}

				template<std::integral I>
					requires(trivial)
				constexpr void divide(I val) noexcept
				{
					data_ /= val;
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(trivial && big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr void divide(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept
				{
					data_ /= other.data_;
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(!trivial && big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr void divide(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept(fixed_precision && !is_checked)
				{
					divide(other.data_);
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(trivial && !big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr void divide(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept(!is_checked)
				{
					if(*this < other)
					{
						zero_out();
						return;
					}

					double_limb_type base = 1;
					for(auto it = other.begin(); it != other.end(); ++it)
					{
						data_ /= *it * base;
						base *= max_block_10;
					}
				}

				template<std::size_t MinBits2, signed_type SignType2, checked_type Checked2, class Allocator2>
					requires(!trivial && !big_int<MinBits2, SignType2, Checked2, Allocator2>::trivial)
				constexpr void divide(const big_int<MinBits2, SignType2, Checked2, Allocator2>& other) noexcept(fixed_precision && !is_checked)
				{
					if(*this < other)
					{
						zero_out();
						return;
					}

					// TODO
				}

				template<std::integral I>
					requires(!trivial)
				constexpr void divide(I val) noexcept(fixed_precision && !is_checked)
				{
					if(*this < val)
					{
						zero_out();
						return;
					}

					if (val < max_block_10)
					{
						limb_type carry {0};
						for (auto it = rbegin(); it != rend(); ++it)
						{
							double_limb_type cur = *it + carry * static_cast<double_limb_type>(1) * max_block_10;
							*it = static_cast<limb_type>(cur / val);
							carry = static_cast<limb_type>(cur % val);
						}

						remove_leading_zeros();
					}
					else
					{
						big_int placeholder {val};
						multiply(placeholder);
					}
				}

				constexpr void zero_out() noexcept
				{
					if constexpr (trivial)
					{
						data_ = 0;
					}
					else if constexpr (fixed_precision)
					{
						std::fill(begin(), end(), 0);
						data_.size_ = 0;
					}
					else
					{
						data_.data_.clear();
					}
				}

				constexpr bool resize(std::size_t s, limb_type value) noexcept(fixed_precision)
					requires(!trivial)
				{
					if constexpr (fixed_precision)
					{
						if (data_.data_.size() < s)
						{
							return false;
						}
						if (s > size())
						{
							std::fill(end() + s, data_.data_.end(), value);
						}
						else if(s < size())
						{
							std::fill(end() - s, end(), value);
						}

						data_.size_ = s;
					}
					else
					{
						data_.data_.resize(s, value);
					}

					return true;
				}

				[[nodiscard]] constexpr auto begin() noexcept
					requires(!trivial)
				{
					return std::begin(data_.data_);
				}

				[[nodiscard]] constexpr const auto begin() const noexcept
					requires(!trivial)
				{
					return std::begin(data_.data_);
				}

				[[nodiscard]] constexpr const auto cbegin() const noexcept
					requires(!trivial)
				{
					return std::cbegin(data_.data_);
				}

				[[nodiscard]] constexpr auto end() noexcept
					requires(!trivial)
				{
					if constexpr (fixed_precision)
					{
						return begin() + size();
					}
					else
					{
						return std::end(data_.data_);
					}
				}

				[[nodiscard]] constexpr const auto end() const noexcept
					requires(!trivial)
				{
					if constexpr (fixed_precision)
					{
						return begin() + size();
					}
					else
					{
						return std::end(data_.data_);
					}
				}

				[[nodiscard]] constexpr const auto cend() const noexcept
					requires(!trivial)
				{
					if constexpr (fixed_precision)
					{
						return cbegin() + size();
					}
					else
					{
						return std::cend(data_.data_);
					}
				}

				[[nodiscard]] constexpr auto rbegin() noexcept
					requires(!trivial)
				{
					if constexpr (fixed_precision)
					{
						return std::rbegin(data_.data_) + std::size(data_.data_) - size();
					}
					else
					{
						return std::rbegin(data_.data_);
					}
				}

				[[nodiscard]] constexpr const auto rbegin() const noexcept
					requires(!trivial)
				{
					if constexpr (fixed_precision)
					{
						return std::rbegin(data_.data_) + std::size(data_.data_) - size();
					}
					else
					{
						return std::rbegin(data_.data_);
					}
				}

				[[nodiscard]] constexpr const auto crbegin() const noexcept
					requires(!trivial)
				{
					if constexpr (fixed_precision)
					{
						return std::crbegin(data_.data_) + std::size(data_.data_) - size();
					}
					else
					{
						return std::crbegin(data_.data_);
					}
				}

				[[nodiscard]] constexpr auto rend() noexcept
					requires(!trivial)
				{
					return std::rend(data_.data_);
				}

				[[nodiscard]] constexpr const auto rend() const noexcept
					requires(!trivial)
				{
					return std::rend(data_.data_);
				}

				[[nodiscard]] constexpr const auto crend() const noexcept
					requires(!trivial)
				{
					return std::crend(data_.data_);
				}
		};

		template<std::size_t MinBits, signed_type SignType, checked_type Checked, class Allocator>
		constexpr void swap(big_int<MinBits, SignType, Checked, Allocator>& lhs, big_int<MinBits, SignType, Checked, Allocator>& rhs) noexcept
		{
			lhs.swap(rhs);
		}

		namespace details
		{
			template<class T, class T2>
			struct big_int_supertype;

			template<std::size_t MinBits, std::size_t MinBits2, signed_type SignType, signed_type SignType2, checked_type Checked, checked_type Checked2, class Allocator, class Allocator2>
			struct big_int_supertype<big_int<MinBits, SignType, Checked, Allocator>, big_int<MinBits2, SignType2, Checked2, Allocator2>>
			{
				using first = big_int<MinBits, SignType, Checked, Allocator>;
				using second = big_int<MinBits2, SignType2, Checked2, Allocator2>;

				static constexpr std::size_t bits_used = MinBits ? 
													MinBits2 ?
														std::max(MinBits, MinBits2)
															 : 0
														  : 0;

				static constexpr signed_type sign = (SignType == signed_type::signed_magnitude ||
									   SignType2 == signed_type::signed_magnitude) ? signed_type::signed_magnitude
																				   : signed_type::unsigned_magnitude;

				static constexpr checked_type checked = (Checked == checked_type::checked ||
										  Checked2 == checked_type::checked) ? checked_type::checked
																			 : checked_type::unchecked;

				using allocator_type = Allocator;

				using type = big_int<bits_used, sign, checked, allocator_type>;
									
			};

			template<std::size_t MinBits, signed_type SignType, checked_type Checked, class Allocator, std::integral I>
			struct big_int_supertype<big_int<MinBits, SignType, Checked, Allocator>, I>
			{
				using first = big_int<MinBits, SignType, Checked, Allocator>;
				using second = I;

				static constexpr std::size_t bits_used = MinBits ? std::max(MinBits, cmoon::bits_in_type<I>)
																 : 0;

				static constexpr signed_type sign = (SignType == signed_type::signed_magnitude ||
													 std::is_signed_v<I>) ? signed_type::signed_magnitude
																		  : signed_type::unsigned_magnitude;

				static constexpr checked_type checked = Checked;

				using allocator_type = Allocator;

				using type = big_int<bits_used, sign, checked, allocator_type>;
			};

			template<class T, class T2>
			using big_int_supertype_t = typename big_int_supertype<T, T2>::type;
		}

		template<std::size_t MinBits, std::size_t MinBits2, signed_type SignType, signed_type SignType2, checked_type Checked, checked_type Checked2, class Allocator, class Allocator2>
		[[nodiscard]] constexpr auto operator+(const big_int<MinBits, SignType, Checked, Allocator>& lhs, const big_int<MinBits2, SignType2, Checked2, Allocator2>& rhs)
		{
			using return_t = details::big_int_supertype_t<big_int<MinBits, SignType, Checked, Allocator>, big_int<MinBits2, SignType2, Checked2, Allocator2>>;

			return_t result {lhs};
			result += rhs;
			return result;
		}

		template<std::size_t MinBits, std::size_t MinBits2, signed_type SignType, signed_type SignType2, checked_type Checked, checked_type Checked2, class Allocator, class Allocator2>
		[[nodiscard]] constexpr auto operator-(const big_int<MinBits, SignType, Checked, Allocator>& lhs, const big_int<MinBits2, SignType2, Checked2, Allocator2>& rhs)
		{
			using return_t = details::big_int_supertype_t<big_int<MinBits, SignType, Checked, Allocator>, big_int<MinBits2, SignType2, Checked2, Allocator2>>;

			return_t result {lhs};
			result -= rhs;
			return result;
		}

		template<std::size_t MinBits, std::size_t MinBits2, signed_type SignType, signed_type SignType2, checked_type Checked, checked_type Checked2, class Allocator, class Allocator2>
		[[nodiscard]] constexpr auto operator*(const big_int<MinBits, SignType, Checked, Allocator>& lhs, const big_int<MinBits2, SignType2, Checked2, Allocator2>& rhs)
		{
			using return_t = details::big_int_supertype_t<big_int<MinBits, SignType, Checked, Allocator>, big_int<MinBits2, SignType2, Checked2, Allocator2>>;

			return_t result {lhs};
			result *= rhs;
			return result;
		}

		template<std::size_t MinBits, std::size_t MinBits2, signed_type SignType, signed_type SignType2, checked_type Checked, checked_type Checked2, class Allocator, class Allocator2>
		[[nodiscard]] constexpr auto operator/(const big_int<MinBits, SignType, Checked, Allocator>& lhs, const big_int<MinBits2, SignType2, Checked2, Allocator2>& rhs)
		{
			using return_t = details::big_int_supertype_t<big_int<MinBits, SignType, Checked, Allocator>, big_int<MinBits2, SignType2, Checked2, Allocator2>>;

			return_t result {lhs};
			result /= rhs;
			return result;
		}

		template<std::size_t MinBits, signed_type SignType, checked_type Checked, class Allocator, std::integral I>
		[[nodiscard]] constexpr auto operator+(const big_int<MinBits, SignType, Checked, Allocator>& lhs, I rhs)
		{
			using return_t = details::big_int_supertype_t<big_int<MinBits, SignType, Checked, Allocator>, I>;

			return_t result{lhs};
			result += rhs;
			return result;
		}

		template<std::size_t MinBits, signed_type SignType, checked_type Checked, class Allocator, std::integral I>
		[[nodiscard]] constexpr auto operator+(I lhs, const big_int<MinBits, SignType, Checked, Allocator>& rhs)
		{
			using return_t = details::big_int_supertype_t<big_int<MinBits, SignType, Checked, Allocator>, I>;

			return_t result{lhs};
			result += rhs;
			return result;
		}

		template<std::size_t MinBits, signed_type SignType, checked_type Checked, class Allocator, std::integral I>
		[[nodiscard]] constexpr auto operator-(const big_int<MinBits, SignType, Checked, Allocator>& lhs, I rhs)
		{
			using return_t = details::big_int_supertype_t<big_int<MinBits, SignType, Checked, Allocator>, I>;

			return_t result{lhs};
			result -= rhs;
			return result;
		}

		template<std::size_t MinBits, signed_type SignType, checked_type Checked, class Allocator, std::integral I>
		[[nodiscard]] constexpr auto operator-(I lhs, const big_int<MinBits, SignType, Checked, Allocator>& rhs)
		{
			using return_t = details::big_int_supertype_t<big_int<MinBits, SignType, Checked, Allocator>, I>;

			return_t result{lhs};
			result -= rhs;
			return result;
		}

		template<std::size_t MinBits, signed_type SignType, checked_type Checked, class Allocator, std::integral I>
		[[nodiscard]] constexpr auto operator*(const big_int<MinBits, SignType, Checked, Allocator>& lhs, I rhs)
		{
			using return_t = details::big_int_supertype_t<big_int<MinBits, SignType, Checked, Allocator>, I>;

			return_t result{lhs};
			result *= rhs;
			return result;
		}

		template<std::size_t MinBits, signed_type SignType, checked_type Checked, class Allocator, std::integral I>
		[[nodiscard]] constexpr auto operator*(I lhs, const big_int<MinBits, SignType, Checked, Allocator>& rhs)
		{
			using return_t = details::big_int_supertype_t<big_int<MinBits, SignType, Checked, Allocator>, I>;

			return_t result{lhs};
			result *= rhs;
			return result;
		}

		template<std::size_t MinBits, signed_type SignType, checked_type Checked, class Allocator, std::integral I>
		[[nodiscard]] constexpr auto operator/(const big_int<MinBits, SignType, Checked, Allocator>& lhs, I rhs)
		{
			using return_t = details::big_int_supertype_t<big_int<MinBits, SignType, Checked, Allocator>, I>;

			return_t result{lhs};
			result /= rhs;
			return result;
		}

		template<std::size_t MinBits, signed_type SignType, checked_type Checked, class Allocator, std::integral I>
		[[nodiscard]] constexpr auto operator/(I lhs, const big_int<MinBits, SignType, Checked, Allocator>& rhs)
		{
			using return_t = details::big_int_supertype_t<big_int<MinBits, SignType, Checked, Allocator>, I>;

			return_t result{lhs};
			result /= rhs;
			return result;
		}

		template<std::size_t MinBits = 0, checked_type Checked = checked_type::unchecked, class Allocator = std::allocator<limb_type>>
		[[nodiscard]] constexpr big_int<MinBits, signed_type::unsigned_magnitude, Checked, Allocator> factorial(std::size_t a)
		{
			big_int<MinBits, signed_type::unsigned_magnitude, Checked, Allocator> b{1u};

			for (std::size_t i {2}; i <= a; ++i)
			{
				b *= i;
			}

			return b;
		}
	}
}

namespace std
{
	template<std::size_t MinBits, cmoon::multiprecision::signed_type SignType, cmoon::multiprecision::checked_type Checked, class Allocator>
	constexpr void swap(cmoon::multiprecision::big_int<MinBits, SignType, Checked, Allocator>& lhs, cmoon::multiprecision::big_int<MinBits, SignType, Checked, Allocator>& rhs) noexcept
	{
		lhs.swap(rhs);
	}
}