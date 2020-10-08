#pragma once

#include <cstddef>
#include <array>
#include <vector>
#include <memory>
#include <type_traits>
#include <algorithm>
#include <cmath>

#include "collin/multiprecision/multiprecision.hpp"
#include "collin/math.hpp"
#include "collin/platform.hpp"
#include "collin/algorithm.hpp"

namespace collin
{
	namespace multiprecision
	{
		template<std::size_t AmountDigits, checked_type Checked = checked_type::unchecked, class Allocator = std::allocator<limb_type>>
		class big_float
		{
			template<std::size_t AmountDigits2,checked_type Checked2, class Allocator2>
			friend class big_float;

			template<std::size_t Digits>
			struct trivial_type_t
			{
				using type = void;
			};

			template<std::size_t Digits>
				requires(collin::platform::float_trivial_type<Digits> && Digits != 0)
			struct trivial_type_t<Digits>
			{
				using type = collin::platform::least_digits_float<Digits>;
			};

			struct prevent_void_parameter {};

			public:
				static constexpr auto amount_digits = AmountDigits;
				static constexpr auto check_type = Checked;
				static constexpr auto is_checked = check_type == checked_type::checked;

				using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<limb_type>;

				static constexpr auto fixed_precision = amount_digits != 0;

				static_assert(fixed_precision ? true : !std::is_void_v<Allocator>);

				using trivial_type = typename trivial_type_t<amount_digits>::type;

				static constexpr auto trivial = !std::is_void_v<trivial_type>;

				[[nodiscard]] static constexpr big_float positive_zero() noexcept
				{
					return big_float{};
				}

				[[nodiscard]] static constexpr big_float negative_zero() noexcept
				{
					big_float f;
					if constexpr (trivial)
					{
						f.data_ = -0.0;
					}
					else
					{
						f.data_.positive_ = false;
					}
					return f;
				}

				[[nodiscard]] static constexpr big_float positive_infinity() noexcept(fixed_precision)
				{
					big_float f;
					f.set_infinity(true);
					return f;
				}

				[[nodiscard]] static constexpr big_float negative_infinity() noexcept(fixed_precision)
				{
					big_float f;
					f.set_infinity(false);
					return f;
				}

				[[nodiscard]] static constexpr big_float NaN() noexcept(fixed_precision)
				{
					big_float f;
					f.set_NaN();
					return f;
				}

				constexpr big_float() = default;
				constexpr big_float(const big_float&) = default;
				constexpr big_float(big_float&&) noexcept = default;

				constexpr big_float& operator=(const big_float&) = default;
				constexpr big_float& operator=(big_float&&) noexcept = default;

				template<collin::concepts::integral I>
				constexpr big_float(I val)
					requires(trivial)
					: data_{static_cast<trivial_type>(val)} {}

				constexpr big_float(std::conditional_t<std::is_void_v<trivial_type>, prevent_void_parameter, trivial_type> v)
					requires(trivial)
					: data_{v} {}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
					requires(trivial && big_float<AmountDigits2, Checked2, Allocator2>::trivial)
				constexpr big_float(const big_float<AmountDigits2, Checked2, Allocator2>& other)
					: data_{other.data_} {}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
					requires(!trivial)
				constexpr big_float(const big_float<AmountDigits2, Checked2, Allocator2>& other)
				{
					copy(other);
				}

				constexpr big_float(signed_double_limb_type val)
					requires(!trivial)
				{
					assign_integer(val);
				}

				template<collin::concepts::floating_point F>
					requires(!trivial)
				constexpr big_float(F val)
				{
					assign_float(val);
				}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
					requires(trivial && big_float<AmountDigits2, Checked2, Allocator2>::trivial)
				constexpr big_float& operator=(const big_float<AmountDigits2, Checked2, Allocator2>& other)
				{
					data_ = other.data_;
					return *this;
				}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
					requires(!trivial)
				constexpr big_float& operator=(const big_float<AmountDigits2, Checked2, Allocator2>& other)
				{
					copy(other);
					return *this;
				}

				[[nodiscard]] constexpr big_float operator+() const noexcept
				{
					return *this;
				}

				[[nodiscard]] constexpr big_float operator-() const noexcept
				{
					auto result = *this;
					result.flip_sign();
					return result;
				}

				[[nodiscard]] constexpr operator signed_double_limb_type() const noexcept
				{
					if constexpr (trivial)
					{
						return static_cast<signed_double_limb_type>(data_);
					}
					else
					{
						if (is_zero())
						{
							return 0;
						}

						signed_double_limb_type result {0};
						for (auto v : data_.data_)
						{
							result <<= log;
							result += v;
						}

						if (signed_double_limb_type shift = static_cast<signed_double_limb_type>(log) * (exponent() + 1 - static_cast<signed_double_limb_type>(size())); shift > 0)
						{
							result <<= shift;
						}
						else if (shift < 0)
						{
							result >>= -shift;
						}

						return positive() ? result : -result;
					}
				}

				template<collin::concepts::integral I>
				[[nodiscard]] constexpr operator I() const noexcept
				{
					return static_cast<I>(static_cast<signed_double_limb_type>(*this));
				}

				template<collin::concepts::floating_point F>
				[[nodiscard]] constexpr operator F() const noexcept
				{
					if constexpr (trivial)
					{
						return static_cast<F>(data_);
					}
					else
					{
						if (is_special())
						{
							if (is_zero())
							{
								return static_cast<F>(positive() ? 0.0 : -0.0);
							}
							else if (is_infinity())
							{
								return positive() ? std::numeric_limits<F>::infinity()
												  : -std::numeric_limits<F>::infinity();
							}
							else
							{
								return std::numeric_limits<F>::quiet_NaN();
							}
						}

						F x = 0.0;
						for (auto it = crbegin(); it != crend(); ++it)
						{
							x /= static_cast<double_limb_type>(1) << log;
							x += *it;
						}

						for (signed_double_limb_type i {0}; i < exponent(); ++i)
						{
							x *= static_cast<signed_double_limb_type>(1) << log;
						}
						for (signed_double_limb_type i {0}; i > exponent(); --i)
						{
							x /= static_cast<signed_double_limb_type>(1) << log;
						}

						return positive() ? x : -x;
					}
				}


				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
				constexpr big_float& operator+=(const big_float<AmountDigits2, Checked2, Allocator2>& other)
				{
					add_or_subtract(other, false);
					return *this;
				}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
				constexpr big_float& operator-=(const big_float<AmountDigits2, Checked2, Allocator2>& other)
				{
					add_or_subtract(other, true);
					return *this;
				}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
				constexpr big_float& operator*=(const big_float<AmountDigits2, Checked2, Allocator2>& other)
				{
					multiply(other);
					return *this;
				}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
				constexpr big_float& operator/=(const big_float<AmountDigits2, Checked2, Allocator2>& other)
				{
					divide(other);
					return *this;
				}

