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
		};

		template<class T, std::size_t RowSize>
		class column_iterator : public std::iterator<std::bidirectional_iterator_tag, T>
		{
			using base = std::iterator<std::bidirectional_iterator_tag, T>;

			public:
				constexpr column_iterator(typename base::pointer item, std::size_t row_size) noexcept
					: storage_{item, collin::extent_type<RowSize>{row_size}} {}

				constexpr column_iterator& operator--()
				{
					storage_.data_ -= row_size_.size();
					return *this;
				}

				constexpr column_iterator operator--(int)
				{
					auto temp = *this;
					--(*this);
					return temp;
				}

				constexpr typename base::reference operator*()
				{
					return *storage_.data_;
				}

				constexpr column_iterator& operator++()
				{
					storage_.data_ += storage_.size();
					return *this;
				}

				constexpr column_iterator operator++(int)
				{
					auto temp = *this;
					++(*this);
					return temp;
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
					return *(end() - 1);
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
			private:
				pointer first_;
				collin::extent_type<Extent> size_;
				collin::extent_type<RowSize> row_size_;
		};

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

				std::size_t rows() const noexcept
				{
					return data_.dimensions()[0];
				}

				std::size_t cols() const noexcept
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

				row<value_type, std::dynamic_extent> get_row(std::size_t i)
				{
					return {&data_.get(i, 0), cols()};
				}

				row<const value_type, std::dynamic_extent> get_row(std::size_t i) const
				{
					return {&data_.get(i, 0), cols()};
				}

				column<value_type, std::dynamic_extent, std::dynamic_extent> get_column(std::size_t i)
				{
					return {&data_.get(0, i), cols(), rows()};
				}

				column<const value_type, std::dynamic_extent, std::dynamic_extent> get_column(std::size_t i) const
				{
					return {&data_.get(0, i), cols(), rows()};
				}

				const_pointer data() const noexcept
				{
					return data_.data();
				}

				pointer data() noexcept
				{
					return data_.data();
				}

				iterator begin() noexcept
				{
					return data_.begin();
				}

				const_iterator begin() const noexcept
				{
					return data_.begin();
				}

				const_iterator cbegin() const noexcept
				{
					return data_.cbegin();
				}

				iterator end() noexcept
				{
					return data_.end();
				}

				const_iterator end() const noexcept
				{
					return data_.end();
				}

				const_iterator cend() const noexcept
				{
					return data_.cend();
				}

				reverse_iterator rbegin() noexcept
				{
					return data_.rbegin();
				}

				reverse_iterator rbegin() const noexcept
				{
					return data_.rbegin();
				}

				const_reverse_iterator crbegin() const noexcept
				{
					return data_.crbegin();
				}

				reverse_iterator rend() noexcept
				{
					return data_.rend();
				}

				reverse_iterator rend() const noexcept
				{
					return data_.rend();
				}

				const_reverse_iterator crend() const noexcept
				{
					return data_.crend();
				}

				size_type size() const noexcept
				{
					return data_.size();
				}
				
				template<class Matrix, typename = std::enable_if_t<
		/* requires */	is_either_matrix_v<Matrix>
				>>
				matrix& operator+=(const Matrix& other)
				{
					if (rows() != other.rows() || cols() != other.cols())
					{
						throw std::invalid_argument{"cannot add matrices of different dimension sizes"};
					}

					auto it = begin();
					auto other_it = other.begin();

					for (; it != end(); ++it, ++other_it)
					{
						*it = *it + *other_it;
					}

					return *this;
				}

				template<class Matrix, typename = std::enable_if_t<
		/* requires */	is_either_matrix_v<Matrix>
				>>
				matrix& operator-=(const Matrix& other)
				{
					if (rows() != other.rows() || cols() != other.cols())
					{
						throw std::invalid_argument{"cannot subtract matrices of different dimension sizes"};
					}

					auto it = begin();
					auto other_it = other.begin();

					for (; it != end(); ++it, ++other_it)
					{
						*it = *it - *other_it;
					}

					return *this;
				}

				template<class Rep2>
				matrix& operator*=(const Rep2& other)
				{
					for (auto& value : data_)
					{
						value = value * other;
					}

					return *this;
				}

				matrix transpose() const
				{
					matrix m{cols(), rows()};

					for (std::size_t i = 0; i < rows(); i++)
					{
						for (std::size_t j = 0; j < cols(); j++)
						{
							m(j, i) = (*this)(i, j);
						}
					}

					return m;
				}
			private:
				storage_t data_;
		};

		template<class Matrix1, class Matrix2, typename = std::enable_if_t<
/* requires */	is_either_matrix_v<Matrix1> && is_either_matrix_v<Matrix2>
		>>
		constexpr bool operator==(const Matrix1& lhs, const Matrix2& rhs)
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
		constexpr bool operator!=(const Matrix1& lhs, const Matrix2& rhs)
		{
			return !(lhs == rhs);
		}

		template<class Matrix1, class Matrix2, typename = std::enable_if_t<
