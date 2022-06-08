export module cmoon.simd.where_expression;

namespace cmoon
{
	export
	template<class M, class T>
	class const_where_expression
	{
		public:
			const_where_expression(const const_where_expression&) = delete;
			const_where_expression& operator=(const const_where_expression&) = delete;

			T operator-() const && noexcept;
			T operator+() const && noexcept;
			T operator~() const && noexcept;

			template<class U, class Flags>
			void copy_to(U* mem, Flags f) const &&;
		private:
			const M mask;
			T& data;
	};

	export
	template<class M, class T>
	class where_expression : public const_where_expression<M, T>
	{
		public:
			template<class U>
			void operator=(U&& x) && noexcept;

			template<class U>
			void operator+=(U&& x) && noexcept;

			template<class U>
			void operator-=(U&& x) && noexcept;

			template<class U>
			void operator*=(U&& x) && noexcept;

			template<class U>
			void operator/=(U&& x) && noexcept;

			template<class U>
			void operator%=(U&& x) && noexcept;

			template<class U>
			void operator&=(U&& x) && noexcept;

			template<class U>
			void operator|=(U&& x) && noexcept;

			template<class U>
			void operator^=(U&& x) && noexcept;

			template<class U>
			void operator<<=(U&& x) && noexcept;

			template<class U>
			void operator>>=(U&& x) && noexcept;

			void operator++() && noexcept;
			void operator++(int) && noexcept;
			void operator--() && noexcept;
			void operator--(int) && noexcept;

			template<class U, class Flags>
			void copy_from(const U* mem, Flags) &&;
	};
}