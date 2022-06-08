export module cmoon.simd.simd;

import <cstddef>;
import <cstdint>;
import <concepts>;
import <utility>;
import <type_traits>;

import <immintrin.h>;

import cmoon.concepts;

import cmoon.simd.compatible;
import cmoon.simd.native;
import cmoon.simd.fixed_size;
import cmoon.simd.simd_size;
import cmoon.simd.impl.operations;
import cmoon.simd.impl.deduce_physical;
import cmoon.simd.deduce;
import cmoon.simd.tag;

namespace cmoon
{
	export
	template<class T, class Abi>
	class simd_mask
	{
		public:
			simd_mask() = delete;
			~simd_mask() = delete;
			simd_mask(const simd_mask&) = delete;
			simd_mask& operator=(const simd_mask&) = delete;
	};

	export
	template<class T, class Abi = simd_abi::compatible<T>>
	class simd
	{
		public:
			simd() = delete;
			~simd() = delete;
			simd(const simd&) = delete;
			simd& operator=(const simd&) = delete;
	};

	export
	template<class T, class Abi>
		requires(simd_abi::has_physical_type<T, simd_abi::simd_size_v<T, Abi>>)
	class simd<T, Abi>
	{
		template<class T2, class Abi2>
		friend class simd;

		public:
			using value_type = T;
			using abi_type = Abi;
			using mask_type = simd_mask<value_type, abi_type>;

			[[nodiscard]] static constexpr std::size_t size() noexcept
			{
				return simd_abi::simd_size_v<value_type, abi_type>;
			}

			using physical_type = simd_abi::deduce_physical_t<value_type, size()>;
		private:
			using op = cmoon::simd_abi::simd_operations<value_type, physical_type, size()>;

			class reference_t
			{
				public:
					reference_t() = delete;
					reference_t(const reference_t&) = delete;

					[[nodiscard]] operator value_type() const noexcept
					{
						return std::as_const(*owner)[index];
					}

					template<std::convertible_to<value_type> U>
					reference_t operator=(U&& x) && noexcept
					{
						value_type buf[op::physical_size()];
						op::store_all(buf, owner->v);
						buf[index] = std::forward<U>(x);
						owner->v = op::load_all(buf);

						return {*owner, index};
					}

					template<class U>
						requires(cmoon::plus_assignable<value_type, U>)
					reference_t operator+=(U&& x) && noexcept
					{
						value_type buf[op::physical_size()];
						op::store_all(buf, owner->v);
						buf[index] += std::forward<U>(x);
						owner->v = op::load_all(buf);

						return {*owner, index};
					}

					template<class U>
						requires(cmoon::minus_assignable<value_type, U>)
					reference_t operator-=(U&& x) && noexcept
					{
						value_type buf[op::physical_size()];
						op::store_all(buf, owner->v);
						buf[index] -= std::forward<U>(x);
						owner->v = op::load_all(buf);

						return {*owner, index};
					}

					template<class U>
						requires(cmoon::multiply_assignable<value_type, U>)
					reference_t operator*=(U&& x) && noexcept
					{
						value_type buf[op::physical_size()];
						op::store_all(buf, owner->v);
						buf[index] *= std::forward<U>(x);
						owner->v = op::load_all(buf);

						return {*owner, index};
					}

					template<class U>
						requires(cmoon::divide_assignable<value_type, U>)
					reference_t operator/=(U&& x) && noexcept
					{
						value_type buf[op::physical_size()];
						op::store_all(buf, owner->v);
						buf[index] /= std::forward<U>(x);
						owner->v = op::load_all(buf);

						return {*owner, index};
					}

					template<class U>
						requires(cmoon::modulus_assignable<value_type, U>)
					reference_t operator%=(U&& x) && noexcept
					{
						value_type buf[op::physical_size()];
						op::store_all(buf, owner->v);
						buf[index] %= std::forward<U>(x);
						owner->v = op::load_all(buf);

						return {*owner, index};
					}

					template<class U>
						requires(cmoon::bit_or_assignable<value_type, U>)
					reference_t operator|=(U&& x) && noexcept
					{
						value_type buf[op::physical_size()];
						op::store_all(buf, owner->v);
						buf[index] |= std::forward<U>(x);
						owner->v = op::load_all(buf);

						return {*owner, index};
					}

