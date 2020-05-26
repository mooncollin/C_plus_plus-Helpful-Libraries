#pragma once

#include <iterator>
#include <type_traits>
#include <array>

namespace collin
{
	constexpr std::size_t dynamic_extent = static_cast<std::size_t>(-1);

	template<class Type>
	class span_iterator
	{
		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type = std::remove_cv_t<Type>;
			using difference_type = std::ptrdiff_t;
			using pointer = Type*;
			using reference = Type&;

			constexpr span_iterator() = default;
			constexpr span_iterator(pointer begin, pointer end, pointer current)
				: begin_(begin), end_(end), current_(current) {}
			
			constexpr reference operator*() const noexcept
			{
				return *current_;
			}

			constexpr pointer operator->() const noexcept
			{
				return current_;
			}

			constexpr span_iterator& operator++() noexcept
			{
				++current_;
				return *this;
			}

			constexpr span_iterator operator++(int) noexcept
			{
				auto ret = *this;
				++*this;
				return ret;
			}

			constexpr span_iterator& operator--() noexcept
			{
				--current_;
				return *this;
			}

			constexpr span_iterator operator--(int) noexcept
			{
				auto ret = *this;
				--*this;
				return ret;
			}

			constexpr span_iterator& operator+=(const difference_type n) noexcept
			{
				current_ += n;
				return *this;
			}

			constexpr span_iterator operator+(const difference_type n) const noexcept
			{
				auto ret = *this;
				ret += n;
				return ret;
			}

			friend constexpr span_iterator operator+(const difference_type n, const span_iterator& rhs) noexcept
			{
				return rhs + n;
			}

			constexpr span_iterator& operator-=(const difference_type n) noexcept
			{
				current_ -= n;
				return *this;
			}

			constexpr span_iterator operator-(const difference_type n) const noexcept
			{
				auto ret = *this;
				ret -= n;
				return ret;
			}

			constexpr difference_type operator-(const span_iterator& rhs) const noexcept
			{
				return current_ - rhs.current_;
			}

			constexpr reference operator[](const difference_type n) const noexcept
			{
				return *(*this + n);
			}

			constexpr bool operator==(const span_iterator& rhs) noexcept
			{
				return current_ == rhs.current_;
			}

			constexpr bool operator!=(const span_iterator& rhs) noexcept
			{
				return !(*this == rhs);
			}

			constexpr bool operator<(const span_iterator& rhs) noexcept
			{
				return current_ < rhs.current_;
			}

			constexpr bool operator>(const span_iterator& rhs) noexcept
			{
				return rhs < *this;
			}

			constexpr bool operator<=(const span_iterator& rhs) noexcept
			{
				return !(*this < lhs);
			}

			constexpr bool operator>=(const span_iterator& rhs) noexcept
			{
				return !(lhs < *this);
			}
				
		private:
			pointer begin_ {nullptr};
			pointer end_ {nullptr};
			pointer current_ {nullptr};
	};

	

	template<std::size_t Ext>
	class extent_type
	{
		public:
			using size_type = std::size_t;

			constexpr extent_type() noexcept = default;

			template<size_type Other>
			constexpr extent_type(extent_type<Other> ext)
			{
				static_assert(Other == Ext);
			}

			constexpr extent_type(size_type size)
			{
			}

			constexpr size_type size() const noexcept
			{
				return Ext;
			}
	};

	template<>
	class extent_type<dynamic_extent>
	{
		public:
			using size_type = std::size_t;

			template<size_type Other>
			explicit constexpr extent_type(extent_type<Other> ext) : size_(ext.size()) {}
			explicit constexpr extent_type(size_type size) : size_(size) {}

			constexpr size_type size() const noexcept
			{
				return size_;
			}
		private:
			size_type size_;
	};

	template<class ElementType, std::size_t Extent>
	class span
	{
		public:
			using element_type = ElementType;
			using value_type = std::remove_cv_t<ElementType>;
			using size_type = std::size_t;
			using pointer = element_type*;
			using const_pointer = const element_type*;
			using reference = element_type&;
			using const_reference = const element_type&;
			using difference_type = std::ptrdiff_t;

			using iterator = span_iterator<ElementType>;
			using const_iterator = span_iterator<const ElementType>;
			using reverse_iterator = std::reverse_iterator<iterator>;
			using const_reverse_iterator = std::reverse_iterator<const_iterator>;

			constexpr span() noexcept : storage_(nullptr, extent_type<0>()) {}

			constexpr span(pointer ptr, size_type count) noexcept : storage_(ptr, count)
			{
			}
			constexpr span(pointer firstElem, pointer lastElem) noexcept
				: span(firstElem, static_cast<std::size_t>(lastElem - firstElem)) {}

			template<std::size_t N>
			constexpr span(element_type (&arr)[N]) noexcept
				: storage_(arr + 0, extent_type<N>()) {}

			template<std::size_t N>
			constexpr span(std::array<element_type, N>& arr) noexcept
				: storage_(arr.data(), extent_type<N>()) {}

