export module cmoon.simd.impl.operations;

import <concepts>;
import <algorithm>;
import <type_traits>;
import <cstdint>;
import <functional>;
import <limits>;

import <immintrin.h>;

import cmoon.meta;
import cmoon.platform;

import cmoon.simd.simd_size;
import cmoon.simd.memory_alignment;
import cmoon.simd.tag;

namespace cmoon::simd_abi
{
	template<class Derived, class T, class V, std::size_t N>
	struct simd_operations_base
	{
		using value_type = T;
		using physical_type = V;

		[[nodiscard]] inline static constexpr std::size_t virtual_size() noexcept
		{
			return N;
		}

		[[nodiscard]] inline static constexpr std::size_t physical_size() noexcept
		{
			return simd_size_v<value_type, physical_type>;
		}

		[[nodiscard]] inline static physical_type zero() noexcept
		{
			return Derived::broadcast(value_type(0));
		}

		[[nodiscard]] inline static physical_type max() noexcept
		{
			return Derived::broadcast(std::numeric_limits<value_type>::max());
		}

		template<class G>
		inline static physical_type gen(G&& g) noexcept
		{
			using g_args = cmoon::meta::make_index_sequence<virtual_size()>;

			return [&]<std::size_t... I>(cmoon::meta::value_list<I...>) {
				return Derived::set(g(std::integral_constant<std::size_t, I>{})...);
			}(g_args{});
		}

		template<simd_flag_type Flag = element_aligned_tag>
		[[nodiscard]] inline static physical_type load(const value_type* mem, Flag = {}) noexcept
		{
			if constexpr (virtual_size() == physical_size())
			{
				return Derived::load_all(mem);
			}
			else
			{
				value_type buf[physical_size()];
				std::copy_n(mem, virtual_size(), buf);
				return Derived::load_all(buf);
			}
		}

		template<std::size_t A>
			requires(A != alignof(value_type))
		[[nodiscard]] inline static physical_type load(const value_type* mem, overaligned_tag<A> f) noexcept
		{
			if constexpr (virtual_size() == physical_size())
			{
				return Derived::load_all(mem, f);
			}
			else
			{
				alignas(A) value_type buf[physical_size()];
				std::copy_n(mem, virtual_size(), buf);
				return Derived::load_all(buf, f);
			}
		}

		template<simd_flag_type Flag = element_aligned_tag>
		inline static void store(value_type* buf, physical_type v, Flag = {}) noexcept
		{
			if constexpr (virtual_size() == physical_size())
			{
				Derived::store_all(buf, v);
			}
			else
			{
				value_type buf2[physical_size()];
				Derived::store_all(buf2, v);
				std::copy_n(buf2, virtual_size(), buf);
			}
		}

		template<std::size_t A>
			requires(A != alignof(value_type))
		inline static void store(value_type* buf, physical_type v, overaligned_tag<A> f) noexcept
		{
			if constexpr (virtual_size() == physical_size())
			{
				Derived::store_all(buf, v, f);
			}
			else
			{
				alignas(A) value_type buf2[physical_size()];
				Derived::store_all(buf2, v, f);
				std::copy_n(buf2, virtual_size(), buf);
			}
		}

		[[nodiscard]] inline static value_type get(physical_type v, std::size_t i) noexcept
		{
			value_type buf[physical_size()];
			Derived::store_all(buf, v);
			return buf[i];
		}

		template<class F>
		[[nodiscard]] inline static physical_type transform(physical_type v1, physical_type v2, F&& f) noexcept
		{
			value_type buf1[physical_size()];
			value_type buf2[physical_size()];

			Derived::store_all(buf1, v1);
			Derived::store_all(buf2, v2);

			std::ranges::transform(buf1, buf2, buf2, std::forward<F>(f));

			return Derived::load_all(buf2);
		}

		template<class F>
		[[nodiscard]] inline static physical_type transform(physical_type v1, F&& f) noexcept
		{
			value_type buf1[physical_size()];

			Derived::store_all(buf1, v1);

			std::ranges::transform(buf1, buf1, std::forward<F>(f));

			return Derived::load_all(buf1);
		}

		[[nodiscard]] inline static physical_type plus(physical_type v1, physical_type v2) noexcept
		{
			return transform(v1, v2, std::plus<value_type>{});
		}

		[[nodiscard]] inline static physical_type minus(physical_type v1, physical_type v2) noexcept
		{
			return transform(v1, v2, std::minus<value_type>{});
		}

		[[nodiscard]] inline static physical_type multiply(physical_type v1, physical_type v2) noexcept
		{
			return transform(v1, v2, std::multiplies<value_type>{});
		}

		[[nodiscard]] inline static physical_type modulus(physical_type v1, physical_type v2) noexcept
		{
			return transform(v1, v2, std::modulus<value_type>{});
		}

