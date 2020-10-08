#pragma once

#include <iterator>
#include <type_traits>
#include <array>

namespace collin
{
	constexpr std::size_t dynamic_extent = static_cast<std::size_t>(-1);

	template<std::size_t Ext>
	class extent_type
	{
		public:
			using size_type = std::size_t;

			constexpr extent_type() noexcept = default;

			template<size_type Other>
			constexpr extent_type(extent_type<Other> ext) noexcept
			{
				static_assert(Other == Ext);
			}

			constexpr extent_type(size_type) noexcept
			{
			}

			[[nodiscard]] constexpr size_type size() const noexcept
			{
				return Ext;
			}

			constexpr void swap(extent_type& other) noexcept {}
	};

	template<>
	class extent_type<dynamic_extent>
	{
		public:
			using size_type = std::size_t;

			template<size_type Other>
			explicit constexpr extent_type(extent_type<Other> ext) noexcept : size_(ext.size()) {}
			explicit constexpr extent_type(size_type size) noexcept : size_(size) {}

			[[nodiscard]] constexpr size_type size() const noexcept
			{
				return size_;
			}

			constexpr void swap(extent_type& other) noexcept
			{
				std::swap(size_, other.size_);
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

			using iterator = pointer;
			using const_iterator = const iterator;
			using reverse_iterator = std::reverse_iterator<iterator>;
			using const_reverse_iterator = std::reverse_iterator<const_iterator>;

			constexpr span() noexcept
				: storage_(nullptr, extent_type<0>()) {}

			constexpr span(pointer ptr, size_type count) noexcept
				: storage_(ptr, count) {}

			constexpr span(pointer firstElem, pointer lastElem) noexcept
				: storage_(firstElem, static_cast<std::size_t>(lastElem - firstElem)) {}

			template<std::size_t N>
			constexpr span(element_type (&arr)[N]) noexcept
				: storage_(&arr, extent_type<N>()) {}

			template<std::size_t N>
			constexpr span(std::array<element_type, N>& arr) noexcept
				: storage_(arr.data(), extent_type<N>()) {}

			template<class Container>
			constexpr span(Container& cont) noexcept
				: storage_(cont.data(), cont.size()) {}

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
				return data();
			}

			constexpr iterator end() const noexcept
			{
				return data() + size();
			}

			constexpr const_iterator cbegin() const noexcept
			{
				return data();
			}

			constexpr const_iterator cend() const noexcept
			{
				return data() + size();
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
		return span<ElementType, dynamic_extent>(begin, end);
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