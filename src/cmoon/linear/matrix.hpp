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

#include "cmoon/multidimensional.hpp"
#include "cmoon/utility.hpp"

namespace cmoon
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

				constexpr row(pointer first, std::size_t size)
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

				template<size_type Count>
				[[nodiscard]] constexpr row<element_type, Count> first() const noexcept
				{
					return {data(), Count};
				}

				[[nodiscard]] constexpr row<element_type, std::dynamic_extent> first(size_type count) const noexcept
				{
					return {data(), count};
				}

				template<size_type Count>
				[[nodiscard]] constexpr row<element_type, Count> last() const noexcept
				{
					return {data() + (size() - Count), Count};
				}

				[[nodiscard]] constexpr row<element_type, std::dynamic_extent> last(size_type count) const noexcept
				{
					return {data() + (size() - count), count};
				}

				template<size_type Offset, size_type Count = std::dynamic_extent>
				[[nodiscard]] constexpr row<element_type, (Count != std::dynamic_extent) ? Count : (Extent != std::dynamic_extent) ? Extent - Offset : std::dynamic_extent>
					subrow() const noexcept
				{
					return {data() + Offset, (Count == std::dynamic_extent) ? size() - Offset : Count};
				}

				[[nodiscard]] constexpr row<element_type, std::dynamic_extent> subrow(size_type offset, size_type count = std::dynamic_extent) const noexcept
				{
					return {data() + offset, (count == std::dynamic_extent) ? size() - offset : count};
				}

				template<class T2>
					requires(requires(const_reference t, const T2& t2) { { t + t2 } -> std::convertible_to<T>; })
				constexpr row& operator+=(const row<T2, Extent>& other) noexcept
				{
					std::ranges::transform(*this, other, begin(), std::plus<>{});
					return *this;
				}

				template<class T2>
					requires(requires(const_reference t, const T2& t2) { { t - t2 } -> std::convertible_to<T>; })
				constexpr row& operator-=(const row<T2, Extent>& other) noexcept
				{
					std::ranges::transform(*this, other, begin(), std::minus<>{});
					return *this;
				}

				constexpr row& operator*=(const_reference other) noexcept
				{
					std::ranges::transform(*this, begin(), [&other](const auto& v) { return v * other; });
					return *this;
				}

				constexpr row& operator/=(const_reference other) noexcept
				{
					std::ranges::transform(*this, begin(), [&other](const auto& v) { return v / other; });
					return *this;
				}

				constexpr row& operator%=(const_reference other) noexcept
				{
					std::ranges::transform(*this, begin(), [&other](const auto& v) { return v % other; });
					return *this;
				}

				constexpr value_type sum() const noexcept
				{
					return std::accumulate(begin(), end(), value_type{});
				}

				constexpr value_type product() const noexcept
				{
					return std::accumulate(begin(), end(), value_type{ 1 }, std::multiplies<>{});
				}

				constexpr void swap(row rhs) noexcept
				{
					std::ranges::swap_ranges(*this, rhs);
				}

				[[nodiscard]] constexpr operator row<const T, Extent>() const noexcept
				{
					return {data(), size()};
				}
		};

		template<class T, class T2, std::size_t Extent, std::size_t Extent2>
			requires(requires(const T& t, const T2& t2){ {t == t2} -> std::convertible_to<bool>; })
		[[nodiscard]] constexpr bool operator==(const row<T, Extent>& lhs, const row<T2, Extent2>& rhs) noexcept
		{
			if constexpr (Extent != Extent2 && (Extent != std::dynamic_extent && Extent2 != std::dynamic_extent))
			{
				return false;
			}
			else
			{
				return std::ranges::equal(lhs, rhs);
			}
		}

		template<class T, class T2, std::size_t Extent, std::size_t Extent2>
			requires(requires(const T& t, const T2& t2){ {t == t2} -> std::convertible_to<bool>; })
		[[nodiscard]] constexpr bool operator!=(const row<T, Extent>& lhs, const row<T2, Extent2>& rhs) noexcept
		{
			return !(lhs == rhs);
		}

		template<class T, std::size_t RowSize>
		class column_iterator
		{
			public:
				using iterator_category = std::bidirectional_iterator_tag;
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

				[[nodiscard]] constexpr column_iterator operator-(difference_type amount) noexcept
				{
					auto c_it{*this};
					c_it -= amount;
					return c_it;
				}

				[[nodiscard]] constexpr difference_type operator-(const column_iterator& rhs) noexcept
				{
					return (storage_.data_ - rhs.storage_.data_) / storage_.size();
				}

				constexpr reference operator*() noexcept
				{
					return *storage_.data_;
				}

				constexpr const reference operator*() const noexcept
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

				[[nodiscard]] constexpr column_iterator operator+(std::size_t amount) noexcept
				{
					auto c_it{*this};
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

				[[nodiscard]] constexpr bool operator<(const column_iterator& other) const noexcept
				{
					return storage_.data_ < other.storage_.data_;
				}

				[[nodiscard]] constexpr bool operator>(const column_iterator& other) const noexcept
				{
					return other < *this;
				}

				[[nodiscard]] constexpr bool operator<=(const column_iterator& other) const noexcept
				{
					return !(other < *this);
				}

				[[nodiscard]] constexpr bool operator>=(const column_iterator& other) const noexcept
				{
					return !(*this < other);
				}

				constexpr pointer operator->()
				{
					return storage_.data_;
				}

				constexpr const pointer operator->() const
				{
					return storage_.data_;
				}

				constexpr reference operator[](difference_type amount) noexcept
				{
					return *(*this + amount);
				}

				constexpr const reference operator[](difference_type amount) const noexcept
				{
					return *(*this + amount);
				}

				constexpr void swap(column_iterator& rhs) noexcept
				{
					std::swap(storage_, rhs.storage_);
				}
			private:
				template<class ExtentType>
				struct storage_type : public ExtentType
				{
					template<class OtherExtentType>
					constexpr storage_type(pointer data, OtherExtentType ext) noexcept
						: ExtentType(ext), data_(data) {}

					pointer data_;

					constexpr friend void swap(storage_type& lhs, storage_type& rhs) noexcept
					{
						std::swap(static_cast<ExtentType&>(lhs), static_cast<ExtentType&>(rhs));
						std::swap(lhs.data_, rhs.data_);
					}
				};

				storage_type<cmoon::extent_type<RowSize>> storage_;
		};

		template<class T, std::size_t Extent, std::size_t RowSize>
		class column
		{
			public:
				using element_type = T;
				using value_type = std::remove_cv_t<element_type>;
				using size_type = std::size_t;
				using difference_type = std::ptrdiff_t;
				using pointer = element_type*;
				using const_pointer = const element_type*;
				using reference = element_type&;
				using const_reference = const element_type&;
				using iterator = column_iterator<element_type, RowSize>;
				using reverse_iterator = std::reverse_iterator<iterator>;

				constexpr column(pointer first, size_type size, size_type row_size) noexcept
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
					return std::make_reverse_iterator(end());
				}

				[[nodiscard]] constexpr reverse_iterator rend() const noexcept
				{
					return std::make_reverse_iterator(begin());
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
					return storage_.data_;
				}

				[[nodiscard]] constexpr size_type size() const noexcept
				{
					return storage_.main_size();
				}

				[[nodiscard]] constexpr size_type row_size() const noexcept
				{
					return storage_.row_size();
				}

				[[nodiscard]] constexpr bool empty() const noexcept
				{
					return size() == 0;
				}

				template<size_type Count>
				[[nodiscard]] constexpr column<element_type, Count, RowSize> first() const noexcept
				{
					return {data(), Count, row_size()};
				}

				[[nodiscard]] constexpr column<element_type, std::dynamic_extent, RowSize> first(size_type count) const noexcept
				{
					return {data(), count, row_size()};
				}

				template<size_type Count>
				[[nodiscard]] constexpr column<element_type, Count, RowSize> last() const noexcept
				{
					return {data() + ((size() - Count) * row_size()), Count, row_size()};
				}

				[[nodiscard]] constexpr column<element_type, std::dynamic_extent, RowSize> last(size_type count) const noexcept
				{
					return {data() + ((size() - count) * row_size()), count, row_size()};
				}

				template<size_type Offset, size_type Count = std::dynamic_extent>
				[[nodiscard]] constexpr column<element_type, (Count != std::dynamic_extent) ? Count : (Extent != std::dynamic_extent) ? Extent - Offset : std::dynamic_extent, RowSize>
					subcolumn() const noexcept
				{
					return {data() + Offset * row_size(), (Count == std::dynamic_extent) ? size() - Offset : Count, row_size()};
				}

				[[nodiscard]] constexpr column<element_type, std::dynamic_extent, RowSize> subcolumn(size_type offset, size_type count = std::dynamic_extent) const noexcept
				{
					return {data() + offset * row_size(), (count == std::dynamic_extent) ? size() - offset : count, row_size()};
				}

				template<class T2>
					requires(requires(const_reference t, const T2& t2) { { t + t2 } -> std::convertible_to<T>; })
				constexpr column& operator+=(const column<T2, Extent, RowSize>& other) noexcept
				{
					std::ranges::transform(*this, other, begin(), std::plus<>{});
					return *this;
				}

				template<class T2>
					requires(requires(const_reference t, const T2& t2) { { t - t2 } -> std::convertible_to<T>; })
				constexpr column& operator-=(const column<T2, Extent, RowSize>& other) noexcept
				{
					std::ranges::transform(*this, other, begin(), std::minus<>{});
					return *this;
				}

				constexpr column& operator*=(const_reference other) noexcept
				{
					std::ranges::transform(*this, begin(), [&other](const auto& v) { return v * other; });
					return *this;
				}

				constexpr column& operator/=(const_reference other) noexcept
				{
					std::ranges::transform(*this, begin(), [&other](const auto& v) { return v / other; });
					return *this;
				}

				constexpr column& operator%=(const_reference other) noexcept
				{
					std::ranges::transform(*this, begin(), [&other](const auto& v) { return v % other; });
					return *this;
				}

				constexpr value_type sum() const noexcept
				{
					return std::accumulate(begin(), end(), value_type{});
				}

				constexpr value_type product() const noexcept
				{
					return std::accumulate(begin(), end(), value_type{ 1 }, std::multiplies<>{});
				}

				constexpr void swap(column rhs) noexcept
				{
					std::swap_ranges(begin(), end(), rhs.begin());
				}

				[[nodiscard]] constexpr operator column<const value_type, Extent, RowSize>() const noexcept
				{
					return {data(), size(), row_size()};
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

					ExtentType ex_;
					ExtentType2 ex2_;
					pointer data_;
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
				// Technically, both sizes should either be dynamic or compile-time.
				std::conditional_t<Extent == std::dynamic_extent || RowSize == std::dynamic_extent,
					storage_type<cmoon::extent_type<Extent>, cmoon::extent_type<RowSize>>,
					storage_type2<cmoon::extent_type<Extent>, cmoon::extent_type<RowSize>>> storage_;
		};

		template<class T, class T2, std::size_t Extent, std::size_t RowSize, std::size_t Extent2, std::size_t RowSize2>
			requires(requires(const T& t, const T2& t2){ {t == t2} -> std::convertible_to<bool>; })
		[[nodiscard]] constexpr bool operator==(const column<T, Extent, RowSize>& lhs, const column<T2, Extent2, RowSize2>& rhs) noexcept
		{
			if constexpr ((Extent != Extent2 || RowSize != RowSize2) && (Extent != std::dynamic_extent && Extent2 != std::dynamic_extent) && (RowSize != std::dynamic_extent && RowSize2 != std::dynamic_extent))
			{
				return false;
			}
			else
			{
				if(std::size(lhs) != std::size(rhs) || lhs.row_size() != rhs.row_size())
				{
					return false;
				}

				return std::ranges::equal(lhs, rhs);
			}
		}

		template<class T, class T2, std::size_t Extent, std::size_t RowSize>
			requires(requires(const T& t, const T2& t2){ {t == t2} -> std::convertible_to<bool>; })
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
				using iterator_category = std::bidirectional_iterator_tag;
				using value_type = T;
				using difference_type = std::ptrdiff_t;
				using pointer = value_type*;
				using reference = value_type&;

				constexpr diagonal_iterator() = default;

				constexpr diagonal_iterator(pointer item, const diagonal<T, RowSize>* owner) noexcept
					: current_{item}, owner_{owner} {}

				constexpr diagonal_iterator& operator--() noexcept
				{
					current_ -= 1;
					if (distance_to_beginning() != -1 &&
						distance_to_end() != -1)
					{
						current_ -= owner_->size();
					}

					return *this;
				}

				constexpr diagonal_iterator operator--(int) noexcept
				{
					auto temp = *this;
					--(*this);
					return temp;
				}

				[[nodiscard]] constexpr reference operator*() noexcept
				{
					return *current_;
				}

				[[nodiscard]] constexpr const reference operator*() const noexcept
				{
					return *current_;
				}

				constexpr diagonal_iterator& operator++() noexcept
				{
					current_ += 1;

					if (distance_to_beginning() != 0 &&
						distance_to_end() != 0)
					{
						current_ += owner_->size();
					}

					return *this;
				}

				constexpr diagonal_iterator operator++(int) noexcept
				{
					auto temp = *this;
					++(*this);
					return temp;
				}

				constexpr diagonal_iterator& operator-=(difference_type amount) noexcept
				{
					const auto max_back = std::min(distance_to_beginning(), amount * static_cast<difference_type>(owner_->size()) - amount);
					if (distance_to_end() == 0 && max_back != distance_to_beginning())
					{
						current_ -= 1;
					}
					current_ -= max_back;
					return *this;
				}

				[[nodiscard]] constexpr diagonal_iterator operator-(difference_type amount) const noexcept
				{
					auto result {*this};
					result -= amount;
					return result;
				}

				[[nodiscard]] constexpr difference_type operator-(const diagonal_iterator& other) const noexcept
				{
					return (current_ - other.current_ + 1) / owner_->size();
				}

				constexpr diagonal_iterator& operator+=(difference_type amount) noexcept
				{
					const auto max_forward = std::min(distance_to_end(), amount * static_cast<difference_type>(owner_->size()) + amount);
					if (distance_to_beginning() == -1 && max_forward != distance_to_end())
					{
						current_ += 1;
					}
					current_ += max_forward;
					return *this;
				}

				[[nodiscard]] constexpr diagonal_iterator operator+(difference_type amount) const noexcept
				{
					auto result {*this};
					result += amount;
					return result;
				}

				[[nodiscard]] constexpr bool operator==(const diagonal_iterator& other) const noexcept
				{
					return current_ == other.current_;
				}

				[[nodiscard]] constexpr bool operator!=(const diagonal_iterator& other) const noexcept
				{
					return !(*this == other);
				}

				[[nodiscard]] constexpr bool operator<(const diagonal_iterator& other) const noexcept
				{
					return current_ < other.current_;
				}

				[[nodiscard]] constexpr bool operator>(const diagonal_iterator& other) const noexcept
				{
					return other < *this;
				}

				[[nodiscard]] constexpr bool operator<=(const diagonal_iterator& other) const noexcept
				{
					return !(other < *this);
				}

				[[nodiscard]] constexpr bool operator>=(const diagonal_iterator& other) const noexcept
				{
					return !(*this < other);
				}

				constexpr pointer operator->()
				{
					return current_;
				}

				constexpr const pointer operator->() const
				{
					return current_;
				}

				constexpr reference operator[](difference_type i) noexcept
				{
					return *(*this + i);
				}

				constexpr const reference operator[](difference_type i) const noexcept
				{
					return *(*this + i);
				}

				constexpr void swap(diagonal_iterator& rhs) noexcept
				{
					std::swap(current_, rhs.current_);
					std::swap(owner_, rhs.owner_);
				}
			private:
				pointer current_;

				[[nodiscard]] constexpr difference_type distance_to_beginning() const noexcept
				{
					return current_ - owner_->begin().current_;
				}

				[[nodiscard]] constexpr difference_type distance_to_end() const noexcept
				{
					return current_ - owner_->end().current_;
				}

				// C++20 gives constexpr std::reference_wrapper,
				// so we's gotta wait until then and use a crappy pointer.
				//
				// It is rather strange to hold the owner who makes the iterator
				// but unfortunately, when we iterate through the diagonal, we have
				// a tendency to iterate far past the end, which doesn't work well
				// in constexpr conditions. We use the owner to make sure our current
				// pointer does not go too far past the beginning or end of the container.
				
				const diagonal<T, RowSize>* owner_;
		};

		// We are assuming a square matrix. Maybe I'm kind enough
		// to make this work for MxN matrix sometime.
		template<class T, std::size_t Size>
		class diagonal
		{
			public:
				using element_type = T;
				using value_type = std::remove_cv_t<element_type>;
				using size_type = std::size_t;
				using difference_type = std::ptrdiff_t;
				using pointer = element_type*;
				using const_pointer = const pointer;
				using reference = element_type&;
				using const_reference = const reference;
				using iterator = diagonal_iterator<element_type, Size>;
				using reverse_iterator = std::reverse_iterator<iterator>;

				constexpr diagonal(pointer first, size_type size)
					: storage_{first, size} {}

				[[nodiscard]] constexpr iterator begin() const noexcept
				{
					return {data(),this};
				}

				[[nodiscard]] constexpr iterator end() const noexcept
				{
					return {data() + storage_.size() * storage_.size(), this};
				}

				[[nodiscard]] constexpr reverse_iterator rbegin() const noexcept
				{
					return std::make_reverse_iterator(end());
				}

				[[nodiscard]] constexpr reverse_iterator rend() const noexcept
				{
					return std::make_reverse_iterator(begin());
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

				template<size_type Count>
				[[nodiscard]] constexpr diagonal<element_type, Count> first() const noexcept
				{
					return {data(), Count};
				}

				[[nodiscard]] constexpr diagonal<element_type, std::dynamic_extent> first(size_type count) const noexcept
				{
					return {data(), count};
				}

				template<size_type Count>
				[[nodiscard]] constexpr diagonal<element_type, std::dynamic_extent> last() const noexcept
				{
					return {data() + ((size() - Count) * size()), Count};
				}

				[[nodiscard]] constexpr diagonal<element_type, std::dynamic_extent> last(size_type count) const noexcept
				{
					return {data() + ((size() - count) * size()), count};
				}

				template<size_type Offset, size_type Count = std::dynamic_extent>
				[[nodiscard]] constexpr diagonal<element_type, (Count != std::dynamic_extent) ? Count : (Size != std::dynamic_extent) ? Size - Offset : std::dynamic_extent>
					subdiagonal() const noexcept
				{
					return {data() + Offset * size(), (Count == std::dynamic_extent) ? size() - Offset : Count};
				}

				[[nodiscard]] constexpr diagonal<element_type, std::dynamic_extent> subdiagonal(size_type offset, size_type count = std::dynamic_extent) const noexcept
				{
					return {data() + offset * size(), (count == std::dynamic_extent) ? size() - offset : count};
				}

				template<class T2>
					requires(requires(const_reference t, const T2& t2) { { t + t2 } -> std::convertible_to<T>; })
				constexpr diagonal& operator+=(const diagonal<T2, Size>& other) noexcept
				{
					std::ranges::transform(*this, other, begin(), std::plus<>{});
					return *this;
				}

				template<class T2>
					requires(requires(const_reference t, const T2& t2) { { t - t2 } -> std::convertible_to<T>; })
				constexpr diagonal& operator-=(const diagonal<T2, Size>& other) noexcept
				{
					std::ranges::transform(*this, other, begin(), std::minus<>{});
					return *this;
				}

				constexpr diagonal& operator*=(const_reference other) noexcept
				{
					std::ranges::transform(*this, begin(), [&other](const auto& v) { return v * other; });
					return *this;
				}

				constexpr diagonal& operator/=(const_reference other) noexcept
				{
					std::ranges::transform(*this, begin(), [&other](const auto& v) { return v / other; });
					return *this;
				}

				constexpr diagonal& operator%=(const_reference other) noexcept
				{
					std::ranges::transform(*this, begin(), [&other](const auto& v) { return v % other; });
					return *this;
				}

				constexpr value_type sum() const noexcept
				{
					return std::accumulate(begin(), end(), value_type{});
				}

				constexpr value_type product() const noexcept
				{
					return std::accumulate(begin(), end(), value_type{1}, std::multiplies<>{});
				}

				constexpr void swap(diagonal rhs) noexcept
				{
					std::ranges::swap_ranges(*this, rhs);
				}

				[[nodiscard]] constexpr operator diagonal<const T, Size>() const noexcept
				{
					return {data(), size()};
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

				storage_type<cmoon::extent_type<Size>> storage_;
		};

		template<class T, class T2, std::size_t Size, std::size_t Size2>
			requires(requires(const T& t, const T2& t2){ {t == t2} -> std::convertible_to<bool>; })
		[[nodiscard]] constexpr bool operator==(const diagonal<T, Size>& lhs, const diagonal<T2, Size2>& rhs) noexcept
		{
			if constexpr(Size != Size2 && (Size != std::dynamic_extent && Size2 != std::dynamic_extent))
			{
				return false;
			}
			else
			{
				if(std::size(lhs) != std::size(rhs))
				{
					return false;
				}

				return std::ranges::equal(lhs, rhs);
			}
		}

		template<class T, class T2, std::size_t Extent>
			requires(requires(const T& t, const T2& t2){ {t == t2} -> std::convertible_to<bool>; })
		[[nodiscard]] constexpr bool operator!=(const diagonal<T, Extent>& lhs, const diagonal<T2, Extent>& rhs) noexcept
		{
			return !(lhs == rhs);
		}

		template<class T, std::size_t Extent>
		class rows_iterator
		{
			public:
				using iterator_category = std::random_access_iterator_tag;
				using difference_type = std::ptrdiff_t;
				using value_type = row<T, Extent>;
				using pointer = value_type*;
				using reference = value_type&;

				constexpr rows_iterator(value_type start)
					: position_{start} {}

				constexpr rows_iterator& operator--() noexcept
				{
					position_ = value_type{position_.data() - position_.size(), position_.size()};
					return *this;
				}

				constexpr rows_iterator operator--(int) noexcept
				{
					auto temp {*this};
					--(*this);
					return temp;
				}

				constexpr rows_iterator& operator-=(difference_type amount) noexcept
				{
					position_ = value_type{position_.data() - (position_.size() * amount), position_.size()};
					return *this;
				}

				[[nodiscard]] friend constexpr rows_iterator operator-(const rows_iterator& lhs, difference_type amount) noexcept
				{
					auto result {lhs};
					result -= amount;
					return result;
				}

				constexpr rows_iterator& operator++() noexcept
				{
					position_ = value_type{position_.data() + position_.size(), position_.size()};
					return *this;
				}

				constexpr rows_iterator operator++(int) noexcept
				{
					auto temp {*this};
					++(*this);
					return temp;
				}

				constexpr rows_iterator& operator+=(difference_type amount) noexcept
				{
					position_ = value_type{position_.data() + (position_.size() * amount), position_.size()};
					return *this;
				}

				[[nodiscard]] friend constexpr rows_iterator operator+(const rows_iterator& lhs, difference_type amount) noexcept
				{
					auto result {lhs};
					result += amount;
					return result;
				}

				constexpr reference operator*() noexcept
				{
					return position_;
				}

				constexpr const reference operator*() const noexcept
				{
					return position_;
				}

				constexpr pointer operator->()
				{
					return std::addressof(position_);
				}

				constexpr const pointer operator->() const
				{
					return std::addressof(position_);
				}

				[[nodiscard]] constexpr bool operator==(const rows_iterator& other) const noexcept
				{
					return position_.data() == other.position_.data();
				}

				[[nodiscard]] constexpr bool operator!=(const rows_iterator& other) const noexcept
				{
					return !(*this == other);
				}
			private:
				value_type position_;
		};

		template<class T, std::size_t Size, std::size_t Extent>
		class rows_slice
		{
			public:
				using value_type = row<T, Extent>;
				using size_type = std::size_t;
				using difference_type = std::ptrdiff_t;
				using reference = value_type&;
				using const_reference = const reference;
				using pointer = value_type*;
				using const_pointer = const pointer;
				using iterator = rows_iterator<T, Extent>;
				using reverse_iterator = std::reverse_iterator<iterator>;

				constexpr rows_slice(value_type start, size_type size)
					: storage_{start, size} {}

				constexpr reference operator[](size_type pos) const noexcept
				{
					return *(begin() + pos);
				}

				constexpr reference at(size_type pos) const
				{
					if (pos >= size())
					{
						throw std::out_of_range{};
					}

					return operator[](pos);
				}
				
				constexpr reference front() const noexcept
				{
					return storage_.data_;
				}

				constexpr reference back() const noexcept
				{
					return *(end() - 1);
				}

				[[nodiscard]] constexpr iterator begin() const noexcept
				{
					return iterator{storage_.data_};
				}

				[[nodiscard]] constexpr iterator end() const noexcept
				{
					return iterator{storage_.data_} + size();
				}

				[[nodiscard]] constexpr reverse_iterator rbegin() const noexcept
				{
					return reverse_iterator{end()};
				}

				[[nodiscard]] constexpr reverse_iterator rend() const noexcept
				{
					return reverse_iterator{begin()};
				}

				[[nodiscard]] constexpr bool empty() const noexcept
				{
					return size() == 0;
				}

				[[nodiscard]] constexpr size_type size() const noexcept
				{
					return storage_.size();
				}

				[[nodiscard]] constexpr size_type row_size() const noexcept
				{
					return storage_.data_.size();
				}

				[[nodiscard]] constexpr operator rows_slice<const T, Size, Extent>() const noexcept
				{
					return {storage_.data_, size()};
				}
			private:
				template<class ExtentType>
				struct storage_type : public ExtentType
				{
					template<class OtherExtentType>
					constexpr storage_type(value_type data, OtherExtentType ex)
						: ExtentType{ex}, data_{data} {}

					value_type data_;
				};

				storage_type<cmoon::extent_type<Size>> storage_;
		};

		template<class T, std::size_t Extent, std::size_t RowSize>
		class columns_iterator
		{
			public:
				using iterator_category = std::random_access_iterator_tag;
				using difference_type = std::ptrdiff_t;
				using value_type = column<T, Extent, RowSize>;
				using pointer = value_type*;
				using reference = value_type&;

				constexpr columns_iterator(value_type start)
					: position_{start} {}

				constexpr columns_iterator& operator--() noexcept
				{
					position_ = value_type{position_.data() - 1, position_.size(), position_.row_size()};
					return *this;
				}

				constexpr columns_iterator operator--(int) noexcept
				{
					auto temp {*this};
					--(*this);
					return temp;
				}

				constexpr columns_iterator& operator-=(difference_type amount) noexcept
				{
					position_ = value_type{position_.data() - amount, position_.size(), position_.row_size()};
					return *this;
				}

				[[nodiscard]] friend constexpr columns_iterator operator-(const columns_iterator& lhs, difference_type amount) noexcept
				{
					auto result {lhs};
					result -= amount;
					return result;
				}

				constexpr columns_iterator& operator++() noexcept
				{
					position_ = value_type{position_.data() + 1, position_.size(), position_.row_size()};
					return *this;
				}

				constexpr columns_iterator operator++(int) noexcept
				{
					auto temp {*this};
					++(*this);
					return temp;
				}

				constexpr columns_iterator& operator+=(difference_type amount) noexcept
				{
					position_ = value_type{position_.data() + amount, position_.size(), position_.row_size()};
					return *this;
				}

				[[nodiscard]] friend constexpr columns_iterator operator+(const columns_iterator& lhs, difference_type amount) noexcept
				{
					auto result {lhs};
					result += amount;
					return result;
				}

				constexpr reference operator*() noexcept
				{
					return position_;
				}

				constexpr const reference operator*() const noexcept
				{
					return position_;
				}

				constexpr pointer operator->()
				{
					return std::addressof(position_);
				}

				constexpr const pointer operator->() const
				{
					return std::addressof(position_);
				}

				[[nodiscard]] constexpr bool operator==(const columns_iterator& other) const noexcept
				{
					return position_.data() == other.position_.data();
				}

				[[nodiscard]] constexpr bool operator!=(const columns_iterator& other) const noexcept
				{
					return !(*this == other);
				}
			private:
				value_type position_;
		};
		
		template<class T, std::size_t Size, std::size_t Extent, std::size_t RowSize>
		class columns_slice
		{
			public:
				using value_type = column<T, Extent, RowSize>;
				using size_type = std::size_t;
				using difference_type = std::ptrdiff_t;
				using reference = value_type&;
				using const_reference = const reference;
				using pointer = value_type*;
				using const_pointer = const pointer;
				using iterator = columns_iterator<T, Extent, RowSize>;
				using reverse_iterator = std::reverse_iterator<iterator>;

				constexpr columns_slice(value_type start, size_type size)
					: storage_{start, size} {}

				constexpr reference operator[](size_type pos) const noexcept
				{
					return *(begin() + pos);
				}

				constexpr reference at(size_type pos) const
				{
					if (pos >= size())
					{
						throw std::out_of_range{};
					}

					return operator[](pos);
				}
				
				constexpr reference front() const noexcept
				{
					return storage_.data_;
				}

				constexpr reference back() const noexcept
				{
					return *(end() - 1);
				}

				[[nodiscard]] constexpr iterator begin() const noexcept
				{
					return iterator{storage_.data_};
				}

				[[nodiscard]] constexpr iterator end() const noexcept
				{
					return iterator{storage_.data_} + size();
				}

				[[nodiscard]] constexpr reverse_iterator rbegin() const noexcept
				{
					return reverse_iterator{end()};
				}

				[[nodiscard]] constexpr reverse_iterator rend() const noexcept
				{
					return reverse_iterator{begin()};
				}

				[[nodiscard]] constexpr bool empty() const noexcept
				{
					return size() == 0;
				}

				[[nodiscard]] constexpr size_type size() const noexcept
				{
					return storage_.size();
				}

				[[nodiscard]] constexpr size_type row_size() const noexcept
				{
					return storage_.data_.row_size();
				}

				[[nodiscard]] constexpr size_type column_size() const noexcept
				{
					return storage_.data_.size();
				}

				[[nodiscard]] constexpr operator columns_slice<const T, Size, Extent, RowSize>() const noexcept
				{
					return {storage_.data_, size(), column_size(), row_size()};
				}
			private:

				template<class ExtentType>
				struct storage_type : public ExtentType
				{
					template<class OtherExtentType>
					constexpr storage_type(value_type data, OtherExtentType ex)
						: ExtentType{ex}, data_{data} {}

					value_type data_;
				};

				storage_type<cmoon::extent_type<Size>> storage_;
		};

		template<class Rep>
		class matrix
		{
			using storage_t = cmoon::fixed_multidimensional_array<Rep, 2>;

			template<class Rep2>
			friend class matrix;

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

				template<std::size_t R, std::size_t C>
				matrix(const fixed_matrix<Rep, R, C>& other)
					: data_{other.data_} {}

				matrix(matrix&&) noexcept = default;

				matrix& operator=(const matrix&) = default;
				matrix& operator=(matrix&&) noexcept = default;

				matrix& operator=(std::initializer_list<Rep> elements)
				{
					data_ = elements;
					return *this;
				}

				template<std::size_t R, std::size_t C>
				matrix& operator=(const fixed_matrix<Rep, R, C>& other)
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

				template<std::size_t Count = std::dynamic_extent>
				[[nodiscard]] rows_slice<value_type, Count, std::dynamic_extent> get_rows_slice(std::size_t start = 0) noexcept
				{
					return {get_row(start), Count == std::dynamic_extent ? rows() : Count};
				}

				template<std::size_t Count = std::dynamic_extent>
				[[nodiscard]] rows_slice<const value_type, Count, std::dynamic_extent> get_rows_slice(std::size_t start = 0) const noexcept
				{
					return {get_row(start), Count == std::dynamic_extent ? rows() : Count};
				}

				[[nodiscard]] rows_slice<value_type, std::dynamic_extent, std::dynamic_extent> get_rows_slice(std::size_t start = 0, std::size_t count = std::dynamic_extent) noexcept
				{
					return {get_row(start), count == std::dynamic_extent ? rows() : count};
				}

				[[nodiscard]] rows_slice<const value_type, std::dynamic_extent, std::dynamic_extent> get_rows_slice(std::size_t start = 0, std::size_t count = std::dynamic_extent) const noexcept
				{
					return {get_row(start), count == std::dynamic_extent ? rows() : count};
				}

				template<std::size_t Count = std::dynamic_extent>
				[[nodiscard]] columns_slice<value_type, Count, std::dynamic_extent, std::dynamic_extent> get_columns_slice(std::size_t start = 0) noexcept
				{
					return {get_column(start), Count == std::dynamic_extent ? rows() : Count};
				}

				template<std::size_t Count = std::dynamic_extent>
				[[nodiscard]] columns_slice<const value_type, Count, std::dynamic_extent, std::dynamic_extent> get_columns_slice(std::size_t start = 0) const noexcept
				{
					return {get_column(start), Count == std::dynamic_extent ? rows() : Count};
				}

				[[nodiscard]] columns_slice<value_type, std::dynamic_extent, std::dynamic_extent, std::dynamic_extent> get_columns_slice(std::size_t start = 0, std::size_t count = std::dynamic_extent) noexcept
				{
					return {get_column(start), count == std::dynamic_extent ? rows() : count};
				}

				[[nodiscard]] columns_slice<const value_type, std::dynamic_extent, std::dynamic_extent, std::dynamic_extent> get_columns_slice(std::size_t start = 0, std::size_t count = std::dynamic_extent) const noexcept
				{
					return {get_column(start), count == std::dynamic_extent ? rows() : count};
				}

				[[nodiscard]] row<value_type, std::dynamic_extent> get_row(std::size_t i) noexcept
				{
					return {std::addressof(data_.get(i, 0)), cols()};
				}

				[[nodiscard]] row<const value_type, std::dynamic_extent> get_row(std::size_t i) const noexcept
				{
					return {std::addressof(data_.get(i, 0)), cols()};
				}

				[[nodiscard]] column<value_type, std::dynamic_extent, std::dynamic_extent> get_column(std::size_t i) noexcept
				{
					return {std::addressof(data_.get(0, i)), rows(), cols()};
				}

				[[nodiscard]] column<const value_type, std::dynamic_extent, std::dynamic_extent> get_column(std::size_t i) const noexcept
				{
					return {std::addressof(data_.get(0, i)), rows(), cols()};
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
					requires(requires(const Rep& t, const Rep2& t2) { { t + t2 } -> std::convertible_to<Rep>; })
				matrix& operator+=(const matrix<Rep2>& other)
				{
					if (rows() != other.rows() || cols() != other.cols())
					{
						throw std::invalid_argument{"cannot add matrices of different dimension sizes"};
					}
					std::ranges::transform(*this, other, begin(), std::plus<>{});
					return *this;
				}

				template<class Rep2, std::size_t Rows, std::size_t Cols>
					requires(requires(const Rep& t, const Rep2& t2) { { t + t2 } -> std::convertible_to<Rep>; })
				matrix& operator+=(const fixed_matrix<Rep2, Rows, Cols>& other)
				{
					if (rows() != Rows || cols() != Cols)
					{
						throw std::invalid_argument{"cannot add matrices of different dimension sizes"};
					}
					std::ranges::transform(*this, other, begin(), std::plus<>{});
					return *this;
				}

				template<class Rep2>
					requires(requires(const Rep& t, const Rep2& t2) { { t - t2 } -> std::convertible_to<Rep>; })
				matrix& operator-=(const matrix<Rep2>& other)
				{
					if (rows() != other.rows() || cols() != other.cols())
					{
						throw std::invalid_argument{"cannot subtract matrices of different dimension sizes"};
					}
					std::ranges::transform(*this, other, begin(), std::minus<>{});
					return *this;
				}
				
				template<class Rep2, std::size_t Rows, std::size_t Cols>
					requires(requires(const Rep& t, const Rep2& t2) { { t - t2 } -> std::convertible_to<Rep>; })
				matrix& operator-=(const fixed_matrix<Rep2, Rows, Cols>& other)
				{
					if (rows() != Rows || cols() != Cols)
					{
						throw std::invalid_argument{"cannot subtract matrices of different dimension sizes"};
					}
					std::ranges::transform(*this, other, begin(), std::minus<>{});
					return *this;
				}

				template<class Rep2>
					requires(requires(const Rep& t, const Rep2& t2) { { t * t2 } -> std::convertible_to<Rep>; })
				matrix& operator*=(const Rep2& other) noexcept
				{
					std::ranges::transform(*this, begin(), [&other](const auto& v) { return v * other; });
					return *this;
				}

				template<class Rep2>
					requires(requires(const Rep& t, const Rep2& t2) { { t / t2 } -> std::convertible_to<Rep>; })
				matrix& operator/=(const Rep2& other) noexcept
				{
					std::ranges::transform(*this, begin(), [&other](const auto& v) { return v / other; });
					return *this;
				}

				template<class Rep2>
					requires(requires(const Rep& t, const Rep2& t2) { { t% t2 } -> std::convertible_to<Rep>; })
				matrix& operator%=(const Rep2& other) noexcept
				{
					std::ranges::transform(*this, begin(), [&other](const auto& v) { return v % other; });
					return *this;
				}

				constexpr value_type sum() const noexcept
				{
					return std::accumulate(begin(), end(), value_type{});
				}

				constexpr value_type product() const noexcept
				{
					return std::accumulate(begin(), end(), value_type{1}, std::multiplies<>{});
				}

				[[nodiscard]] constexpr bool operator==(const matrix& rhs) const
				{
					if (rows() != rhs.rows() || cols() != rhs.cols())
					{
						return false;
					}

					return std::ranges::equal(*this, rhs);
				}

				[[nodiscard]] constexpr bool operator!=(const matrix& rhs) const
				{
					return !(*this == rhs);
				}

				template<std::size_t Rows, std::size_t Cols>
				[[nodiscard]] bool operator==(const fixed_matrix<value_type, Rows, Cols>& rhs) const
				{
					if (rows() != Rows || cols() != Cols)
					{
						return false;
					}

					return std::ranges::equal(*this, rhs);
				}

				template<std::size_t Rows, std::size_t Cols>
				[[nodiscard]] bool operator!=(const fixed_matrix<value_type, Rows, Cols>& rhs) const
				{
					return !(*this == rhs);
				}
			private:
				storage_t data_;
		};

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
					for (std::size_t j {0}; j < new_dim; ++j)
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
					for (std::size_t j {0}; j < new_dim; ++j)
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

			template<matrix_type M1, matrix_type M2, matrix_type M3>
			[[nodiscard]] constexpr auto mult_std_impl(const M1& lhs, const M2& rhs, M3& m)
			{
				const auto rows = lhs.rows();
				const auto columns = rhs.cols();
				const auto columns2 = lhs.cols();

				for (std::size_t i {0}; i < rows; ++i)
				{
					for (std::size_t j {0}; j < columns; ++j)
					{
						if (std::is_constant_evaluated())
						{
							for (std::size_t n {0}; n < columns2; ++n)
							{
								m(i, j) += lhs(i, n) * rhs(n, j);
							}
						}
						else
						{
							m(i, j) = std::transform_reduce(std::begin(lhs.get_row(i)),
															std::end(lhs.get_row(i)),
															std::begin(rhs.get_column(j)), std::common_type_t<typename M3::value_type>{0});
						}
					}
				}
			}

			template<matrix_type M1, matrix_type M2>
			[[nodiscard]] constexpr auto mult_std(const M1& lhs, const M2& rhs) noexcept
			{
				using common_type = std::common_type_t<typename M1::value_type, typename M2::value_type>;
				matrix<common_type> m {lhs.rows(), rhs.cols()};
				mult_std_impl(lhs, rhs, m);
				return m;
			}

			template<class T, class T2, std::size_t Rows, std::size_t Cols, std::size_t Cols2>
			[[nodiscard]] constexpr auto mult_std(const fixed_matrix<T, Rows, Cols>& lhs, const fixed_matrix<T2, Cols, Cols2>& rhs) noexcept
			{
				fixed_matrix<std::common_type_t<T, T2>, Rows, Cols2> m;
				mult_std_impl(lhs, rhs, m);
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

				auto a11_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 0, 0, std::cref(a));
				auto a12_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 0, 1, std::cref(a));
				auto a21_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 1, 0, std::cref(a));
				auto a22_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 1, 1, std::cref(a));

				auto b11_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::cref(b));
				auto b12_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 1, std::cref(b));
				auto b21_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 1, 0, std::cref(b));
				auto b22_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 1, 1, std::cref(b));

				const auto a11 = a11_future.get();
				const auto a22 = a22_future.get();

				const auto b11 = b11_future.get();
				const auto b22 = b22_future.get();
				auto m1_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a11 + a22, b11 + b22);

				const auto a21 = a21_future.get();
				auto m2_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a21 + a22, std::cref(b11));

				const auto b12 = b12_future.get();
				auto m3_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), std::cref(a11), b12 - b22);
				auto m6_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a21 - a11, b11 + b12);

				const auto a12 = a12_future.get();
				auto m5_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a11 + a12, std::cref(b22));

				const auto b21 = b21_future.get();
				auto m4_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), std::cref(a22), b21 - b11);
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

					auto a11_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 0, 0, std::cref(a));
					auto a12_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 0, 1, std::cref(a));
					auto a21_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 1, 0, std::cref(a));
					auto a22_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 1, 1, std::cref(a));

					auto b11_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::cref(b));
					auto b12_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 1, std::cref(b));
					auto b21_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 1, 0, std::cref(b));
					auto b22_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 1, 1, std::cref(b));

					const auto a11 = a11_future.get();
					const auto a22 = a22_future.get();

					const auto b11 = b11_future.get();
					const auto b22 = b22_future.get();
					auto m1_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a11 + a22, b11 + b22);

					const auto a21 = a21_future.get();
					auto m2_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a21 + a22, std::cref(b11));

					const auto b12 = b12_future.get();
					auto m3_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), std::cref(a11), b12 - b22);
					auto m6_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a21 - a11, b11 + b12);

					const auto a12 = a12_future.get();
					auto m5_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a11 + a12, std::cref(b22));

					const auto b21 = b21_future.get();
					auto m4_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), std::cref(a22), b21 - b11);
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

					auto a11_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 0, 0, std::cref(a));
					auto a12_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 0, 1, std::cref(a));
					auto a21_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 1, 0, std::cref(a));
					auto a22_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 1, 1, std::cref(a));

					auto b11_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::cref(b));
					auto b12_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 1, std::cref(b));
					auto b21_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 1, 0, std::cref(b));
					auto b22_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 1, 1, std::cref(b));

					const auto a11 = a11_future.get();
					const auto a22 = a22_future.get();

					const auto b11 = b11_future.get();
					const auto b22 = b22_future.get();
					auto m1_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a11 + a22, b11 + b22);

					const auto a21 = a21_future.get();
					auto m2_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a21 + a22, std::cref(b11));

					const auto b12 = b12_future.get();
					auto m3_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), std::cref(a11), b12 - b22);
					auto m6_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a21 - a11, b11 + b12);

					const auto a12 = a12_future.get();
					auto m5_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a11 + a12, std::cref(b22));

					const auto b21 = b21_future.get();
					auto m4_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), std::cref(a22), b21 - b11);
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
				else
				{
					if (std::is_constant_evaluated())
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

						auto a11_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 0, 0, std::cref(a));
						auto a12_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 0, 1, std::cref(a));
						auto a21_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 1, 0, std::cref(a));
						auto a22_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 1, 1, std::cref(a));

						auto b11_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::cref(b));
						auto b12_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 1, std::cref(b));
						auto b21_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 1, 0, std::cref(b));
						auto b22_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 1, 1, std::cref(b));

						const auto a11 = a11_future.get();
						const auto a22 = a22_future.get();

						const auto b11 = b11_future.get();
						const auto b22 = b22_future.get();
						auto m1_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a11 + a22, b11 + b22);

						const auto a21 = a21_future.get();
						auto m2_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a21 + a22, std::cref(b11));

						const auto b12 = b12_future.get();
						auto m3_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), std::cref(a11), b12 - b22);
						auto m6_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a21 - a11, b11 + b12);

						const auto a12 = a12_future.get();
						auto m5_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), a11 + a12, std::cref(b22));

						const auto b21 = b21_future.get();
						auto m4_future = std::async(std::launch::async, static_cast<f_t>(mult_strassen), std::cref(a22), b21 - b11);
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
				throw std::invalid_argument {"cannot multiplies matrices that do not share columns with rows"};
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
			using storage_t = cmoon::constant_multidimensional_array<Rep, Rows, Cols>;

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

				constexpr fixed_matrix(fixed_matrix&&) noexcept = default;

				template<std::convertible_to<Rep>... Elements>
					requires (sizeof...(Elements) > 0)
				constexpr fixed_matrix(Elements&&... elements)
					: data_{std::forward<Elements>(elements)...} {}

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
					return {std::addressof(data_.get(i, 0)), Cols};
				}

				[[nodiscard]] constexpr row<const value_type, Cols> get_row(std::size_t i) const noexcept
				{
					return {std::addressof(data_.get(i, 0)), Cols};
				}

				[[nodiscard]] constexpr column<value_type, Rows, Cols> get_column(std::size_t i) noexcept
				{
					return {std::addressof(data_.get(0, i)), Rows, Cols};
				}

				[[nodiscard]] constexpr column<const value_type, Rows, Cols> get_column(std::size_t i) const noexcept
				{
					return {std::addressof(data_.get(0, i)), Rows, Cols};
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

				template<std::size_t Count = std::dynamic_extent>
				[[nodiscard]] rows_slice<value_type, Count, Cols> get_rows_slice(std::size_t start = 0) noexcept
				{
					return {get_row(start), Count == std::dynamic_extent ? rows() : Count};
				}

				template<std::size_t Count = std::dynamic_extent>
				[[nodiscard]] rows_slice<const value_type, Count, Cols> get_rows_slice(std::size_t start = 0) const noexcept
				{
					return {get_row(start), Count == std::dynamic_extent ? rows() : Count};
				}

				[[nodiscard]] rows_slice<value_type, std::dynamic_extent, Cols> get_rows_slice(std::size_t start = 0, std::size_t count = std::dynamic_extent) noexcept
				{
					return {get_row(start), count == std::dynamic_extent ? rows() : count};
				}

				[[nodiscard]] rows_slice<const value_type, std::dynamic_extent, Cols> get_rows_slice(std::size_t start = 0, std::size_t count = std::dynamic_extent) const noexcept
				{
					return {get_row(start), count == std::dynamic_extent ? rows() : count};
				}

				template<std::size_t Count = std::dynamic_extent>
				[[nodiscard]] columns_slice<value_type, Count, std::dynamic_extent, Cols> get_columns_slice(std::size_t start = 0) noexcept
				{
					return {get_column(start), Count == std::dynamic_extent ? rows() : Count};
				}

				template<std::size_t Count = std::dynamic_extent>
				[[nodiscard]] columns_slice<const value_type, Count, Rows, Cols> get_columns_slice(std::size_t start = 0) const noexcept
				{
					return {get_column(start), Count == std::dynamic_extent ? rows() : Count};
				}

				[[nodiscard]] columns_slice<value_type, std::dynamic_extent, Rows, Cols> get_columns_slice(std::size_t start = 0, std::size_t count = std::dynamic_extent) noexcept
				{
					return {get_column(start), count == std::dynamic_extent ? rows() : count};
				}

				[[nodiscard]] columns_slice<const value_type, std::dynamic_extent, Rows, Cols> get_columns_slice(std::size_t start = 0, std::size_t count = std::dynamic_extent) const noexcept
				{
					return {get_column(start), count == std::dynamic_extent ? rows() : count};
				}

				fixed_matrix& operator+=(const matrix<Rep>& other)
				{
					if (Rows != other.rows() || Cols != other.cols())
					{
						throw std::invalid_argument{"cannot add matrices of different dimension sizes"};
					}
					std::ranges::transform(*this, other, begin(), std::plus<>{});
					return *this;
				}

				constexpr fixed_matrix& operator+=(const fixed_matrix<Rep, Rows, Cols>& other) noexcept
				{
					std::ranges::transform(*this, other, begin(), std::plus<>{});
					return *this;
				}

				fixed_matrix& operator-=(const matrix<Rep>& other)
				{
					if (Rows != other.rows() || Cols != other.cols())
					{
						throw std::invalid_argument{"cannot subtract matrices of different dimension sizes"};
					}
					std::ranges::transform(*this, other, begin(), std::minus<>{});
					return *this;
				}

				constexpr fixed_matrix& operator-=(const fixed_matrix<Rep, Rows, Cols>& other) noexcept
				{
					std::ranges::transform(*this, other, begin(), std::minus<>{});
					return *this;
				}

				constexpr fixed_matrix& operator*=(const Rep& other) noexcept
				{
					std::ranges::transform(*this, begin(), [&other](const auto& v) { return v * other; });
					return *this;
				}

				constexpr fixed_matrix& operator/=(const Rep& other) noexcept
				{
					std::ranges::transform(*this, begin(), [&other](const auto& v) { return v / other; });
					return *this;
				}

				constexpr fixed_matrix& operator%=(const Rep& other) noexcept
				{
					std::ranges::transform(*this, begin(), [&other](const auto& v) { return v % other; });
					return *this;
				}

				constexpr value_type sum() const noexcept
				{
					return std::accumulate(begin(), end(), value_type{});
				}

				constexpr value_type product() const noexcept
				{
					return std::accumulate(begin(), end(), value_type{1}, std::multiplies<>{});
				}

				[[nodiscard]] constexpr bool operator==(const fixed_matrix& rhs) const
				{
					return std::ranges::equal(*this, rhs);
				}

				[[nodiscard]] constexpr bool operator!=(const fixed_matrix& rhs) const
				{
					return !(*this == rhs);
				}

				[[nodiscard]] bool operator==(const matrix<value_type>& rhs) const
				{
					if (rows() != rhs.rows() || cols() != rhs.cols())
					{
						return false;
					}

					return std::ranges::equal(*this, rhs);
				}

				[[nodiscard]] bool operator!=(const matrix<value_type>& rhs) const
				{
					return !(*this == rhs);
				}
			private:
				storage_t data_;

				template<class T>
				friend class matrix;
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
		[[nodiscard]] diagonal<T, std::dynamic_extent> main_diagonal(matrix<T>& m)
		{
			if (!is_square_matrix(m))
			{
				throw std::invalid_argument{"can only find the main diagonal of a square matrix"};
			}

			return {m.data(), m.rows()};
		}

		template<class T>
		[[nodiscard]] diagonal<const T, std::dynamic_extent> main_diagonal(const matrix<T>& m)
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
			std::ranges::fill(main_diagonal(m), T{1});

			return m;
		}

		template<class T>
		[[nodiscard]] matrix<T> identity_matrix(std::size_t size) noexcept
		{
			matrix<T> m{size, size};
			std::ranges::fill(main_diagonal(m), T{1});

			return m;
		}
	}
}

namespace std
{
	template<class T, std::size_t Size>
	constexpr void swap(cmoon::linear::diagonal<T, Size> lhs, cmoon::linear::diagonal<T, Size> rhs) noexcept
	{
		lhs.swap(rhs);
	}

	template<class T, std::size_t Size>
	constexpr void swap(cmoon::linear::diagonal_iterator<T, Size>& lhs, cmoon::linear::diagonal_iterator<T, Size>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	template<class T, std::size_t Extent, std::size_t RowSize>
	constexpr void swap(cmoon::linear::column<T, Extent, RowSize> lhs, cmoon::linear::column<T, Extent, RowSize> rhs) noexcept
	{
		lhs.swap(rhs);
	}

	template<class T, std::size_t RowSize>
	constexpr void swap(cmoon::linear::column_iterator<T, RowSize>& lhs, cmoon::linear::column_iterator<T, RowSize>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	template<class T, std::size_t Extent>
	constexpr void swap(cmoon::linear::row<T, Extent> lhs, cmoon::linear::row<T, Extent> rhs) noexcept
	{
		lhs.swap(rhs);
	}
}