				template<collin::concepts::arithmetic A>
				constexpr big_float& operator+=(A other)
				{
					add_or_subtract(other, false);
					return *this;
				}

				template<collin::concepts::arithmetic A>
				constexpr big_float& operator-=(A other)
				{
					add_or_subtract(other, true);
					return *this;
				}

				template<collin::concepts::arithmetic A>
				constexpr big_float& operator*=(A other)
				{
					multiply(other);
					return *this;
				}

				template<collin::concepts::arithmetic A>
				constexpr big_float& operator/=(A other)
				{
					divide(other);
					return *this;
				}

				[[nodiscard]] constexpr signed_double_limb_type exponent() const noexcept
					requires(!trivial)
				{
					return data_.exponent_;
				}

				[[nodiscard]] constexpr bool positive() const noexcept
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

				[[nodiscard]] constexpr bool is_zero() const noexcept
				{
					if constexpr (trivial)
					{
						return data_ == 0.0 || data_ == -0.0;
					}
					else
					{
						return exponent() == zero_exponent &&
							   size() == 0;
					}
				}

				[[nodiscard]] constexpr bool is_positive_zero() const noexcept
				{
					if constexpr (trivial)
					{
						return data_ == 0.0;
					}
					else
					{
						return is_zero() && positive();
					}
				}

				[[nodiscard]] constexpr bool is_negative_zero() const noexcept
				{
					if constexpr (trivial)
					{
						return data_ == -0.0;
					}
					else
					{
						return is_zero() && !positive();
					}
				}

				[[nodiscard]] constexpr bool is_infinity() const noexcept
				{
					if constexpr (trivial)
					{
						return data_ == std::numeric_limits<trivial_type>::infinity() ||
							   data_ == -std::numeric_limits<trivial_type>::infinity();
					}
					else
					{
						return exponent() == zero_exponent &&
							   size() == 1;
					}
				}

				[[nodiscard]] constexpr bool is_positive_infinity() const noexcept
				{
					if constexpr (trivial)
					{
						return data_ == std::numeric_limits<trivial_type>::infinity();
					}
					else
					{
						return is_infinity() && positive();
					}
				}

				[[nodiscard]] constexpr bool is_negative_infinity() const noexcept
				{
					if constexpr (trivial)
					{
						return data_ == -std::numeric_limits<trivial_type>::infinity();
					}
					else
					{
						return is_infinity() && !positive();
					}
				}

				[[nodiscard]] constexpr bool is_NaN() const noexcept
				{
					if constexpr (trivial)
					{
						return std::isnan(data_);
					}
					else
					{
						return exponent() == zero_exponent &&
							   size() == 2;
					}
				}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
				[[nodiscard]] constexpr bool operator==(const big_float<AmountDigits2, Checked2, Allocator2>& rhs) const noexcept
				{
					using other_t = big_float<AmountDigits2, Checked2, Allocator2>;

					if constexpr (trivial && other_t::trivial)
					{
						return data_ == rhs.data_;
					}
					else if constexpr (trivial && !other_t::trivial)
					{
						return other_t{data_}.compare(rhs) == 0;
					}
					else if constexpr (!trivial && other_t::trivial)
					{
						return compare(big_int{rhs.data_}) == 0;
					}
					else
					{
						compare(rhs) == 0;
					}
				}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
				[[nodiscard]] constexpr bool operator!=(const big_float<AmountDigits2, Checked2, Allocator2>& rhs) const noexcept
				{
					return !(*this == rhs);
				}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
				[[nodiscard]] constexpr bool operator<(const big_float<AmountDigits2, Checked2, Allocator2>& rhs) const noexcept
				{
					using other_t = big_float<AmountDigits2, Checked2, Allocator2>;

					if constexpr (trivial && other_t::trivial)
					{
						return data_ < rhs.data_;
					}
					else if constexpr (trivial && !other_t::trivial)
					{
						return other_t{data_}.compare(rhs) == -1;
					}
					else if constexpr (!trivial && other_t::trivial)
					{
						return compare(big_int{rhs.data_}) == -1;
					}
					else
					{
						compare(rhs) == -1;
					}
				}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
				[[nodiscard]] constexpr bool operator>(const big_float<AmountDigits2, Checked2, Allocator2>& rhs) const noexcept
				{
					return rhs < *this;
				}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
				[[nodiscard]] constexpr bool operator<=(const big_float<AmountDigits2, Checked2, Allocator2>& rhs) const noexcept
				{
					return !(rhs < *this);
				}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
				[[nodiscard]] constexpr bool operator>=(const big_float<AmountDigits2, Checked2, Allocator2>& rhs) const noexcept
				{
					return !(*this < rhs);
				}

				template<collin::concepts::floating_point F>
				[[nodiscard]] constexpr bool operator==(F rhs) const noexcept
				{
					if constexpr (trivial)
					{
						return data_ == rhs;
					}
					else
					{
						return compare(big_float{rhs}) == 0;
					}
				}

				template<collin::concepts::floating_point F>
				[[nodiscard]] constexpr bool operator!=(F rhs) const noexcept
				{
					return !(*this == rhs);
				}

				template<collin::concepts::floating_point F>
				[[nodiscard]] constexpr bool operator<(F rhs) const noexcept
				{
					if constexpr (trivial)
					{
						return data_ < rhs;
					}
					else
					{
						return compare(big_float{rhs}) == -1;
					}
				}

				template<collin::concepts::floating_point F>
				[[nodiscard]] constexpr bool operator>(F rhs) const noexcept
				{
					return rhs < *this;
				}