		[[nodiscard]] inline static physical_type logical_and(physical_type v1, physical_type v2) noexcept
		{
			return transform(v1, v2, std::logical_and<value_type>{});
		}

		[[nodiscard]] inline static physical_type logical_or(physical_type v1, physical_type v2) noexcept
		{
			return transform(v1, v2, std::logical_or<value_type>{});
		}

		[[nodiscard]] inline static physical_type logical_not(physical_type v1) noexcept
		{
			return transform(v1, std::logical_not<value_type>{});
		}

		[[nodiscard]] inline static physical_type bit_and(physical_type v1, physical_type v2) noexcept
		{
			return transform(v1, v2, std::bit_and<value_type>{});
		}

		[[nodiscard]] inline static physical_type bit_or(physical_type v1, physical_type v2) noexcept
		{
			return transform(v1, v2, std::bit_or<value_type>{});
		}

		[[nodiscard]] inline static physical_type bit_xor(physical_type v1, physical_type v2) noexcept
		{
			return transform(v1, v2, std::bit_xor<value_type>{});
		}

		[[nodiscard]] inline static physical_type increment(physical_type v) noexcept
		{
			return Derived::plus(v, Derived::broadcast(value_type(1)));
		}

		[[nodiscard]] inline static physical_type decrement(physical_type v) noexcept
		{
			return Derived::minus(v, Derived::broadcast(value_type(1)));
		}

		[[nodiscard]] inline static physical_type negate(physical_type v) noexcept
		{
			return Derived::minus(Derived::zero(), v);
		}

		[[nodiscard]] inline static physical_type eq(physical_type v1, physical_type v2) noexcept
		{
			transform(v1, v2, std::equal_to<value_type>{});
		}

		[[nodiscard]] inline static physical_type neq(physical_type v1, physical_type v2) noexcept
		{
			return Derived::logical_not(Derived::eq(v1, v2));
		}

		[[nodiscard]] inline static physical_type lt(physical_type v1, physical_type v2) noexcept
		{
			return transform(v1, v2, std::less<value_type>{});
		}

		[[nodiscard]] inline static physical_type gt(physical_type v1, physical_type v2) noexcept
		{
			return lt(v2, v1);
		}

		[[nodiscard]] inline static physical_type le(physical_type v1, physical_type v2) noexcept
		{
			return logical_not(lt(v2, v1));
		}

		[[nodiscard]] inline static physical_type ge(physical_type v1, physical_type v2) noexcept
		{
			return logical_not(lt(v1, v2));
		}
	};

	export
	template<class T, class V, std::size_t N>
	struct simd_operations;

	struct simd_m128i_operations
	{
		[[nodiscard]] inline static __m128i zero() noexcept
		{
			return _mm_setzero_si128();
		}

		[[nodiscard]] inline static __m128i max() noexcept
		{
			return _mm_set1_epi8(std::numeric_limits<std::int8_t>::max());
		}

		[[nodiscard]] inline static __m128i left_shift(__m128i v1, int n) noexcept
		{
			if (n >= 64)
			{
				return _mm_slli_epi64(_mm_slli_si128(v1, 8),
									  n - 64);
			}

			return _mm_or_si128(
				_mm_slli_epi64(v1, n),
				_mm_srli_epi64(
					_mm_slli_si128(v1, 8),
					64 - n
				)
			);
		}

		[[nodiscard]] inline static __m128i right_shift(__m128i v1, int n) noexcept
		{
			if (n >= 64)
			{
				return _mm_srli_epi64(_mm_srli_si128(v1, 8),
									  n - 64);
			}

			return _mm_or_si128(
				_mm_srli_epi64(v1, n),
				_mm_slli_epi64(
					_mm_srli_si128(v1, 8),
					64 - n
				)
			);
		}

		[[nodiscard]] inline static __m128i left_shift(__m128i v1, __m128i v2) noexcept
		{
			return left_shift(v1, _mm_extract_epi32(v2, 1));
		}

		[[nodiscard]] inline static __m128i right_shift(__m128i v1, __m128i v2) noexcept
		{
			return right_shift(v1, _mm_extract_epi32(v2, 1));
		}

		[[nodiscard]] inline static __m128i bit_and(__m128i v1, __m128i v2) noexcept
		{
			return _mm_and_si128(v1, v2);
		}

		[[nodiscard]] inline static __m128i bit_or(__m128i v1, __m128i v2) noexcept
		{
			return _mm_or_si128(v1, v2);
		}

		[[nodiscard]] inline static __m128i bit_xor(__m128i v1, __m128i v2) noexcept
		{
			return _mm_xor_si128(v1, v2);
		}

