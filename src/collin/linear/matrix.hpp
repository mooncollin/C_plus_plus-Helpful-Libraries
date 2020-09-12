#pragma once

#include <limits>
#include <vector>
#include <array>
#include <type_traits>
#include <tuple>
#include <stdexcept>
#include <numeric>
#include <algorithm>
#include <span>
#include <iterator>
#include <execution>
#include <functional>
#include <future>

#include "collin/type_traits.hpp"
#include "collin/concepts.hpp"
#include "collin/multidimensional.hpp"
#include "collin/iterator.hpp"
#include "collin/math.hpp"
#include "collin/numeric.hpp"
#include "collin/algorithm.hpp"
#include "collin/utility.hpp"
#include "collin/span.hpp"
#include "collin/ranges/ranges.hpp"

namespace collin
{
	namespace linear
	{
		template<class Rep>
		class matrix;

		template<class Rep, std::size_t Rows, std::size_t Cols>
		class fixed_matrix;

		template<class T, std::size_t S>
		using square_matrix = fixed_matrix<T, S, S>;

		template<class T>
		struct is_matrix : std::false_type {};

		template<class Rep>
		struct is_matrix<matrix<Rep>> : std::true_type {};

		template<class T>
		constexpr auto is_matrix_v = is_matrix<T>::value;

		template<class T>
		concept regular_matrix_type = is_matrix_v<T>;

		template<class T>
		struct is_fixed_matrix : std::false_type {};

		template<class Rep, std::size_t Rows, std::size_t Cols>
		struct is_fixed_matrix<fixed_matrix<Rep, Rows, Cols>> : std::true_type {};

		template<class T>
		constexpr auto is_fixed_matrix_v = is_fixed_matrix<T>::value;

		template<class T>
		concept fixed_matrix_type = is_fixed_matrix_v<T>;

		template<class T>
		concept matrix_type = regular_matrix_type<T> || fixed_matrix_type<T>;

		template<class T, std::size_t Extent>
		class row : private std::span<T, Extent>
		{
			using base = std::span<T, Extent>;

			public:
				using element_type = typename base::element_type;
				using value_type = typename base::value_type;
				using size_type = typename base::size_type;
				using difference_type = typename base::difference_type;
				using pointer = typename base::pointer;
				using const_pointer = typename base::const_pointer;
				using reference = typename base::reference;
				using const_reference = typename base::const_reference;
				using iterator = typename base::iterator;
				using reverse_iterator = typename base::reverse_iterator;

				constexpr row(T* first, std::size_t size)
					: base{first, size} {}

				[[nodiscard]] constexpr iterator begin() const noexcept
				{
					return base::begin();
				}

				[[nodiscard]] constexpr iterator end() const noexcept
				{
					return base::end();
				}

				[[nodiscard]] constexpr reverse_iterator rbegin() const noexcept
				{
					return base::rbegin();
				}

				[[nodiscard]] constexpr reverse_iterator rend() const noexcept
				{
					return base::rend();
				}

				[[nodiscard]] constexpr reference front() const noexcept
				{
					return base::front();
				}

				[[nodiscard]] constexpr reference back() const noexcept
				{
					return base::back();
				}

				[[nodiscard]] constexpr reference operator[](size_type i) const noexcept
				{
					return base::operator[](i);
				}

				[[nodiscard]] constexpr pointer data() const noexcept
				{
					return base::data();
				}

				[[nodiscard]] constexpr size_type size() const noexcept
				{
					return base::size();
				}

				[[nodiscard]] constexpr bool empty() const noexcept
				{
					return base::empty();
				}

				template<class T2>
					requires(requires(const T& t, const T2& t2) { { t + t2 } -> collin::concepts::convertible_to<T>; })
				constexpr row& operator+=(const row<T2, Extent>& other) noexcept
				{
					transform_with(other, std::plus<>{});
					return *this;
				}

				template<class T2>
					requires(requires(const T& t, const T2& t2) { { t - t2 } -> collin::concepts::convertible_to<T>; })
				constexpr row& operator-=(const row<T2, Extent>& other) noexcept
				{
					transform_with(other, std::minus<>{});
					return *this;
				}

				template<class T2>
				requires(requires(const T& t, const T2& t2) { { t * t2 } -> collin::concepts::convertible_to<T>; })
				constexpr row& operator*=(const T2& other) noexcept
				{
					transform_with(other, std::multiplies<>{});
					return *this;
				}

				template<class T2>
					requires(requires(const T& t, const T2& t2) { { t / t2 } -> collin::concepts::convertible_to<T>; })
				constexpr row& operator/=(const T2& other) noexcept
				{
					transform_with(other, std::divides<>{});
					return *this;
				}

				template<class T2>
					requires(requires(const T& t, const T2& t2) { { t % t2 } -> collin::concepts::convertible_to<T>; })
				constexpr row& operator%=(const T2& other) noexcept
				{
					transform_with(other, std::modulus<>{});
					return *this;
				}

				constexpr void swap(row& other) noexcept
				{
					if constexpr(std::is_constant_evaluated())
					{
						std::swap_ranges(begin(), end(), other.begin());
					}
					else
					{
						std::swap_ranges(std::execution::par_unseq, begin(), end(), other.begin());
					}
				}
			private:
				template<class T2, class F>
				constexpr void transform_with(const T2& other, F&& f) noexcept
				{
					if constexpr (std::is_constant_evaluated())
					{
						std::transform(begin(), end(), begin(), std::bind(std::forward<F>(f), std::placeholders::_1, other));
					}
					else
					{
						std::transform(std::execution::par_unseq, begin(), end(), begin(), std::bind(std::forward<F>(f), std::placeholders::_1, other));
					}
				}

				template<class T2, class F>
				constexpr void transform_with(const row<T2, Extent>& other, F&& f) noexcept
				{
					if constexpr (std::is_constant_evaluated())
					{
						std::transform(begin(), end(), other.begin(), begin(), std::forward<F>(f));
					}
					else
					{
						std::transform(std::execution::par_unseq, begin(), end(), other.begin(), begin(), std::forward<F>(f));
					}
				}
		};

		template<class T, std::size_t Size>
		constexpr void swap(row<T, Size>& lhs, row<T, Size>& rhs) noexcept
		{
			lhs.swap(rhs);
		}