				template<collin::concepts::floating_point F>
				[[nodiscard]] constexpr bool operator<=(F rhs) const noexcept
				{
					return !(rhs < *this);
				}

				template<collin::concepts::floating_point F>
				[[nodiscard]] constexpr bool operator>=(F rhs) const noexcept
				{
					return !(*this < rhs);
				}

				template<collin::concepts::floating_point F>
				[[nodiscard]] friend constexpr bool operator==(F lhs, const big_float& rhs) noexcept
				{
					if constexpr (trivial)
					{
						return lhs == rhs.data_;
					}
					else
					{
						return big_float{lhs}.compare(rhs) == 0;
					}
				}

				template<collin::concepts::floating_point F>
				[[nodiscard]] friend constexpr bool operator!=(F lhs, const big_float& rhs) noexcept
				{
					return !(lhs == rhs);
				}

				template<collin::concepts::floating_point F>
				[[nodiscard]] friend constexpr bool operator<(F lhs, const big_float& rhs) noexcept
				{
					if constexpr (trivial)
					{
						lhs < rhs.data_;
					}
					else
					{
						return big_float{lhs}.compare(rhs) == -1;
					}
				}

				template<collin::concepts::floating_point F>
				[[nodiscard]] friend constexpr bool operator>(F lhs, const big_float& rhs) noexcept
				{
					return rhs < lhs;
				}

				template<collin::concepts::floating_point F>
				[[nodiscard]] friend constexpr bool operator<=(F lhs, const big_float& rhs) noexcept
				{
					return !(rhs < lhs);
				}

				template<collin::concepts::floating_point F>
				[[nodiscard]] friend constexpr bool operator>=(F lhs, const big_float& rhs) noexcept
				{
					return !(lhs < rhs);
				}
			private:
				static constexpr auto zero_exponent = std::numeric_limits<signed_double_limb_type>::min();

				static constexpr auto min_exponent = zero_exponent + static_cast<signed_double_limb_type>(AmountDigits);
				static constexpr auto max_exponent = std::numeric_limits<signed_double_limb_type>::max() - 1;

				static constexpr auto log = bits_per_limb;
				static constexpr auto range = static_cast<signed_double_limb_type>(std::numeric_limits<limb_type>::max()) + 1;
				
				struct dynamic_t
				{
					dynamic_t(const allocator_type& a = allocator_type{})
						: data_{a} {}

					std::vector<limb_type> data_;
					signed_double_limb_type exponent_ {zero_exponent};
					bool positive_{true};

					void swap(dynamic_t& other) noexcept
					{
						std::swap(data_, other.data_);
						std::swap(exponent_, other.exponent_);
						std::swap(positive_, other.positive_);
					}
				};

				struct fixed_t
				{
					std::array<limb_type, amount_digits> data_ {};
					std::size_t size_ {};
					signed_double_limb_type exponent_ {zero_exponent};
					bool positive_{true};

					void swap(fixed_t& other) noexcept
					{
						std::swap(data_, other.data_);
						std::swap(size_, other.size_);
						std::swap(exponent_, other.exponent_);
						std::swap(positive_, other.positive_);
					}
				};

				using data_type = std::conditional_t<trivial, trivial_type,
									std::conditional_t<fixed_precision, fixed_t, dynamic_t>>;

				
				data_type data_ {};

				constexpr void set_zero(bool positive) noexcept
				{
					if constexpr (trivial)
					{
						data_ = static_cast<trivial_type>(positive ? 0.0 : -0.0);
					}
					else
					{
						clear();
						data_.exponent_ = zero_exponent;
						data_.positive_ = positive;
					}
				}

				constexpr void set_infinity(bool positive) noexcept
				{
					if constexpr (trivial)
					{
						data_ = positive ? std::numeric_limits<trivial_type>::infinity()
										 : -std::numeric_limits<trivial_type>::infinity();
					}
					else
					{
						data_.exponent_ = zero_exponent;
						data_.positive_ = positive;
						resize(1);
					}
				}

				constexpr void set_NaN() noexcept
				{
					if constexpr (trivial)
					{
						data_ = std::numeric_limits<trivial_type>::quiet_NaN();
					}
					else
					{
						data_.exponent_ = zero_exponent;
						data_.positive_ = true;
						resize(2);
					}
				}

