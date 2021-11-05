module;

#include "cmoon/linear/matrix_definitions.hpp"

export module cmoon.linear.matrix;

import <memory>;
import <initializer_list>;
import <span>;
import <stdexcept>;
import <algorithm>;

import cmoon.multidimensional;

import cmoon.linear.row;
import cmoon.linear.column;
import cmoon.linear.diagonal;
import cmoon.linear.rows_slice;
import cmoon.linear.columns_slice;
import cmoon.linear.matrix_multiplication;

namespace cmoon::linear
{
	export
	template<class Rep, class Allocator = std::allocator<Rep>>
	class matrix
	{
		using storage_t = cmoon::fixed_multidimensional_array<Rep, 2, Allocator>;

		template<class Rep2, class Allocator2>
		friend class matrix;

		public:
			using value_type = typename storage_t::value_type;
			using allocator_type = typename storage_t::allocator_type;
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

			matrix(const Allocator& alloc = Allocator{})
				: data_{{0, 0}, alloc} {}

			matrix(std::size_t row, std::size_t col, const Allocator& alloc = Allocator{})
				: data_{{row, col}, alloc} {}

			matrix(const matrix&) = default;

			template<std::size_t R, std::size_t C>
			matrix(const static_matrix<Rep, R, C>& other, const Allocator& alloc = Allocator{})
				: data_{other.data_, alloc} {}

			matrix(matrix&&) noexcept = default;

			matrix& operator=(const matrix&) = default;
			matrix& operator=(matrix&&) noexcept = default;

			matrix& operator=(std::initializer_list<Rep> elements)
			{
				data_ = elements;
				return *this;
			}

			template<std::size_t R, std::size_t C>
			matrix& operator=(const static_matrix<Rep, R, C>& other)
			{
				data_ = other.data_;
				return *this;
			}

			void resize(std::size_t rows, std::size_t cols)
			{
				data_.dimensions(rows, cols);
			}

			[[nodiscard]] const Allocator get_allocator() const noexcept
			{
				return data_.get_allocator();
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
			[[nodiscard]] friend auto operator+(const matrix& lhs, const matrix<Rep2, Allocator>& rhs)
			{
				if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols())
				{
					throw std::invalid_argument{"cannot add matrices of different dimension sizes"};
				}

				matrix<std::decay_t<decltype(std::declval<Rep>() + std::declval<Rep2>())>, Allocator> m {lhs.rows(), lhs.cols(), lhs.get_allocator()};
				std::transform(lhs.begin(), lhs.end(), rhs.begin(), m.begin(), std::plus<>{});
				return m;
			}

			template<class Rep2, std::size_t Rows, std::size_t Cols>
			[[nodiscard]] friend auto operator+(const matrix& lhs, const static_matrix<Rep2, Rows, Cols>& rhs)
			{
				if (lhs.rows() != Rows || rhs.cols() != Cols)
				{
					throw std::invalid_argument{"cannot add matrices of different dimension sizes"};
				}

				matrix<std::decay_t<decltype(std::declval<Rep>() + std::declval<Rep2>())>, Allocator> m {lhs.rows(), lhs.cols()};
				std::transform(lhs.begin(), lhs.end(), rhs.begin(), m.begin(), std::plus<>{});
				return m;
			}

			template<class Rep2>
			[[nodiscard]] friend auto operator-(const matrix& lhs, const matrix<Rep2, Allocator>& rhs)
			{
				if (lhs.rows() != rhs.rows() || rhs.cols() != rhs.cols())
				{
					throw std::invalid_argument{"cannot subtract matrices of different dimension sizes"};
				}

				matrix<std::decay_t<decltype(std::declval<Rep>() - std::declval<Rep2>())>, Allocator> m {lhs.rows(), lhs.cols(), lhs.get_allocator()};
				std::transform(lhs.begin(), lhs.end(), rhs.begin(), m.begin(), std::minus<>{});
				return m;
			}

			template<class Rep2, std::size_t Rows, std::size_t Cols>
			[[nodiscard]] friend auto operator-(const matrix& lhs, const static_matrix<Rep2, Rows, Cols>& rhs)
			{
				if (lhs.rows() != Rows || rhs.cols() != Cols)
				{
					throw std::invalid_argument{"cannot subtract matrices of different dimension sizes"};
				}

				matrix<std::decay_t<decltype(std::declval<Rep>() - std::declval<Rep2>())>, Allocator> m{lhs.rows(), lhs.cols()};
				std::transform(lhs.begin(), lhs.end(), rhs.begin(), m.begin(), std::minus<>{});
				return m;
			}

			template<class Rep2>
				requires(requires(value_type rep, Rep2 rep2) { rep * rep2; })
			[[nodiscard]] friend auto operator*(const matrix& lhs, const Rep2& rhs) noexcept
			{
				matrix<std::decay_t<decltype(std::declval<Rep>() * std::declval<Rep2>())>, Allocator> m {lhs.rows(), lhs.cols(), lhs.get_allocator()};
				std::transform(lhs.begin(), lhs.end(), m.begin(), std::bind(std::multiplies{}, std::placeholders::_1, rhs));
				return m;
			}

			template<class Rep2>
				requires(requires(value_type rep, Rep2 rep2) { rep / rep2; })
			[[nodiscard]] friend auto operator/(const matrix& lhs, const Rep2& rhs) noexcept
			{
				matrix<std::decay_t<decltype(std::declval<Rep>() / std::declval<Rep2>())>, Allocator> m{lhs.rows(), lhs.cols(), lhs.get_allocator()};
				std::transform(lhs.begin(), lhs.end(), m.begin(), std::bind(std::divides{}, std::placeholders::_1, rhs));
				return m;
			}