					template<class U>
						requires(cmoon::bit_and_assignable<value_type, U>)
					reference_t operator&=(U&& x) && noexcept
					{
						value_type buf[op::physical_size()];
						op::store_all(buf, owner->v);
						buf[index] &= std::forward<U>(x);
						owner->v = op::load_all(buf);

						return {*owner, index};
					}

					template<class U>
						requires(cmoon::bit_xor_assignable<value_type, U>)
					reference_t operator^=(U&& x) && noexcept
					{
						value_type buf[op::physical_size()];
						op::store_all(buf, owner->v);
						buf[index] ^= std::forward<U>(x);
						owner->v = op::load_all(buf);

						return {*owner, index};
					}

					reference_t operator++() && noexcept
					{
						value_type buf[op::physical_size()];
						op::store_all(buf, owner->v);
						++buf[index];
						owner->v = op::load_all(buf);

						return {*owner, index};
					}

					value_type operator++(int) && noexcept
					{
						value_type buf[op::physical_size()];
						op::store_all(buf, owner->v);
						const auto tmp {buf[index]++};
						owner->v = op::load_all(buf);

						return tmp;
					}

					reference_t operator--() && noexcept
					{
						value_type buf[op::physical_size()];
						op::store_all(buf, owner->v);
						--buf[index];
						owner->v = op::load_all(buf);

						return {*owner, index};
					}

					value_type operator--(int) && noexcept
					{
						value_type buf[op::physical_size()];
						op::store_all(buf, owner->v);
						const auto tmp {buf[index]--};
						owner->v = op::load_all(buf);

						return tmp;
					}

					void swap(reference_t&& other) && noexcept
					{
						value_type buf[op::physical_size()];
						value_type buf2[op::physical_size()];

						op::store_all(buf, owner->v);
						op::store_all(buf2, other.owner->v);

						std::swap(buf[index], buf2[other.index]);

						owner->v = op::load_all(buf);
						other.owner->v = op::load_all(buf2);
					}

					void swap(value_type& a) && noexcept
					{
						value_type buf[op::physical_size()];
						op::store_all(buf, owner->v);
						std::swap(a, buf[index]);
						owner->v = op::load_all(buf);
					}

					friend void swap(reference_t&& a, reference_t&& b) noexcept
					{
						std::move(a).swap(std::move(b));
					}

					friend void swap(value_type& a, reference_t&& b) noexcept
					{
						std::move(b).swap(a);
					}

					friend void swap(reference_t&& a, value_type& b) noexcept
					{
						std::move(a).swap(b);
					}

					[[nodiscard]] friend bool operator==(const reference_t& a, const reference_t& b) noexcept
					{
						return static_cast<value_type>(a) == static_cast<value_type>(b);
					}

					[[nodiscard]] friend bool operator!=(const reference_t& a, const reference_t& b) noexcept
					{
						return !(a == b);
					}

					[[nodiscard]] friend bool operator<(const reference_t& a, const reference_t& b) noexcept
					{
						return static_cast<value_type>(a) < static_cast<value_type>(b);
					}

					[[nodiscard]] friend bool operator>(const reference_t& a, const reference_t& b) noexcept
					{
						return b < a;
					}

					[[nodiscard]] friend bool operator<=(const reference_t& a, const reference_t& b) noexcept
					{
						return !(b < a);
					}

					[[nodiscard]] friend bool operator>=(const reference_t& a, const reference_t& b) noexcept
					{
						return !(a < b);
					}
				private:
					reference_t(simd& owner, std::size_t index) noexcept
						: owner{std::addressof(owner)}, index{index} {}

					simd* owner;
					std::size_t index;

					friend class simd;
			};

			friend class reference_t;
		public:
			using reference = reference_t;

			simd() noexcept = default;

			simd(physical_type v) noexcept
				: v{v} {}

			template<std::convertible_to<value_type> U>
			simd(U&& u) noexcept
				: v{op::broadcast(static_cast<value_type>(std::forward<U>(u)))} {}

			simd(const simd<value_type, simd_abi::fixed_size<size()>>& x) noexcept
				: v{static_cast<physical_type>(x)} {}

			template<std::convertible_to<value_type> U>
				requires(can_simd_convert<op, U, physical_type>)
			simd(const simd<U, simd_abi::fixed_size<size()>>& x) noexcept
				: v{op::template convert(static_cast<typename simd<U, simd_abi::fixed_size<size()>>::physical_type>(x))} {}

