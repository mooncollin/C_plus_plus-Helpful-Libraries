module;

#include "cmoon/linear/matrix_definitions.hpp"

export module cmoon.linear.static_matrix;

import <cstddef>;
import <algorithm>;
import <numeric>;
import <concepts>;
import <stdexcept>;
import <type_traits>;

import cmoon.math;
import cmoon.multidimensional;

import cmoon.linear.row;
import cmoon.linear.column;
import cmoon.linear.diagonal;
import cmoon.linear.rows_slice;
import cmoon.linear.columns_slice;
import cmoon.linear.matrix_multiplication;
import cmoon.linear.square_matrix;

namespace cmoon::linear
{
	export
	template<class Rep, std::size_t Rows, std::size_t Cols>
	class static_matrix
	{
		using storage_t = cmoon::static_multidimensional_array<Rep, Rows, Cols>;

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

			constexpr static_matrix() noexcept(std::is_nothrow_default_constructible_v<storage_t>) = default;

			template<std::convertible_to<Rep>... Elements>
				requires (sizeof...(Elements) > 0)
			constexpr static_matrix(Elements&&... elements) noexcept(std::is_nothrow_constructible_v<storage_t, Elements...>)
				: data_{std::forward<Elements>(elements)...} {}

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

			template<class Rep2, class Allocator>
			[[nodiscard]] friend auto operator+(const static_matrix& lhs, const matrix<Rep2, Allocator>& rhs)
			{
				if (Rows != rhs.rows() || Cols != rhs.cols())
				{
					throw std::invalid_argument{"cannot add matrices of different dimension sizes"};
				}

				static_matrix<std::decay_t<decltype(std::declval<Rep>() + std::declval<Rep2>())>, Rows, Cols> m;
				std::transform(lhs.begin(), lhs.end(), rhs.begin(), m.begin(), std::plus<>{});
				return m;
			}

			template<class Rep2, class Allocator>
			[[nodiscard]] friend auto operator-(const static_matrix& lhs, const matrix<Rep2, Allocator>& rhs)
			{
				if (Rows != rhs.rows() || Cols != rhs.cols())
				{
					throw std::invalid_argument{"cannot subtract matrices of different dimension sizes"};
				}

				static_matrix<std::decay_t<decltype(std::declval<Rep>() - std::declval<Rep2>())>, Rows, Cols> m;
				std::transform(lhs.begin(), lhs.end(), rhs.begin(), m.begin(), std::minus<>{});
				return m;
			}

			template<class Rep2>
			[[nodiscard]] friend constexpr auto operator+(const static_matrix& lhs, const static_matrix<Rep2, Rows, Cols>& rhs) noexcept
			{
				static_matrix<std::decay_t<decltype(std::declval<Rep>() + std::declval<Rep2>())>, Rows, Cols> m;
				std::transform(lhs.begin(), lhs.end(), rhs.begin(), m.begin(), std::plus<>{});
				return m;
			}

			template<class Rep2>
			[[nodiscard]] friend constexpr auto operator-(const static_matrix& lhs, const static_matrix<Rep2, Rows, Cols>& rhs) noexcept
			{
				static_matrix<std::decay_t<decltype(std::declval<Rep>() - std::declval<Rep2>())>, Rows, Cols> m;
				std::transform(lhs.begin(), lhs.end(), rhs.begin(), m.begin(), std::minus<>{});
				return m;
			}

			template<class Rep2>
				requires(requires(Rep rep, Rep2 rep2) { rep * rep2; })
			[[nodiscard]] friend constexpr static_matrix operator*(const static_matrix& lhs, const Rep2& rhs) noexcept
			{
				static_matrix<std::decay_t<decltype(std::declval<Rep>() * std::declval<Rep2>())>, Rows, Cols> m;
				std::transform(lhs.begin(), lhs.end(), m.begin(), std::bind(std::multiplies<>{}, std::placeholders::_1, rhs));
				return m;
			}

			template<class Rep2>
				requires(requires(Rep rep, Rep2 rep2) { rep / rep2; })
			[[nodiscard]] friend constexpr auto operator/(const static_matrix& lhs, const Rep2& rhs) noexcept
			{
				static_matrix<std::decay_t<decltype(std::declval<Rep>() / std::declval<Rep2>())>, Rows, Cols> m;
				std::transform(lhs.begin(), lhs.end(), m.begin(), std::bind(std::divides<>{}, std::placeholders::_1, rhs));
				return m;
			}

