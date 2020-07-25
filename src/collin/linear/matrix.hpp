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

#include "collin/type_traits.hpp"
#include "collin/multidimensional.hpp"
#include "collin/iterator.hpp"
#include "collin/math.hpp"
#include "collin/numeric.hpp"
#include "collin/algorithm.hpp"
#include "collin/utility.hpp"
#include "collin/span.hpp"

namespace collin
{
	namespace linear
	{
		template<class Rep>
		class matrix;

		template<class Rep, std::size_t Rows, std::size_t Cols>
		class fixed_matrix;

		template<class T>
		struct is_matrix : std::false_type {};

		template<class Rep>
		struct is_matrix<matrix<Rep>> : std::true_type {};

		template<class T>
		constexpr auto is_matrix_v = is_matrix<T>::value;

		template<class T>
		struct is_fixed_matrix : std::false_type {};

		template<class Rep, std::size_t Rows, std::size_t Cols>
		struct is_fixed_matrix<fixed_matrix<Rep, Rows, Cols>> : std::true_type {};

		template<class T>
		constexpr auto is_fixed_matrix_v = is_fixed_matrix<T>::value;

		template<class T>
		constexpr auto is_either_matrix_v = is_matrix_v<T> || is_fixed_matrix_v<T>;

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

				constexpr iterator begin() const noexcept
				{
					return base::begin();
				}

				constexpr iterator end() const noexcept
				{
					return base::end();
				}

				constexpr reverse_iterator rbegin() const noexcept
				{
					return base::rbegin();
				}

				constexpr reverse_iterator rend() const noexcept
				{
					return base::rend();
				}

				constexpr reference front() const
				{
					return base::front();
				}

				constexpr reference back() const
				{
					return base::back();
				}

				constexpr reference operator[](size_type i) const
				{
					return base::operator[](i);
				}

				constexpr pointer data() const noexcept
				{
					return base::data();
				}

				constexpr size_type size() const noexcept
				{
					return base::size();
				}

				[[nodiscard]] constexpr bool empty() const noexcept
				{
					return base::empty();
				}

				template<class T2, std::size_t Size2, typename = std::enable_if_t<
					std::is_convertible_v<T2, T>
				>>
				constexpr row& operator+=(const row<T2, Size2>& other)
				{
					auto this_it = begin();
					auto other_it = other.begin();
					for (; this_it != end() && other_it != other.end(); ++this_it, ++other_it)
					{
						*this_it += *other_it;
					}

					return *this;
				}

				template<class T2, std::size_t Size2, typename = std::enable_if_t<
					std::is_convertible_v<T2, T>
				>>
				constexpr row& operator-=(const row<T2, Size2>& other)
				{
					auto this_it = begin();
					auto other_it = other.begin();
					for (; this_it != end() && other_it != other.end(); ++this_it, ++other_it)
					{
						*this_it -= *other_it;
					}

					return *this;
				}

				template<class T2, typename = std::enable_if_t<
					std::is_convertible_v<T2, T>
				>>
				constexpr row& operator*=(const T2& other)
				{
					for (auto& value : *this)
					{
						value *= other;
					}
				}

				constexpr void swap(row& other)
				{
					std::swap_ranges(begin(), end(), other.begin());
				}
		};

		template<class T, std::size_t Size>
		constexpr void swap(row<T, Size>& lhs, row<T, Size>& rhs)
		{
			lhs.swap(rhs);
		}

		template<class T, class T2, std::size_t Extent>
		constexpr bool operator==(const row<T, Extent>& lhs, const row<T2, Extent>& rhs)
		{
			if(std::size(lhs) != std::size(rhs))
			{
				return false;
			}

			auto lhs_it = std::begin(lhs);
			auto rhs_it = std::begin(rhs);

			for(; lhs_it != std::end(lhs); ++lhs_it, ++rhs_it)
			{
				if(*lhs_it != *rhs_it)
				{
					return false;
				}
			}

			return true;
		}