			template<std::convertible_to<value_type> U>
			simd(const simd<U, simd_abi::fixed_size<size()>>& x) noexcept
			{
				using other_op = typename simd_abi::fixed_size<size()>::op;

				value_type buf1[op::physical_size()];
				U buf2[simd<U, other_op::physical_size()>];

				other_op::store_all(buf2, x.v);

				std::transform(buf2,
							   buf2 + size(),
							   buf1,
							   [](const auto& i) { return static_cast<value_type>(i); });

				v = op::load_all(buf1);
			}

			template<std::invocable<std::integral_constant<std::size_t, 0>> G>
			simd(G&& gen) noexcept
				: v{op::gen(std::forward<G>(gen))} {}

			template<simd_flag_type Flags>
			simd(const value_type* mem, Flags f) noexcept
				: v{op::load(mem, f)} {}

			template<std::convertible_to<value_type> U, simd_flag_type Flags>
			simd(const U* mem, Flags f) noexcept
			{
				copy_from(mem, f);
			}

			[[nodiscard]] reference operator[](std::size_t i) noexcept
			{
				return {*this, i};
			}

			[[nodiscard]] value_type operator[](std::size_t i) const noexcept
			{
				return op::get(v, i);
			}

			template<std::convertible_to<value_type> U, simd_flag_type Flags>
			void copy_from(const U* mem, [[maybe_unused]] Flags f) noexcept
			{
				if constexpr (std::same_as<U, value_type>)
				{
					v = op::load(mem, f);
				}
				else
				{
					value_type buf[op::physical_size()];
					std::transform(mem,
								   mem + size(),
								   buf,
								   [](const auto& m) { return static_cast<value_type>(m); });
					v = op::load_all(buf);
				}
			}

			template<std::convertible_to<value_type> U, simd_flag_type Flags>
			void copy_to(U* mem, [[maybe_unused]] Flags f) const noexcept
			{
				if constexpr (std::same_as<U, value_type>)
				{
					op::store(mem, v, f);
				}
				else
				{
					value_type buf[op::physical_size()];
					op::store_all(buf, v);
					std::transform(buf,
								   buf + size(),
								   mem,
								   [](const auto& m) { return static_cast<value_type>(m); });
				}
			}

			[[nodiscard]] explicit operator physical_type() const noexcept
			{
				return v;
			}

			simd& operator++() noexcept
			{
				v = op::increment(v);
				return *this;
			}

			simd operator++(int) noexcept
			{
				auto tmp {*this};
				++*this;
				return tmp;
			}

			simd& operator--() noexcept
			{
				v = op::decrement(v);
				return *this;
			}

			simd operator--(int) noexcept
			{
				auto tmp{*this};
				--*this;
				return tmp;
			}

			[[nodiscard]] simd operator+() const noexcept
			{
				return *this;
			}

			[[nodiscard]] simd operator-() const noexcept
			{
				return simd{op::negate(v)};
			}

			simd& operator+=(const simd& other) noexcept
			{
				v = op::plus(v, other.v);
				return *this;
			}

			simd& operator-=(const simd& other) noexcept
			{
				v = op::minus(v, other.v);
				return *this;
			}

			simd& operator*=(const simd& other) noexcept
			{
				v = op::multiply(v, other.v);
				return *this;
			}

			simd& operator/=(const simd& other) noexcept
			{
				v = op::divide(v, other.v);
				return *this;
			}

			simd& operator%=(const simd& other) noexcept
			{
				v = op::modulus(v, other.v);
				return *this;
			}

			simd& operator&=(const simd& other) noexcept
			{
				v = op::bit_and(v, other.v);
				return *this;
			}

			simd& operator|=(const simd& other) noexcept
			{
				v = op::bit_or(v, other.v);
				return *this;
			}

			simd& operator^=(const simd& other) noexcept
			{
				v = op::bit_xor(v, other.v);
				return *this;
			}

			simd& operator<<=(const simd& other) noexcept
			{
				v = op::left_shift(v, other.v);
				return *this;
			}

			simd& operator>>=(const simd& other) noexcept
			{
				v = op::right_shift(v, other.v);
				return *this;
			}

			simd& operator<<=(int n) noexcept
			{
				v = op::left_shift(v, n);
				return *this;
			}

			simd& operator>>=(int n) noexcept
			{
				v = op::right_shift(v, n);
				return *this;
			}

			[[nodiscard]] friend simd operator+(simd lhs, const simd& rhs) noexcept
			{
				return lhs += rhs;
			}

			[[nodiscard]] friend simd operator-(simd lhs, const simd& rhs) noexcept
			{
				return lhs -= rhs;
			}