			template<class Container>
			constexpr span(Container& cont) noexcept
				: storage_(cont.data(), cont.size()) {}

			constexpr span(const span& other) noexcept = default;
			~span() noexcept = default;

			constexpr operator pointer() const noexcept
			{
				return data();
			}

			template<std::size_t Count>
			constexpr span<element_type, Count> first() const noexcept
			{
				return {data(), Count};
			}

			template<std::size_t Count>
			constexpr span<element_type, Count> last() const noexcept
			{
				return {data() + (size() - Count), Count};
			}

			constexpr span<element_type, dynamic_extent> first(size_type count) const noexcept
			{
				return {data(), count};
			}

			constexpr span<element_type, dynamic_extent> last(size_type count) const noexcept
			{
				return { data() + (size() - count), count };
			}

			constexpr span<element_type, dynamic_extent> subspan(size_type offset, size_type count = dynamic_extent) const
			{
				return {data() + offset, count};
			}

			constexpr size_type size() const noexcept
			{
				return storage_.size();
			}

			constexpr size_type size_bytes() const noexcept
			{
				return size() * sizeof(element_type);
			}

			constexpr bool empty() const noexcept
			{
				return size() == 0;
			}

			constexpr reference operator[](size_type idx) const noexcept
			{
				return data()[idx];
			}

			constexpr reference front() const noexcept
			{
				return data()[0];
			}

			constexpr reference back() const noexcept
			{
				return data()[size() - 1];
			}

			constexpr pointer data() const noexcept
			{
				return storage_.data();
			}

			constexpr iterator begin() const noexcept
			{
				return {data(), data() + size(), data()};
			}

			constexpr iterator end() const noexcept
			{
				const auto data = storage_.data();
				const auto endData = data + storage_.size();
				return {data, endData, endData};
			}

			constexpr const_iterator cbegin() const noexcept
			{
				const auto data = storage_.data();
				return {data, data + size(), data};
			}

			constexpr const_iterator cend() const noexcept
			{
				const auto data = storage_.data();
				const auto endData = data + storage_.size();
				return {data, endData, endData};
			}

			constexpr reverse_iterator rbegin() const noexcept {return reverse_iterator{end()};}
			constexpr reverse_iterator rend() const noexcept {return reverse_iterator{begin()};}

			constexpr const_reverse_iterator crbegin() const noexcept
			{
				return const_reverse_iterator{cend()};
			}
			constexpr const_reverse_iterator crend() const noexcept
			{
				return const_reverse_iterator{cbegin()};
			}

		private:
			template<class ExtentType>
			class storage_type : public ExtentType
			{
				public:
					template<class OtherExtentType>
					constexpr storage_type(pointer data, OtherExtentType ext)
						: ExtentType(ext), data_(data) {}

					constexpr pointer data() const noexcept
					{
						return data_;
					}
				private:
					pointer data_;
			};

			storage_type<extent_type<Extent>> storage_;
	};

	template<class ElementType>
	constexpr span<ElementType, dynamic_extent> make_span(ElementType* p, std::size_t size) noexcept
	{
		return span<ElementType, dynamic_extent>(p, size);
	}

	template<class ElementType>
	constexpr span<ElementType, dynamic_extent> make_span(ElementType* begin, ElementType* end) noexcept
	{
		return make_span(begin, end - begin);
	}

	template<class ElementType, std::size_t N>
	constexpr span<ElementType, N> make_span(std::array<ElementType, N>& arr) noexcept
	{
		return span<ElementType, N>(arr);
	}

	template<class ElementType, std::size_t N>
	constexpr span<ElementType, N> make_span(ElementType(&arr)[N]) noexcept
	{
		return span<ElementType, N>(arr);
	}

	template<class Container>
	constexpr span<typename Container::value_type, dynamic_extent> make_span(Container& c) noexcept
	{
		return span<typename Container::value_type, dynamic_extent>(c);
	}

	template<class ElementType, std::size_t Extent>
	struct calculate_byte_size : std::integral_constant<std::size_t, sizeof(ElementType) * Extent>
	{
		static_assert(Extent < dynamic_extent / sizeof(ElementType));
	};

	template<class ElementType>
	struct calculate_byte_size<ElementType, dynamic_extent>
		: std::integral_constant<std::size_t, dynamic_extent> {};

	template<class ElementType, std::size_t Extent>
	span<const std::byte, calculate_byte_size<ElementType, Extent>::value>
		as_bytes(span<ElementType, Extent> s) noexcept
	{
		return {reinterpret_cast<const std::byte*>(s.data()), s.size_bytes()};
	}

	template<class ElementType, std::size_t Extent>
	span<std::byte, calculate_byte_size<ElementType, Extent>::value>
		as_writable_bytes(span<ElementType, Extent> s) noexcept
	{
		return { reinterpret_cast<std::byte*>(s.data()), s.size_bytes() };
	}
}