				constexpr void clear() noexcept
				{
					if constexpr (trivial)
					{
						data_ = static_cast<trivial_type>(0.0L);
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

				constexpr void flip_sign() noexcept
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

				[[nodiscard]] constexpr std::size_t size() const noexcept
				{
					if constexpr (trivial)
					{
						return 0;
					}
					else if constexpr (fixed_precision)
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

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
				constexpr void copy(const big_float<AmountDigits2, Checked2, Allocator2>& other) noexcept(fixed_precision)
				{
					using other_t = big_float<AmountDigits2, Checked2, Allocator2>;

					if constexpr (trivial && other_t::trivial)
					{
						data_ = other.data_;
					}
					else
					{
						data_.exponent_ = other.exponent();
						data_.positive_ = other.positive();
						if constexpr (fixed_precision)
						{
							resize(std::min(data_.data_.size(), other.size()));
							std::copy_n(std::begin(other.data_.data_), size(), std::begin(data_.data_));
						}
						else
						{
							data_.data_ = other.data_.data_;
						}
					}
				}

				constexpr void assign_integer(signed_double_limb_type val, signed_double_limb_type exponent = 0) noexcept(fixed_precision && !is_checked)
				{
					if constexpr (trivial)
					{
						data_ = static_cast<decltype(data_)>(val);
					}
					else
					{
						if (val == 0)
						{
							set_zero(true);
						}

						data_.positive_ = val > 0;

						auto real_val = static_cast<signed_double_limb_type>(positive() ? val : -val);

						constexpr signed_double_limb_type shift = 64 - log;
						constexpr signed_double_limb_type mask = (range - 1) << shift;

						data_.exponent_ = shift / log;
						while (!(real_val & mask))
						{
							--data_.exponent_;
							real_val <<= log;
						}

						clear();
						data_.exponent_ += exponent;
						while (real_val)
						{
							if constexpr (fixed_precision)
							{
								if (size() >= amount_digits)
								{
									break;
								}
							}

							push_back(static_cast<limb_type>((real_val & mask) >> shift));
							real_val <<= log;
						}
					}
				}

				template<collin::concepts::floating_point F>
				constexpr void assign_float(F val)
				{
					if constexpr (trivial)
					{
						data_ = static_cast<trivial_type>(val);
					}
					else
					{
						constexpr F p_zero = 0.0;
						constexpr F n_zero = -0.0;
						constexpr F p_infinity = std::numeric_limits<F>::infinity();
						constexpr F n_infinity = -std::numeric_limits<F>::infinity();

						if (val == p_zero)
						{
							set_zero(true);
						}
						else if (val == n_zero)
						{
							set_zero(false);
						}
						else if (val == p_infinity)
						{
							set_infinity(true);
						}
						else if (val == n_infinity)
						{
							set_infinity(false);
						}
						else if (std::isnan(val))
						{
							set_NaN();
						}
						else
						{
							collin::math::basic_rational<signed_double_limb_type> r{collin::math::abs(val)};
							assign_integer(r.numerator());
							divide(big_float{r.denominator()});
							data_.positive_ = val > 0;
						}
					}
				}

				constexpr void push_back(limb_type l) noexcept(fixed_precision && !is_checked)
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

				constexpr bool resize(std::size_t s, limb_type value = 0) noexcept(fixed_precision)
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
							std::fill(end() + (s - size()), data_.data_.end(), value);
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

				[[nodiscard]] constexpr bool is_special() const noexcept
					requires(!trivial)
				{
					return exponent() == zero_exponent;
				}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
				[[nodiscard]] constexpr int compare(const big_float<AmountDigits2, Checked2, Allocator2>& rhs) const noexcept
					requires(!trivial && !big_float<AmountDigits2, Checked2, Allocator2>::trivial)
				{
					if (positive() != rhs.positive())
					{
						return positive() ? 1 : -1;
					}

					if (!positive())
					{
						return -compare_absolute(rhs);
					}

					return compare_absolute(rhs);
				}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
				[[nodiscard]] constexpr int compare_absolute(const big_float<AmountDigits2, Checked2, Allocator2>& rhs) const noexcept
					requires(!trivial && !big_float<AmountDigits2, Checked2, Allocator2>::trivial)
				{
					if (is_special() || rhs.is_special())
					{
						if (is_infinity())
						{
							return rhs.is_infinity() ? 0 : 1;
						}
						else if (rhs.is_infinity())
						{
							return -1;
						}
						else if (is_zero())
						{
							return rhs.is_zero() ? 0 : -1;
						}
						else if (rhs.is_zero())
						{
							return 1;
						}
						else
						{
							return -1;
						}
					}
					else if (exponent() > rhs.exponent())
					{
						return 1;
					}
					else if (exponent() < rhs.exponent())
					{
						return -1;
					}

					return collin::algorithms::sequence_compare(begin(), end(), rhs.begin(), rhs.end());
				}

				constexpr void round(bool carry, signed_double_limb_type previous_digit)
					requires(!trivial && fixed_precision)
				{
					typename decltype(data_.data_)::reverse_iterator pos;
					const auto reverse_end = data_.data_.rend();
					bool round;

					if (-previous_digit > range / 2)
					{
						pos = data_.data_.rbegin();
						round = true;
						previous_digit += range;
						while (round && pos != reverse_end)
						{
							signed_double_limb_type sum = static_cast<signed_double_limb_type>(*pos) - round;
							round = sum < 0;
							if (round)
							{
								sum += range;
							}

							*pos = static_cast<limb_type>(sum);
							++pos;
						}
						if (round)
						{
							carry = false;
						}
					}

					if (carry)
					{
						round = false;
						if (size() == amount_digits)
						{
							pos = data_.data_.rbegin();
							round = (*pos > range / 2 ||
									 *pos == range / 2 && previous_digit >= 0);
							++pos;
							while (round && pos != reverse_end)
							{
								signed_double_limb_type sum = static_cast<signed_double_limb_type>(*pos) + round;
								round = sum >= range;
								if (round)
								{
									sum -= range;
								}
								*pos = static_cast<limb_type>(sum);
								++pos;
							}
						}
						else
						{
							++data_.size_;
						}

						std::copy(rbegin() + 1, rend() - 1, rbegin());
						front() = static_cast<double_limb_type>(carry) + static_cast<double_limb_type>(round);
						if (exponent() == max_exponent)
						{
							set_infinity(positive());
						}
						++data_.exponent_;
					}
					else if (previous_digit >= range / 2)
					{
						round = true;
						pos = data_.data_.rbegin();
						while (round && pos != reverse_end)
						{
							double_limb_type sum = static_cast<double_limb_type>(*pos) + round;
							round = sum >= range;
							if (round)
							{
								sum -= range;
							}
							*pos = static_cast<limb_type>(sum);
							++pos;
						}
						if (round)
						{
							resize(1);
							front() = 1;
							if (exponent() == max_exponent)
							{
								set_infinity(positive());
							}

							++data_.exponent_;
						}
					}

					while (size() && back() == 0)
					{
						resize(size() - 1);
						if (size() == 0)
						{
							set_zero(positive());
						}
					}

					if (exponent() > min_exponent)
					{
						set_zero(positive());
					}
					else if (exponent() > max_exponent)
					{
						set_infinity(positive());
					}
				}

				constexpr void round(double_limb_type previous_digit)
					requires(!trivial && fixed_precision)
				{
					typename decltype(data_.data_)::reverse_iterator pos;
					const auto reverse_end = data_.data_.rend();
					bool round;

					if (previous_digit >= range / 2)
					{
						round = true;
						pos = data_.data_.rbegin();
						while (round && pos != reverse_end)
						{
							double_limb_type sum = static_cast<double_limb_type>(*pos) + round;
							round = sum >= range;
							if (round)
							{
								sum -= range;
							}
							*pos = static_cast<limb_type>(sum);
							++pos;
						}
						if (round)
						{
							resize(1);
							front() = 1;
							if (exponent() == max_exponent)
							{
								set_infinity(positive());
								return;
							}
							++data_.exponent_;
						}
					}

					while (size() && back() == 0)
					{
						resize(size() - 1);
						if (size() == 0)
						{
							set_zero(positive());
						}
					}

					if (exponent() < min_exponent)
					{
						set_zero(positive());
					}
					else if (exponent() > max_exponent)
					{
						set_infinity(positive());
					}
				}

				template<collin::concepts::arithmetic A>
				constexpr void add_or_subtract(A other, bool minus) noexcept(trivial)
				{
					if constexpr (trivial)
					{
						if (minus)
						{
							data_ -= other;
						}
						else
						{
							data_ += other;
						}
					}
					else
					{
						add_or_subtract(big_float{other}, minus);
					}
				}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
				constexpr void add_or_subtract(const big_float<AmountDigits2, Checked2, Allocator2>& other, bool minus)
				{
					using other_t = big_float<AmountDigits2, Checked2, Allocator2>;

					if constexpr (trivial && other_t::trivial)
					{
						if (minus)
						{
							data_ -= other.data_;
						}
						else
						{
							data_ += other.data_;
						}
					}
					else if constexpr (trivial && !other_t::trivial)
					{
						other_t result{data_};
						result.add_or_subtract(other, minus);
						data_ = static_cast<trivial_type>(result);
					}
					else if constexpr (!trivial && other_t::trivial)
					{
						add_or_subtract(big_float{other.data_}, minus);
					}
					else
					{
						if (is_special() || other.is_special())
						{
							if (other.is_zero())
							{
								// Do Nothing
							}
							else if (is_zero())
							{
								*this = minus ? -other : other;
							
							}
							else if (is_NaN() || other.is_NaN())
							{
								set_NaN();
							}
							else if (positive_infinity())
							{
								if (other.is_negative_infinity())
								{
									set_NaN();
								}
							}
							else if (is_negative_infinity())
							{
								if (other.positive_infinity())
								{
									set_NaN();
								}
							}
							else if (other.positive_infinity())
							{
								set_infinity(true);
							}
							else if (other.is_negative_infinity())
							{
								set_infinity(false);
							}

							return;
						}

						if (compare_absolute(other) < 0)
						{
							add_or_subtract_helper(other, *this, other.positive() != minus, positive());
						}
						else
						{
							add_or_subtract_helper(*this, other, positive(), other.positive() != minus);
						}
					}
				}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2,
						 std::size_t AmountDigits3, checked_type Checked3, class Allocator3>
						 requires(!trivial)
				constexpr void add_or_subtract_helper(const big_float<AmountDigits2, Checked2, Allocator2>& a, const big_float<AmountDigits3, Checked3, Allocator3>& b, bool sign_a, bool sign_b)
				{
					auto add = sign_a == sign_b;
					data_.positive_ = sign_a;

					signed_double_limb_type delta = a.exponent() - b.exponent();
					if constexpr (fixed_precision)
					{
						if (delta > amount_digits + 1)
						{
							*this = a;
							data_.positive_ = sign_a;
							return;
						}
					}

					signed_double_limb_type previous_digit {0};
					data_.exponent_ = a.exponent();

					auto carry = false;
					auto shift = false;

					signed_double_limb_type pos;
					signed_double_limb_type least_a = a.size();
					signed_double_limb_type least_b = b.size() + delta;

					if (least_a > least_b)
					{
						pos = least_a - 1;
						resize(least_a);

						while (pos >= least_b)
						{
							(*this)[pos] = a[pos];
							--pos;
						}
					}
					else
					{
						if (!add && least_b > amount_digits && a.front() == 1)
						{
							shift = true;
							--data_.exponent_;
							--least_b;
							--least_a;
							--delta;
						}

						const signed_double_limb_type least_b_used = amount_digits ? std::min(least_b, static_cast<signed_double_limb_type>(amount_digits)) : least_b;

						pos = least_b_used - 1;
						resize(least_b_used);

						if (pos >= least_a)
						{
							const signed_double_limb_type stop_b = std::max(delta, least_a);
							if (add)
							{
								while (pos >= stop_b)
								{
									(*this)[pos] = b[pos - delta] + carry;
									if (carry && (*this)[pos] != 0)
									{
										carry = false;
									}
									--pos;
								}
							}
							else
							{
								while (pos >= stop_b)
								{
									(*this)[pos] = static_cast<limb_type>(range - b[pos - delta] - carry);
									carry = true;
									--pos;
								}
							}

							if (stop_b > least_a)
							{
								if (carry)
								{
									if (add)
									{
										while (pos >= least_a)
										{
											(*this)[pos] = carry;
											carry = false;
											--pos;
										}
									}
									else
									{
										while (pos >= least_a)
										{
											(*this)[pos] = range - 1;
											--pos;
										}
									}
								}
								else
								{
									while (pos >= least_a)
									{
										(*this)[pos] = 0;
										--pos;
									}
								}
							}
						}
					}

					if (add)
					{
						while (pos - delta >= 0)
						{
							signed_double_limb_type sum = static_cast<signed_double_limb_type>(a[pos]) + static_cast<signed_double_limb_type>(b[pos - delta]) + carry;
							carry = sum >= range;
							if (carry)
							{
								sum -= range;
							}

							(*this)[pos] = static_cast<limb_type>(sum);
							--pos;
						}

						while (pos >= 0)
						{
							signed_double_limb_type sum = static_cast<signed_double_limb_type>(a[pos]) + carry;
							carry = sum >= range;
							if (carry)
							{
								sum -= range;
							}
							(*this)[pos] = static_cast<limb_type>(sum);
							--pos;
						}

						if constexpr (fixed_precision)
						{
							if (least_b >= amount_digits + 1)
							{
								previous_digit = b[amount_digits - delta];
							}
						}
					}
					else
					{
						while (pos - delta >= 0)
						{
							signed_double_limb_type sum = static_cast<signed_double_limb_type>(a[pos + shift]) - static_cast<signed_double_limb_type>(b[pos - delta]) - carry;
							carry = sum < 0;
							if (carry)
							{
								sum += range;
							}
							(*this)[pos] = static_cast<limb_type>(sum);
							--pos;
						}

						while (pos >= 0)
						{
							signed_double_limb_type sum = static_cast<signed_double_limb_type>(a[pos + shift]) - carry;
							carry = sum < 0;
							if (carry)
							{
								sum += range;
							}
							(*this)[pos] = static_cast<limb_type>(sum);
							--pos;
						}
						
						if constexpr (fixed_precision)
						{
							if (least_b >= amount_digits + 1)
							{
								previous_digit = -static_cast<signed_double_limb_type>(b[amount_digits - delta]);
								if (least_b > amount_digits + 1)
								{
									--previous_digit;
								}
							}
						}
						

						if (shift)
						{
							carry = !carry;
						}

						if (!carry)
						{
							pos = 0;
							for (; pos < size(); ++pos)
							{
								if ((*this)[pos] > 0)
								{
									break;
								}
							}

							const auto adjust = pos;

							if (adjust == size() || exponent() - adjust < -max_exponent)
							{
								set_zero(positive());
								return;
							}
							else if (adjust > 0)
							{
								data_.exponent_ -= adjust;
								resize(size() - adjust);

								// TODO: This looks dangerous
								for (pos = 0; pos < size(); ++pos)
								{
									(*this)[pos] = (*this)[pos + adjust];
								}
							}
						}
					}

					// TODO: Do a round with the carry and previous_digit
					if constexpr (fixed_precision)
					{
						round(carry, previous_digit);
					}
				}

				template<collin::concepts::arithmetic A>
				constexpr void multiply(A other) noexcept(trivial)
				{
					if constexpr (trivial)
					{
						data_ *= other;
					}
					else
					{
						multiply(big_float{other});
					}
				}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
				constexpr void multiply(const big_float<AmountDigits2, Checked2, Allocator2>& other)
				{
					using other_t = big_float<AmountDigits2, Checked2, Allocator2>;

					if constexpr (trivial && other_t::trivial)
					{
						data_ *= other.data_;
					}
					else if constexpr (trivial && !other_t::trivial)
					{
						other_t result{data_};
						result.multiply(other);
						data_ = static_cast<trivial_type>(result);
					}
					else if constexpr (!trivial && other_t::trivial)
					{
						multiply(big_float{other.data_});
					}
					else
					{
						auto exponent = this->exponent() + other.exponent() + 1;
						data_.positive_ = positive() == other.positive();

						if (is_special() || other.is_special())
						{
							if (is_NaN() || other.is_NaN())
							{
								set_NaN();
							}
							else if (is_zero() || other.is_zero())
							{
								set_zero(positive());
							}
							else
							{
								set_infinity(positive());
							}

							return;
						}
						else if (exponent < min_exponent)
						{
							set_zero(positive());
							return;
						}
						else if (exponent > max_exponent)
						{
							set_infinity(positive());
							return;
						}

						auto length = size() + other.size();
						if constexpr (fixed_precision)
						{
							if constexpr (is_checked)
							{
								if (length > amount_digits)
								{
									throw std::runtime_error{"Multiply cannot exceed past storage"};
								}
							}
						}

						std::conditional_t<fixed_precision,
							std::array<double_limb_type, 2 * amount_digits>,
							std::vector<double_limb_type>> temp;

						if constexpr (fixed_precision)
						{
							std::fill(std::begin(temp), std::end(temp), 0);
						}
						else
						{
							temp.resize(2 * length, 0);
						}

						for (std::size_t i {size()}; i--;)
						{
							for (std::size_t j {std::min(other.size(), length - i)}; j--;)
							{
								const auto result = static_cast<double_limb_type>((*this)[i]) * static_cast<double_limb_type>(other[j]);
								temp[i + j + 1] += result & (range - 1);
								temp[i + j] += result >> log;
							}
						}

						for (std::size_t i {length}; i-- > 1;)
						{
							temp[i - 1] += temp[i] >> log;
							temp[i] &= range - 1;
						}

						auto result = temp.data();
						if (temp[0] == 0)
						{
							result = std::addressof(temp[1]);
							--exponent;
							--length;
						}

						double_limb_type previous_digit {0};
						if constexpr (fixed_precision)
						{
							if (length > amount_digits)
							{
								length = amount_digits;
								previous_digit = result[length];
							}
						}

						for (std::size_t i {0}; i < length; ++i)
						{
							if constexpr (!fixed_precision)
							{
								if (i == size())
								{
									push_back(0);
								}
							}

							(*this)[i] = static_cast<limb_type>(result[i]);
						}

						if constexpr (fixed_precision)
						{
							resize(length);
						}

						data_.exponent_ = exponent;

						if constexpr (fixed_precision)
						{
							round(previous_digit);
						}
					}
				}

				template<collin::concepts::arithmetic A>
				constexpr void divide(A other) noexcept(trivial)
				{
					if constexpr (trivial)
					{
						data_ /= other;
					}
					else
					{
						divide(big_float{other});
					}
				}

				template<std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
				constexpr void divide(const big_float<AmountDigits2, Checked2, Allocator2>& other)
				{
					using other_t = big_float<AmountDigits2, Checked2, Allocator2>;

					if constexpr (trivial && other_t::trivial)
					{
						data_ /= other.data_;
					}
					else if constexpr (trivial && !other_t::trivial)
					{
						other_t result{data_};
						result.divide(other);
						data_ = static_cast<trivial_type>(result);
					}
					else if constexpr (!trivial && other_t::trivial)
					{
						divide(big_float{other.data_});
					}
					else
					{
						auto exponent = this->exponent() - other.exponent();
						data_.positive_ = positive() == other.positive();

						if (is_special() || other.is_special())
						{
							if (is_NaN() || other.is_NaN() || other.is_zero())
							{
								set_NaN();
							}
							else if (is_zero() || other.is_infinity())
							{
								set_zero(positive());
							}
							else
							{
								set_infinity(positive());
							}

							return;
						}
						else if (exponent < min_exponent)
						{
							set_zero(positive());
							return;
						}
						else if (exponent > max_exponent)
						{
							set_infinity(positive());
							return;
						}

						if constexpr (is_checked)
						{
							if (size() == 0)
							{
								throw std::runtime_error{"Cannot divide in current state"};
							}
						}

						constexpr std::size_t limit = 2 * (amount_digits + 2);
						constexpr std::size_t numerator_limit = 2 * limit + 2;

						const std::size_t v_limit = fixed_precision ? limit : 2 * (std::max(size(), other.size()) + 2);
						const std::size_t v_num_limit = fixed_precision ? numerator_limit : 2 * v_limit + 2;

						std::conditional_t<fixed_precision, 
							std::array<signed_double_limb_type, numerator_limit>,
							std::vector<signed_double_limb_type>> t;

						if constexpr (!fixed_precision)
						{
							t.resize(v_num_limit);
						}

						for (std::size_t i {0}; i < size(); ++i)
						{
							t[2 * i] = (*this)[i] >> (log / 2);
							t[2 * i + 1] = (*this)[i] & ((static_cast<signed_double_limb_type>(1) << (log / 2)) - 1);
						}

						if constexpr (fixed_precision)
						{
							std::fill(std::begin(t) + (2 * size()), std::end(t), 0);
						}
						
						std::conditional_t<fixed_precision,
							std::array<signed_double_limb_type, limit>,
							std::vector<signed_double_limb_type>> d;

						if constexpr (!fixed_precision)
						{
							d.resize(v_limit);
						}

						signed_double_limb_type ad = other[0];
						auto length = 2 * other.size();
						signed_double_limb_type shift = (ad < (1 << (log / 2))) ? 1 : 0;
						if (shift)
						{
							--length;
							d[0] = ad;
							ad <<= log / 2;
							for (std::size_t i {2}; i < 2 * other.size(); i += 2)
							{
								d[i - 1] = other[i / 2] >> (log / 2);
								d[i] = other[i / 2] & ((1 << (log / 2)) - 1);
							}
						}
						else
						{
							for (std::size_t i {2}; i < 2 * other.size(); i += 2)
							{
								d[i] = other[i / 2] >> (log / 2);
								d[i + 1] = other[i / 2] & ((1 << (log / 2)) - 1);
							}
						}

						std::conditional_t<fixed_precision,
							std::array<signed_double_limb_type, limit>,
							std::vector<signed_double_limb_type>> r;

						if constexpr (!fixed_precision)
						{
							r.resize(v_limit);
						}

						auto an = t[0];
						for (std::size_t i {0}; i < v_limit; ++i)
						{
							an <<= log / 2;
							an += t[i + 1];
							signed_double_limb_type q = an / ad;
							if (q)
							{
								if (2 < length)
								{
									t[i + 2] -= q * d[2];
									for (std::size_t j {length}; j-- > 3;)
									{
										signed_double_limb_type p = t[i + j] - q * d[j];
										t[i + j - 1] += p >> (log / 2);
										t[i + j] = p & ((1 << (log / 2)) - 1);
									}
								}
							}

							an %= ad;

							r[i] = q;
						}

						for (std::size_t i {limit}; i-- > 1;)
						{
							signed_double_limb_type carry = r[i] >> (log / 2);
							if (carry != 0)
							{
								r[i] &= (1 << (log / 2)) - 1;
								r[i - 1] += carry;
							}
						}

						auto pr = r.data();
						while (pr[0] == 0)
						{
							++pr;
							--shift;
						}

						data_.exponent_ = exponent + (shift >> 1);
						const auto shift_even = collin::math::is_even(shift);
						if constexpr (fixed_precision)
						{
							data_.size_ = amount_digits;
						}
						else
						{
							data_.data_.resize(r.size() / 2 - !shift_even - 1);
						}

						signed_double_limb_type previous_digit {0};
						if (shift_even)
						{
							(*this)[0] = static_cast<limb_type>(pr[0]);
							for (std::size_t i {1}; i < size(); ++i)
							{
								(*this)[i] = static_cast<limb_type>((pr[2 * i - 1] << (log / 2)) + pr[2 * i]);
							}
							previous_digit = (pr[2 * size() - 1] << (log / 2)) + pr[2 * size()];
						}
						else
						{
							for (std::size_t i {0}; i < size(); ++i)
							{
								(*this)[i] = static_cast<limb_type>((pr[2 * i] << (log / 2)) + pr[2 * i + 1]);
							}
							previous_digit = (pr[2 * size()] << (log / 2)) + pr[2 * size() + 1];
						}

						if constexpr (fixed_precision)
						{
							round(previous_digit);
						}
					}
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

				constexpr limb_type& operator[](std::size_t index) noexcept
					requires(!trivial)
				{
					return data_.data_[index];
				}

				constexpr const limb_type& operator[](std::size_t index) const noexcept
					requires(!trivial)
				{
					return data_.data_[index];
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

				constexpr limb_type& back() noexcept
					requires(!trivial)
				{
					return (*this)[size() - 1];
				}

				constexpr const limb_type& back() const noexcept
					requires(!trivial)
				{
					return (*this)[size() - 1];
				}

				constexpr auto data() noexcept
				{
					if constexpr (trivial)
					{
						return std::addressof(data_);
					}
					else
					{
						return data_.data_.data();
					}
				}

				constexpr const auto data() const noexcept
				{
					if constexpr (trivial)
					{
						return std::addressof(data_);
					}
					else
					{
						return data_.data_.data();
					}
				}
		};

		namespace details
		{
			template<class T, class T2>
			struct big_float_supertype;

			template<std::size_t AmountDigits, std::size_t AmountDigits2, checked_type Checked, checked_type Checked2, class Allocator, class Allocator2>
			struct big_float_supertype<big_float<AmountDigits, Checked, Allocator>, big_float<AmountDigits2, Checked2, Allocator2>>
			{
				using first = big_float<AmountDigits, Checked, Allocator>;
				using second = big_float<AmountDigits2, Checked2, Allocator2>;

				static constexpr std::size_t digits_used = AmountDigits ?
															AmountDigits2 ?
																std::max(AmountDigits, AmountDigits2)
																: 0
															: 0;

				static constexpr checked_type checked = (Checked == checked_type::checked || Checked2 == checked_type::checked)
															? checked_type::checked
															: checked_type::unchecked;

				using allocator_type = Allocator;

				using type = big_float<digits_used, checked, allocator_type>;
			};

			template<std::size_t AmountDigits, checked_type Checked, class Allocator, collin::concepts::floating_point F>
			struct big_float_supertype<big_float<AmountDigits, Checked, Allocator>, F>
			{
				using first = big_float<AmountDigits, Checked, Allocator>;
				using second = F;

				static constexpr std::size_t digits_used = AmountDigits ? std::max(AmountDigits, std::numeric_limits<F>::max_exponent10)
																		: 0;

				static constexpr checked_type checked = Checked;

				using allocator_type = Allocator;

				using type = big_float<digits_used, checked, allocator_type>;
			};

			template<std::size_t AmountDigits, checked_type Checked, class Allocator, collin::concepts::integral I>
			struct big_float_supertype<big_float<AmountDigits, Checked, Allocator>, I> : public big_float_supertype<big_float<AmountDigits, Checked, Allocator>, long double> {};

			template<class T, class T2>
			using big_float_supertype_t = typename big_float_supertype<T, T2>::type;
		}

		template<std::size_t AmountDigits, checked_type Checked, class Allocator, std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
		[[nodiscard]] constexpr auto operator+(const big_float<AmountDigits, Checked, Allocator>& lhs, const big_float<AmountDigits2, Checked2, Allocator2>& rhs)
		{
			using return_t = details::big_float_supertype_t<big_float<AmountDigits, Checked, Allocator>, big_float<AmountDigits2, Checked2, Allocator2>>;

			return_t result {lhs};
			result += rhs;
			return result;
		}

		template<std::size_t AmountDigits, checked_type Checked, class Allocator, collin::concepts::arithmetic A>
		[[nodiscard]] constexpr auto operator+(const big_float<AmountDigits, Checked, Allocator>& lhs, A rhs)
		{
			using return_t = details::big_float_supertype_t<big_float<AmountDigits, Checked, Allocator>, A>;

			return_t result {lhs};
			result += rhs;
			return result;
		}

		template<std::size_t AmountDigits, checked_type Checked, class Allocator, collin::concepts::arithmetic A>
		[[nodiscard]] constexpr auto operator+(A lhs, const big_float<AmountDigits, Checked, Allocator>& rhs)
		{
			using return_t = details::big_float_supertype_t<big_float<AmountDigits, Checked, Allocator>, A>;

			return_t result {lhs};
			result += rhs;
			return result;
		}

		template<std::size_t AmountDigits, checked_type Checked, class Allocator, std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
		[[nodiscard]] constexpr auto operator-(const big_float<AmountDigits, Checked, Allocator>& lhs, const big_float<AmountDigits2, Checked2, Allocator2>& rhs)
		{
			using return_t = details::big_float_supertype_t<big_float<AmountDigits, Checked, Allocator>, big_float<AmountDigits2, Checked2, Allocator2>>;

			return_t result {lhs};
			result -= rhs;
			return result;
		}

		template<std::size_t AmountDigits, checked_type Checked, class Allocator, collin::concepts::arithmetic A>
		[[nodiscard]] constexpr auto operator-(const big_float<AmountDigits, Checked, Allocator>& lhs, A rhs)
		{
			using return_t = details::big_float_supertype_t<big_float<AmountDigits, Checked, Allocator>, A>;

			return_t result {lhs};
			result -= rhs;
			return result;
		}

		template<std::size_t AmountDigits, checked_type Checked, class Allocator, collin::concepts::arithmetic A>
		[[nodiscard]] constexpr auto operator-(A lhs, const big_float<AmountDigits, Checked, Allocator>& rhs)
		{
			using return_t = details::big_float_supertype_t<big_float<AmountDigits, Checked, Allocator>, A>;

			return_t result {lhs};
			result -= rhs;
			return result;
		}

		template<std::size_t AmountDigits, checked_type Checked, class Allocator, std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
		[[nodiscard]] constexpr auto operator*(const big_float<AmountDigits, Checked, Allocator>& lhs, const big_float<AmountDigits2, Checked2, Allocator2>& rhs)
		{
			using return_t = details::big_float_supertype_t<big_float<AmountDigits, Checked, Allocator>, big_float<AmountDigits2, Checked2, Allocator2>>;

			return_t result {lhs};
			result *= rhs;
			return result;
		}

		template<std::size_t AmountDigits, checked_type Checked, class Allocator, collin::concepts::arithmetic A>
		[[nodiscard]] constexpr auto operator*(const big_float<AmountDigits, Checked, Allocator>& lhs, A rhs)
		{
			using return_t = details::big_float_supertype_t<big_float<AmountDigits, Checked, Allocator>, A>;

			return_t result {lhs};
			result *= rhs;
			return result;
		}

		template<std::size_t AmountDigits, checked_type Checked, class Allocator, collin::concepts::arithmetic A>
		[[nodiscard]] constexpr auto operator*(A lhs, const big_float<AmountDigits, Checked, Allocator>& rhs)
		{
			using return_t = details::big_float_supertype_t<big_float<AmountDigits, Checked, Allocator>, A>;

			return_t result {lhs};
			result *= rhs;
			return result;
		}

		template<std::size_t AmountDigits, checked_type Checked, class Allocator, std::size_t AmountDigits2, checked_type Checked2, class Allocator2>
		[[nodiscard]] constexpr auto operator/(const big_float<AmountDigits, Checked, Allocator>& lhs, const big_float<AmountDigits2, Checked2, Allocator2>& rhs)
		{
			using return_t = details::big_float_supertype_t<big_float<AmountDigits, Checked, Allocator>, big_float<AmountDigits2, Checked2, Allocator2>>;

			return_t result {lhs};
			result /= rhs;
			return result;
		}

		template<std::size_t AmountDigits, checked_type Checked, class Allocator, collin::concepts::arithmetic A>
		[[nodiscard]] constexpr auto operator/(const big_float<AmountDigits, Checked, Allocator>& lhs, A rhs)
		{
			using return_t = details::big_float_supertype_t<big_float<AmountDigits, Checked, Allocator>, A>;

			return_t result {lhs};
			result /= rhs;
			return result;
		}

		template<std::size_t AmountDigits, checked_type Checked, class Allocator, collin::concepts::arithmetic A>
		[[nodiscard]] constexpr auto operator/(A lhs, const big_float<AmountDigits, Checked, Allocator>& rhs)
		{
			using return_t = details::big_float_supertype_t<big_float<AmountDigits, Checked, Allocator>, A>;

			return_t result {lhs};
			result /= rhs;
			return result;
		}
	}
}