			[[nodiscard]] friend simd operator*(simd lhs, const simd& rhs) noexcept
			{
				return lhs *= rhs;
			}

			[[nodiscard]] friend simd operator/(simd lhs, const simd& rhs) noexcept
			{
				return lhs /= rhs;
			}

			[[nodiscard]] friend simd operator%(simd lhs, const simd& rhs) noexcept
			{
				return lhs %= rhs;
			}

			[[nodiscard]] friend simd operator&(simd lhs, const simd& rhs) noexcept
			{
				return lhs &= rhs;
			}

			[[nodiscard]] friend simd operator|(simd lhs, const simd& rhs) noexcept
			{
				return lhs |= rhs;
			}

			[[nodiscard]] friend simd operator^(simd lhs, const simd& rhs) noexcept
			{
				return lhs ^= rhs;
			}

			[[nodiscard]] friend simd operator<<(simd lhs, const simd& rhs) noexcept
			{
				return lhs <<= rhs;
			}

			[[nodiscard]] friend simd operator>>(simd lhs, const simd& rhs) noexcept
			{
				return lhs >>= rhs;
			}

			[[nodiscard]] friend simd operator<<(simd lhs, int n) noexcept
			{
				return lhs <<= n;
			}

			[[nodiscard]] friend simd operator>>(simd lhs, int n) noexcept
			{
				return lhs >>= n;
			}

			[[nodiscard]] friend mask_type operator==(const simd& lhs, const simd& rhs) noexcept
			{
				return mask_type{op::eq(lhs.v, rhs.v)};
			}

			[[nodiscard]] friend mask_type operator!=(const simd& lhs, const simd& rhs) noexcept
			{
				return mask_type{op::neq(lhs.v, rhs.v)};
			}

			[[nodiscard]] friend mask_type operator<(const simd& lhs, const simd& rhs) noexcept
			{
				return mask_type{op::lt(lhs.v, rhs.v)};
			}

			[[nodiscard]] friend mask_type operator>(const simd& lhs, const simd& rhs) noexcept
			{
				return mask_type{op::gt(lhs.v, rhs.v)};
			}

			[[nodiscard]] friend mask_type operator<=(const simd& lhs, const simd& rhs) noexcept
			{
				return mask_type{op::le(lhs.v, rhs.v)};
			}

			[[nodiscard]] friend mask_type operator>=(const simd& lhs, const simd& rhs) noexcept
			{
				return mask_type{op::ge(lhs.v, rhs.v)};
			}
		private:
			physical_type v;
	};

	export
	template<class T, std::size_t N>
	using fixed_size_simd = simd<T, simd_abi::fixed_size<N>>;

	export
	template<class T>
	using native_simd = simd<T, simd_abi::native<T>>;

	export
	template<class T>
	struct is_simd : std::false_type {};

	export
	template<class T, class Abi>
		requires(std::destructible<simd<T, Abi>>)
	struct is_simd<simd<T, Abi>> : std::true_type {};

	export
	template<class T>
	inline constexpr auto is_simd_v = is_simd<T>::value;

	export
	template<class T, class Abi>
		requires(is_simd_v<simd<T, Abi>>)
	class simd_mask<T, Abi>
	{
		public:
			using value_type = bool;
			using abi_type = Abi;
			using simd_type = simd<T, abi_type>;

			[[nodiscard]] static constexpr std::size_t size() noexcept
			{
				return simd_abi::simd_size_v<T, abi_type>;
			}

			using physical_type = simd_abi::deduce_physical_t<T, size()>;
		private:
			using op = cmoon::simd_abi::simd_operations<T, physical_type, size()>;

			class reference_t
			{
				public:
					reference_t() = delete;
					reference_t(const reference_t&) = delete;

					[[nodiscard]] operator value_type() const noexcept
					{
						return std::as_const(*owner)[index];
					}

					template<std::convertible_to<value_type> U>
					reference_t operator=(U&& x) && noexcept
					{
						T buf[op::physical_size()];
						op::store_all(buf, owner->v);
						buf[index] = std::forward<U>(x);
						owner->v = op::load_all(buf);

						return {*owner, index};
					}

					template<class U>
						requires(cmoon::bit_or_assignable<value_type, U>)
					reference_t operator|=(U&& x) && noexcept
					{
						value_type buf[op::physical_size()];
						op::store_all(buf, owner->v);
						buf[index] |= std::forward<U>(x);
						owner->v = op::load_all(buf);

						return {*owner, index};
					}