		template<class T, class T2, std::size_t Extent>
		constexpr bool operator!=(const row<T, Extent>& lhs, const row<T2, Extent>& rhs)
		{
			return !(lhs == rhs);
		}

		template<class T, std::size_t RowSize>
		class column_iterator : public std::iterator<std::bidirectional_iterator_tag, T>
		{
			using base = std::iterator<std::bidirectional_iterator_tag, T>;

			public:
				constexpr column_iterator(typename base::pointer item, std::size_t row_size) noexcept
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

				friend constexpr column_iterator operator-(const column_iterator& lhs, std::size_t amount) noexcept
				{
					auto c_it{lhs};
					c_it -= amount;
					return c_it;
				}

				constexpr typename base::reference operator*()
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

				friend constexpr column_iterator operator+(const column_iterator& lhs, std::size_t amount) noexcept
				{
					auto c_it{lhs};
					c_it += amount;
					return c_it;
				}

				constexpr bool operator==(const column_iterator& other) const noexcept
				{
					return storage_.data_ == other.storage_.data_;
				}

				constexpr bool operator!=(const column_iterator& other) const noexcept
				{
					return !(*this == other);
				}

				constexpr typename base::pointer operator->()
				{
					return storage_.data_;
				}
			private:
				template<class ExtentType>
				struct storage_type : public ExtentType
				{
					template<class OtherExtentType>
					constexpr storage_type(typename base::pointer data, OtherExtentType ext)
						: ExtentType(ext), data_(data) {}

					typename base::pointer data_;
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

				constexpr column(pointer first, size_type size, size_type row_size)
					: first_{first}, size_{size}, row_size_{row_size} {}

				constexpr iterator begin() const noexcept
				{
					return {data(), row_size_.size()};
				}

				constexpr iterator end() const noexcept
				{
					return {data() + (size_.size() * row_size_.size()), row_size_.size()};
				}

				constexpr reverse_iterator rbegin() const noexcept
				{
					return {end()};
				}

				constexpr reverse_iterator rend() const noexcept
				{
					return {begin()};
				}

				constexpr reference front() const
				{
					return *data();
				}

				constexpr reference back() const
				{
					return *(--end());
				}

				constexpr reference operator[](size_type i) const
				{
					return *(data() + (i * row_size_.size()));
				}

				constexpr pointer data() const noexcept
				{
					return first_;
				}

				constexpr size_type size() const noexcept
				{
					return size_;
				}

				[[nodiscard]] constexpr bool empty() const noexcept
				{
					return size() == 0;
				}

				template<class T2, std::size_t Size2, std::size_t RowSize2, typename = std::enable_if_t<
					std::is_convertible_v<T2, T>
				>>
				constexpr column& operator+=(const column<T2, Size2, RowSize2>& other)
				{
					auto this_it = begin();
					auto other_it = other.begin();
					for (; this_it != end() && other_it != other.end(); ++this_it, ++other_it)
					{
						*this_it = *this_it + *other_it;
					}

					return *this;
				}

				template<class T2, std::size_t Size2, std::size_t RowSize2, typename = std::enable_if_t<
					std::is_convertible_v<T2, T>
				>>
				constexpr column& operator-=(const column<T2, Size2, RowSize2>& other)
				{
					auto this_it = begin();
					auto other_it = other.begin();
					for (; this_it != end() && other_it != other.end(); ++this_it, ++other_it)
					{
						*this_it = *this_it - *other_it;
					}

					return *this;
				}

				template<class T2, typename = std::enable_if_t<
					std::is_convertible_v<T2, T>
				>>
				constexpr column& operator*=(const T2& other)
				{
					for (auto& value : *this)
					{
						value = value * other;
					}
				}

				constexpr void swap(column& other)
				{
					std::swap_ranges(begin(), end(), other.begin());
				}
			private:
				pointer first_;
				collin::extent_type<Extent> size_;
				collin::extent_type<RowSize> row_size_;
		};