			template<class Rep2>
				requires(requires(value_type rep, Rep2 rep2) { rep % rep2; })
			[[nodiscard]] friend auto operator%(const matrix& lhs, const Rep2& rhs) noexcept
			{
				matrix<std::decay_t<decltype(std::declval<Rep>() % std::declval<Rep2>())>, Allocator> m{lhs.rows(), lhs.cols(), lhs.get_allocator()};
				std::transform(lhs.begin(), lhs.end(), m.begin(), std::bind(std::modulus{}, std::placeholders::_1, rhs));
				return m;
			}
				
			matrix& operator+=(const matrix& other)
			{
				if (rows() != other.rows() || cols() != other.cols())
				{
					throw std::invalid_argument{"cannot add matrices of different dimension sizes"};
				}

				std::transform(begin(), end(), other.begin(), begin(), std::plus<>{});
				return *this;
			}

			matrix& operator-=(const matrix& other)
			{
				if (rows() != other.rows() || cols() != other.cols())
				{
					throw std::invalid_argument{"cannot subtract matrices of different dimension sizes"};
				}

				std::transform(begin(), end(), other.begin(), begin(), std::minus<>{});
				return *this;
			}
				
			template<std::size_t Rows, std::size_t Cols>
			matrix& operator-=(const static_matrix<Rep, Rows, Cols>& other)
			{
				if (rows() != Rows || cols() != Cols)
				{
					throw std::invalid_argument{"cannot subtract matrices of different dimension sizes"};
				}

				std::transform(begin(), end(), other.begin(), begin(), std::minus<>{});
				return *this;
			}

			matrix& operator*=(const Rep& other) noexcept
			{
				std::transform(begin(), end(), begin(), std::bind(std::multiplies{}, std::placeholders::_1, other));
				return *this;
			}

			matrix& operator/=(const Rep& other) noexcept
			{
				std::transform(begin(), end(), begin(), std::bind(std::divides{}, std::placeholders::_1, other));
				return *this;
			}

			matrix& operator%=(const Rep& other) noexcept
			{
				std::transform(begin(), end(), begin(), std::bind(std::modulus{}, std::placeholders::_1, other));
				return *this;
			}

			Rep sum() const noexcept
			{
				return std::reduce(begin(), end());
			}

			constexpr Rep product() const noexcept
			{
				return std::reduce(begin(), end(), Rep{1}, std::multiplies<>{});
			}

			[[nodiscard]] friend bool operator==(const matrix& lhs, const matrix& rhs) noexcept
			{
				return (lhs.rows() == rhs.rows() && lhs.cols() == rhs.cols()) &&
						std::equal(lhs.begin(), lhs.end(), rhs.begin());
			}

			[[nodiscard]] friend bool operator!=(const matrix&, const matrix&) noexcept = default;

			template<std::size_t Rows, std::size_t Cols>
			[[nodiscard]] friend bool operator==(const matrix& lhs, const static_matrix<Rep, Rows, Cols>& rhs) noexcept
			{
				return (lhs.rows() == Rows && lhs.cols() == Cols) &&
						std::equal(lhs.begin(), lhs.end(), rhs.begin());
			}

			template<std::size_t Rows, std::size_t Cols>
			[[nodiscard]] friend bool operator!=(const matrix& lhs, const static_matrix<Rep, Rows, Cols>& rhs) noexcept
			{
				return !(lhs == rhs);
			}
		private:
			storage_t data_;
	};

	export
	template<matrix_type Matrix, matrix_type Matrix2>
	[[nodiscard]] typename auto operator*(const Matrix& lhs, const Matrix2& rhs)
	{
		if (lhs.cols() != rhs.rows())
		{
			throw std::invalid_argument {"cannot multiplies matrices that do not share columns with rows"};
		}

		if (is_square_matrix(lhs) && 
			is_square_matrix(rhs) &&
			cmoon::is_power_of_2(lhs.cols()))
		{
			return details::strassen_mult_par(lhs, rhs);
		}
		else
		{
			return details::standard_mult(lhs, rhs);
		}
	}

	export
	template<class T, class Allocator>
	[[nodiscard]] bool is_square_matrix(const matrix<T, Allocator>& m) noexcept
	{
		return m.rows() == m.cols();
	}

	export
	template<class T, class Allocator>
	[[nodiscard]] diagonal<T, std::dynamic_extent> main_diagonal(matrix<T, Allocator>& m)
	{
		if (!is_square_matrix(m))
		{
			throw std::invalid_argument{"can only find the main diagonal of a square matrix"};
		}

		return {m.data(), m.rows()};
	}

	export
	template<class T, class Allocator>
	[[nodiscard]] diagonal<const T, std::dynamic_extent> main_diagonal(const matrix<T, Allocator>& m)
	{
		if (!is_square_matrix(m))
		{
			throw std::invalid_argument{"can only find the main diagonal of a square matrix"};
		}

		return {m.data(), m.rows()};
	}

	export
	template<class T, class Allocator = std::allocator<T>>
	[[nodiscard]] matrix<T, Allocator> identity_matrix(std::size_t size) noexcept
	{
		matrix<T, Allocator> m{size, size};
		std::ranges::fill(main_diagonal(m), T{1});

		return m;
	}
}