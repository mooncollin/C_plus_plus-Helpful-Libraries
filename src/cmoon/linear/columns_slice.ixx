export module cmoon.linear.columns_slice;

import <cstddef>;
import <iterator>;

import cmoon.utility;

import cmoon.linear.column;

namespace cmoon::linear
{
	export
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

			[[nodiscard]] friend constexpr columns_iterator operator+(const columns_iterator& lhs, difference_type rhs) noexcept
			{
				return columns_iterator{value_type{lhs.position_.data() + rhs, lhs.position_.size(), lhs.position_.row_size()}};
			}

			[[nodiscard]] friend constexpr columns_iterator operator-(const columns_iterator& lhs, difference_type rhs) noexcept
			{
				return lhs + -rhs;
			}

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

			[[nodiscard]] friend constexpr bool operator==(const columns_iterator& lhs, const columns_iterator& rhs) noexcept
			{
				return lhs.position_.data() == rhs.position_.data();
			}

			[[nodiscard]] friend constexpr bool operator!=(const columns_iterator&, const columns_iterator&) noexcept = default;
		private:
			value_type position_;
	};
	
	export
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
}