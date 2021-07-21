module;

#include "cmoon/linear/matrix_definitions.hpp"

export module cmoon.linear.matrix_multiplication;

import <type_traits>;
import <numeric>;
import <future>;

import cmoon.linear.is_matrix;
import cmoon.linear.square_matrix;


namespace cmoon::linear
{
	template<matrix_type M1, matrix_type M2>
	struct common_matrix_type;

	template<class T, class T2, class Allocator>
	struct common_matrix_type<matrix<T, Allocator>, matrix<T2, Allocator>>
	{
		using common_rep = std::common_type_t<T, T2>;
		using type = matrix<common_rep, Allocator>;
		using half_type_first = matrix<T, Allocator>;
		using half_type_second = matrix<T2, Allocator>;
		using half_type_common = type;
		static constexpr auto is_fixed {false};
	};

	template<class T, class T2, class Allocator, std::size_t Rows, std::size_t Cols>
	struct common_matrix_type<matrix<T, Allocator>, static_matrix<T2, Rows, Cols>>
		: public common_matrix_type<matrix<T, Allocator>, matrix<T2, Allocator>> {};

	template<class T, class T2, class Allocator, std::size_t Rows, std::size_t Cols>
	struct common_matrix_type<static_matrix<T2, Rows, Cols>, matrix<T, Allocator>>
		: public common_matrix_type<matrix<T, Allocator>, matrix<T2, Allocator>> {};

	template<class T, class T2, std::size_t Rows, std::size_t Cols, std::size_t Cols2>
	struct common_matrix_type<static_matrix<T, Rows, Cols>, static_matrix<T2, Cols, Cols2>>
	{
		using common_rep = std::common_type_t<T, T2>;
		using type = static_matrix<common_rep, Rows, Cols2>;
		using half_type_first = static_matrix<T, Rows / 2, Cols / 2>;
		using half_type_second = static_matrix<T2, Cols / 2, Cols2 / 2>;
		using half_type_common = static_matrix<common_rep, Rows / 2, Cols2 / 2>;
		static constexpr auto is_fixed {true};
	};

	constexpr std::size_t strassen_dim_cutoff {300};
	constexpr std::size_t strassen_dim_cutoff2 {1000};

