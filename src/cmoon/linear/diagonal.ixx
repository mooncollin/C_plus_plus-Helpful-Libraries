export module cmoon.linear.diagonal;

import <cstddef>;
import <iterator>;
import <type_traits>;
import <algorithm>;

import cmoon.utility;

namespace cmoon::linear
{
	export
	template<class T, std::size_t Size>
	class diagonal;

	export
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

			[[nodiscard]] friend constexpr diagonal_iterator operator-(const diagonal_iterator& lhs, difference_type rhs) noexcept
			{
				auto r {lhs};
				r -= rhs;
				return r;
			}

			[[nodiscard]] friend constexpr diagonal_iterator operator+(const diagonal_iterator& lhs, difference_type rhs) noexcept
			{
				auto r{lhs};
				r += rhs;
				return r;
			}

			[[nodiscard]] friend constexpr difference_type operator-(const diagonal_iterator& lhs, const diagonal_iterator& rhs) noexcept
			{
				return (lhs.current_ - rhs.current_ + 1) / lhs.owner_->size();
			}

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
				const auto beg_distance = distance_to_beginning();
				const auto end_distance = distance_to_end();

				const auto max_back = std::min(beg_distance, amount * static_cast<difference_type>(owner_->size()) - amount);
				if (end_distance == 0 && max_back != beg_distance)
				{
					current_ -= 1;
				}
				current_ -= max_back;
				return *this;
			}

			constexpr diagonal_iterator& operator+=(difference_type amount) noexcept
			{
				const auto beg_distance = distance_to_beginning();
				const auto end_distance = distance_to_end();

				const auto max_forward = std::min(end_distance, amount * static_cast<difference_type>(owner_->size()) + amount);
				if (beg_distance == -1 && max_forward != end_distance)
				{
					current_ += 1;
				}
				current_ += max_forward;
				return *this;
			}

			[[nodiscard]] constexpr reference operator*() noexcept
			{
				return *current_;
			}

			[[nodiscard]] constexpr const reference operator*() const noexcept
			{
				return *current_;
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

			[[nodiscard]] friend constexpr auto operator<=>(const diagonal_iterator&, const diagonal_iterator&) noexcept = default;
		private:
			pointer current_;

			[[nodiscard]] constexpr difference_type distance_to_beginning() const noexcept
			{
				return std::distance(current_, owner_->begin().current_);
			}

			[[nodiscard]] constexpr difference_type distance_to_end() const noexcept
			{
				return std::distance(current_, owner_->end().current_);
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
	export
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

			constexpr diagonal& operator+=(const diagonal& other) noexcept
			{
				std::transform(begin(), end(), other.begin(), begin(), std::plus<>{});
				return *this;
			}

			constexpr diagonal& operator-=(const diagonal& other) noexcept
			{
				std::transform(begin(), end(), other.begin(), begin(), std::minus<>{});
				return *this;
			}

			constexpr diagonal& operator*=(const_reference other) noexcept
			{
				std::transform(begin(), end(), begin(), std::bind(std::multiplies{}, std::placeholders::_1, other));
				return *this;
			}

			constexpr diagonal& operator/=(const_reference other) noexcept
			{
				std::transform(begin(), end(), begin(), std::bind(std::divides{}, std::placeholders::_1, other));
				return *this;
			}

			constexpr diagonal& operator%=(const_reference other) noexcept
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

			constexpr void swap(diagonal& rhs) noexcept
			{
				std::swap_ranges(begin(), end(), rhs.begin());
			}

			[[nodiscard]] constexpr operator diagonal<const T, Size>() const noexcept
			{
				return {data(), size()};
			}

			template<std::size_t Size2>
			[[nodiscard]] friend constexpr bool operator==(const diagonal& lhs, const diagonal<T, Size2>& rhs) noexcept
			{
				return std::size(lhs) == std::size(rhs) &&
						std::equal(lhs.begin(), lhs.end(), rhs.begin());
			}

			template<std::size_t Size2>
			[[nodiscard]] friend constexpr bool operator!=(const diagonal& lhs, const diagonal<T, Size2>& rhs) noexcept
			{
				return !(lhs == rhs);
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
}

namespace std
{
	export
	template<class T, std::size_t Size>
	constexpr void swap(cmoon::linear::diagonal<T, Size>& lhs, cmoon::linear::diagonal<T, Size>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	template<class T, std::size_t Size>
	constexpr void swap(cmoon::linear::diagonal_iterator<T, Size>& lhs, cmoon::linear::diagonal_iterator<T, Size>& rhs) noexcept
	{
		lhs.swap(rhs);
	}
}