			template<class Rep2>
				requires(requires(Rep rep, Rep2 rep2) { rep % rep2; })
			[[nodiscard]] friend constexpr auto operator%(const static_matrix& lhs, const Rep2& rhs) noexcept
			{
				static_matrix<std::decay_t<decltype(std::declval<Rep>() % std::declval<Rep2>())>, Rows, Cols> m;
				std::transform(lhs.begin(), lhs.end(), m.begin(), std::bind(std::modulus<>{}, std::placeholders::_1, rhs));
				return m;
			}

			template<class Allocator>
			static_matrix& operator+=(const matrix<Rep, Allocator>& other)
			{
				if (Rows != other.rows() || Cols != other.cols())
				{
					throw std::invalid_argument{"cannot add matrices of different dimension sizes"};
				}

				std::transform(begin(), end(), other.begin(), begin(), std::plus<>{});
				return *this;
			}

			template<class Allocator>
			static_matrix& operator-=(const matrix<Rep, Allocator>& other)
			{
				if (Rows != other.rows() || Cols != other.cols())
				{
					throw std::invalid_argument{"cannot subtract matrices of different dimension sizes"};
				}

				std::transform(begin(), end(), other.begin(), begin(), std::minus<>{});
				return *this;
			}

			constexpr static_matrix& operator+=(const static_matrix& other) noexcept
			{
				std::transform(begin(), end(), other.begin(), begin(), std::plus<>{});
				return *this;
			}

			constexpr static_matrix& operator-=(const static_matrix& other) noexcept
			{
				std::transform(begin(), end(), other.begin(), begin(), std::minus<>{});
				return *this;
			}

			constexpr static_matrix& operator*=(const Rep& other) noexcept
			{
				std::transform(begin(), end(), begin(), std::bind(std::multiplies<>{}, std::placeholders::_1, other));
				return *this;
			}

			constexpr static_matrix& operator/=(const Rep& other) noexcept
			{
				std::transform(begin(), end(), begin(), std::bind(std::divides<>{}, std::placeholders::_1, other));
				return *this;
			}

			constexpr static_matrix& operator%=(const Rep& other) noexcept
			{
				std::transform(begin(), end(), begin(), std::bind(std::modulus<>{}, std::placeholders::_1, other));
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

			[[nodiscard]] friend constexpr bool operator==(const static_matrix& lhs, const static_matrix& rhs) noexcept
			{
				return std::equal(lhs.begin(), lhs.end(), rhs.begin());
			}

			[[nodiscard]] friend constexpr bool operator!=(const static_matrix&, const static_matrix&) noexcept = default;

			template<class Allocator>
			[[nodiscard]] friend bool operator==(const static_matrix& lhs, const matrix<Rep, Allocator>& rhs) noexcept
			{
				return (lhs.rows() == rhs.rows() && lhs.cols() == rhs.cols()) &&
						std::equal(lhs.begin(), lhs.end(), rhs.begin());
			}

			template<class Allocator>
			[[nodiscard]] friend bool operator!=(const static_matrix& lhs, const matrix<Rep, Allocator>& rhs) noexcept
			{
				return !(lhs == rhs);
			}
		private:
			storage_t data_ {};

			template<class T, class Allocator>
			friend class matrix;
	};

	export
	template<class T>
	using matrix3x2 = static_matrix<T, 3, 2>;

	export
	template<class T, class T2, std::size_t Rows, std::size_t Cols, std::size_t Cols2>
	[[nodiscard]] constexpr auto operator*(const static_matrix<T, Rows, Cols>& lhs, const static_matrix<T2, Cols, Cols2>& rhs) noexcept
	{
		if constexpr (Rows == Cols && Cols == Cols2 &&
						cmoon::is_power_of_2(Cols))
		{
			return strassen_mult_par(lhs, rhs);
		}
		else
		{
			return standard_mult(lhs, rhs);
		}
	}

	export
	template<class T, std::size_t Rows, std::size_t Cols>
	[[nodiscard]] constexpr bool is_square_matrix(const static_matrix<T, Rows, Cols>&) noexcept
	{
		return Rows == Cols;
	}

	export
	template<class T, std::size_t Size>
	[[nodiscard]] constexpr diagonal<T, Size> main_diagonal(square_matrix<T, Size>& m) noexcept
	{
		return {m.data(), Size};
	}

	export
	template<class T, std::size_t Size>
	[[nodiscard]] constexpr diagonal<const T, Size> main_diagonal(const square_matrix<T, Size>& m) noexcept
	{
		return {m.data(), Size};
	}

	export
	template<class T, std::size_t S>
	[[nodiscard]] constexpr square_matrix<T, S> identity_matrix() noexcept
	{
		square_matrix<T, S> m;
		std::ranges::fill(main_diagonal(m), T{1});

		return m;
	}
}