		[[nodiscard]] inline static __m128i logical_not(__m128i v1) noexcept
		{
			return _mm_andnot_si128(v1, max());
		}
	};

	export
	template<std::size_t N>
	struct simd_operations<std::int8_t, __m128i, N> : public simd_m128i_operations, simd_operations_base<simd_operations<std::int8_t, __m128i, N>, std::int8_t, __m128i, N>
	{
		private:
			using base = simd_operations_base<simd_operations<std::int8_t, __m128i, N>, std::int8_t, __m128i, N>;
		public:
		using value_type = std::int8_t;
		using physical_type = __m128i;

		static inline const auto mask_lo {_mm_set_epi8(-128, -128, -128, -128, -128, -128, -128, -128, 14, 12, 10, 8, 6, 4, 2, 0)};
		static inline const auto mask_hi {_mm_set_epi8(14, 12, 10, 8, 6, 4, 2, 0, -128, -128, -128, -128, -128, -128, -128, -128)};

		using simd_m128i_operations::zero;
		using simd_m128i_operations::max;
		using simd_m128i_operations::logical_not;
		using simd_m128i_operations::bit_and;
		using simd_m128i_operations::bit_or;
		using simd_m128i_operations::bit_xor;

		[[nodiscard]] inline static physical_type broadcast(value_type v1) noexcept
		{
			return _mm_set1_epi8(v1);
		}

		template<std::convertible_to<value_type>... Args>
			requires(sizeof...(Args) == base::virtual_size())
		[[nodiscard]] inline static physical_type set(Args... args) noexcept
		{
			if constexpr (base::virtual_size() == base::physical_size())
			{
				return _mm_setr_epi8(args...);
			}
			else
			{
				using extra_args = cmoon::meta::gen_value_list_t<base::physical_size() - base::virtual_size(), value_type(0)>;

				return [=]<auto... Zeros>(cmoon::meta::value_list<Zeros...>) {
					return _mm_setr_epi8(args..., Zeros...);
				}(extra_args{});
			}
		}

		template<simd_flag_type Flag = element_aligned_tag>
		inline static void store_all(value_type* buf, physical_type v, [[maybe_unused]] Flag = {}) noexcept
		{
			_mm_store_si128(reinterpret_cast<__m128i*>(buf), v);
		}