		template<class T, std::size_t Size, std::size_t RowSize>
		constexpr void swap(column<T, Size, RowSize>& lhs, column<T, Size, RowSize>& rhs)
		{
			lhs.swap(rhs);
		}

		template<class T, class T2, std::size_t Extent, std::size_t RowSize>
		constexpr bool operator==(const column<T, Extent, RowSize>& lhs, const column<T2, Extent, RowSize>& rhs)
		{
			if(std::size(lhs) != std::size(rhs))
			{
				return false;
			}

			auto lhs_it = std::begin(lhs);
			auto rhs_it = std::begin(rhs);

			for(; lhs_it != std::end(lhs); ++lhs_it, ++rhs_it)
			{
				if(*lhs_it != *rhs_it)
				{
					return false;
				}
			}

			return true;
		}

		template<class T, class T2, std::size_t Extent, std::size_t RowSize>
		constexpr bool operator!=(const column<T, Extent, RowSize>& lhs, const column<T2, Extent, RowSize>& rhs)
		{
			return !(lhs == rhs);
		}

		template<class T, std::size_t Size>
		class diagonal;

		template<class T, std::size_t RowSize>
		class diagonal_iterator : public std::iterator<std::bidirectional_iterator_tag, T>
		{
			using base = std::iterator<std::bidirectional_iterator_tag, T>;

			public:
				constexpr diagonal_iterator(typename base::pointer item, const diagonal<T, RowSize>& owner) noexcept
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

				constexpr typename base::reference operator*()
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

				constexpr typename base::pointer operator->()
				{
					return current_;
				} 
			private:
				typename base::pointer current_;

				// C++20 gives constexpr std::reference_wrapper,
				// so we's gotta wait until then and use a crappy pointer.
				//
				// It is rather strange to hold the owner who makes the iterator
				// but unfortunately, when we iterate through the diagonal, we have
				// the tedency to iterate far past the end, which doesn't work well
				// in constexpr conditions. We use the owner to make sure our current
				// pointer does not go too far past the beginning or end of the container.
				const diagonal<T, RowSize>* owner_;
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

				[[nodiscard]] constexpr reference operator[](size_type i) const
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

				constexpr void swap(diagonal& other)
				{
					std::swap_ranges(begin(), end(), other.begin());
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
		};

		template<class T, std::size_t S>
		constexpr void swap(diagonal<T, S>& lhs, diagonal<T, S>& rhs)
		{
			lhs.swap(rhs);
		}

		template<class T, class T2, std::size_t Extent>
		constexpr bool operator==(const diagonal<T, Extent>& lhs, const diagonal<T2, Extent>& rhs)
		{
			if(std::size(lhs) != std::size(rhs))
			{
				return false;
			}

			auto lhs_it = std::begin(lhs);
			auto rhs_it = std::begin(rhs);

			for(; lhs_it != std::end(lhs); ++lhs_it, ++rhs_it)
			{
				if(*lhs_it != *rhs_it)
				{
					return false;
				}
			}

			return true;
		}

		template<class T, class T2, std::size_t Extent>
		constexpr bool operator!=(const diagonal<T, Extent>& lhs, const diagonal<T2, Extent>& rhs)
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

				template<class Rep2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<Rep2, Rep>
				>>
				matrix(const matrix<Rep2>& other)
					: data_{other.data_} {}

				template<class Rep2, std::size_t R, std::size_t C, typename = std::enable_if_t<
		/* requires */ std::is_convertible_v<Rep2, Rep>
				>>
				matrix(const fixed_matrix<Rep2, R, C>& other)
					: data_{other.data_} {}

				matrix(matrix&&) noexcept = default;

				template<class Rep2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<Rep2, Rep>
				>>
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

				template<class Rep2, std::size_t R, std::size_t C, typename = std::enable_if_t<
		/* requires */ std::is_convertible_v<Rep2, Rep>
				>>
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

				reference operator()(std::size_t r, std::size_t c)
				{	
					return data_.get(r, c);
				}