	template<class T, class Allocator>
	[[nodiscard]] matrix<T, Allocator> get_part(std::size_t pi, std::size_t pj, const matrix<T, Allocator>& m) noexcept
	{
		const std::size_t new_dim {m.rows() / 2};
		matrix<T, Allocator> p{new_dim, new_dim};
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

	export
	template<matrix_type M1, matrix_type M2>
	[[nodiscard]] constexpr typename common_matrix_type<M1, M2>::type standard_mult(const M1& lhs, const M2& rhs) noexcept
	{
		using matrix_specs = common_matrix_type<M1, M2>;

		const auto rows = lhs.rows();
		const auto columns = rhs.cols();

		typename matrix_specs::type m;
		if constexpr (!matrix_specs::is_fixed)
		{
			m.resize(rows, columns);
		}

		for (std::size_t i {0}; i < rows; ++i)
		{
			for (std::size_t j {0}; j < columns; ++j)
			{
				if (std::is_constant_evaluated())
				{
					for (std::size_t n {0}; n < lhs.cols(); ++n)
					{
						m(i, j) += lhs(i, n) * rhs(n, j);
					}
				}
				else
				{
					m(i, j) = std::transform_reduce(std::begin(lhs.get_row(i)),
													std::end(lhs.get_row(i)),
													std::begin(rhs.get_column(j)), typename matrix_specs::common_rep{0});
				}
			}
		}

		return m;
	}

	export
	template<matrix_type M1, matrix_type M2>
	[[nodiscard]] typename common_matrix_type<M1, M2>::type strassen_mult(const M1& a, const M2& b) noexcept
	{
		using matrix_specs = common_matrix_type<M1, M2>;
		const auto n = a.rows();

		if (n < strassen_dim_cutoff)
		{
			return standard_mult(a, b);
		}

		typename matrix_specs::type result;
		if constexpr (!matrix_specs::is_fixed)
		{
			result.resize(n, n);
		}

		const auto k = n / 2;

		typename matrix_specs::type a11{k, k};
		typename matrix_specs::type a12{k, k};
		typename matrix_specs::type a21{k, k};
		typename matrix_specs::type a22{k, k};
		typename matrix_specs::type b11{k, k};
		typename matrix_specs::type b12{k, k};
		typename matrix_specs::type b21{k, k};
		typename matrix_specs::type b22{k, k};
		if constexpr (!matrix_specs::is_fixed)
		{
			a11.resize(k, k);
			a12.resize(k, k);
			a21.resize(k, k);
			a22.resize(k, k);
			b11.resize(k, k);
			b12.resize(k, k);
			b21.resize(k, k);
			b22.resize(k, k);
		}

		for (std::size_t i {0}; i < k; ++i)
		{
			for (std::size_t j {0}; j < k; ++j)
			{
				a11(i, j) = a(i, j);
				a12(i, j) = a(i, k + j);
				a21(i, j) = a(k + i, j);
				a22(i, j) = a(k + i, k + j);
						
				b11(i, j) = b(i, j);
				b12(i, j) = b(i, k + j);
				b21(i, j) = b(k + i, j);
				b22(i, j) = b(k + i, k + j);
			}
		}

		const auto p1 = strassen_mult(a11, b12 - b22);
		const auto p2 = strassen_mult(a11 + a12, b22);
		const auto p3 = strassen_mult(a21 + a22, b11);
		const auto p4 = strassen_mult(a22, b21 - b11);
		const auto p5 = strassen_mult(a11 + a22, b11 + b22);
		const auto p6 = strassen_mult(a12 - a22, b21 + b22);
		const auto p7 = strassen_mult(a11 - a21, b11 + b12);

		const auto c11 = ((p5 + p4) + p6) - p2;
		const auto c12 = p1 + p2;
		const auto c21 = p3 + p4;
		const auto c22 = ((p5 + p1) - p3) - p7;

		for (std::size_t i {0}; i < k; ++i)
		{
			for (std::size_t j {0}; j < k; ++j)
			{
				result(i, j) = c11(i, j);
				result(i, j + k) = c12(i, j);
				result(k + i, j) = c21(i, j);
				result(k + i, k + j) = c22(i, j);
			}
		}

		return result;
	}

	template<matrix_type M1, matrix_type M2>
	[[nodiscard]] inline typename common_matrix_type<M1, M2>::type strassen_mult_par(const M1& a, const M2& b);

	export
	template<matrix_type M1, matrix_type M2>
	[[nodiscard]] typename common_matrix_type<M1, M2>::type strassen_mult_par_impl(const M1& a, const M2& b)
	{
		using matrix_specs = common_matrix_type<M1, M2>;
		using result_t = typename matrix_specs::type;

		using get_part_a_t = typename matrix_specs::half_type_first(*)(std::size_t, std::size_t, const M1&);
		using get_part_b_t = typename matrix_specs::half_type_second(*)(std::size_t, std::size_t, const M2&);
		using f_t = typename matrix_specs::half_type_common(*)(const typename matrix_specs::half_type_first&, const typename matrix_specs::half_type_second&);
		using set_part_f_t = void(*)(std::size_t, std::size_t, typename matrix_specs::type&, const typename matrix_specs::half_type_common&);

		auto b12_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 1, std::cref(b));
		auto a11_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 0, 0, std::cref(a));
		auto a12_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 0, 1, std::cref(a));
		auto a21_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 1, 0, std::cref(a));
		auto a22_future = std::async(std::launch::async, static_cast<get_part_a_t>(get_part), 1, 1, std::cref(a));

		auto b11_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 0, 0, std::cref(b));
		auto b21_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 1, 0, std::cref(b));
		auto b22_future = std::async(std::launch::async, static_cast<get_part_b_t>(get_part), 1, 1, std::cref(b));

		const auto b12 = b12_future.get();
		const auto b22 = b22_future.get();
		const auto b12_22 = b12 - b22;
		const auto a11 = a11_future.get();
		auto p1_future = std::async(std::launch::async, static_cast<f_t>(strassen_mult_par), std::cref(a11), std::cref(b12_22));

		const auto a12 = a12_future.get();
		const auto a1112 = a11 + a12;
		auto p2_future = std::async(std::launch::async, static_cast<f_t>(strassen_mult_par), std::cref(a1112), std::cref(b22));

		const auto a21 = a21_future.get();
		const auto a22 = a22_future.get();
		const auto a2122 = a21 + a22;
		const auto b11 = b11_future.get();
		auto p3_future = std::async(std::launch::async, static_cast<f_t>(strassen_mult_par), std::cref(a2122), std::cref(b11));

		const auto b21 = b21_future.get();
		const auto b21_11 = b21 - b11;
		auto p4_future = std::async(std::launch::async, static_cast<f_t>(strassen_mult_par), std::cref(a22), std::cref(b21_11));

		const auto a1122 = a11 + a22;
		const auto b1122 = b11 + b22;
		auto p5_future = std::async(std::launch::async, static_cast<f_t>(strassen_mult_par), std::cref(a1122), std::cref(b1122));
				
		const auto a12_a22 = a12 - a22;
		const auto b2122 = b21 + b22;
		auto p6_future = std::async(std::launch::async, static_cast<f_t>(strassen_mult_par), std::cref(a12_a22), std::cref(b2122));

		const auto a21_11 = a21 - a11;
		const auto b1112 = b11 + b12;
		auto p7_future = std::async(std::launch::async, static_cast<f_t>(strassen_mult_par), std::cref(a21_11), std::cref(b1112));

		result_t c;
		if constexpr (!matrix_specs::is_fixed)
		{
			c.resize(a.rows(), a.cols());
		}

		const auto p1 = p1_future.get();
		const auto p2 = p2_future.get();
		const auto c12 = p1 + p2;

		const auto p3 = p3_future.get();
		const auto p4 = p4_future.get();
		const auto c21 = p3 + p4;

		const auto p5 = p5_future.get();
		const auto p6 = p6_future.get();
		const auto c11 = ((p5 + p4) + p6) - p2;

		const auto p7 = p7_future.get();
		const auto c22 = ((p5 + p1) - p3) - p7;

		const auto k = a.rows() / 2;

		for (std::size_t i{0}; i < k; ++i)
		{
			for (std::size_t j{0}; j < k; ++j)
			{
				c(i, j) = c11(i, j);
				c(i, j + k) = c12(i, j);
				c(k + i, j) = c21(i, j);
				c(k + i, k + j) = c22(i, j);
			}
		}

		return c;
	}

	export
	template<matrix_type M1, matrix_type M2>
	[[nodiscard]] inline typename common_matrix_type<M1, M2>::type strassen_mult_par(const M1& a, const M2& b)
	{
		using matrix_specs = common_matrix_type<M1, M2>;
		if constexpr (matrix_specs::is_fixed)
		{
			if constexpr (M1::num_rows < strassen_dim_cutoff)
			{
				return standard_mult(a, b);
			}
			else if constexpr (M1::num_rows < strassen_dim_cutoff2)
			{
				return strassen_mult(a, b);
			}
			else
			{
				return strassen_mult_par_impl(a, b);
			}
		}
		else
		{
			if (a.rows() < strassen_dim_cutoff)
			{
				return standard_mult(a, b);
			}
			else if (a.rows() < strassen_dim_cutoff2)
			{
				return strassen_mult(a, b);
			}
			else
			{
				return strassen_mult_par_impl(a, b);
			}
		}
	}
}