					template<class U>
						requires(cmoon::bit_and_assignable<value_type, U>)
					reference_t operator&=(U&& x) && noexcept
					{
						value_type buf[op::physical_size()];
						op::store_all(buf, owner->v);
						buf[index] &= std::forward<U>(x);
						owner->v = op::load_all(buf);

						return {*owner, index};
					}

					template<class U>
						requires(cmoon::bit_xor_assignable<value_type, U>)
					reference_t operator^=(U&& x) && noexcept
					{
						value_type buf[op::physical_size()];
						op::store_all(buf, owner->v);
						buf[index] ^= std::forward<U>(x);
						owner->v = op::load_all(buf);

						return {*owner, index};
					}

					void swap(reference_t&& other) && noexcept
					{
						value_type buf[op::physical_size()];
						value_type buf2[op::physical_size()];

						op::store_all(buf, owner->v);
						op::store_all(buf2, other.owner->v);

						std::swap(buf[index], buf2[other.index]);

						owner->v = op::load_all(buf);
						other.owner->v = op::load_all(buf2);
					}

					void swap(value_type& a) && noexcept
					{
						value_type buf[op::physical_size()];
						op::store_all(buf, owner->v);
						std::swap(a, buf[index]);
						owner->v = op::load_all(buf);
					}

					friend void swap(reference_t&& a, reference_t&& b) noexcept
					{
						std::move(a).swap(std::move(b));
					}

					friend void swap(value_type& a, reference_t&& b) noexcept
					{
						std::move(b).swap(a);
					}

					friend void swap(reference_t&& a, value_type& b) noexcept
					{
						std::move(a).swap(b);
					}

					[[nodiscard]] friend bool operator==(const reference_t& a, const reference_t& b) noexcept
					{
						return static_cast<value_type>(a) == static_cast<value_type>(b);
					}

					[[nodiscard]] friend bool operator!=(const reference_t& a, const reference_t& b) noexcept
					{
						return !(a == b);
					}

					[[nodiscard]] friend bool operator<(const reference_t& a, const reference_t& b) noexcept
					{
						return static_cast<value_type>(a) < static_cast<value_type>(b);
					}

					[[nodiscard]] friend bool operator>(const reference_t& a, const reference_t& b) noexcept
					{
						return b < a;
					}

					[[nodiscard]] friend bool operator<=(const reference_t& a, const reference_t& b) noexcept
					{
						return !(b < a);
					}

					[[nodiscard]] friend bool operator>=(const reference_t& a, const reference_t& b) noexcept
					{
						return !(a < b);
					}
				private:
					reference_t(simd_mask& owner, std::size_t index) noexcept
						: owner{std::addressof(owner)}, index{index} {}

					simd_mask* owner;
					std::size_t index;

					friend class simd_mask;
			};

			friend class reference_t;
		public:
			using reference = reference_t;

			simd_mask() noexcept = default;

			simd_mask(physical_type v) noexcept
				: v{v} {}

			simd_mask(value_type u) noexcept
				: v{u} {}

			simd_mask(const simd_mask<value_type, simd_abi::fixed_size<size()>>& x) noexcept
				: v{static_cast<physical_type>(x)} {}

			template<std::convertible_to<value_type> U>
				requires(can_simd_convert<op, U, physical_type>)
			simd_mask(const simd_mask<U, simd_abi::fixed_size<size()>>& x) noexcept
				: v{op::template convert(static_cast<typename simd_mask<U, simd_abi::fixed_size<size()>>::physical_type>(x))} {}

			template<std::convertible_to<value_type> U>
			simd_mask(const simd_mask<U, simd_abi::fixed_size<size()>>& x) noexcept
			{
				using other_op = typename simd_abi::fixed_size<size()>::op;

				value_type buf1[op::physical_size()];
				U buf2[simd_mask<U, other_op::physical_size()>];

				other_op::store_all(buf2, static_cast<typename other_op::physical_type>(x.v));

				std::transform(buf2,
							   buf2 + size(),
							   buf1,
							   [](const auto& i) { return static_cast<value_type>(i); });

				v = op::load_all(buf1);
			}

			template<simd_flag_type Flags>
			simd_mask(const value_type* mem, Flags f) noexcept
				: v{mem, f} {}

			[[nodiscard]] explicit operator physical_type() const noexcept
			{
				return static_cast<physical_type>(v);
			}

			[[nodiscard]] reference operator[](std::size_t i) noexcept
			{
				return {*this, i};
			}