				const_reference operator()(std::size_t r, std::size_t c) const
				{	
					return data_.get(r, c);
				}

				[[nodiscard]] row<value_type, std::dynamic_extent> get_row(std::size_t i)
				{
					return {&data_.get(i, 0), cols()};
				}

				[[nodiscard]] row<const value_type, std::dynamic_extent> get_row(std::size_t i) const
				{
					return {&data_.get(i, 0), cols()};
				}

				[[nodiscard]] column<value_type, std::dynamic_extent, std::dynamic_extent> get_column(std::size_t i)
				{
					return {&data_.get(0, i), cols(), rows()};
				}

				[[nodiscard]] column<const value_type, std::dynamic_extent, std::dynamic_extent> get_column(std::size_t i) const
				{
					return {&data_.get(0, i), cols(), rows()};
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
				
				template<class Rep2, typename = std::enable_if_t<
		/* requires */ std::is_convertible_v<Rep2, Rep>
				>>
				matrix& operator+=(const matrix<Rep2>& other)
				{
					if (rows() != other.rows() || cols() != other.cols())
					{
						throw std::invalid_argument{"cannot add matrices of different dimension sizes"};
					}

					auto it = begin();
					auto other_it = other.begin();

					for (; it != end(); ++it, ++other_it)
					{
						*it += *other_it;
					}

					return *this;
				}

				template<class Rep2, std::size_t Rows, std::size_t Cols, typename = std::enable_if_t<
		/* requires */ std::is_convertible_v<Rep2, Rep>
				>>
				matrix& operator+=(const fixed_matrix<Rep2, Rows, Cols>& other)
				{
					if (rows() != Rows || cols() != Cols)
					{
						throw std::invalid_argument{"cannot add matrices of different dimension sizes"};
					}

					auto it = begin();
					auto other_it = other.begin();

					for (; it != end(); ++it, ++other_it)
					{
						*it += *other_it;
					}

					return *this;
				}

				template<class Rep2, typename = std::enable_if_t<
		/* requires */ std::is_convertible_v<Rep2, Rep>
				>>
				matrix& operator-=(const matrix<Rep2>& other)
				{
					if (rows() != other.rows() || cols() != other.cols())
					{
						throw std::invalid_argument{"cannot subtract matrices of different dimension sizes"};
					}

					auto it = begin();
					auto other_it = other.begin();

					for (; it != end(); ++it, ++other_it)
					{
						*it -= *other_it;
					}

					return *this;
				}
				
				template<class Rep2, std::size_t Rows, std::size_t Cols, typename = std::enable_if_t<
		/* requires */ std::is_convertible_v<Rep2, Rep>
				>>
				matrix& operator-=(const fixed_matrix<Rep2, Rows, Cols>& other)
				{
					if (rows() != Rows || cols() != Cols)
					{
						throw std::invalid_argument{"cannot subtract matrices of different dimension sizes"};
					}

					auto it = begin();
					auto other_it = other.begin();

					for (; it != end(); ++it, ++other_it)
					{
						*it -= *other_it;
					}

					return *this;
				}

				template<class Rep2>
				matrix& operator*=(const Rep2& other)
				{
					for (auto& value : data_)
					{
						value *= other;
					}

					return *this;
				}
			private:
				storage_t data_;
		};

		template<class Matrix1, class Matrix2, typename = std::enable_if_t<
/* requires */	is_either_matrix_v<Matrix1> && is_either_matrix_v<Matrix2>
		>>
		[[nodiscard]] constexpr bool operator==(const Matrix1& lhs, const Matrix2& rhs)
		{
			if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols())
			{
				return false;
			}

			auto it = lhs.begin();
			auto other_it = rhs.begin();

			for (; it != lhs.end(); ++it, ++other_it)
			{
				if (*it != *other_it)
				{
					return false;
				}
			}

			return true;
		}