		template<std::size_t A>
			requires(A != alignof(value_type))
		inline static void store_all(value_type* buf, physical_type v, overaligned_tag<A>) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(buf), v);
		}

		template<std::size_t I>
			requires(I < base::virtual_size())
		[[nodiscard]] inline static value_type get(physical_type v) noexcept
		{
			return static_cast<value_type>(_mm_extract_epi8(v, static_cast<int>(I)));
		}

		[[nodiscard]] inline static value_type get(physical_type v, std::size_t i) noexcept
		{
			if constexpr (base::virtual_size() == 1)
			{
				return get<0>(v);
			}
			else
			{
				return base::get(v, i);
			}
		}

		template<simd_flag_type Flag = element_aligned_tag>
		[[nodiscard]] inline static physical_type load_all(const value_type* mem, [[maybe_unused]] Flag = {}) noexcept
		{
			return _mm_load_si128(reinterpret_cast<const __m128i*>(mem));
		}

		template<std::size_t A>
			requires(A != alignof(value_type))
		[[nodiscard]] inline static physical_type load_all(const value_type* mem, overaligned_tag<A>) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(mem));
		}

		[[nodiscard]] inline static physical_type plus(physical_type v1, physical_type v2) noexcept
		{
			return _mm_add_epi8(v1, v2);
		}

		[[nodiscard]] inline static physical_type minus(physical_type v1, physical_type v2) noexcept
		{
			return _mm_sub_epi8(v1, v2);
		}

		[[nodiscard]] inline static physical_type multiply(physical_type v1, physical_type v2) noexcept
		{
			const auto v1_lo {_mm_cvtepu8_epi16(v1)};
			const auto v1_hi {_mm_unpackhi_epi8(v1, _mm_setzero_si128())};
			const auto v2_lo {_mm_cvtepu8_epi16(v2)};
			const auto v2_hi {_mm_unpackhi_epi8(v2, _mm_setzero_si128())};
			const auto r_lo {_mm_mullo_epi16(v1_lo, v2_lo)};
			const auto r_hi {_mm_mullo_epi16(v1_hi, v2_hi)};

			return _mm_or_si128(_mm_shuffle_epi8(r_lo, mask_lo),
								_mm_shuffle_epi8(r_hi, mask_hi));
		}

		[[nodiscard]] inline static physical_type divide(physical_type v1, physical_type v2) noexcept
		{
			return _mm_div_epi8(v1, v2);
		}

		[[nodiscard]] inline static physical_type eq(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmpeq_epi8(v1, v2);
		}

		[[nodiscard]] inline static physical_type lt(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmplt_epi8(v1, v2);
		}

		[[nodiscard]] inline static physical_type gt(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmpgt_epi8(v1, v2);
		}
	};

	export
	template<std::size_t N>
	struct simd_operations<std::int16_t, __m128i, N> : public simd_m128i_operations, simd_operations_base<simd_operations<std::int16_t, __m128i, N>, std::int16_t, __m128i, N>
	{
		private:
			using base = simd_operations_base<simd_operations<std::int16_t, __m128i, N>, std::int16_t, __m128i, N>;
		public:
		using value_type = std::int16_t;
		using physical_type = __m128i;

		using simd_m128i_operations::zero;
		using simd_m128i_operations::max;
		using simd_m128i_operations::logical_not;
		using simd_m128i_operations::bit_and;
		using simd_m128i_operations::bit_or;
		using simd_m128i_operations::bit_xor;

		template<std::same_as<std::int8_t>>
		[[nodiscard]] inline static physical_type convert(physical_type v) noexcept
		{
			return _mm_cvtepu8_epi16(v);
		}

		[[nodiscard]] inline static physical_type broadcast(value_type v1) noexcept
		{
			return _mm_set1_epi16(v1);
		}

		template<std::convertible_to<value_type>... Args>
			requires(sizeof...(Args) == base::virtual_size())
		[[nodiscard]] inline static physical_type set(Args... args) noexcept
		{
			if constexpr (base::virtual_size() == base::physical_size())
			{
				return _mm_setr_epi16(args...);
			}
			else
			{
				using extra_args = cmoon::meta::gen_value_list_t<base::physical_size() - base::virtual_size(), value_type(0)>;

				return [=]<auto... Zeros>(cmoon::meta::value_list<Zeros...>) {
					return _mm_setr_epi16(args..., Zeros...);
				}(extra_args{});
			}
		}

		template<simd_flag_type Flag = element_aligned_tag>
		inline static void store_all(value_type* buf, physical_type v, [[maybe_unused]] Flag = {}) noexcept
		{
			_mm_store_si128(reinterpret_cast<__m128i*>(buf), v);
		}

		template<std::size_t A>
			requires(A != alignof(value_type))
		inline static void store_all(value_type* buf, physical_type v, overaligned_tag<A>) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(buf), v);
		}

		template<std::size_t I>
			requires(I < base::virtual_size())
		[[nodiscard]] inline static value_type get(physical_type v) noexcept
		{
			return static_cast<value_type>(_mm_extract_epi16(v, static_cast<int>(I)));
		}

		[[nodiscard]] inline static value_type get(physical_type v, std::size_t i) noexcept
		{
			if constexpr (base::virtual_size() == 1)
			{
				return get<0>(v);
			}
			else
			{
				return base::get(v, i);
			}
		}

		template<simd_flag_type Flag = element_aligned_tag>
		[[nodiscard]] inline static physical_type load_all(const value_type* mem, [[maybe_unused]] Flag = {}) noexcept
		{
			return _mm_load_si128(reinterpret_cast<const __m128i*>(mem));
		}

		template<std::size_t A>
			requires(A != alignof(value_type))
		[[nodiscard]] inline static physical_type load_all(const value_type* mem, overaligned_tag<A>) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(mem));
		}

		[[nodiscard]] inline static physical_type plus(physical_type v1, physical_type v2) noexcept
		{
			return _mm_add_epi16(v1, v2);
		}

		[[nodiscard]] inline static physical_type minus(physical_type v1, physical_type v2) noexcept
		{
			return _mm_sub_epi16(v1, v2);
		}

		[[nodiscard]] inline static physical_type multiply(physical_type v1, physical_type v2) noexcept
		{
			return _mm_mullo_epi16(v1, v2);
		}

		[[nodiscard]] inline static physical_type divide(physical_type v1, physical_type v2) noexcept
		{
			return _mm_div_epi16(v1, v2);
		}

		[[nodiscard]] inline static physical_type eq(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmpeq_epi16(v1, v2);
		}

		[[nodiscard]] inline static physical_type lt(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmplt_epi16(v1, v2);
		}

		[[nodiscard]] inline static physical_type gt(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmpgt_epi16(v1, v2);
		}
	};

	export
	template<std::size_t N>
	struct simd_operations<std::int32_t, __m128i, N> : public simd_m128i_operations, simd_operations_base<simd_operations<std::int32_t, __m128i, N>, std::int32_t, __m128i, N>
	{
		private:
			using base = simd_operations_base<simd_operations<std::int32_t, __m128i, N>, std::int32_t, __m128i, N>;
		public:
		using value_type = std::int32_t;
		using physical_type = __m128i;

		using simd_m128i_operations::zero;
		using simd_m128i_operations::max;
		using simd_m128i_operations::logical_not;
		using simd_m128i_operations::bit_and;
		using simd_m128i_operations::bit_or;
		using simd_m128i_operations::bit_xor;

		template<std::same_as<float>>
		[[nodiscard]] inline static physical_type convert(__m128 v) noexcept
		{
			return _mm_cvtps_epi32(v);
		}
		
		template<std::same_as<double>>
		[[nodiscard]] inline static physical_type convert(__m128d v) noexcept
		{
			return _mm_cvtpd_epi32(v);
		}

		template<std::same_as<std::int8_t>>
		[[nodiscard]] inline static physical_type convert(physical_type v) noexcept
		{
			return _mm_cvtepu8_epi32(v);
		}

		template<std::same_as<std::int16_t>>
		[[nodiscard]] inline static physical_type convert(physical_type v) noexcept
		{
			return _mm_cvtepu16_epi32(v);
		}

		[[nodiscard]] inline static physical_type broadcast(value_type v1) noexcept
		{
			return _mm_set1_epi32(v1);
		}

		template<std::convertible_to<value_type>... Args>
			requires(sizeof...(Args) == base::virtual_size())
		[[nodiscard]] inline static physical_type set(Args... args) noexcept
		{
			if constexpr (base::virtual_size() == base::physical_size())
			{
				return _mm_setr_epi32(args...);
			}
			else
			{
				using extra_args = cmoon::meta::gen_value_list_t<base::physical_size() - base::virtual_size(), value_type(0)>;

				return [=]<auto... Zeros>(cmoon::meta::value_list<Zeros...>) {
					return _mm_setr_epi32(args..., Zeros...);
				}(extra_args{});
			}
		}

		template<simd_flag_type Flag = element_aligned_tag>
		inline static void store_all(value_type* buf, physical_type v, [[maybe_unused]] Flag = {}) noexcept
		{
			_mm_store_si128(reinterpret_cast<__m128i*>(buf), v);
		}

		template<std::size_t A>
			requires(A != alignof(value_type))
		inline static void store_all(value_type* buf, physical_type v, overaligned_tag<A>) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(buf), v);
		}

		template<std::size_t I>
			requires(I < base::virtual_size())
		[[nodiscard]] inline static value_type get(physical_type v) noexcept
		{
			return static_cast<value_type>(_mm_extract_epi32(v, static_cast<int>(I)));
		}

		[[nodiscard]] inline static value_type get(physical_type v, std::size_t i) noexcept
		{
			if constexpr (base::virtual_size() == 1)
			{
				return get<0>(v);
			}
			else
			{
				return base::get(v, i);
			}
		}

		template<simd_flag_type Flag = element_aligned_tag>
		[[nodiscard]] inline static physical_type load_all(const value_type* mem, [[maybe_unused]] Flag = {}) noexcept
		{
			return _mm_load_si128(reinterpret_cast<const __m128i*>(mem));
		}

		template<std::size_t A>
			requires(A != alignof(value_type))
		[[nodiscard]] inline static physical_type load_all(const value_type* mem, overaligned_tag<A>) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(mem));
		}

		[[nodiscard]] inline static physical_type plus(physical_type v1, physical_type v2) noexcept
		{
			return _mm_add_epi32(v1, v2);
		}

		[[nodiscard]] inline static physical_type minus(physical_type v1, physical_type v2) noexcept
		{
			return _mm_sub_epi32(v1, v2);
		}

		[[nodiscard]] inline static physical_type multiply(physical_type v1, physical_type v2) noexcept
		{
			return _mm_mullo_epi32(v1, v2);
		}

		[[nodiscard]] inline static physical_type divide(physical_type v1, physical_type v2) noexcept
		{
			return _mm_div_epi32(v1, v2);
		}

		[[nodiscard]] inline static physical_type eq(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmpeq_epi32(v1, v2);
		}

		[[nodiscard]] inline static physical_type lt(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmplt_epi32(v1, v2);
		}

		[[nodiscard]] inline static physical_type gt(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmpgt_epi32(v1, v2);
		}
	};

	export
	template<std::size_t N>
	struct simd_operations<std::int64_t, __m128i, N> : public simd_m128i_operations, simd_operations_base<simd_operations<std::int64_t, __m128i, N>, std::int64_t, __m128i, N>
	{
		private:
			using base = simd_operations_base<simd_operations<std::int64_t, __m128i, N>, std::int64_t, __m128i, N>;
		public:
		using value_type = std::int64_t;
		using physical_type = __m128i;

		using simd_m128i_operations::zero;
		using simd_m128i_operations::max;
		using simd_m128i_operations::logical_not;
		using simd_m128i_operations::bit_and;
		using simd_m128i_operations::bit_or;
		using simd_m128i_operations::bit_xor;

		template<std::same_as<std::int16_t>>
		[[nodiscard]] inline static physical_type convert(physical_type v) noexcept
		{
			return _mm_cvtepu16_epi64(v);
		}

		template<std::same_as<std::int32_t>>
		[[nodiscard]] inline static physical_type convert(physical_type v) noexcept
		{
			return _mm_cvtepu32_epi64(v);
		}

		[[nodiscard]] inline static physical_type broadcast(value_type v1) noexcept
		{
			return _mm_set1_epi64x(v1);
		}

		template<std::convertible_to<value_type>... Args>
			requires(sizeof...(Args) == base::virtual_size())
		[[nodiscard]] inline static physical_type set(Args... args) noexcept
		{
			if constexpr (base::virtual_size() == base::physical_size())
			{
				return _mm_setr_epi64(args...);
			}
			else
			{
				using extra_args = cmoon::meta::gen_value_list_t<base::physical_size() - base::virtual_size(), value_type(0)>;

				return [=]<auto... Zeros>(cmoon::meta::value_list<Zeros...>) {
					return _mm_setr_epi64(args..., Zeros...);
				}(extra_args{});
			}
		}

		template<simd_flag_type Flag = element_aligned_tag>
		inline static void store_all(value_type* buf, physical_type v, [[maybe_unused]] Flag = {}) noexcept
		{
			_mm_store_si128(reinterpret_cast<__m128i*>(buf), v);
		}

		template<std::size_t A>
			requires(A != alignof(value_type))
		inline static void store_all(value_type* buf, physical_type v, overaligned_tag<A>) noexcept
		{
			_mm_storeu_si128(reinterpret_cast<__m128i*>(buf), v);
		}

		template<std::size_t I>
			requires(I < base::virtual_size())
		[[nodiscard]] inline static value_type get(physical_type v) noexcept
		{
			return static_cast<value_type>(_mm_extract_epi64(v, static_cast<int>(I)));
		}

		[[nodiscard]] inline static value_type get(physical_type v, std::size_t i) noexcept
		{
			if constexpr (base::virtual_size() == 1)
			{
				return get<0>(v);
			}
			else
			{
				return base::get(v, i);
			}
		}

		template<simd_flag_type Flag = element_aligned_tag>
		[[nodiscard]] inline static physical_type load_all(const value_type* mem, [[maybe_unused]] Flag = {}) noexcept
		{
			return _mm_load_si128(reinterpret_cast<const __m128i*>(mem));
		}

		template<std::size_t A>
			requires(A != alignof(value_type))
		[[nodiscard]] inline static physical_type load_all(const value_type* mem, overaligned_tag<A>) noexcept
		{
			return _mm_loadu_si128(reinterpret_cast<const __m128i*>(mem));
		}

		[[nodiscard]] inline static physical_type plus(physical_type v1, physical_type v2) noexcept
		{
			return _mm_add_epi64(v1, v2);
		}

		[[nodiscard]] inline static physical_type minus(physical_type v1, physical_type v2) noexcept
		{
			return _mm_sub_epi64(v1, v2);
		}

		[[nodiscard]] inline static physical_type multiply(physical_type v1, physical_type v2) noexcept
		{
			const auto ac {_mm_mul_epu32(v1, v2)};
			const auto b {_mm_srli_epi64(v1, 32)};
			const auto bc {_mm_mul_epu32(b, v2)};
			const auto d {_mm_srli_epi64(v2, 32)};
			const auto ad {_mm_mul_epu32(v1, d)};
			const auto high {_mm_slli_epi64(_mm_add_epi64(bc, ad),
											 32)};

			return _mm_add_epi64(high, ac);
		}

		[[nodiscard]] inline static physical_type divide(physical_type v1, physical_type v2) noexcept
		{
			return _mm_div_epi64(v1, v2);
		}

		[[nodiscard]] inline static physical_type eq(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmpeq_epi64(v1, v2);
		}

		[[nodiscard]] inline static physical_type lt(physical_type v1, physical_type v2) noexcept
		{
			return gt(v2, v1);
		}

		[[nodiscard]] inline static physical_type gt(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmpgt_epi64(v1, v2);
		}
	};

	export
	template<std::size_t N>
	struct simd_operations<float, __m128, N> : public simd_operations_base<simd_operations<float, __m128, N>, float, __m128, N>
	{
		private:
			using base = simd_operations_base<simd_operations<float, __m128, N>, float, __m128, N>;
		public:
		using value_type = float;
		using physical_type = __m128;

		template<std::same_as<std::int32_t>>
		[[nodiscard]] inline static physical_type convert(__m128i v) noexcept
		{
			return _mm_castsi128_ps(v);
		}

		[[nodiscard]] inline static physical_type zero() noexcept
		{
			return _mm_setzero_ps();
		}

		[[nodiscard]] inline static physical_type broadcast(value_type v1) noexcept
		{
			return _mm_set_ps1(v1);
		}

		template<std::convertible_to<value_type>... Args>
			requires(sizeof...(Args) == base::virtual_size())
		[[nodiscard]] inline static physical_type set(Args... args) noexcept
		{
			if constexpr (base::virtual_size() == base::physical_size())
			{
				return _mm_setr_ps(args...);
			}
			else
			{
				using extra_args = cmoon::meta::gen_value_list_t<base::physical_size() - base::virtual_size(), value_type(0)>;

				return [=]<auto... Zeros>(cmoon::meta::value_list<Zeros...>) {
					return _mm_setr_ps(args..., Zeros...);
				}(extra_args{});
			}
		}

		template<simd_flag_type Flag = element_aligned_tag>
		inline static void store_all(value_type* buf, physical_type v, [[maybe_unused]] Flag = {}) noexcept
		{
			_mm_store_ps(buf, v);
		}

		template<std::size_t A>
			requires(A != alignof(value_type))
		inline static void store_all(value_type* buf, physical_type v, overaligned_tag<A>) noexcept
		{
			_mm_storeu_ps(buf, v);
		}

		template<std::size_t I>
			requires(I < base::virtual_size())
		[[nodiscard]] inline static value_type get(physical_type v) noexcept
		{
			return _mm_cvtss_f32(
				_mm_shuffle_ps(v, v, _MM_SHUFFLE(static_cast<unsigned int>(I),
												 static_cast<unsigned int>(I),
												 static_cast<unsigned int>(I),
												 static_cast<unsigned int>(I)))
			);
		}

		[[nodiscard]] inline static value_type get(physical_type v, std::size_t i) noexcept
		{
			if constexpr (base::virtual_size() == 1)
			{
				return _mm_cvtss_f32(v);
			}
			else
			{
				return base::get(v, i);
			}
		}

		template<simd_flag_type Flag = element_aligned_tag>
		[[nodiscard]] inline static physical_type load_all(const value_type* mem, Flag = {}) noexcept
		{
			return _mm_load_ps(mem);
		}

		template<std::size_t A>
			requires(A != alignof(value_type))
		[[nodiscard]] inline static physical_type load_all(const value_type* mem, overaligned_tag<A>) noexcept
		{
			return _mm_loadu_ps(mem);
		}

		[[nodiscard]] inline static physical_type plus(physical_type v1, physical_type v2) noexcept
		{
			return _mm_add_ps(v1, v2);
		}

		[[nodiscard]] inline static physical_type minus(physical_type v1, physical_type v2) noexcept
		{
			return _mm_sub_ps(v1, v2);
		}

		[[nodiscard]] inline static physical_type multiply(physical_type v1, physical_type v2) noexcept
		{
			return _mm_mul_ps(v1, v2);
		}

		[[nodiscard]] inline static physical_type divide(physical_type v1, physical_type v2) noexcept
		{
			return _mm_div_ps(v1, v2);
		}

		[[nodiscard]] inline static physical_type bit_and(physical_type v1, physical_type v2) noexcept
		{
			return _mm_and_ps(v1, v2);
		}

		[[nodiscard]] inline static physical_type bit_or(physical_type v1, physical_type v2) noexcept
		{
			return _mm_or_ps(v1, v2);
		}

		[[nodiscard]] inline static physical_type bit_xor(physical_type v1, physical_type v2) noexcept
		{
			return _mm_xor_ps(v1, v2);
		}

		[[nodiscard]] inline static physical_type eq(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmpeq_ps(v1, v2);
		}

		[[nodiscard]] inline static physical_type neq(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmpneq_ps(v1, v2);
		}

		[[nodiscard]] inline static physical_type lt(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmplt_ps(v1, v2);
		}

		[[nodiscard]] inline static physical_type gt(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmpgt_ps(v1, v2);
		}

		[[nodiscard]] inline static physical_type le(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmple_ps(v1, v2);
		}

		[[nodiscard]] inline static physical_type ge(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmpge_ps(v1, v2);
		}
	};

	export
	template<std::size_t N>
	struct simd_operations<double, __m128d, N> : public simd_operations_base<simd_operations<double, __m128d, N>, double, __m128d, N>
	{
		private:
			using base = simd_operations_base<simd_operations<double, __m128d, N>, double, __m128d, N>;
		public:
		using value_type = double;
		using physical_type = __m128d;

		template<std::same_as<std::int32_t>>
		[[nodiscard]] inline static physical_type convert(__m128i v) noexcept
		{
			return _mm_cvtepi32_pd(v);
		}

		[[nodiscard]] inline static physical_type zero() noexcept
		{
			return _mm_setzero_pd();
		}

		[[nodiscard]] inline static physical_type broadcast(value_type v1) noexcept
		{
			return _mm_set_pd1(v1);
		}

		template<std::convertible_to<value_type>... Args>
			requires(sizeof...(Args) == base::virtual_size())
		[[nodiscard]] inline static physical_type set(Args... args) noexcept
		{
			if constexpr (base::virtual_size() == base::physical_size())
			{
				return _mm_set_pd(args...);
			}
			else
			{
				using extra_args = cmoon::meta::gen_value_list_t<base::physical_size() - base::virtual_size(), value_type(0)>;

				return [=]<auto... Zeros>(cmoon::meta::value_list<Zeros...>) {
					return _mm_set_pd(args..., Zeros...);
				}(extra_args{});
			}
		}

		template<simd_flag_type Flag = element_aligned_tag>
		inline static void store_all(value_type* buf, physical_type v, [[maybe_unused]] Flag = {}) noexcept
		{
			_mm_store_pd(buf, v);
		}

		template<std::size_t A>
			requires(A != alignof(value_type))
		inline static void store_all(value_type* buf, physical_type v, overaligned_tag<A>) noexcept
		{
			_mm_storeu_pd(buf, v);
		}

		template<std::size_t I>
			requires(I < base::virtual_size())
		[[nodiscard]] inline static value_type get(physical_type v) noexcept
		{
			return get(v, I);
		}

		[[nodiscard]] inline static value_type get(physical_type v, std::size_t i) noexcept
		{
			if constexpr (base::virtual_size() == 1)
			{
				return _mm_cvtsd_f64(v);
			}
			else
			{
				return base::get(v, i);
			}
		}

		template<simd_flag_type Flag = element_aligned_tag>
		[[nodiscard]] inline static physical_type load_all(const value_type* mem, Flag = {}) noexcept
		{
			return _mm_load_pd(mem);
		}

		template<std::size_t A>
			requires(A != alignof(value_type))
		[[nodiscard]] inline static physical_type load_all(const value_type* mem, overaligned_tag<A>) noexcept
		{
			return _mm_loadu_pd(mem);
		}

		[[nodiscard]] inline static physical_type plus(physical_type v1, physical_type v2) noexcept
		{
			return _mm_add_pd(v1, v2);
		}

		[[nodiscard]] inline static physical_type minus(physical_type v1, physical_type v2) noexcept
		{
			return _mm_sub_pd(v1, v2);
		}

		[[nodiscard]] inline static physical_type multiply(physical_type v1, physical_type v2) noexcept
		{
			return _mm_mul_pd(v1, v2);
		}

		[[nodiscard]] inline static physical_type divide(physical_type v1, physical_type v2) noexcept
		{
			return _mm_div_pd(v1, v2);
		}

		[[nodiscard]] inline static physical_type bit_and(physical_type v1, physical_type v2) noexcept
		{
			return _mm_and_pd(v1, v2);
		}

		[[nodiscard]] inline static physical_type bit_or(physical_type v1, physical_type v2) noexcept
		{
			return _mm_or_pd(v1, v2);
		}

		[[nodiscard]] inline static physical_type bit_xor(physical_type v1, physical_type v2) noexcept
		{
			return _mm_xor_pd(v1, v2);
		}

		[[nodiscard]] inline static physical_type eq(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmpeq_pd(v1, v2);
		}

		[[nodiscard]] inline static physical_type neq(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmpneq_pd(v1, v2);
		}

		[[nodiscard]] inline static physical_type lt(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmplt_pd(v1, v2);
		}

		[[nodiscard]] inline static physical_type gt(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmpgt_pd(v1, v2);
		}

		[[nodiscard]] inline static physical_type le(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmple_pd(v1, v2);
		}

		[[nodiscard]] inline static physical_type ge(physical_type v1, physical_type v2) noexcept
		{
			return _mm_cmpge_pd(v1, v2);
		}
	};

	export
	template<class T>
	struct is_simd_operations : std::false_type {};

	export
	template<class T, class V, std::size_t N>
	struct is_simd_operations<simd_operations<T, V, N>> : std::true_type {};

	export
	template<class T>
	inline constexpr auto is_simd_operations_v = is_simd_operations<T>::value;

	export
	template<class T>
	concept simd_operations_type = is_simd_operations_v<T>;

	export
	template<class T, class From, class V>
	concept can_simd_convert = simd_operations_type<T> &&
		requires(V v)
	{
		{ T::template convert<From>(v) } -> std::same_as<typename T::physical_type>;
	};
}