		template<class T, class T2, std::size_t Extent, std::size_t Extent2>
			requires(requires(const T& t, const T2& t2){ {t == t2} -> collin::concepts::boolean; })
		[[nodiscard]] constexpr bool operator==(const row<T, Extent>& lhs, const row<T2, Extent2>& rhs) noexcept
		{
			if constexpr (Extent != Extent2)
			{
				return false;
			}
			else
			{
				if(std::size(lhs) != std::size(rhs))
				{
					return false;
				}

				if constexpr (std::is_constant_evaluated())
				{
					return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs));
				}
				else
				{
					return std::equal(std::execution::par_unseq, std::begin(lhs), std::end(lhs), std::begin(rhs));
				}
			}
		}

		template<class T, class T2, std::size_t Extent, std::size_t Extent2>
			requires(requires(const T& t, const T2& t2){ {t == t2} -> collin::concepts::boolean; })
		[[nodiscard]] constexpr bool operator!=(const row<T, Extent>& lhs, const row<T2, Extent2>& rhs) noexcept
		{
			return !(lhs == rhs);
		}

		template<class T, std::size_t RowSize>
		class column_iterator
		{
			public:
				using iterator_category = std::random_access_iterator_tag;
				using value_type = T;
				using difference_type = std::ptrdiff_t;
				using pointer = value_type*;
				using reference = value_type&;

				constexpr column_iterator(pointer item, std::size_t row_size) noexcept
					: storage_{item, row_size} {}

				constexpr column_iterator& operator--() noexcept
				{
					storage_.data_ -= storage_.size();
					return *this;
				}

				constexpr column_iterator operator--(int) noexcept
				{
					auto temp = *this;
					--(*this);
					return temp;
				}

				constexpr column_iterator& operator-=(std::size_t amount) noexcept
				{
					storage_.data_ -= storage_.size() * amount;
					return *this;
				}

				[[nodiscard]] friend constexpr column_iterator operator-(const column_iterator& lhs, std::size_t amount) noexcept
				{
					auto c_it{lhs};
					c_it -= amount;
					return c_it;
				}

				[[nodiscard]] friend constexpr difference_type operator-(const column_iterator& lhs, const column_iterator& rhs) noexcept
				{
					return (lhs.storage_.data_ - rhs.storage_.data_) / lhs.storage_.size();
				}

				[[nodiscard]] constexpr typename reference operator*() noexcept
				{
					return *storage_.data_;
				}

				constexpr column_iterator& operator++() noexcept
				{
					storage_.data_ += storage_.size();
					return *this;
				}

				constexpr column_iterator operator++(int) noexcept
				{
					auto temp = *this;
					++(*this);
					return temp;
				}

				constexpr column_iterator& operator+=(std::size_t amount) noexcept
				{
					storage_.data_ += storage_.size() * amount;
					return *this;
				}

				[[nodiscard]] friend constexpr column_iterator operator+(const column_iterator& lhs, std::size_t amount) noexcept
				{
					auto c_it{lhs};
					c_it += amount;
					return c_it;
				}

				[[nodiscard]] constexpr bool operator==(const column_iterator& other) const noexcept
				{
					return storage_.data_ == other.storage_.data_;
				}

				[[nodiscard]] constexpr bool operator!=(const column_iterator& other) const noexcept
				{
					return !(*this == other);
				}

				constexpr typename pointer operator->()
				{
					return storage_.data_;
				}
			private:
				template<class ExtentType>
				struct storage_type : public ExtentType
				{
					template<class OtherExtentType>
					constexpr storage_type(pointer data, OtherExtentType ext) noexcept
						: ExtentType(ext), data_(data) {}

					pointer data_;
				};

				storage_type<collin::extent_type<RowSize>> storage_;
		};

		template<class T, std::size_t Extent, std::size_t RowSize>
		class column
		{
			public:
				using element_type = T;
				using value_type = std::remove_cv_t<T>;
				using size_type = std::size_t;
				using difference_type = std::ptrdiff_t;
				using pointer = T*;
				using const_pointer = const T*;
				using reference = T&;
				using const_reference = const T&;
				using iterator = column_iterator<T, RowSize>;
				using reverse_iterator = std::reverse_iterator<iterator>;

				constexpr column(pointer first, size_type size, size_type row_size) noexcept
					//: first_{first}, size_{size}, row_size_{row_size} {}
					: storage_{first, size, row_size} {}

				[[nodiscard]] constexpr iterator begin() const noexcept
				{
					return {data(), row_size()};
				}

				[[nodiscard]] constexpr iterator end() const noexcept
				{
					return {data() + (size() * row_size()), row_size()};
				}

				[[nodiscard]] constexpr reverse_iterator rbegin() const noexcept
				{
					return {end()};
				}

				[[nodiscard]] constexpr reverse_iterator rend() const noexcept
				{
					return {begin()};
				}

				[[nodiscard]] constexpr reference front() const noexcept
				{
					return *data();
				}

				[[nodiscard]] constexpr reference back() const noexcept
				{
					return *(--end());
				}

				[[nodiscard]] constexpr reference operator[](size_type i) const noexcept
				{
					return *(data() + (i * row_size()));
				}

				[[nodiscard]] constexpr pointer data() const noexcept
				{
					//return first_;
					return storage_.data_;
				}

				[[nodiscard]] constexpr size_type size() const noexcept
				{
					//return size_;
					return storage_.main_size();
				}

				[[nodiscard]] constexpr size_type row_size() const noexcept
				{
					//return row_size_;
					return storage_.row_size();
				}

				[[nodiscard]] constexpr bool empty() const noexcept
				{
					return size() == 0;
				}

				template<class T2>
					requires(requires(const T& t, const T2& t2) { { t + t2 } -> collin::concepts::convertible_to<T>; })
				constexpr column& operator+=(const column<T2, Extent, RowSize>& other) noexcept
				{
					transform_with(other, std::plus<>{});
					return *this;
				}

				template<class T2>
					requires(requires(const T& t, const T2& t2) { { t - t2 } -> collin::concepts::convertible_to<T>; })
				constexpr column& operator-=(const column<T2, Extent, RowSize>& other) noexcept
				{
					transform_with(other, std::minus<>{});
					return *this;
				}

				template<class T2>
					requires(requires(const T& t, const T2& t2) { { t * t2 } -> collin::concepts::convertible_to<T>; })
				constexpr column& operator*=(const T2& other) noexcept
				{
					transform_with(other, std::multiplies<>{});
					return *this;
				}

				template<class T2>
					requires(requires(const T& t, const T2& t2) { { t / t2 } -> collin::concepts::convertible_to<T>; })
				constexpr column& operator/=(const T2& other) noexcept
				{
					transform_with(other, std::divides<>{});
					return *this;
				}

				template<class T2>
					requires(requires(const T& t, const T2& t2) { { t % t2 } -> collin::concepts::convertible_to<T>; })
				constexpr column& operator%=(const T2& other) noexcept
				{
					transform_with(other, std::modulus<>{});
					return *this;
				}

				constexpr void swap(column& other) noexcept
				{
					if constexpr (std::is_constant_evaluated())
					{
						std::swap_ranges(begin(), end(), other.begin());
					}
					else
					{
						std::swap_ranges(std::execution::par_unseq, begin(), end(), other.begin());
					}
				}
			private:

				// Run time extents
				template<class ExtentType, class ExtentType2>
				struct storage_type
				{
					template<class OtherExtentType, class OtherExtentType2>
					storage_type(pointer data, OtherExtentType ext, OtherExtentType2 ext2) noexcept
						: ex_{ext}, ex2_{ext2} , data_{data} {}

					[[nodiscard]] typename ExtentType::size_type main_size() const noexcept
					{
						return ex_.size();
					}

					[[nodiscard]] typename ExtentType2::size_type row_size() const noexcept
					{
						return ex2_.size();
					}

					pointer data_;
					ExtentType ex_;
					ExtentType2 ex2_;
				};

				// Compile time extents
				template<class ExtentType, class ExtentType2>
				struct storage_type2
				{
					template<class OtherExtentType>
					constexpr storage_type2(pointer data, OtherExtentType, ExtentType2) noexcept
						: data_{data} {}

					[[nodiscard]] constexpr typename ExtentType::size_type main_size() const noexcept
					{
						return ExtentType{}.size();
					}

					[[nodiscard]] constexpr typename ExtentType2::size_type row_size() const noexcept
					{
						return ExtentType2{}.size();
					}

					pointer data_;
				};

				// Optimizing away size and row-size if those are compile-time.
				// Technically, both sizes should be either dynamic or compile-time.
				std::conditional_t<Extent == dynamic_extent || RowSize == dynamic_extent,
					storage_type<collin::extent_type<Extent>, collin::extent_type<RowSize>>,
					storage_type2<collin::extent_type<Extent>, collin::extent_type<RowSize>>> storage_;

				template<class T2, class F>
				constexpr void transform_with(const T2& other, F&& f) noexcept
				{
					if constexpr (std::is_constant_evaluated())
					{
						std::transform(begin(), end(), begin(), std::bind(std::forward<F>(f), std::placeholders::_1, other));
					}
					else
					{
						std::transform(std::execution::par_unseq, begin(), end(), begin(), std::bind(std::forward<F>(f), std::placeholders::_1, other));
					}
				}

				template<class T2, class F>
				constexpr void transform_with(const column<T2, Extent, RowSize>& other, F&& f) noexcept
				{
					if constexpr (std::is_constant_evaluated())
					{
						std::transform(begin(), end(), other.begin(), begin(), std::forward<F>(f));
					}
					else
					{
						std::transform(std::execution::par_unseq, begin(), end(), other.begin(), begin(), std::forward<F>(f));
					}
				}
		};

		template<class T, std::size_t Size, std::size_t RowSize>
		constexpr void swap(column<T, Size, RowSize>& lhs, column<T, Size, RowSize>& rhs) noexcept
		{
			lhs.swap(rhs);
		}

		template<class T, class T2, std::size_t Extent, std::size_t RowSize, std::size_t Extent2, std::size_t RowSize2>
			requires(requires(const T& t, const T2& t2){ {t == t2} -> collin::concepts::boolean; })
		[[nodiscard]] constexpr bool operator==(const column<T, Extent, RowSize>& lhs, const column<T2, Extent2, RowSize2>& rhs) noexcept
		{
			if constexpr (Extent != Extent2 || RowSize != RowSize2)
			{
				return false;
			}
			else
			{
				if(std::size(lhs) != std::size(rhs) || lhs.row_size() != rhs.row_size())
				{
					return false;
				}

				if constexpr (std::is_constant_evaluated())
				{
					return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs));
				}
				else
				{
					return std::equal(std::execution::par_unseq, std::begin(lhs), std::end(lhs), std::begin(rhs));
				}
			}
		}

		template<class T, class T2, std::size_t Extent, std::size_t RowSize>
			requires(requires(const T& t, const T2& t2){ {t == t2} -> collin::concepts::boolean; })
		[[nodiscard]] constexpr bool operator!=(const column<T, Extent, RowSize>& lhs, const column<T2, Extent, RowSize>& rhs)
		{
			return !(lhs == rhs);
		}

		template<class T, std::size_t Size>
		class diagonal;

		template<class T, std::size_t RowSize>
		class diagonal_iterator
		{
			public:
				using iterator_category = std::random_access_iterator_tag;
				using value_type = T;
				using difference_type = std::ptrdiff_t;
				using pointer = value_type*;
				using reference = value_type&;

				constexpr diagonal_iterator(pointer item, const diagonal<T, RowSize>& owner) noexcept
					: current_{item}, owner_{&owner} {}

				constexpr diagonal_iterator& operator--() noexcept
				{
					current_ -= 1;
					if (*this != std::begin(*owner_))
					{
						current_ -= std::size(*owner_);
					}

					return *this;
				}

				constexpr diagonal_iterator operator--(int) noexcept
				{
					auto temp = *this;
					--(*this);
					return temp;
				}

				[[nodiscard]] constexpr typename reference operator*() noexcept
				{
					return *current_;
				}

				constexpr diagonal_iterator& operator++() noexcept
				{
					current_ += 1;
					if (*this != std::end(*owner_))
					{
						current_ += std::size(*owner_);
					}

					return *this;
				}

				constexpr diagonal_iterator operator++(int) noexcept
				{
					auto temp = *this;
					++(*this);
					return temp;
				}

				[[nodiscard]] constexpr bool operator==(const diagonal_iterator& other) const noexcept
				{
					return current_ == other.current_;
				}

				[[nodiscard]] constexpr bool operator!=(const diagonal_iterator& other) const noexcept
				{
					return !(*this == other);
				}

				constexpr pointer operator->()
				{
					return current_;
				} 
			private:
				pointer current_;

				// C++20 gives constexpr std::reference_wrapper,
				// so we's gotta wait until then and use a crappy pointer.
				//
				// It is rather strange to hold the owner who makes the iterator
				// but unfortunately, when we iterate through the diagonal, we have
				// a tendency to iterate far past the end, which doesn't work well
				// in constexpr conditions. We use the owner to make sure our current
				// pointer does not go too far past the beginning or end of the container.
				//
				// mutable const?! Yes.
				mutable const diagonal<T, RowSize>* owner_;
		};

		template<class T, std::size_t Size>
		class diagonal
		{
			public:
				using element_type = T;
				using value_type = std::remove_cv_t<T>;
				using size_type = std::size_t;
				using difference_type = std::ptrdiff_t;
				using pointer = T*;
				using const_pointer = const T*;
				using reference = T&;
				using const_reference = const T&;
				using iterator = diagonal_iterator<T, Size>;
				using reverse_iterator = std::reverse_iterator<iterator>;

				constexpr diagonal(pointer first, size_type size)
					: storage_{first, size} {}

				[[nodiscard]] constexpr iterator begin() const noexcept
				{
					return {data(), *this};
				}

				[[nodiscard]] constexpr iterator end() const noexcept
				{
					return {data() + storage_.size() * storage_.size(), *this};
				}

				[[nodiscard]] constexpr reverse_iterator rbegin() const noexcept
				{
					return {end()};
				}

				[[nodiscard]] constexpr reverse_iterator rend() const noexcept
				{
					return {begin()};
				}

				[[nodiscard]] constexpr reference front() const noexcept
				{
					return *data();
				}

				[[nodiscard]] constexpr reference back() const noexcept
				{
					return *(--end());
				}

				[[nodiscard]] constexpr reference operator[](size_type i) const noexcept
				{
					return *(data() + (i * storage_.size()) + i);
				}

				[[nodiscard]] constexpr pointer data() const noexcept
				{
					return storage_.data_;
				}

				[[nodiscard]] constexpr size_type size() const noexcept
				{
					return storage_.size();
				}

				[[nodiscard]] constexpr bool empty() const noexcept
				{
					return size() == 0;
				}

				template<class T2>
					requires(requires(const T& t, const T2& t2) { { t + t2 } -> collin::concepts::convertible_to<T>; })
				constexpr diagonal& operator+=(const diagonal<T2, Size>& other) noexcept
				{
					transform_with(other, std::plus<>{});
					return *this;
				}

				template<class T2>
					requires(requires(const T& t, const T2& t2) { { t - t2 } -> collin::concepts::convertible_to<T>; })
				constexpr diagonal& operator-=(const diagonal<T2, Size>& other) noexcept
				{
					transform_with(other, std::minus<>{});
					return *this;
				}

				template<class T2>
					requires(requires(const T& t, const T2& t2) { { t * t2 } -> collin::concepts::convertible_to<T>; })
				constexpr diagonal& operator*=(const T2& other) noexcept
				{
					transform_with(other, std::multiplies<>{});
					return *this;
				}

				template<class T2>
					requires(requires(const T& t, const T2& t2) { { t / t2 } -> collin::concepts::convertible_to<T>; })
				constexpr diagonal& operator/=(const T2& other) noexcept
				{
					transform_with(other, std::divides<>{});
					return *this;
				}

				template<class T2>
					requires(requires(const T& t, const T2& t2) { { t % t2 } -> collin::concepts::convertible_to<T>; })
				constexpr diagonal& operator%=(const T2& other) noexcept
				{
					transform_with(other, std::modulus<>{});
					return *this;
				}

				constexpr void swap(diagonal& other) noexcept
				{
					if constexpr (std::is_constant_evaluated())
					{
						std::swap_ranges(begin(), end(), other.begin());
					}
					else
					{
						std::swap_ranges(std::execution::par_unseq, begin(), end(), other.begin());
					}
				}
			private:
				template<class ExtentType>
				struct storage_type : public ExtentType
				{
					template<class OtherExtentType>
					constexpr storage_type(pointer data, OtherExtentType ext)
						: ExtentType(ext), data_(data) {}

					pointer data_;
				};

				storage_type<collin::extent_type<Size>> storage_;

				template<class T2, class F>
				constexpr void transform_with(const T2& other, F&& f) noexcept
				{
					if constexpr (std::is_constant_evaluated())
					{
						std::transform(begin(), end(), begin(), std::bind(std::forward<F>(f), std::placeholders::_1, other));
					}
					else
					{
						std::transform(std::execution::par_unseq, begin(), end(), begin(), std::bind(std::forward<F>(f), std::placeholders::_1, other));
					}
				}

				template<class T2, class F>
				constexpr void transform_with(const diagonal<T2, Size>& other, F&& f) noexcept
				{
					if constexpr (std::is_constant_evaluated())
					{
						std::transform(begin(), end(), other.begin(), begin(), std::forward<F>(f));
					}
					else
					{
						std::transform(std::execution::par_unseq, begin(), end(), other.begin(), begin(), std::forward<F>(f));
					}
				}
		};

		template<class T, std::size_t S>
		constexpr void swap(diagonal<T, S>& lhs, diagonal<T, S>& rhs) noexcept
		{
			lhs.swap(rhs);
		}

		template<class T, class T2, std::size_t Size, std::size_t Size2>
			requires(requires(const T& t, const T2& t2){ {t == t2} -> collin::concepts::boolean; })
		[[nodiscard]] constexpr bool operator==(const diagonal<T, Size>& lhs, const diagonal<T2, Size2>& rhs) noexcept
		{
			if constexpr(Size != Size2)
			{
				return false;
			}
			else
			{
				if(std::size(lhs) != std::size(rhs))
				{
					return false;
				}

				if constexpr (std::is_constant_evaluated())
				{
					return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs));
				}
				else
				{
					return std::equal(std::execution::par_unseq, std::begin(lhs), std::end(lhs), std::begin(rhs));
				}
			}
		}

		template<class T, class T2, std::size_t Extent>
			requires(requires(const T& t, const T2& t2){ {t == t2} -> collin::concepts::boolean; })
		[[nodiscard]] constexpr bool operator!=(const diagonal<T, Extent>& lhs, const diagonal<T2, Extent>& rhs) noexcept
		{
			return !(lhs == rhs);
		}

		template<class Rep>
		class matrix
		{
			using storage_t = multidimensional::fixed_multidimensional_array<Rep, 2>;

			public:
				using value_type = typename storage_t::value_type;
				using size_type = typename storage_t::size_type;
				using difference_type = typename storage_t::difference_type;
				using reference = typename storage_t::reference;
				using const_reference = typename storage_t::const_reference;
				using pointer = typename storage_t::pointer;
				using const_pointer = typename storage_t::const_pointer;
				using iterator = typename storage_t::iterator;
				using const_iterator = typename storage_t::const_iterator;
				using reverse_iterator = typename storage_t::reverse_iterator;
				using const_reverse_iterator = typename storage_t::const_reverse_iterator;

				matrix()
					: data_{0, 0} {}

				matrix(std::size_t row, std::size_t col)
					: data_{row, col} {}

				matrix(const matrix&) = default;

				template<collin::concepts::convertible_to<Rep> Rep2>
				matrix(const matrix<Rep2>& other)
					: data_{other.data_} {}

				template<collin::concepts::convertible_to<Rep> Rep2, std::size_t R, std::size_t C>
				matrix(const fixed_matrix<Rep2, R, C>& other)
					: data_{other.data_} {}

				matrix(matrix&&) noexcept = default;

				template<collin::concepts::convertible_to<Rep> Rep2>
				matrix& operator=(const matrix<Rep2>& other)
				{
					if (this != std::addressof(other))
					{
						data_ = other.data_;
					}

					return *this;
				}

				matrix& operator=(const matrix&) = default;
				matrix& operator=(matrix&&) noexcept = default;

				matrix& operator=(std::initializer_list<Rep> elements)
				{
					data_ = elements;
					return *this;
				}

				template<collin::concepts::convertible_to<Rep> Rep2, std::size_t R, std::size_t C>
				matrix& operator=(const fixed_matrix<Rep2, R, C>& other)
				{
					data_ = other.data_;
					return *this;
				}

				void resize(std::size_t rows, std::size_t cols)
				{
					data_.dimensions(rows, cols);
				}

				[[nodiscard]] std::size_t rows() const noexcept
				{
					return data_.dimensions()[0];
				}

				[[nodiscard]] std::size_t cols() const noexcept
				{
					return data_.dimensions()[1];
				}

				reference operator()(std::size_t r, std::size_t c) noexcept
				{
					return data_.get(r, c);
				}

				const_reference operator()(std::size_t r, std::size_t c) const noexcept
				{	
					return data_.get(r, c);
				}

				reference operator[](std::size_t index) noexcept
				{
					return data_[index];
				}

				const_reference operator[](std::size_t index) const noexcept
				{
					return data_[index];
				}

				[[nodiscard]] row<value_type, std::dynamic_extent> get_row(std::size_t i) noexcept
				{
					return {&data_.get(i, 0), cols()};
				}

				[[nodiscard]] row<const value_type, std::dynamic_extent> get_row(std::size_t i) const noexcept
				{
					return {&data_.get(i, 0), cols()};
				}

				[[nodiscard]] column<value_type, std::dynamic_extent, std::dynamic_extent> get_column(std::size_t i) noexcept
				{
					return {&data_.get(0, i), rows(), cols()};
				}

				[[nodiscard]] column<const value_type, std::dynamic_extent, std::dynamic_extent> get_column(std::size_t i) const noexcept
				{
					return {&data_.get(0, i), rows(), cols()};
				}

				[[nodiscard]] const_pointer data() const noexcept
				{
					return data_.data();
				}

				[[nodiscard]] pointer data() noexcept
				{
					return data_.data();
				}

				[[nodiscard]] iterator begin() noexcept
				{
					return data_.begin();
				}

				[[nodiscard]] const_iterator begin() const noexcept
				{
					return data_.begin();
				}

				[[nodiscard]] const_iterator cbegin() const noexcept
				{
					return data_.cbegin();
				}

				[[nodiscard]] iterator end() noexcept
				{
					return data_.end();
				}

				[[nodiscard]] const_iterator end() const noexcept
				{
					return data_.end();
				}

				[[nodiscard]] const_iterator cend() const noexcept
				{
					return data_.cend();
				}

				[[nodiscard]] reverse_iterator rbegin() noexcept
				{
					return data_.rbegin();
				}

				[[nodiscard]] reverse_iterator rbegin() const noexcept
				{
					return data_.rbegin();
				}

				[[nodiscard]] const_reverse_iterator crbegin() const noexcept
				{
					return data_.crbegin();
				}

				[[nodiscard]] reverse_iterator rend() noexcept
				{
					return data_.rend();
				}

				[[nodiscard]] reverse_iterator rend() const noexcept
				{
					return data_.rend();
				}

				[[nodiscard]] const_reverse_iterator crend() const noexcept
				{
					return data_.crend();
				}

				[[nodiscard]] size_type size() const noexcept
				{
					return data_.size();
				}
				
				template<class Rep2>
					requires(requires(const Rep& t, const Rep2& t2) { { t + t2 } -> collin::concepts::convertible_to<Rep>; })
				matrix& operator+=(const matrix<Rep2>& other)
				{
					if (rows() != other.rows() || cols() != other.cols())
					{
						throw std::invalid_argument{"cannot add matrices of different dimension sizes"};
					}
					transform_with(other, std::plus<>{});
					return *this;
				}

				template<class Rep2, std::size_t Rows, std::size_t Cols>
					requires(requires(const Rep& t, const Rep2& t2) { { t + t2 } -> collin::concepts::convertible_to<Rep>; })
				matrix& operator+=(const fixed_matrix<Rep2, Rows, Cols>& other)
				{
					if (rows() != Rows || cols() != Cols)
					{
						throw std::invalid_argument{"cannot add matrices of different dimension sizes"};
					}
					transform_with(other, std::plus<>{});
					return *this;
				}

				template<class Rep2>
					requires(requires(const Rep& t, const Rep2& t2) { { t - t2 } -> collin::concepts::convertible_to<Rep>; })
				matrix& operator-=(const matrix<Rep2>& other)
				{
					if (rows() != other.rows() || cols() != other.cols())
					{
						throw std::invalid_argument{"cannot subtract matrices of different dimension sizes"};
					}
					transform_with(other, std::minus<>{});
					return *this;
				}
				
				template<class Rep2, std::size_t Rows, std::size_t Cols>
					requires(requires(const Rep& t, const Rep2& t2) { { t - t2 } -> collin::concepts::convertible_to<Rep>; })
				matrix& operator-=(const fixed_matrix<Rep2, Rows, Cols>& other)
				{
					if (rows() != Rows || cols() != Cols)
					{
						throw std::invalid_argument{"cannot subtract matrices of different dimension sizes"};
					}
					transform_with(other, std::minus<>{});
					return *this;
				}

				template<class Rep2>
					requires(requires(const Rep& t, const Rep2& t2) { { t * t2 } -> collin::concepts::convertible_to<Rep>; })
				matrix& operator*=(const Rep2& other) noexcept
				{
					transform_with(other, std::multiplies<>{});
					return *this;
				}

				template<class Rep2>
					requires(requires(const Rep& t, const Rep2& t2) { { t / t2 } -> collin::concepts::convertible_to<Rep>; })
				matrix& operator/=(const Rep2& other) noexcept
				{
					transform_with(other, std::divides<>{});
					return *this;
				}

				template<class Rep2>
					requires(requires(const Rep& t, const Rep2& t2) { { t% t2 } -> collin::concepts::convertible_to<Rep>; })
				matrix& operator%=(const Rep2& other) noexcept
				{
					transform_with(other, std::modulus<>{});
					return *this;
				}
			private:
				storage_t data_;

				template<class Rep2, class F>
				void transform_with(const matrix<Rep2>& other, F&& f) noexcept
				{
					std::transform(std::execution::par_unseq, begin(), end(), other.begin(), begin(), std::forward<F>(f));
				}

				template<class Rep2, std::size_t Rows, std::size_t Cols, class F>
				void transform_with(const fixed_matrix<Rep2, Rows, Cols>& other, F&& f) noexcept
				{
					std::transform(std::execution::par_unseq, begin(), end(), other.begin(), begin(), std::forward<F>(f));
				}

				template<class T2, class F>
				void transform_with(const T2& other, F&& f) noexcept
				{
					std::transform(std::execution::par_unseq, begin(), end(), begin(), std::bind(std::forward<F>(f), std::placeholders::_1, other));
				}
		};

		template<matrix_type Matrix1, matrix_type Matrix2>
		[[nodiscard]] constexpr bool operator==(const Matrix1& lhs, const Matrix2& rhs) noexcept
		{
			if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols())
			{
				return false;
			}

			if constexpr (std::is_constant_evaluated())
			{
				return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs));
			}
			else
			{
				return std::equal(std::execution::par_unseq, std::begin(lhs), std::end(lhs), std::begin(rhs));
			}
		}

		template<matrix_type Matrix1, matrix_type Matrix2>
		[[nodiscard]] constexpr bool operator!=(const Matrix1& lhs, const Matrix2& rhs) noexcept
		{
			return !(lhs == rhs);
		}

		template<matrix_type Matrix1, matrix_type Matrix2>
		[[nodiscard]] constexpr auto operator+(const Matrix1& lhs, const Matrix2& rhs)
		{
			auto m {lhs};
			m += rhs;
			return m;
		}

		template<matrix_type Matrix1, matrix_type Matrix2>
		[[nodiscard]] constexpr auto operator-(const Matrix1& lhs, const Matrix2& rhs)
		{
			auto m {lhs};
			m -= rhs;
			return m;
		}

		namespace details
		{
			constexpr std::size_t strassen_dim_cutoff {300};

			template<class T>
			[[nodiscard]] matrix<T> get_part(std::size_t pi, std::size_t pj, const matrix<T>& m) noexcept
			{
				const std::size_t new_dim {m.rows() / 2};
				matrix<T> p{new_dim, new_dim};
				pi *= new_dim;
				pj *= new_dim;

				for (std::size_t i {0}; i < new_dim; ++i)
				{
					for (std::size_t j{0}; j < new_dim; ++j)
					{
						p(i, j) = m(i + pi, j + pj);
					}
				}

				return p;
			}

			template<class T, std::size_t Rows>
			[[nodiscard]] constexpr square_matrix<T, Rows / 2> get_part(std::size_t pi, std::size_t pj, const square_matrix<T, Rows>& m) noexcept
			{
				constexpr auto new_dim = Rows / 2;
				square_matrix<T, new_dim> p;
				pi *= new_dim;
				pj *= new_dim;

				for (std::size_t i {0}; i < new_dim; ++i)
				{
					for (std::size_t j{0}; j < new_dim; ++j)
					{
						p(i, j) = m(i + pi, j + pj);
					}
				}

				return p;
			}

			template<matrix_type M1, matrix_type M2>
			constexpr void set_part(std::size_t pi, std::size_t pj, M1& m, const M2& p) noexcept
			{
				const auto dim = p.rows();
				pi *= dim;
				pj *= dim;

				for (std::size_t i {0}; i < dim; ++i)
				{
					for (std::size_t j {0}; j < dim; ++j)
					{
						m(i + pi, j + pj) = p(i, j);
					}
				}
			}

			template<matrix_type M1, matrix_type M2>
			[[nodiscard]] constexpr auto mult_std(const M1& lhs, const M2& rhs) noexcept
			{
				using common_type = std::common_type_t<typename M1::value_type, typename M2::value_type>;

				matrix<common_type> m {lhs.rows(), rhs.cols()};
				for (std::size_t i {0}; i < lhs.rows(); ++i)
				{
					for (std::size_t j {0}; j < rhs.cols(); ++j)
					{
						m(i, j) = std::transform_reduce(std::begin(lhs.get_row(i)),
														std::begin(lhs.get_row(i)) + lhs.cols(),
														std::begin(rhs.get_column(j)), common_type{0});
					}
				}

				return m;
			}

			template<class T, class T2, std::size_t Rows, std::size_t Cols, std::size_t Cols2>
			[[nodiscard]] constexpr auto mult_std(const fixed_matrix<T, Rows, Cols>& lhs, const fixed_matrix<T2, Cols, Cols2>& rhs) noexcept
			{
				fixed_matrix<std::common_type_t<T, T2>, Rows, Cols2> m;

				for (std::size_t i {0}; i < Rows; ++i)
				{
					for (std::size_t j {0}; j < Cols2; ++j)
					{
						if constexpr (std::is_constant_evaluated())
						{
							for (std::size_t n {0}; n < Cols; ++n)
							{
								m(i, j) += lhs(i, n) * rhs(n, j);
							}
						}
						else
						{
							m(i, j) = std::transform_reduce(std::begin(lhs.get_row(i)),
															std::begin(lhs.get_row(i)) + lhs.cols(),
															std::begin(rhs.get_column(j)), common_type{0});
						}
					}
				}

				return m;
			}

			template<class T, class T2>
			[[nodiscard]] auto mult_strassen(const matrix<T>& a, const matrix<T2>& b) noexcept
			{
				if (a.rows() < strassen_dim_cutoff)
				{
					return mult_std(a, b);
				}

				using get_part_a_t = std::remove_cvref_t<decltype(a)>(*)(std::size_t, std::size_t, decltype(a));
				using get_part_b_t = std::remove_cvref_t<decltype(b)>(*)(std::size_t, std::size_t, decltype(b));
				using common_type = std::common_type_t<T, T2>;
				using f_t = matrix<common_type>(*)(decltype(a), decltype(b));
				using set_part_f_t = void(*)(std::size_t, std::size_t, matrix<common_type>&, const matrix<common_type>&);

				auto a11_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 0, 0, std::ref(a));
				auto a12_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 0, 1, std::ref(a));
				auto a21_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 1, 0, std::ref(a));
				auto a22_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 1, 1, std::ref(a));

				auto b11_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::ref(b));
				auto b12_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::ref(b));
				auto b21_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::ref(b));
				auto b22_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::ref(b));

				const auto a11 = a11_future.get();
				const auto a22 = a22_future.get();

				const auto b11 = b11_future.get();
				const auto b22 = b22_future.get();
				auto m1_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a11 + a22, b11 + b22);

				const auto a21 = a21_future.get();
				auto m2_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a21 + a22, std::ref(b11));

				const auto b12 = b12_future.get();
				auto m3_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), std::ref(a11), b12 - b22);
				auto m6_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a21 - a11, b11 + b12);

				const auto a12 = a12_future.get();
				auto m5_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a11 + a12, std::ref(b22));

				const auto b21 = b21_future.get();
				auto m4_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), std::ref(a22), b21 - b11);
				auto m7_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a12 - a22, b21 + b22);

				matrix<common_type> c{a.rows(), a.cols()};

				const auto m1 = m1_future.get();
				const auto m4 = m4_future.get();
				const auto m5 = m5_future.get();
				const auto m7 = m7_future.get();
				const auto v1 = std::async(std::launch::async, static_cast<set_part_f_t>(set_part), 0, 0, std::ref(c), m1 + m4 - m5 + m7);

				const auto m3 = m3_future.get();
				const auto v2 = std::async(std::launch::async, static_cast<set_part_f_t>(set_part), 0, 1, std::ref(c), m3 + m5);

				const auto m2 = m2_future.get();
				const auto v3 = std::async(std::launch::async, static_cast<set_part_f_t>(set_part), 1, 0, std::ref(c), m2 + m4);

				const auto m6 = m6_future.get();
				const auto v4 = std::async(std::launch::async, static_cast<set_part_f_t>(set_part), 1, 1, std::ref(c), m1 - m2 + m3 + m6);

				v1.wait();
				v2.wait();
				v3.wait();
				v4.wait();

				return c;
			}

			template<class T, class T2, std::size_t Rows, std::size_t Cols>
			[[nodiscard]] auto mult_strassen(const matrix<T>& a, const fixed_matrix<T2, Rows, Cols>& b) noexcept
			{
				if constexpr (Rows < strassen_dim_cutoff)
				{
					return mult_std(a, b);
				}
				else
				{
					using get_part_a_t = std::remove_cvref_t<decltype(a)>(*)(std::size_t, std::size_t, decltype(a));
					using get_part_b_t = std::remove_cvref_t<decltype(b)>(*)(std::size_t, std::size_t, decltype(b));
					using common_type = std::common_type_t<T, T2>;
					using f_t = matrix<common_type>(*)(decltype(a), decltype(b));
					using set_part_f_t = void(*)(std::size_t, std::size_t, matrix<common_type>&, const matrix<common_type>&);

					auto a11_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 0, 0, std::ref(a));
					auto a12_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 0, 1, std::ref(a));
					auto a21_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 1, 0, std::ref(a));
					auto a22_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 1, 1, std::ref(a));

					auto b11_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::ref(b));
					auto b12_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::ref(b));
					auto b21_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::ref(b));
					auto b22_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::ref(b));

					const auto a11 = a11_future.get();
					const auto a22 = a22_future.get();

					const auto b11 = b11_future.get();
					const auto b22 = b22_future.get();
					auto m1_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a11 + a22, b11 + b22);

					const auto a21 = a21_future.get();
					auto m2_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a21 + a22, std::ref(b11));

					const auto b12 = b12_future.get();
					auto m3_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), std::ref(a11), b12 - b22);
					auto m6_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a21 - a11, b11 + b12);

					const auto a12 = a12_future.get();
					auto m5_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a11 + a12, std::ref(b22));

					const auto b21 = b21_future.get();
					auto m4_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), std::ref(a22), b21 - b11);
					auto m7_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a12 - a22, b21 + b22);

					matrix<common_type> c {a.rows(), a.cols()};

					const auto m1 = m1_future.get();
					const auto m4 = m4_future.get();
					const auto m5 = m5_future.get();
					const auto m7 = m7_future.get();
					const auto v1 = std::async(std::launch::async, static_cast<set_part_f_t>(set_part), 0, 0, std::ref(c), m1 + m4 - m5 + m7);

					const auto m3 = m3_future.get();
					const auto v2 = std::async(std::launch::async, static_cast<set_part_f_t>(set_part), 0, 1, std::ref(c), m3 + m5);

					const auto m2 = m2_future.get();
					const auto v3 = std::async(std::launch::async, static_cast<set_part_f_t>(set_part), 1, 0, std::ref(c), m2 + m4);

					const auto m6 = m6_future.get();
					const auto v4 = std::async(std::launch::async, static_cast<set_part_f_t>(set_part), 1, 1, std::ref(c), m1 - m2 + m3 + m6);

					v1.wait();
					v2.wait();
					v3.wait();
					v4.wait();

					return c;
				}
			}

			template<class T, class T2, std::size_t Rows, std::size_t Cols>
			[[nodiscard]] auto mult_strassen(const fixed_matrix<T, Rows, Cols>& a, const matrix<T2>& b) noexcept
			{
				if constexpr (Rows < strassen_dim_cutoff)
				{
					return mult_std(a, b);
				}
				else
				{
					using get_part_a_t = std::remove_cvref_t<decltype(a)>(*)(std::size_t, std::size_t, decltype(a));
					using get_part_b_t = std::remove_cvref_t<decltype(b)>(*)(std::size_t, std::size_t, decltype(b));
					using common_type = std::common_type_t<T, T2>;
					using f_t = matrix<common_type>(*)(decltype(a), decltype(b));
					using set_part_f_t = void(*)(std::size_t, std::size_t, matrix<common_type>&, const matrix<common_type>&);

					auto a11_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 0, 0, std::ref(a));
					auto a12_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 0, 1, std::ref(a));
					auto a21_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 1, 0, std::ref(a));
					auto a22_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 1, 1, std::ref(a));

					auto b11_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::ref(b));
					auto b12_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::ref(b));
					auto b21_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::ref(b));
					auto b22_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::ref(b));

					const auto a11 = a11_future.get();
					const auto a22 = a22_future.get();

					const auto b11 = b11_future.get();
					const auto b22 = b22_future.get();
					auto m1_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a11 + a22, b11 + b22);

					const auto a21 = a21_future.get();
					auto m2_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a21 + a22, std::ref(b11));

					const auto b12 = b12_future.get();
					auto m3_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), std::ref(a11), b12 - b22);
					auto m6_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a21 - a11, b11 + b12);

					const auto a12 = a12_future.get();
					auto m5_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a11 + a12, std::ref(b22));

					const auto b21 = b21_future.get();
					auto m4_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), std::ref(a22), b21 - b11);
					auto m7_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a12 - a22, b21 + b22);

					matrix<common_type> c {a.rows(), a.cols()};

					const auto m1 = m1_future.get();
					const auto m4 = m4_future.get();
					const auto m5 = m5_future.get();
					const auto m7 = m7_future.get();
					const auto v1 = std::async(std::launch::async, static_cast<set_part_f_t>(set_part), 0, 0, std::ref(c), m1 + m4 - m5 + m7);

					const auto m3 = m3_future.get();
					const auto v2 = std::async(std::launch::async, static_cast<set_part_f_t>(set_part), 0, 1, std::ref(c), m3 + m5);

					const auto m2 = m2_future.get();
					const auto v3 = std::async(std::launch::async, static_cast<set_part_f_t>(set_part), 1, 0, std::ref(c), m2 + m4);

					const auto m6 = m6_future.get();
					const auto v4 = std::async(std::launch::async, static_cast<set_part_f_t>(set_part), 1, 1, std::ref(c), m1 - m2 + m3 + m6);

					v1.wait();
					v2.wait();
					v3.wait();
					v4.wait();

					return c;
				}
			}

			template<class T, class T2, std::size_t Rows>
			[[nodiscard]] constexpr auto mult_strassen(const square_matrix<T, Rows>& a, const square_matrix<T2, Rows>& b) noexcept
			{
				if constexpr (Rows < strassen_dim_cutoff)
				{
					return mult_std(a, b);
				}
				else if constexpr(std::is_constant_evaluated())
				{
					const auto a11 = get_part(0, 0, a);
					const auto a12 = get_part(0, 1, a);
					const auto a21 = get_part(1, 0, a);
					const auto a22 = get_part(1, 1, a);

					const auto b11 = get_part(0, 0, b);
					const auto b12 = get_part(0, 1, b);
					const auto b21 = get_part(1, 0, b);
					const auto b22 = get_part(1, 1, b);

					const auto m1 = mult_strassen(a11 + a22, b11 + b22);
					const auto m2 = mult_strassen(a21 + a22, b11);
					const auto m3 = mult_strassen(a11, b12 - b22);
					const auto m4 = mult_strassen(a22, b21 - b11);
					const auto m5 = mult_strassen(a11 + a12, b22);
					const auto m6 = mult_strassen(a21 - a11, b11 + b12);
					const auto m7 = mult_strassen(a12 - a22, b21 + b22);

					square_matrix<std::common_type_t<T, T2>, Rows> c;
					set_part(0, 0, c, m1 + m4 - m5 + m7);
					set_part(0, 1, c, m3 + m5);
					set_part(1, 0, c, m2 + m4);
					set_part(1, 1, c, m1 - m2 + m3 + m6);

					return c;
				}
				else
				{
					using get_part_a_t = std::remove_cvref_t<decltype(a)>(*)(std::size_t, std::size_t, decltype(a));
					using get_part_b_t = std::remove_cvref_t<decltype(b)>(*)(std::size_t, std::size_t, decltype(b));
					using common_type = std::common_type_t<T, T2>;
					using f_t = matrix<common_type>(*)(decltype(a), decltype(b));
					using set_part_f_t = void(*)(std::size_t, std::size_t, matrix<common_type>&, const matrix<common_type>&);

					auto a11_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 0, 0, std::ref(a));
					auto a12_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 0, 1, std::ref(a));
					auto a21_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 1, 0, std::ref(a));
					auto a22_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 1, 1, std::ref(a));

					auto b11_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::ref(b));
					auto b12_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::ref(b));
					auto b21_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::ref(b));
					auto b22_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::ref(b));

					const auto a11 = a11_future.get();
					const auto a22 = a22_future.get();

					const auto b11 = b11_future.get();
					const auto b22 = b22_future.get();
					auto m1_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a11 + a22, b11 + b22);

					const auto a21 = a21_future.get();
					auto m2_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a21 + a22, std::ref(b11));

					const auto b12 = b12_future.get();
					auto m3_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), std::ref(a11), b12 - b22);
					auto m6_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a21 - a11, b11 + b12);

					const auto a12 = a12_future.get();
					auto m5_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a11 + a12, std::ref(b22));

					const auto b21 = b21_future.get();
					auto m4_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), std::ref(a22), b21 - b11);
					auto m7_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a12 - a22, b21 + b22);

					square_matrix<std::common_type_t<T, T2>, Rows> c;

					const auto m1 = m1_future.get();
					const auto m4 = m4_future.get();
					const auto m5 = m5_future.get();
					const auto m7 = m7_future.get();
					const auto v1 = std::async(std::launch::async, static_cast<set_part_f_t>(set_part), 0, 0, std::ref(c), m1 + m4 - m5 + m7);

					const auto m3 = m3_future.get();
					const auto v2 = std::async(std::launch::async, static_cast<set_part_f_t>(set_part), 0, 1, std::ref(c), m3 + m5);

					const auto m2 = m2_future.get();
					const auto v3 = std::async(std::launch::async, static_cast<set_part_f_t>(set_part), 1, 0, std::ref(c), m2 + m4);

					const auto m6 = m6_future.get();
					const auto v4 = std::async(std::launch::async, static_cast<set_part_f_t>(set_part), 1, 1, std::ref(c), m1 - m2 + m3 + m6);

					v1.wait();
					v2.wait();
					v3.wait();
					v4.wait();

					return c;
				}
			}
		}

		template<matrix_type Matrix, class Other>
			requires(!matrix_type<Other>)
		[[nodiscard]] constexpr Matrix operator*(const Matrix& lhs, const Other& rhs) noexcept
		{
			auto m {lhs};
			m *= rhs;
			return m;
		}

		template<matrix_type Matrix, matrix_type Matrix2>
		[[nodiscard]] auto operator*(const Matrix& lhs, const Matrix2& rhs)
		{
			if (lhs.cols() != rhs.rows())
			{
				throw std::invalid_argument {"cannot multiply matrices that do not share columns with rows"};
			}

			if (is_square_matrix(lhs) && is_square_matrix(rhs))
			{
				return details::mult_strassen(lhs, rhs);
			}
			else
			{
				return details::mult_std(lhs, rhs);
			}
		}

		template<class T, class T2, std::size_t Rows, std::size_t Cols, std::size_t Cols2>
		[[nodiscard]] constexpr auto operator*(const fixed_matrix<T, Rows, Cols>& lhs, const fixed_matrix<T2, Cols, Cols2>& rhs) noexcept
		{
			if constexpr (Rows == Cols && Cols == Cols2)
			{
				return details::mult_strassen(lhs, rhs);
			}
			else
			{
				return details::mult_std(lhs, rhs);
			}
		}

		template<matrix_type Matrix, class Other>
			requires(!matrix_type<Other>)
		[[nodiscard]] constexpr Matrix operator/(const Matrix& lhs, const Other& rhs) noexcept
		{
			auto m{lhs};
			m /= rhs;
			return m;
		}

		template<class Rep, std::size_t Rows, std::size_t Cols>
		class fixed_matrix
		{
			using storage_t = multidimensional::constant_multidimensional_array<Rep, Rows, Cols>;

			public:
				using value_type = typename storage_t::value_type;
				using size_type = typename storage_t::size_type;
				using difference_type = typename storage_t::difference_type;
				using reference = typename storage_t::reference;
				using const_reference = typename storage_t::const_reference;
				using pointer = typename storage_t::pointer;
				using const_pointer = typename storage_t::const_pointer;
				using iterator = typename storage_t::iterator;
				using const_iterator = typename storage_t::const_iterator;
				using reverse_iterator = typename storage_t::reverse_iterator;
				using const_reverse_iterator = typename storage_t::const_reverse_iterator;

				static constexpr auto num_rows = Rows;
				static constexpr auto num_columns = Cols;

				constexpr fixed_matrix() = default;

				constexpr fixed_matrix(const fixed_matrix&) = default;

				template<collin::concepts::convertible_to<Rep> Rep2>
				constexpr fixed_matrix(const fixed_matrix<Rep2, Rows, Cols>& other)
					: data_{other.data_} {}

				constexpr fixed_matrix(fixed_matrix&&) noexcept = default;

				template<collin::concepts::convertible_to<Rep>... Elements>
					requires (sizeof...(Elements) > 0)
				constexpr fixed_matrix(Elements&&... elements)
					: data_{std::forward<Elements>(elements)...} {}

				template<collin::concepts::convertible_to<Rep> Rep2>
				constexpr fixed_matrix& operator=(const fixed_matrix<Rep2, Rows, Cols>& other)
				{
					if (this != std::addressof(other))
					{
						data_ = other.data_;
					}

					return *this;
				}

				constexpr fixed_matrix& operator=(const fixed_matrix&) = default;
				constexpr fixed_matrix& operator=(fixed_matrix&&) noexcept = default;

				[[nodiscard]] constexpr std::size_t rows() const noexcept
				{
					return Rows;
				}

				[[nodiscard]] constexpr std::size_t cols() const noexcept
				{
					return Cols;
				}

				constexpr reference operator()(std::size_t r, std::size_t c) noexcept
				{	
					return data_.get(r, c);
				}

				constexpr const_reference operator()(std::size_t r, std::size_t c) const noexcept
				{	
					return data_.get(r, c);
				}

				constexpr reference operator[](std::size_t index) noexcept
				{
					return data_[index];
				}

				constexpr const_reference operator[](std::size_t index) const noexcept
				{
					return data_[index];
				}

				[[nodiscard]] constexpr row<value_type, Cols> get_row(std::size_t i) noexcept
				{
					return {&data_.get(i, 0), Cols};
				}

				[[nodiscard]] constexpr row<const value_type, Cols> get_row(std::size_t i) const noexcept
				{
					return {&data_.get(i, 0), Cols};
				}

				[[nodiscard]] constexpr column<value_type, Rows, Cols> get_column(std::size_t i) noexcept
				{
					return {&data_.get(0, i), Rows, Cols};
				}

				[[nodiscard]] constexpr column<const value_type, Rows, Cols> get_column(std::size_t i) const noexcept
				{
					return {&data_.get(0, i), Rows, Cols};
				}

				[[nodiscard]] constexpr const_pointer data() const noexcept
				{
					return data_.data();
				}

				[[nodiscard]] constexpr pointer data() noexcept
				{
					return data_.data();
				}

				[[nodiscard]] constexpr iterator begin() noexcept
				{
					return data_.begin();
				}

				[[nodiscard]] constexpr const_iterator begin() const noexcept
				{
					return data_.begin();
				}

				[[nodiscard]] constexpr const_iterator cbegin() const noexcept
				{
					return data_.cbegin();
				}

				[[nodiscard]] constexpr iterator end() noexcept
				{
					return data_.end();
				}

				[[nodiscard]] constexpr const_iterator end() const noexcept
				{
					return data_.end();
				}

				[[nodiscard]] constexpr const_iterator cend() const noexcept
				{
					return data_.cend();
				}

				[[nodiscard]] constexpr reverse_iterator rbegin() noexcept
				{
					return data_.rbegin();
				}

				[[nodiscard]] constexpr reverse_iterator rbegin() const noexcept
				{
					return data_.rbegin();
				}

				[[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
				{
					return data_.crbegin();
				}

				[[nodiscard]] constexpr reverse_iterator rend() noexcept
				{
					return data_.rend();
				}

				[[nodiscard]] constexpr reverse_iterator rend() const noexcept
				{
					return data_.rend();
				}

				[[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
				{
					return data_.crend();
				}

				[[nodiscard]] constexpr size_type size() const noexcept
				{
					return data_.size();
				}

				template<class Rep2>
					requires(requires(const Rep& t, const Rep2& t2) { { t + t2 } -> collin::concepts::convertible_to<Rep>; })
				fixed_matrix& operator+=(const matrix<Rep2>& other)
				{
					if (Rows != other.rows() || Cols != other.cols())
					{
						throw std::invalid_argument{"cannot add matrices of different dimension sizes"};
					}
					transform_with(other, std::plus<>{});
					return *this;
				}

				template<class Rep2>
					requires(requires(const Rep& t, const Rep2& t2) { { t + t2 } -> collin::concepts::convertible_to<Rep>; })
				constexpr fixed_matrix& operator+=(const fixed_matrix<Rep2, Rows, Cols>& other) noexcept
				{
					transform_with(other, std::plus<>{});
					return *this;
				}

				template<class Rep2>
					requires(requires(const Rep& t, const Rep2& t2) { { t - t2 } -> collin::concepts::convertible_to<Rep>; })
				fixed_matrix& operator-=(const matrix<Rep2>& other)
				{
					if (Rows != other.rows() || Cols != other.cols())
					{
						throw std::invalid_argument{"cannot subtract matrices of different dimension sizes"};
					}
					transform_with(other, std::minus<>{});
					return *this;
				}

				template<class Rep2>
					requires(requires(const Rep& t, const Rep2& t2) { { t - t2 } -> collin::concepts::convertible_to<Rep>; })
				constexpr fixed_matrix& operator-=(const fixed_matrix<Rep2, Rows, Cols>& other) noexcept
				{
					transform_with(other, std::minus<>{});
					return *this;
				}

				template<class Rep2>
					requires(requires(const Rep& t, const Rep2& t2) { { t * t2 } -> collin::concepts::convertible_to<Rep>; })
				constexpr fixed_matrix& operator*=(const Rep2& other) noexcept
				{
					transform_with(other, std::multiplies<>{});
					return *this;
				}

				template<class Rep2>
					requires(requires(const Rep& t, const Rep2& t2) { { t / t2 } -> collin::concepts::convertible_to<Rep>; })
				constexpr fixed_matrix& operator/=(const Rep2& other) noexcept
				{
					transform_with(other, std::divides<>{});
					return *this;
				}

				template<class Rep2>
					requires(requires(const Rep& t, const Rep2& t2) { { t % t2 } -> collin::concepts::convertible_to<Rep>; })
				constexpr fixed_matrix& operator%=(const Rep2& other) noexcept
				{
					transform_with(other, std::modulus<>{});
					return *this;
				}
			private:
				storage_t data_;

				template<class T>
				friend class matrix;

				template<class Rep2, class F>
				constexpr void transform_with(const matrix<Rep2>& other, F&& f) noexcept
				{
					std::transform(std::execution::par_unseq, begin(), end(), other.begin(), begin(), std::forward<F>(f));
				}

				template<class Rep2, class F>
				constexpr void transform_with(const fixed_matrix<Rep2, Rows, Cols>& other, F&& f) noexcept
				{
					if constexpr (std::is_constant_evaluated())
					{
						std::transform(begin(), end(), other.begin(), begin(), std::forward<F>(f));
					}
					else
					{
						std::transform(std::execution::par_unseq, begin(), end(), other.begin(), begin(), std::forward<F>(f));
					}
				}

				template<class T2, class F>
				constexpr void transform_with(const T2& other, F&& f) noexcept
				{
					if constexpr (std::is_constant_evaluated())
					{
						std::transform(begin(), end(), begin(), [&other, f2 = std::forward<F>(f)](const auto& v) { return f2(v, other); });
					}
					else
					{
						std::transform(std::execution::par_unseq, begin(), end(), begin(), [&other, f2 = std::forward<F>(f)](const auto& v) { return f2(v, other); });
					}
				}
		};

		template<class T>
		[[nodiscard]] bool is_square_matrix(const matrix<T>& m) noexcept
		{
			return m.rows() == m.cols();
		}

		template<class T, std::size_t Rows, std::size_t Cols>
		[[nodiscard]] constexpr bool is_square_matrix(const fixed_matrix<T, Rows, Cols>&) noexcept
		{
			return Rows == Cols;
		}

		template<class T>
		[[nodiscard]] diagonal<T, collin::dynamic_extent> main_diagonal(matrix<T>& m)
		{
			if (!is_square_matrix(m))
			{
				throw std::invalid_argument{"can only find the main diagonal of a square matrix"};
			}

			return {m.data(), m.rows()};
		}

		template<class T>
		[[nodiscard]] diagonal<const T, collin::dynamic_extent> main_diagonal(const matrix<T>& m)
		{
			if (!is_square_matrix(m))
			{
				throw std::invalid_argument{"can only find the main diagonal of a square matrix"};
			}

			return {m.data(), m.rows()};
		}

		template<class T, std::size_t Size>
		[[nodiscard]] constexpr diagonal<T, Size> main_diagonal(square_matrix<T, Size>& m) noexcept
		{
			return {m.data(), Size};
		}

		template<class T, std::size_t Size>
		[[nodiscard]] constexpr diagonal<const T, Size> main_diagonal(const square_matrix<T, Size>& m) noexcept
		{
			return {m.data(), Size};
		}

		template<class T, std::size_t S>
		[[nodiscard]] constexpr square_matrix<T, S> identity_matrix() noexcept
		{
			square_matrix<T, S> m;
			auto diag = main_diagonal(m);
			std::fill(std::begin(diag), std::end(diag), T{1});

			return m;
		}

		template<class T>
		[[nodiscard]] matrix<T> identity_matrix(std::size_t size) noexcept
		{
			matrix<T> m{size, size};
			auto diag = main_diagonal(m);
			std::fill(std::begin(diag), std::end(diag), T{1});

			return m;
		}
	}
}