			[[nodiscard]] value_type operator[](std::size_t i) const noexcept
			{
				return static_cast<value_type>(v[i]);
			}

			template<simd_flag_type Flags>
			void copy_from(const value_type* mem, [[maybe_unused]] Flags f) noexcept
			{
				v.copy_from(mem, f);
			}

			template<simd_flag_type Flags>
			void copy_to(value_type* mem, [[maybe_unused]] Flags f) const noexcept
			{
				v.copy_to(mem, f);
			}

			simd_mask operator!() const noexcept
			{
				return simd_mask{op::logical_not(static_cast<physical_type>(v))};
			}

			simd_mask& operator&=(const simd_mask& other) noexcept
			{
				v &= other.v;
				return *this;
			}

			simd_mask& operator|=(const simd_mask& other) noexcept
			{
				v |= other.v;
				return *this;
			}

			simd_mask& operator^=(const simd_mask& other) noexcept
			{
				v ^= other.v;
				return *this;
			}

			friend simd_mask operator&(simd_mask lhs, const simd_mask& rhs) noexcept
			{
				return lhs &= rhs;
			}

			friend simd_mask operator&&(simd_mask lhs, const simd_mask& rhs) noexcept
			{
				return lhs &= rhs;
			}

			friend simd_mask operator|(simd_mask lhs, const simd_mask& rhs) noexcept
			{
				return lhs |= rhs;
			}

			friend simd_mask operator||(simd_mask lhs, const simd_mask& rhs) noexcept
			{
				return lhs |= rhs;
			}

			friend simd_mask operator^(simd_mask lhs, const simd_mask& rhs) noexcept
			{
				return lhs ^= rhs;
			}

			friend simd_mask operator==(const simd_mask& lhs, const simd_mask& rhs) noexcept
			{
				return op::eq(static_cast<physical_type>(lhs),
							  static_cast<physical_type>(rhs));
			}

			friend simd_mask operator!=(const simd_mask& lhs, const simd_mask& rhs) noexcept
			{
				return op::neq(static_cast<physical_type>(lhs),
							   static_cast<physical_type>(rhs));
			}
		private:
			simd_type v;
	};

	export
	template<class T>
	using native_simd_mask = simd_mask<T, simd_abi::native<T>>;
	
	export
	template<class T, std::size_t N>
	using fixed_size_simd_mask = simd_mask<T, simd_abi::fixed_size<N>>;

	export
	template<class T>
	struct is_simd_mask : std::false_type {};

	export
	template<class T, class Abi>
		requires(std::destructible<simd_mask<T, Abi>>)
	struct is_simd_mask<simd<T, Abi>> : std::true_type {};

	export
	template<class T>
	inline constexpr auto is_simd_mask_v = is_simd_mask<T>::value;

	export
	template<class T, class V>
	struct rebind_simd;

	export
	template<class T, class U, class Abi>
		requires(requires { typename simd_abi::deduce<T, simd_abi::simd_size_v<U, Abi>>::type; })
	struct rebind_simd<T, simd<U, Abi>> : std::type_identity<simd<T, simd_abi::deduce_t<T, simd_abi::simd_size_v<U, Abi>>>> {};

	export
	template<class T, class U, class Abi>
		requires(requires { typename simd_abi::deduce<T, simd_abi::simd_size_v<U, Abi>, Abi>::type; })
	struct rebind_simd<T, simd_mask<U, Abi>> : std::type_identity<simd_mask<T, simd_abi::deduce_t<T, simd_abi::simd_size_v<U, Abi>, Abi>>> {};

	export
	template<class T, class V>
	using rebind_simd_t = typename rebind_simd<T, V>::type;

	export
	template<std::size_t N, class V>
	struct resize_simd;

	export
	template<std::size_t N, class T, class Abi>
		requires(requires { typename simd_abi::deduce<T, N, Abi>::type; })
	struct resize_simd<N, simd<T, Abi>> : std::type_identity<simd<T, simd_abi::deduce_t<T, N, Abi>>> {};

	export
	template<std::size_t N, class T, class Abi>
		requires(requires { typename simd_abi::deduce<T, N, Abi>::type; })
	struct resize_simd<N, simd_mask<T, Abi>> : std::type_identity<simd_mask<T, simd_abi::deduce_t<T, N, Abi>>> {};

	export
	template<std::size_t N, class V>
	using resize_simd_t = typename resize_simd<N, V>::type;
}