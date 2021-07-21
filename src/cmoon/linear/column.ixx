export module cmoon.linear.column;

import <iterator>;
import <cstddef>;
import <type_traits>;
import <algorithm>;
import <numeric>;
import <span>;
import <functional>;

import cmoon.utility;

namespace cmoon::linear
{
	export
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

			[[nodiscard]] friend constexpr column_iterator operator+(const column_iterator& lhs, difference_type rhs) noexcept
			{
				return column_iterator{lhs.storage_.data_ + (lhs.storage_.size() * rhs), lhs.storage_.size()};
			}

			[[nodiscard]] friend constexpr column_iterator operator-(const column_iterator& lhs, difference_type rhs) noexcept
			{
				return lhs + -rhs;
			}

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

			constexpr column_iterator& operator+=(difference_type amount) noexcept
			{
				storage_.data_ += storage_.size() * amount;
				return *this;
			}

			constexpr reference operator*() noexcept
			{
				return *storage_.data_;
			}

			constexpr const reference operator*() const noexcept
			{
				return *storage_.data_;
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

			[[nodiscard]] friend constexpr bool operator==(const column_iterator& lhs, const column_iterator& rhs) noexcept
			{
				return lhs.storage_.data_ == rhs.storage_.data_;
			}
			[[nodiscard]] friend constexpr bool operator!=(const column_iterator&, const column_iterator&) noexcept = default;
			[[nodiscard]] friend constexpr auto operator<=>(const column_iterator& lhs, const column_iterator& rhs) noexcept
			{
				return lhs.storage_.data_ <=> rhs.storage_.data_;
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

	export
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

			constexpr column& operator+=(const column& other) noexcept
			{
				std::transform(begin(), end(), other.begin(), begin(), std::plus<>{});
				return *this;
			}

			constexpr column& operator-=(const column& other) noexcept
			{
					std::transform(begin(), end(), other.begin(), begin(), std::minus<>{});
				return *this;
			}

			constexpr column& operator*=(const_reference other) noexcept
			{
				std::transform(begin(), end(), begin(), std::bind(std::multiplies{}, std::placeholders::_1, other));
				return *this;
			}

			constexpr column& operator/=(const_reference other) noexcept
			{
				std::transform(begin(), end(), begin(), std::bind(std::divides{}, std::placeholders::_1, other));
				return *this;
			}

			constexpr column& operator%=(const_reference other) noexcept
			{
				std::transform(begin(), end(), begin(), std::bind(std::modulus{}, std::placeholders::_1, other));
				return *this;
			}

			constexpr value_type sum() const noexcept
			{
				return std::reduce(begin(), end());
			}

			constexpr value_type product() const noexcept
			{
				return std::reduce(begin(), end(), value_type{1}, std::multiplies<>{});
			}

			constexpr void swap(column rhs) noexcept
			{
				std::swap_ranges(begin(), end(), rhs.begin());
			}

			[[nodiscard]] constexpr operator column<const value_type, Extent, RowSize>() const noexcept
			{
				return {data(), size(), row_size()};
			}

			template<std::size_t Extent2, std::size_t RowSize2>
			[[nodiscard]] friend constexpr bool operator==(const column& lhs, const column<T, Extent2, RowSize2>& rhs) noexcept
			{
				return (std::size(lhs) == std::size(rhs) && lhs.row_size() == rhs.row_size()) &&
						std::equal(lhs.begin(), lhs.end(), rhs.begin());
			}

			template<std::size_t Extent2, std::size_t RowSize2>
			[[nodiscard]] friend constexpr bool operator!=(const column& lhs, const column<T, Extent2, RowSize2>& rhs) noexcept
			{
				return !(lhs == rhs);
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
}

namespace std
{
	export
	template<class T, std::size_t Extent, std::size_t RowSize>
	constexpr void swap(cmoon::linear::column<T, Extent, RowSize> lhs, cmoon::linear::column<T, Extent, RowSize> rhs) noexcept
	{
		lhs.swap(rhs);
	}

	export
	template<class T, std::size_t RowSize>
	constexpr void swap(cmoon::linear::column_iterator<T, RowSize>& lhs, cmoon::linear::column_iterator<T, RowSize>& rhs) noexcept
	{
		lhs.swap(rhs);
	}
}