/* requires */	is_either_matrix_v<Matrix1> && is_either_matrix_v<Matrix2>
		>>
		constexpr auto operator+(const Matrix1& lhs, const Matrix2& rhs)
		{
			auto m {lhs};
			m += rhs;
			return m;
		}

		template<class Matrix1, class Matrix2, typename = std::enable_if_t<
/* requires */	is_either_matrix_v<Matrix1> && is_either_matrix_v<Matrix2>
		>>
		constexpr auto operator-(const Matrix1& lhs, const Matrix2& rhs)
		{
			auto m {lhs};
			m -= rhs;
			return m;
		}

		template<class Matrix, class Other, typename = std::enable_if_t<
/* requires */	is_either_matrix_v<Matrix>
		>>
		constexpr auto operator*(const Matrix& lhs, const Other& rhs)
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
							sum = sum + (lhs(i, n) * rhs(n, j));
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

				constexpr std::size_t rows() const noexcept
				{
					return Rows;
				}

				constexpr std::size_t cols() const noexcept
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

				constexpr row<value_type, Cols> get_row(std::size_t i)
				{
					return {&data_.get(i, 0), Cols};
				}

				constexpr row<const value_type, Cols> get_row(std::size_t i) const
				{
					return {&data_.get(i, 0), Cols};
				}

				constexpr column<value_type, Cols, Rows> get_column(std::size_t i)
				{
					return {&data_.get(0, i), Cols, Rows};
				}

				constexpr column<const value_type, Cols, Rows> get_column(std::size_t i) const
				{
					return {&data_.get(0, i), Cols, Rows};
				}

				constexpr const_pointer data() const noexcept
				{
					return data_.data();
				}

				constexpr pointer data() noexcept
				{
					return data_.data();
				}

				constexpr iterator begin() noexcept
				{
					return data_.begin();
				}

				constexpr const_iterator begin() const noexcept
				{
					return data_.begin();
				}

				constexpr const_iterator cbegin() const noexcept
				{
					return data_.cbegin();
				}

				constexpr iterator end() noexcept
				{
					return data_.end();
				}

				constexpr const_iterator end() const noexcept
				{
					return data_.end();
				}

				constexpr const_iterator cend() const noexcept
				{
					return data_.cend();
				}

				constexpr reverse_iterator rbegin() noexcept
				{
					return data_.rbegin();
				}

				constexpr reverse_iterator rbegin() const noexcept
				{
					return data_.rbegin();
				}

				constexpr const_reverse_iterator crbegin() const noexcept
				{
					return data_.crbegin();
				}

				constexpr reverse_iterator rend() noexcept
				{
					return data_.rend();
				}

				constexpr reverse_iterator rend() const noexcept
				{
					return data_.rend();
				}

				constexpr const_reverse_iterator crend() const noexcept
				{
					return data_.crend();
				}

				constexpr size_type size() const noexcept
				{
					return data_.size();
				}

				template<class Matrix, typename = std::enable_if_t<
		/* requires */	is_either_matrix_v<Matrix>
				>>
				constexpr fixed_matrix& operator+=(const Matrix& other)
				{
					if constexpr (is_fixed_matrix_v<Matrix>)
					{
						static_assert(Matrix::num_rows == Rows && Matrix::num_columns == Cols, 
							"cannot add matrices of different dimension sizes");
					}
					else
					{
						if (rows() != other.rows() || cols() != other.cols())
						{
							throw std::invalid_argument{"cannot add matrices of different dimension sizes"};
						}
					}

					auto it = begin();
					auto other_it = other.begin();

					for (; it != end(); ++it, ++other_it)
					{
						*it = *it + *other_it;
					}

					return *this;
				}

				template<class Matrix, typename = std::enable_if_t<
		/* requires */	is_either_matrix_v<Matrix>
				>>
				constexpr fixed_matrix& operator-=(const Matrix& other)
				{
					if constexpr (is_fixed_matrix_v<Matrix>)
					{
						static_assert(Matrix::num_rows == Rows && Matrix::num_columns == Cols, 
							"cannot subtract matrices of different dimension sizes");
					}
					else
					{
						if (rows() != other.rows() || cols() != other.cols())
						{
							throw std::invalid_argument{"cannot subtract matrices of different dimension sizes"};
						}
					}

					auto it = begin();
					auto other_it = other.begin();

					for (; it != end(); ++it, ++other_it)
					{
						*it = *it - *other_it;
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

				constexpr fixed_matrix<Rep, Cols, Rows> transpose() const
				{
					fixed_matrix<Rep, Cols, Rows> m;

					for (std::size_t i = 0; i < rows(); i++)
					{
						for (std::size_t j = 0; j < cols(); j++)
						{
							m(j, i) = (*this)(i, j);
						}
					}

					return m;
				}
			private:
				storage_t data_;

				template<class T>
				friend class matrix;
		};

		template<class T, std::size_t S>
		using square_matrix = fixed_matrix<T, S, S>;

		template<class T, std::size_t S>
		constexpr square_matrix<T, S> identity_matrix()
		{
			square_matrix<T, S> m;
			for (std::size_t i = 0; i < S; i++)
			{
				m(i, i) = T{1};
			}

			return m;
		}
	}
}