		template<class Matrix1, class Matrix2, typename = std::enable_if_t<
/* requires */	is_either_matrix_v<Matrix1> && is_either_matrix_v<Matrix2>
		>>
		[[nodiscard]] constexpr bool operator!=(const Matrix1& lhs, const Matrix2& rhs)
		{
			return !(lhs == rhs);
		}

		template<class Matrix1, class Matrix2, typename = std::enable_if_t<
/* requires */	is_either_matrix_v<Matrix1> && is_either_matrix_v<Matrix2>
		>>
		[[nodiscard]] constexpr auto operator+(const Matrix1& lhs, const Matrix2& rhs)
		{
			auto m {lhs};
			m += rhs;
			return m;
		}

		template<class Matrix1, class Matrix2, typename = std::enable_if_t<
/* requires */	is_either_matrix_v<Matrix1> && is_either_matrix_v<Matrix2>
		>>
		[[nodiscard]] constexpr auto operator-(const Matrix1& lhs, const Matrix2& rhs)
		{
			auto m {lhs};
			m -= rhs;
			return m;
		}

		template<class Matrix, class Other, typename = std::enable_if_t<
/* requires */	is_either_matrix_v<Matrix>
		>>
		[[nodiscard]] constexpr auto operator*(const Matrix& lhs, const Other& rhs)
		{
			if constexpr (!is_matrix_v<Other> && !is_fixed_matrix_v<Other>)
			{
				auto m {lhs};
				m *= rhs;
				return m;
			}
			else if constexpr(is_fixed_matrix_v<Matrix> && is_fixed_matrix_v<Other>)
			{
				using common_type = std::common_type_t<typename Matrix::value_type, typename Other::value_type>;

				static_assert(Matrix::num_columns == Other::num_rows,
					"cannot multiply matrices that do not share columns with rows");

				fixed_matrix<common_type, Matrix::num_rows, Other::num_columns> m;

				for (std::size_t i = 0; i < Matrix::num_rows; i++)
				{
					for (std::size_t j = 0; j < Other::num_columns; j++)
					{
						common_type sum {};
						for (std::size_t n = 0; n < Matrix::num_columns; n++)
						{
							sum += (lhs(i, n) * rhs(n, j));
						}

						m(i, j) = sum;
					}
				}

				return m;
			}
			else if constexpr (is_either_matrix_v<Other> && !is_fixed_matrix_v<Matrix>)
			{
				using common_type = std::common_type_t<typename Matrix::value_type, typename Other::value_type>;

				if (lhs.cols() != rhs.rows())
				{
					throw std::invalid_argument {"cannot multiply matrices that do not share columns with rows"};
				}

				matrix<common_type> m {lhs.rows(), rhs.cols()};

				for (std::size_t i = 0; i < lhs.rows(); i++)
				{
					for (std::size_t j = 0; j < rhs.cols(); j++)
					{
						common_type sum {};
						for (std::size_t n = 0; n < lhs.cols(); n++)
						{
							sum = sum + (lhs(i, n) * rhs(n, j));
						}

						m(i, j) = sum;
					}
				}

				return m;
			}
			else
			{
				using enable_ = std::enable_if_t<false>;
			}
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

				constexpr fixed_matrix() noexcept = default;

				constexpr fixed_matrix(const fixed_matrix&) = default;

				template<class Rep2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<Rep2, Rep>
				>>
				constexpr fixed_matrix(const fixed_matrix<Rep2, Rows, Cols>& other)
					: data_{other.data_} {}

				constexpr fixed_matrix(fixed_matrix&&) noexcept = default;

				template<class... Elements, typename = std::enable_if_t<
		/* requires */	(sizeof...(Elements) > 0) && (... && std::is_convertible_v<Elements, Rep>)
				>>
				constexpr fixed_matrix(Elements&&... elements)
					: data_{std::forward<Elements>(elements)...} {}

				template<class Rep2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<Rep2, Rep>
				>>
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

				constexpr reference operator()(std::size_t r, std::size_t c)
				{	
					return data_.get(r, c);
				}

				constexpr const_reference operator()(std::size_t r, std::size_t c) const
				{	
					return data_.get(r, c);
				}

				[[nodiscard]] constexpr row<value_type, Cols> get_row(std::size_t i)
				{
					return {&data_.get(i, 0), Cols};
				}

				[[nodiscard]] constexpr row<const value_type, Cols> get_row(std::size_t i) const
				{
					return {&data_.get(i, 0), Cols};
				}

				[[nodiscard]] constexpr column<value_type, Cols, Rows> get_column(std::size_t i)
				{
					return {&data_.get(0, i), Cols, Rows};
				}

				[[nodiscard]] constexpr column<const value_type, Cols, Rows> get_column(std::size_t i) const
				{
					return {&data_.get(0, i), Cols, Rows};
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

				template<class Rep2, typename = std::enable_if_t<
		/* requires */ std::is_convertible_v<Rep2, Rep>
				>>
				fixed_matrix& operator+=(const matrix<Rep2>& other)
				{
					if (Rows != other.rows() || Cols != other.cols())
					{
						throw std::invalid_argument{"cannot add matrices of different dimension sizes"};
					}

					auto it = begin();
					auto other_it = other.begin();

					for (; it != end(); ++it, ++other_it)
					{
						*it += *other_it;
					}

					return *this;
				}

				template<class Rep2, typename = std::enable_if_t<
		/* requires */ std::is_convertible_v<Rep2, Rep>
				>>
				constexpr fixed_matrix& operator+=(const fixed_matrix<Rep2, Rows, Cols>& other)
				{
					auto it = begin();
					auto other_it = other.begin();

					for (; it != end(); ++it, ++other_it)
					{
						*it += *other_it;
					}

					return *this;
				}

				template<class Rep2, typename = std::enable_if_t<
		/* requires */ std::is_convertible_v<Rep2, Rep>
				>>
				fixed_matrix& operator-=(const matrix<Rep2>& other)
				{
					if (Rows != other.rows() || Cols != other.cols())
					{
						throw std::invalid_argument{"cannot subtract matrices of different dimension sizes"};
					}

					auto it = begin();
					auto other_it = other.begin();

					for (; it != end(); ++it, ++other_it)
					{
						*it -= *other_it;
					}

					return *this;
				}

				template<class Rep2, typename = std::enable_if_t<
		/* requires */ std::is_convertible_v<Rep2, Rep>
				>>
				constexpr fixed_matrix& operator-=(const fixed_matrix<Rep2, Rows, Cols>& other)
				{
					auto it = begin();
					auto other_it = other.begin();

					for (; it != end(); ++it, ++other_it)
					{
						*it -= *other_it;
					}

					return *this;
				}

				template<class Rep2>
				constexpr fixed_matrix& operator*=(const Rep2& other)
				{
					for (auto& value : data_)
					{
						value = value * other;
					}

					return *this;
				}
			private:
				storage_t data_;

				template<class T>
				friend class matrix;
		};

		template<class T, std::size_t S>
		using square_matrix = fixed_matrix<T, S, S>;

		template<class T>
		[[nodiscard]] bool is_square_matrix(const matrix<T>& m) noexcept
		{
			return m.rows() == m.cols();
		}

		template<class T, std::size_t Rows, std::size_t Cols>
		[[nodiscard]] constexpr bool is_square_matrix(const fixed_matrix<T, Rows, Cols>& m)
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
		[[nodiscard]] constexpr square_matrix<T, S> identity_matrix()
		{
			square_matrix<T, S> m;
			auto diag = main_diagonal(m);
			std::fill(std::begin(diag), std::end(diag), T{1});

			return m;
		}

		template<class T>
		[[nodiscard]] matrix<T> identity_matrix(std::size_t size)
		{
			matrix<T> m{size, size};
			auto diag = main_diagonal(m);
			std::fill(std::begin(diag), std::end(diag), T{1});

			return m;
		}
	}
}